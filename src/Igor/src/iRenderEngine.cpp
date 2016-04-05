// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
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

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iRenderEngine::iRenderEngine()
    {
    }

    iRenderEngine::~iRenderEngine()
    {
        if (_cullSectionID != 0)
        {
            iStatistics::getInstance().unregisterSection(_cullSectionID);
        }

        if (_drawSectionID != 0)
        {
            iStatistics::getInstance().unregisterSection(_drawSectionID);
        }
    }

    void iRenderEngine::setScene(iScene* scene)
    {
        _scene = scene;

        if (_scene != nullptr)
        {
            _cullSectionID = iStatistics::getInstance().registerSection("cull", iaColor4f(1, 0, 0, 1), 4);
            _drawSectionID = iStatistics::getInstance().registerSection("draw", iaColor4f(0, 1, 0, 1), 4);
        }
        else
        {
            if (_cullSectionID != 0)
            {
                iStatistics::getInstance().unregisterSection(_cullSectionID);
                _cullSectionID = 0;
            }

            if (_drawSectionID != 0)
            {
                iStatistics::getInstance().unregisterSection(_drawSectionID);
                _drawSectionID = 0;
            }
        }
    }

    iScene* iRenderEngine::getScene()
    {
        return _scene;
    }

    void iRenderEngine::render()
    {
        if (_scene)
        {
            iNodeCamera* camera = _scene->getCamera();

            if (camera)
            {
                iStatistics::getInstance().beginSection(_cullSectionID);
                cullScene(camera);
                iStatistics::getInstance().endSection(_cullSectionID);

                iStatistics::getInstance().beginSection(_drawSectionID);
                drawScene(camera);
                iStatistics::getInstance().endSection(_drawSectionID);
            }
            else
            {
                con_warn("no camera specified");
            }
        }
        else
        {
            con_warn("no scene set");
        }
    }

    void iRenderEngine::cullScene(iNodeCamera* camera)
    {
        iaMatrixf view;
        camera->getViewMatrix(view);
        iRenderer::getInstance().setViewMatrix(view);

        iaMatrixf camMatrix;
        camera->getWorldMatrix(camMatrix);
        iRenderer::getInstance().setCamWorldMatrix(camMatrix);

        iaMatrixf projection;
        iRenderer::getInstance().getProjectionMatrix(projection);
        projection *= view;

        iaMatrixd projectiond; // TODO whole system should switch to double precision

        for (int i = 0; i < 16; ++i)
        {
            projectiond[i] = static_cast<float64>(projection[i]);
        }

        iFrustumd frustum;
        frustum.set(projectiond);
        _scene->getOctree()->resetFilter();
        _scene->getOctree()->filter(frustum);
        _scene->getOctree()->getResult(_cullResult);

        for (auto nodeID : _cullResult)
        {
            iNodeRender* renderNode = static_cast<iNodeRender*>(iNodeFactory::getInstance().getNode(nodeID));
            renderNode->_reached = true;

            if (renderNode != nullptr)
            {
                uint32 materialID = renderNode->getMaterial();
                if (materialID != iMaterial::INVALID_MATERIAL_ID)
                {
                    iMaterialResourceFactory::getInstance().getMaterialGroup(materialID)->addRenderNode(renderNode);
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
                iMaterialResourceFactory::getInstance().getMaterialGroup(materialID)->addRenderNode(renderNode);
            }

            iterRenderables++;
        }
    }

    /*! \todo clean up
    */
    void iRenderEngine::drawScene(iNodeCamera* camera)
    {
        //! \todo not sure yet how to handle multiple lights
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
        while(materialIter != materialGroups->end())
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
                        auto instanceList = (*instanceIter).second._nodes;
                        auto elementIter = instanceList.begin();
                        uint32 instanceCount = instanceList.size();
                        uint32 index = 0;
                        iInstancer* instancer = (*instanceIter).second._instancer;

                        instancer->clearInstances();

                        while (instanceList.end() != elementIter)
                        {
                            if ((*elementIter)->wasReached())
                            {
                                instancer->addInstance((*elementIter)->getWorldMatrix().getData());

                                (*elementIter)->_reached = false;
                                ++elementIter;
                            }
                            else
                            {
                                elementIter = instanceList.erase(elementIter);
                                instanceCount--;
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
                if (materialGroup->_renderNodes.size() > 0)
                {
                    iRenderer::getInstance().setMaterial(materialGroup->getMaterial());

                    auto iter = materialGroup->_renderNodes.begin();
                    while (iter != materialGroup->_renderNodes.end())
                    {
                        if ((*iter)->wasReached())
                        {
                            (*iter)->draw();
                            (*iter)->_reached = false;
                            ++iter;
                        }
                        else
                        {
                            iter = materialGroup->_renderNodes.erase(iter);
                        }
                    }
                }
            }

            ++materialIter;
        }
    }

}