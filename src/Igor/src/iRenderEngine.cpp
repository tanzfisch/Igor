// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iRenderEngine.h>

#include <iScene.h>
#include <iOctree.h>
#include <iNodeCamera.h>
#include <iFrustum.h>
#include <iMaterialGroup.h>
#include <iNodeRender.h>
#include <iNodeVolume.h>
#include <iRenderer.h>
#include <iNodeLight.h>
#include <iNodeMesh.h>
#include <iMesh.h>
#include <iMaterialResourceFactory.h>
#include <iNodeFactory.h>
#include <iStatistics.h>
#include <iTimer.h>
#include <iNodeVisitorRenderBoundings.h>

#include <iaConsole.h>
#include <iaConvert.h>
using namespace IgorAux;

namespace Igor
{

    iRenderEngine::iRenderEngine()
    {
#ifdef USE_VERBOSE_STATISTICS
        _cullSectionID = iStatistics::getInstance().registerSection("renderer:cull", 1);
        _drawSectionID = iStatistics::getInstance().registerSection("renderer:draw", 1);
        _bufferCreationSectionID = iStatistics::getInstance().registerSection("renderer:createBuffers", 1);
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
        iStatistics::getInstance().unregisterSection(_cullSectionID);
        iStatistics::getInstance().unregisterSection(_drawSectionID);
        iStatistics::getInstance().unregisterSection(_bufferCreationSectionID);
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

        iNode* node = iNodeFactory::getInstance().getNode(cameraID);

        if (node != nullptr &&
            node->getType() == iNodeType::iNodeCamera)
        {
            _currentCamera = static_cast<iNodeCamera*>(node);
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

    void iRenderEngine::setScene(iScene* scene)
    {
        _scene = scene;
    }

    iScene* iRenderEngine::getScene()
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
        iStatistics::getInstance().beginSection(_bufferCreationSectionID);
#endif
        createBuffers();

#ifdef USE_VERBOSE_STATISTICS
        iStatistics::getInstance().endSection(_bufferCreationSectionID);
#endif

        if (_scene != nullptr &&
            _currentCamera != nullptr &&
            _currentCamera->getScene() == _scene)
        {
#ifdef USE_VERBOSE_STATISTICS
            iStatistics::getInstance().beginSection(_cullSectionID);
#endif
            cullScene(_currentCamera);
#ifdef USE_VERBOSE_STATISTICS
            iStatistics::getInstance().endSection(_cullSectionID);

            iStatistics::getInstance().beginSection(_drawSectionID);
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
            iStatistics::getInstance().endSection(_drawSectionID);
#endif
        }

    }

    void iRenderEngine::cullScene(iNodeCamera* camera)
    {
        iaMatrixd view;
        iaMatrixd camMatrix;
        camera->getViewMatrix(view);
        camera->getWorldMatrix(camMatrix);
        iRenderer::getInstance().setViewMatrix(view, camMatrix);

        iaMatrixd projection;
        iRenderer::getInstance().getProjectionMatrix(projection);
        projection *= view;

        iFrustumd frustum;
        frustum.set(projection);
        _scene->getOctree()->clearFilter();
        _scene->getOctree()->addFilter(frustum);
        _scene->getOctree()->filter();
        _scene->getOctree()->getResult(_cullResult);

        for (auto nodeID : _cullResult)
        {
            iNodeRender* renderNode = static_cast<iNodeRender*>(iNodeFactory::getInstance().getNode(nodeID));

            if (renderNode != nullptr)
            {
                renderNode->_reached = true;

                iMaterialPtr material = iMaterialResourceFactory::getInstance().getMaterial(renderNode->getMaterial());
                if (material != nullptr)
                {
                    if (renderNode->isVisible())
                    {
                        bool instancing = (material->getRenderStateSet().getRenderStateValue(iRenderState::Instanced) == iRenderStateValue::On);
                        _materialGroups[material->getID()].addRenderNode(renderNode->getID(), instancing);
                    }
                }
            }
        }

        auto renderables = _scene->getRenderables();
        auto iterRenderables = renderables.begin();
        while (iterRenderables != renderables.end())
        {
            iNodeRender* renderNode = (*iterRenderables);
            iMaterialPtr material = iMaterialResourceFactory::getInstance().getMaterial(renderNode->getMaterial());

            if (material != nullptr)
            {
                if (renderNode->isVisible())
                {
                    bool instancing = (material->getRenderStateSet().getRenderStateValue(iRenderState::Instanced) == iRenderStateValue::On);
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
            iMaterialGroup& materialGroup = _materialGroups[material->getID()];

            if (iRenderStateValue::On == material->getRenderStateSet().getRenderStateValue(iRenderState::Instanced))
            {
                // TODO later   
            }
            else
            {
                auto renderNodeIDs = materialGroup.getRenderNodes();
                for (auto renderNodeID : renderNodeIDs)
                {
                    iNodeRender* node = static_cast<iNodeRender*>(iNodeFactory::getInstance().getNode(renderNodeID));
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
        vector<iNodeLight*>& lights = _scene->getLights();

        int lightNum = 0;
        auto light = lights.begin();
        while (lights.end() != light)
        {
            iRenderer::getInstance().setLightAmbient(lightNum, (*light)->getAmbient());
            iRenderer::getInstance().setLightDiffuse(lightNum, (*light)->getDiffuse());
            iRenderer::getInstance().setLightSpecular(lightNum, (*light)->getSpecular());

            if (iLightType::Directional == (*light)->getType())
            {
                iRenderer::getInstance().setLightPosition(lightNum, (*light)->getDirection());
            }
            else
            {
                iRenderer::getInstance().setLightPosition(lightNum, (*light)->getPosition());
            }

            ++light;
            ++lightNum;
        }

        auto materials = iMaterialResourceFactory::getInstance().getSortedMaterials();
        for (auto material : materials)
        {
            iRenderer::getInstance().setMaterial(material, _showWireframe);
            bool instancing = (material->getRenderStateSet().getRenderStateValue(iRenderState::Instanced) == iRenderStateValue::On);
            iMaterialGroup& materialGroup = _materialGroups[material->getID()];
            
            if (instancing)
            {
             /*   materialGroup.get
                 if (!materialGroup->_instancedRenderNodes.empty())
                 {
                     iRenderer::getInstance().setMaterial(materialGroup->getMaterial(), _showWireframe);

                     auto instanceIter = materialGroup->_instancedRenderNodes.begin();
                     while (instanceIter != materialGroup->_instancedRenderNodes.end())
                     {
                         auto instanceList = (*instanceIter).second._renderNodeIDs;
                         auto elementIter = instanceList.begin();
                         uint32 index = 0;
                         iInstancer* instancer = (*instanceIter).second._instancer;

                         instancer->clearInstances();

                         while (instanceList.end() != elementIter)
                         {
                             iNodeRender* node = static_cast<iNodeRender*>(iNodeFactory::getInstance().getNode((*elementIter)));
                             if (node != nullptr)
                             {
                                 if (node->wasReached() &&
                                     node->isVisible() &&
                                     node->getMaterial() == materialGroup->getID())
                                 {
                                     iaMatrixf matrix;
                                     iaConvert::convert(node->getWorldMatrix(), matrix);
                                     instancer->addInstance(matrix.getData());

                                     node->_reached = false;
                                     ++elementIter;
                                 }
                                 else
                                 {
                                     elementIter = instanceList.erase(elementIter);
                                 }
                             }
                             else
                             {
                                 elementIter = instanceList.erase(elementIter);
                             }
                         }

                         iRenderer::getInstance().drawMesh((*instanceIter).first, instancer);

                         if (instanceList.empty())
                         {
                             if ((*instanceIter).second._instancer != nullptr)
                             {
                                 delete (*instanceIter).second._instancer;
                             }

                             instanceIter = materialGroup->_instancedRenderNodes.erase(instanceIter);
                         }
                         else
                         {
                             ++instanceIter;
                         }
                     }
                 }*/
            }
            else
            {
                auto renderNodeIDs = materialGroup.getRenderNodes();
                for (auto renderNodeID : renderNodeIDs)
                {
                    iNodeRender* node = static_cast<iNodeRender*>(iNodeFactory::getInstance().getNode(renderNodeID));
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

}