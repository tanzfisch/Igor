// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/iRenderEngineOld.h>

#include <igor/scene/iScene.h>
#include <igor/data/iOctreeOld.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/scene/nodes/iNodeRender.h>
#include <igor/scene/nodes/iNodeVolume.h>
#include <igor/renderer/iRenderer.h>
#include <igor/scene/nodes/iNodeLight.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/resources/iResourceManager.h>
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
    void iRenderEngineOld::setCamera(iNodeID cameraID)
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

    iNodeID iRenderEngineOld::getCamera() const
    {
        if (_currentCamera != nullptr)
        {
            return _currentCamera->getID();
        }

        return iNode::INVALID_NODE_ID;
    }

    void iRenderEngineOld::setColorIDRendering(bool enabled)
    {
        _renderColorID = enabled;
    }

    bool iRenderEngineOld::isColorIDRendering() const
    {
        return _renderColorID;
    }

    void iRenderEngineOld::setBoundingBoxVisible(bool boundingBox)
    {
        _showBoundingBoxes = boundingBox;
    }

    bool iRenderEngineOld::isBoundingBoxVisible() const
    {
        return _showBoundingBoxes;
    }

    void iRenderEngineOld::setOctreeVisible(bool octree)
    {
        _showOctree = octree;
    }

    bool iRenderEngineOld::isOctreeVisible() const
    {
        return _showOctree;
    }

    void iRenderEngineOld::setScene(iScenePtr scene)
    {
        _scene = scene;
    }

    iScenePtr iRenderEngineOld::getScene() const
    {
        return _scene;
    }

    void iRenderEngineOld::render()
    {
        if (_scene == nullptr ||
            _currentCamera == nullptr)
        {
            return;
        }

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

        iRenderer::getInstance().flush();
    }

    void iRenderEngineOld::cullScene(iNodeCamera *camera)
    {
        iaMatrixd view;
        camera->getViewMatrix(view);

        iaMatrixd projectionMatrix = iRenderer::getInstance().getProjectionMatrix();
        projectionMatrix *= view;
        const iFrustumd frustum(projectionMatrix);
        _scene->setFrustum(frustum);
    }

    void iRenderEngineOld::addNodeToMaterialGroups(iNodeRenderPtr renderNode)
    {
        if (!renderNode->isVisible())
        {
            return;
        }

        iMaterialPtr material = renderNode->getMaterial();
        if (material == nullptr)
        {
            return;
        }

        iShaderPtr shader = material->getShader();

        if (shader == nullptr ||
            !shader->isValid())
        {
            return;
        }

        auto iter = std::find_if(_materialGroups.begin(), _materialGroups.end(),
                                 [&shader](const iMaterialGroup &materialGroup)
                                 { return materialGroup._material == shader; });

        if (iter != _materialGroups.end())
        {
            iter->_renderNodes.push_back(renderNode);
        }
        else
        {
            _materialGroups.push_back({shader, {renderNode}, std::unordered_map<iMeshPtr, iInstaningPackage>()});
        }
    }

    void iRenderEngineOld::updateMaterialGroups()
    {
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

        std::vector<void *> cullResult;
        _scene->getCullResult(cullResult);

        for (void *ptr : cullResult)
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

    void iRenderEngineOld::drawColorIDs()
    {
        iRenderer::getInstance().setShader(iRenderer::getInstance().getColorIDMaterial());

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

    void iRenderEngineOld::drawScene()
    {
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
            iRenderer::getInstance().setShader(materialGroup._material);

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
                    // TODO using random material we find for now
                    materialGroup._instancing[mesh]._material = nodeMesh->getMaterial();
                }

                for (const auto &pair : materialGroup._instancing)
                {
                    iRenderer::getInstance().drawMeshInstanced(pair.first, pair.second._buffer, pair.second._material);
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
            _scene->drawOctree();
        }
    }

    void iRenderEngineOld::addMesh(iMeshPtr mesh, iMaterialPtr material, const iaMatrixd &matrix)
    {
    }

} // namespace igor