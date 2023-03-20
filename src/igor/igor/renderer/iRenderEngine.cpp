// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/iRenderEngine.h>

#include <igor/scene/iScene.h>
#include <igor/data/iOctree.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/scene/nodes/iNodeRender.h>
#include <igor/scene/nodes/iNodeVolume.h>
#include <igor/renderer/iRenderer.h>
#include <igor/scene/nodes/iNodeLight.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/system/iTimer.h>
#include <igor/scene/traversal/iNodeVisitorRenderBoundings.h>
#include <igor/resources/profiler/iProfiler.h>
#include <igor/data/iFrustum.h>

#include <iaux/system/iaConsole.h>
#include <iaux/data/iaConvert.h>
using namespace iaux;

namespace igor
{
    void iRenderEngine::setCurrentCamera(iNodeID cameraID)
    {
        if (cameraID == iNode::INVALID_NODE_ID)
        {
            _currentCamera = nullptr;
            return;
        }

        iNodePtr node = iNodeManager::getInstance().getNode(cameraID);

        if (node != nullptr &&
            node->getType() == iNodeType::iNodeCamera)
        {
            _currentCamera = static_cast<iNodeCamera *>(node);
        }
        else
        {
            con_err("invalid parameter");
        }
    }

    iNodeID iRenderEngine::getCurrentCamera() const
    {
        if (_currentCamera != nullptr)
        {
            return _currentCamera->getID();
        }

        return iNode::INVALID_NODE_ID;
    }

    void iRenderEngine::setColorIDRendering(bool enabled)
    {
        _renderColorID = enabled;
    }

    bool iRenderEngine::isColorIDRendering() const
    {
        return _renderColorID;
    }

    void iRenderEngine::setBoundingBoxVisible(bool boundingBox)
    {
        _showBoundingBoxes = boundingBox;
    }

    bool iRenderEngine::isBoundingBoxVisible() const
    {
        return _showBoundingBoxes;
    }

    void iRenderEngine::setOctreeVisible(bool octree)
    {
        _showOctree = octree;
    }

    bool iRenderEngine::isOctreeVisible() const
    {
        return _showOctree;
    }

    void iRenderEngine::setScene(iScenePtr scene)
    {
        _scene = scene;
    }

    iScenePtr iRenderEngine::getScene() const
    {
        return _scene;
    }

    void iRenderEngine::render()
    {
        if (_scene != nullptr &&
            _currentCamera != nullptr)
        {
            cullScene(_currentCamera);
            updateMaterialGroups();

            iaMatrixd camMatrix;
            _currentCamera->getWorldMatrix(camMatrix);
            iRenderer::getInstance().setViewMatrixFromCam(camMatrix);

            if (_renderColorID)
            {
                drawColorIDs();
            }
            else
            {
                drawScene();
            }
        }
    }

    void iRenderEngine::cullScene(iNodeCamera *camera)
    {
        IGOR_PROFILER_SCOPED(cull);

        iaMatrixd view;
        camera->getViewMatrix(view);

        iaMatrixd projectionMatrix = iRenderer::getInstance().getProjectionMatrix();
        projectionMatrix *= view;
        const iFrustumd frustum(projectionMatrix);
        _scene->getOctree()->filter(frustum);
    }

    void iRenderEngine::addNodeToMaterialGroups(iNodeRenderPtr renderNode)
    {
        iMaterialPtr material = renderNode->getMaterial();

        if (!renderNode->isVisible() ||
            material == nullptr ||
            !material->isValid())
        {
            return;
        }

        auto iter = std::find_if(_materialGroups.begin(), _materialGroups.end(),
                                 [&material](const iMaterialGroup &materialGroup)
                                 { return materialGroup._material == material; });

        if (iter != _materialGroups.end())
        {
            iter->_renderNodes.push_back(renderNode);
        }
        else
        {
            if (material->getRenderState(iRenderState::Instanced) == iRenderStateValue::Off)
            {
                _materialGroups.push_back({material, {renderNode}, std::unordered_map<iMeshPtr, iInstaningPackage>()});
            }
            else
            {
                _materialGroups.push_back({material,
                                           {renderNode},
                                           std::unordered_map<iMeshPtr, iInstaningPackage>()});
            }
        }
    }

    void iRenderEngine::updateMaterialGroups()
    {
        IGOR_PROFILER_SCOPED(mat);

        auto iter = _materialGroups.begin();
        while (iter != _materialGroups.end())
        {
            // remove unused materials
            if (iter->_renderNodes.empty())
            {
                iter = _materialGroups.erase(iter);
                continue;
            }

            iter++;
        }

        for (void *ptr : _scene->getOctree()->getResult())
        {
            addNodeToMaterialGroups(static_cast<iNodeRenderPtr>(ptr));
        }

        for (const auto renderNode : _scene->getRenderables())
        {
            addNodeToMaterialGroups(renderNode);
        }

        std::sort(_materialGroups.begin(), _materialGroups.end(), [](const iMaterialGroup a, const iMaterialGroup b) -> bool
                  { return a._material->getOrder() < b._material->getOrder(); });
    }

    void iRenderEngine::drawColorIDs()
    {
        IGOR_PROFILER_SCOPED(col_id);

        iRenderer::getInstance().setMaterial(iMaterialResourceFactory::getInstance().getColorIDMaterial());

        for (const auto &materialGroup : _materialGroups)
        {
            for (iNodeRenderPtr renderNode : materialGroup._renderNodes)
            {
                if (renderNode->getType() != iNodeType::iNodeMesh)
                {
                    continue;
                }

                iRenderer::getInstance().setColorID(renderNode->getID());
                renderNode->draw();
            }
        }
    }

    void iRenderEngine::drawScene()
    {
        IGOR_PROFILER_SCOPED(draw);

        //! \todo not sure yet how to handle multiple lights. right now it will work only for one light
        auto lights = _scene->getLights();

        int lightNum = 0;
        for (auto light : lights)
        {
            iRenderer::getInstance().setLightAmbient(lightNum, light->getAmbient());
            iRenderer::getInstance().setLightDiffuse(lightNum, light->getDiffuse());
            iRenderer::getInstance().setLightSpecular(lightNum, light->getSpecular());

            if (iLightType::Directional == light->getType())
            {
                iRenderer::getInstance().setLightPosition(lightNum, light->getDirection());
            }
            else
            {
                iRenderer::getInstance().setLightPosition(lightNum, light->getPosition());
            }

            ++lightNum;
        }

        for (auto &materialGroup : _materialGroups)
        {
            iRenderer::getInstance().setMaterial(materialGroup._material);

            if (materialGroup._material->getRenderState(iRenderState::Instanced) == iRenderStateValue::Off)
            {
                for (iNodeRenderPtr renderNode : materialGroup._renderNodes)
                {
                    renderNode->draw();
                }
            }
            else
            {
                for (iNodeRenderPtr renderNode : materialGroup._renderNodes)
                {
                    if (renderNode->getType() != iNodeType::iNodeMesh)
                    {
                        continue;
                    }

                    iNodeMeshPtr nodeMesh = static_cast<iNodeMeshPtr>(renderNode);

                    if (nodeMesh == nullptr ||
                        nodeMesh->getMesh() == nullptr ||
                        !nodeMesh->getMesh()->isValid())
                    {
                        continue;
                    }

                    iMeshPtr mesh = nodeMesh->getMesh();

                    iaMatrixd src = renderNode->getWorldMatrix();
                    iaMatrixf dst;
                    for (int i = 0; i < 16; ++i)
                    {
                        dst[i] = src[i];
                    }

                    if (materialGroup._instancing[mesh]._buffer == nullptr)
                    {
                        materialGroup._instancing[mesh]._buffer = iInstancingBuffer::create(std::vector<iBufferLayoutEntry>{{iShaderDataType::Matrix4x4}}, materialGroup._renderNodes.size());
                    }

                    materialGroup._instancing[mesh]._buffer->addInstance(sizeof(iaMatrixf), dst.getData());
                    // TODO using random target material we find for now
                    materialGroup._instancing[mesh]._targetMaterial = nodeMesh->getTargetMaterial();
                }

                for (const auto &pair : materialGroup._instancing)
                {
                    iRenderer::getInstance().drawBuffer(pair.first, pair.second._buffer, pair.second._targetMaterial);
                    pair.second._buffer->clear();
                }
            }

            materialGroup._renderNodes.clear();
        }

        if (_showBoundingBoxes)
        {
            iNodeVisitorRenderBoundings renderBoundings;
            renderBoundings.traverseTree(_scene->getRoot());
        }

        if (_showOctree)
        {
            _scene->getOctree()->draw();
        }
    }

} // namespace igor