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
    }

    iRenderEngine::~iRenderEngine()
    {
#ifdef USE_VERBOSE_STATISTICS
        iStatistics::getInstance().unregisterSection(_cullSectionID);
        iStatistics::getInstance().unregisterSection(_drawSectionID);
        iStatistics::getInstance().unregisterSection(_bufferCreationSectionID);
#endif
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

        if (_scene != nullptr)
        {
            iNodeCamera* camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().getNode(_scene->getCamera()));

            if (camera != nullptr)
            {
#ifdef USE_VERBOSE_STATISTICS
                iStatistics::getInstance().beginSection(_cullSectionID);
#endif
                cullScene(camera);
#ifdef USE_VERBOSE_STATISTICS
                iStatistics::getInstance().endSection(_cullSectionID);

                iStatistics::getInstance().beginSection(_drawSectionID);
#endif
                drawScene(camera);
#ifdef USE_VERBOSE_STATISTICS
                iStatistics::getInstance().endSection(_drawSectionID);
#endif
            }
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

                uint32 materialID = renderNode->getMaterial();
                if (materialID != iMaterial::INVALID_MATERIAL_ID)
                {
                    if (renderNode->isVisible())
                    {
                        iMaterialResourceFactory::getInstance().getMaterialGroup(materialID)->addRenderNode(renderNode->getID());
                    }
                }
            }
        }

        auto renderables = _scene->getRenderables();
        auto iterRenderables = renderables.begin();
        while (iterRenderables != renderables.end())
        {
            iNodeRender* renderNode = (*iterRenderables);
            uint32 materialID = renderNode->getMaterial();
            if (materialID != iMaterial::INVALID_MATERIAL_ID)
            {
                if (renderNode->isVisible())
                {
                    iMaterialResourceFactory::getInstance().getMaterialGroup(materialID)->addRenderNode(renderNode->getID());
                }
            }

            iterRenderables++;
        }
    }

    void iRenderEngine::drawScene(iNodeCamera* camera)
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

        auto materialGroups = iMaterialResourceFactory::getInstance().getMaterialGroups();
        auto materialIter = materialGroups->begin();
        while (materialIter != materialGroups->end())
        {
            iMaterialGroup* materialGroup = (*materialIter);
            iMaterial* material = materialGroup->getMaterial();
            if (iRenderStateValue::On == material->getRenderStateSet().getRenderStateValue(iRenderState::Instanced))
            {
                if (materialGroup->_instancedRenderNodes.size())
                {
                    iRenderer::getInstance().setMaterial(materialGroup->getMaterial());

                    auto instanceIter = materialGroup->_instancedRenderNodes.begin();
                    while (instanceIter != materialGroup->_instancedRenderNodes.end())
                    {
                        auto instanceList = (*instanceIter).second._renderNodeIDs;
                        auto elementIter = instanceList.begin();
                        uint32 instanceCount = instanceList.size();
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
                                    instanceCount--;
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
                }
            }
            else
            {
                if (materialGroup->_renderNodeIDs.size() > 0)
                {
                    iRenderer::getInstance().setMaterial(materialGroup->getMaterial());

                    auto iter = materialGroup->_renderNodeIDs.begin();
                    while (iter != materialGroup->_renderNodeIDs.end())
                    {
                        iNodeRender* node = static_cast<iNodeRender*>(iNodeFactory::getInstance().getNode((*iter)));
                        if (node != nullptr)
                        {
                            if (node->wasReached() &&
                                node->isVisible() &&
                                node->getMaterial() == materialGroup->getID())
                            {
                                node->draw();
                                node->_reached = false;
                                ++iter;
                            }
                            else
                            {
                                iter = materialGroup->_renderNodeIDs.erase(iter);
                            }
                        }
                        else
                        {
                            iter = materialGroup->_renderNodeIDs.erase(iter);
                        }
                    }
                }
            }

            ++materialIter;
        }
    }

    }