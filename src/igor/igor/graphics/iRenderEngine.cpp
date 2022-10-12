// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/graphics/iRenderEngine.h>

#include <igor/scene/iScene.h>
#include <igor/data/iOctree.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/resources/material/iMaterialGroup.h>
#include <igor/scene/nodes/iNodeRender.h>
#include <igor/scene/nodes/iNodeVolume.h>
#include <igor/graphics/iRenderer.h>
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
    iRenderEngine::iRenderEngine(bool useProfiling)
    {
        if (useProfiling)
        {
            _bufferCreationSectionID = iProfiler::getInstance().createSection("buff");
            _cullSectionID = iProfiler::getInstance().createSection("cull");
            _updateGroupsID = iProfiler::getInstance().createSection("groups");
            _drawSectionID = iProfiler::getInstance().createSection("draw");
        }

        iMaterialResourceFactory::getInstance().registerMaterialCreatedDelegate(iMaterialCreatedDelegate(this, &iRenderEngine::onMaterialCreated));
        iMaterialResourceFactory::getInstance().registerMaterialDestroyedDelegate(iMaterialDestroyedDelegate(this, &iRenderEngine::onMaterialDestroyed));

        auto materials = iMaterialResourceFactory::getInstance().getSortedMaterials();
        for (auto material : materials)
        {
            iMaterialGroup materialGroup;
            materialGroup.setMaterial(material);
            _materialGroups[material->getID()] = materialGroup;
        }
    }

    iRenderEngine::~iRenderEngine()
    {
        iMaterialResourceFactory::getInstance().unregisterMaterialCreatedDelegate(iMaterialCreatedDelegate(this, &iRenderEngine::onMaterialCreated));
        iMaterialResourceFactory::getInstance().unregisterMaterialDestroyedDelegate(iMaterialDestroyedDelegate(this, &iRenderEngine::onMaterialDestroyed));
    }

    void iRenderEngine::onMaterialCreated(iMaterialID materialID)
    {
        iMaterialPtr material = iMaterialResourceFactory::getInstance().getMaterial(materialID);
        iMaterialGroup materialGroup;
        materialGroup.setMaterial(material);
        _materialGroups[materialID] = materialGroup;
    }

    void iRenderEngine::onMaterialDestroyed(iMaterialID materialID)
    {
        auto iter = _materialGroups.find(materialID);

        if (iter != _materialGroups.end())
        {
            _materialGroups.erase(iter);
        }
    }

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

    void iRenderEngine::setWireframeVisible(bool wireframe)
    {
        _showWireframe = wireframe;
    }

    bool iRenderEngine::isWireframeVisible() const
    {
        return _showWireframe;
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

    void iRenderEngine::createBuffers()
    {
        iRenderer::getInstance().createBuffers(10);
    }

    void iRenderEngine::render()
    {
        iProfiler::getInstance().beginSection(_bufferCreationSectionID);
        createBuffers();
        iProfiler::getInstance().endSection(_bufferCreationSectionID);

        if (_scene != nullptr &&
            _currentCamera != nullptr)
        {
            iProfiler::getInstance().beginSection(_cullSectionID);
            cullScene(_currentCamera);
            iProfiler::getInstance().endSection(_cullSectionID);

            iProfiler::getInstance().beginSection(_updateGroupsID);
            updateMaterialGroups();
            iProfiler::getInstance().endSection(_updateGroupsID);

            iProfiler::getInstance().beginSection(_drawSectionID);
            if (_renderColorID)
            {
                drawColorIDs();
            }
            else
            {
                drawScene();
            }
            iProfiler::getInstance().endSection(_drawSectionID);
        }
    }

    void iRenderEngine::cullScene(iNodeCamera *camera)
    {
        iaMatrixd view;
        camera->getViewMatrix(view);
        iaMatrixd camMatrix;
        camera->getWorldMatrix(camMatrix);
        iRenderer::getInstance().setViewMatrix(view, camMatrix);

        iaMatrixd projectionMatrix;
        iRenderer::getInstance().getProjectionMatrix(projectionMatrix);
        projectionMatrix *= view;
        const iFrustumd frustum(projectionMatrix);
        _scene->getOctree()->filter(frustum);
    }

    void iRenderEngine::updateMaterialGroups()
    {
        for (auto &group : _materialGroups)
        {
            group.second.clear();
        }

        for (void *ptr : _scene->getOctree()->getResult())
        {
            iNodeRenderPtr renderNode = static_cast<iNodeRenderPtr>(ptr);

            if (renderNode != nullptr &&
                renderNode->isVisible())
            {
                _materialGroups[renderNode->getMaterial()].addRenderNode(renderNode);
            }
        }

        for (const auto renderNode : _scene->getRenderables())
        {
            if (renderNode->isVisible())
            {
                _materialGroups[renderNode->getMaterial()].addRenderNode(renderNode);
            }
        }
    }

    void iRenderEngine::drawColorIDs()
    {
        iMaterialPtr colorIDMaterial = iMaterialResourceFactory::getInstance().getMaterial(iMaterialResourceFactory::getInstance().getColorIDMaterialID());
        iRenderer::getInstance().setMaterial(colorIDMaterial);

        auto materials = iMaterialResourceFactory::getInstance().getSortedMaterials();
        for (auto material : materials)
        {
            iMaterialGroup &materialGroup = _materialGroups[material->getID()];

            if (iRenderStateValue::On == material->getRenderState(iRenderState::Instanced))
            {
                // TODO no color id for now
            }
            else
            {
                auto renderNodes = materialGroup.getRenderNodes();
                for (auto renderNode : renderNodes)
                {
                    iRenderer::getInstance().setColorID(renderNode->getID());
                    renderNode->draw();
                }
            }
        }
    }

    void iRenderEngine::drawScene()
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

        auto materials = iMaterialResourceFactory::getInstance().getSortedMaterials();
        for (auto material : materials)
        {
            bool instancing = (material->getRenderState(iRenderState::Instanced) == iRenderStateValue::On);
            iMaterialGroup &materialGroup = _materialGroups[material->getID()];

            if (instancing)
            {
                // todo we should not do that every frame
                const auto &instancedRenderNodes = materialGroup.getInstancedRenderNodes();
                iRenderer::getInstance().setMaterial(material, _showWireframe);

                for (const auto &pair : instancedRenderNodes)
                {
                    auto instancer = pair.second._instancer;
                    if (instancer->getInstanceCount() == 0)
                    {
                        continue;
                    }

                    iRenderer::getInstance().setTargetMaterial(pair.second._targetMaterial);
                    iRenderer::getInstance().drawMesh(pair.first, instancer);
                }
            }
            else
            {
                const auto &renderNodes = materialGroup.getRenderNodes();

                if (!renderNodes.empty())
                {
                    iRenderer::getInstance().setMaterial(material, _showWireframe);
                }

                for (auto renderNode : renderNodes)
                {
                    renderNode->draw();
                }
            }
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