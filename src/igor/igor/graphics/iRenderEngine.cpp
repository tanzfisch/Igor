// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/graphics/iRenderEngine.h>

#include <igor/scene/iScene.h>
#include <igor/scene/octree/iOctree.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/data/iFrustum.h>
#include <igor/resources/material/iMaterialGroup.h>
#include <igor/scene/nodes/iNodeRender.h>
#include <igor/scene/nodes/iNodeVolume.h>
#include <igor/graphics/iRenderer.h>
#include <igor/scene/nodes/iNodeLight.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/resources/profiler/iProfiler.h>
#include <igor/system/iTimer.h>
#include <igor/scene/traversal/iNodeVisitorRenderBoundings.h>

#include <iaux/system/iaConsole.h>
#include <iaux/data/iaConvert.h>
using namespace iaux;

namespace igor
{

    iRenderEngine::iRenderEngine()
    {
#ifdef USE_VERBOSE_STATISTICS
        _cullSectionID = iProfiler::getInstance().registerSection("renderer:cull", 1);
        _drawSectionID = iProfiler::getInstance().registerSection("renderer:draw", 1);
        _bufferCreationSectionID = iProfiler::getInstance().registerSection("renderer:createBuffers", 1);
#endif

        iMaterialResourceFactory::getInstance().registerMaterialCreatedDelegate(iMaterialCreatedDelegate(this, &iRenderEngine::onMaterialCreated));
        iMaterialResourceFactory::getInstance().registerMaterialDestroyedDelegate(iMaterialDestroyedDelegate(this, &iRenderEngine::onMaterialDestroyed));

        auto materials = iMaterialResourceFactory::getInstance().getSortedMaterials();
        for (auto material : materials)
        {
            iMaterialGroup materialGroup;
            _materialGroups[material->getID()] = materialGroup;
        }
    }

    iRenderEngine::~iRenderEngine()
    {
        iMaterialResourceFactory::getInstance().unregisterMaterialCreatedDelegate(iMaterialCreatedDelegate(this, &iRenderEngine::onMaterialCreated));
        iMaterialResourceFactory::getInstance().unregisterMaterialDestroyedDelegate(iMaterialDestroyedDelegate(this, &iRenderEngine::onMaterialDestroyed));

#ifdef USE_VERBOSE_STATISTICS
        iProfiler::getInstance().unregisterSection(_cullSectionID);
        iProfiler::getInstance().unregisterSection(_drawSectionID);
        iProfiler::getInstance().unregisterSection(_bufferCreationSectionID);
#endif
    }

    void iRenderEngine::onMaterialCreated(uint64 materialID)
    {
        iMaterialGroup materialGroup;
        _materialGroups[materialID] = materialGroup;
        _dirtyGroups = true;
    }

    void iRenderEngine::onMaterialDestroyed(uint64 materialID)
    {
        auto iter = _materialGroups.find(materialID);

        if (iter != _materialGroups.end())
        {
            _materialGroups.erase(iter);
        }
    }

    void iRenderEngine::setCurrentCamera(uint64 cameraID)
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

    uint64 iRenderEngine::getCurrentCamera() const
    {
        uint64 result = iNode::INVALID_NODE_ID;

        if (_currentCamera != nullptr)
        {
            result = _currentCamera->getID();
        }

        return result;
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

    iScenePtr iRenderEngine::getScene()
    {
        return _scene;
    }

    void iRenderEngine::createBuffers()
    {
        iRenderer::getInstance().createBuffers(10);
    }

    void iRenderEngine::render()
    {
#ifdef USE_VERBOSE_STATISTICS
        iProfiler::getInstance().beginSection(_bufferCreationSectionID);
#endif
        createBuffers();

#ifdef USE_VERBOSE_STATISTICS
        iProfiler::getInstance().endSection(_bufferCreationSectionID);
#endif

        if (_scene != nullptr &&
            _currentCamera != nullptr)
        {
#ifdef USE_VERBOSE_STATISTICS
            iProfiler::getInstance().beginSection(_cullSectionID);
#endif
            cullScene(_currentCamera);
#ifdef USE_VERBOSE_STATISTICS
            iProfiler::getInstance().endSection(_cullSectionID);

            iProfiler::getInstance().beginSection(_drawSectionID);
#endif
            if (_renderColorID)
            {
                drawColorIDs();
            }
            else
            {
                drawScene();
            }
#ifdef USE_VERBOSE_STATISTICS
            iProfiler::getInstance().endSection(_drawSectionID);
#endif
        }
    }

    void iRenderEngine::cullScene(iNodeCamera *camera)
    {
        iaMatrixd view;
        iaMatrixd camMatrix;
        camera->getViewMatrix(view);
        camera->getWorldMatrix(camMatrix);
        iRenderer::getInstance().setViewMatrix(view, camMatrix);

        iaMatrixd frustumMatrix;
        iRenderer::getInstance().getProjectionMatrix(frustumMatrix);
        frustumMatrix *= view;

        iFrustumd frustum;
        frustum.set(frustumMatrix);

        _scene->getOctree()->clearFilter();
        _scene->getOctree()->addFilter(frustum);
        _scene->getOctree()->filter();
        _scene->getOctree()->getResult(_cullResult);

        for (auto nodeID : _cullResult)
        {
            iNodeRender *renderNode = static_cast<iNodeRender *>(iNodeManager::getInstance().getNode(nodeID));

            if (renderNode != nullptr)
            {
                renderNode->_reached = true;

                iMaterialPtr material = iMaterialResourceFactory::getInstance().getMaterial(renderNode->getMaterial());
                if (material != nullptr)
                {
                    if (renderNode->isVisible())
                    {
                        bool instancing = (material->getRenderState(iRenderState::Instanced) == iRenderStateValue::On);
                        _materialGroups[material->getID()].addRenderNode(renderNode->getID(), instancing);
                    }
                }
            }
        }

        auto renderables = _scene->getRenderables();
        auto iterRenderables = renderables.begin();
        while (iterRenderables != renderables.end())
        {
            iNodeRender *renderNode = (*iterRenderables);
            iMaterialPtr material = iMaterialResourceFactory::getInstance().getMaterial(renderNode->getMaterial());

            if (material != nullptr)
            {
                if (renderNode->isVisible())
                {
                    bool instancing = (material->getRenderState(iRenderState::Instanced) == iRenderStateValue::On);
                    _materialGroups[material->getID()].addRenderNode(renderNode->getID(), instancing);
                }
            }

            iterRenderables++;
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
                auto renderNodeIDs = materialGroup.getRenderNodes();
                for (auto renderNodeID : renderNodeIDs)
                {
                    iNodeRender *node = static_cast<iNodeRender *>(iNodeManager::getInstance().getNode(renderNodeID));
                    if (node != nullptr)
                    {
                        if (node->wasReached() &&
                            node->isVisible())
                        {
                            iRenderer::getInstance().setColorID(node->getID());
                            node->draw();
                            node->_reached = false;
                        }
                        else
                        {
                            materialGroup.removeRenderNode(renderNodeID, false);
                        }
                    }
                    else
                    {
                        materialGroup.removeRenderNode(renderNodeID, false);
                    }
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
            iRenderer::getInstance().setMaterial(material, _showWireframe);
            bool instancing = (material->getRenderState(iRenderState::Instanced) == iRenderStateValue::On);
            iMaterialGroup &materialGroup = _materialGroups[material->getID()];

            if (instancing)
            {
                // todo we should not do that every frame
                auto instancedRenderNodes = materialGroup.getInstancedRenderNodes();
                for (auto instancedRenderNode : instancedRenderNodes)
                {
                    const auto renderNodeIDs = instancedRenderNode.second._renderNodeIDs;
                    auto instancer = instancedRenderNode.second._instancer;
                    instancer->clearInstances();

                    for (auto renderNodeID : renderNodeIDs)
                    {
                        iNodeRender *node = static_cast<iNodeRender *>(iNodeManager::getInstance().getNode(renderNodeID));
                        if (node != nullptr)
                        {
                            if (node->wasReached() &&
                                node->isVisible() &&
                                node->getMaterial() == material->getID())
                            {
                                iaMatrixf matrix;
                                const auto worldMatrix = node->getWorldMatrix();
                                for (int i = 0; i < 16; ++i)
                                {
                                    matrix[i] = worldMatrix[i];
                                }

                                instancer->addInstance(matrix.getData());
                                node->_reached = false;
                            }
                            else
                            {
                                materialGroup.removeRenderNode(renderNodeID, true);
                            }
                        }
                        else
                        {
                            materialGroup.removeRenderNode(renderNodeID, true);
                        }
                    }

                    // renderNodeIDs should never be empty
                    iNodeMesh *mesh = static_cast<iNodeMesh *>(iNodeManager::getInstance().getNode(renderNodeIDs[0]));
                    iRenderer::getInstance().setTargetMaterial(mesh->getTargetMaterial());
                    iRenderer::getInstance().drawMesh(instancedRenderNode.first, instancer);
                }
            }
            else
            {
                auto renderNodeIDs = materialGroup.getRenderNodes();
                for (auto renderNodeID : renderNodeIDs)
                {
                    iNodeRender *node = static_cast<iNodeRender *>(iNodeManager::getInstance().getNode(renderNodeID));
                    if (node != nullptr)
                    {
                        if (node->wasReached() &&
                            node->isVisible() &&
                            node->getMaterial() == material->getID())
                        {
                            node->draw();
                            node->_reached = false;
                        }
                        else
                        {
                            materialGroup.removeRenderNode(renderNodeID, false);
                        }
                    }
                    else
                    {
                        materialGroup.removeRenderNode(renderNodeID, false);
                    }
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