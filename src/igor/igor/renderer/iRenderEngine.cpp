// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/iRenderEngine.h>

#include <igor/scene/iScene.h>
#include <igor/data/iOctree.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/resources/material/iMaterialGroup.h>
#include <igor/scene/nodes/iNodeRender.h>
#include <igor/scene/nodes/iNodeVolume.h>
#include <igor/renderer/iRenderer2.h>
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
        IGOR_PROFILER_SCOPED(b_cre);
        iRenderer2::getInstance().createBuffers(10);
    }

    void iRenderEngine::render()
    {
        createBuffers();

        if (_scene != nullptr &&
            _currentCamera != nullptr)
        {
            cullScene(_currentCamera);
            updateMaterialGroups();

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
        iaMatrixd camMatrix;
        camera->getWorldMatrix(camMatrix);
        iRenderer2::getInstance().setViewMatrixFromCam(camMatrix);

        iaMatrixd projectionMatrix = iRenderer2::getInstance().getProjectionMatrix();
        projectionMatrix *= view;
        const iFrustumd frustum(projectionMatrix);
        _scene->getOctree()->filter(frustum);
    }

    void iRenderEngine::updateMaterialGroups()
    {
        IGOR_PROFILER_SCOPED(mat);

        std::vector<iMaterialPtr> materials;
        iMaterialResourceFactory::getInstance().getMaterials(materials);
        for (auto material : materials)
        {
            auto iter = _materialGroups.find(material);
            if(_materialGroups.end() != iter)
            {
                continue;
            }

            _materialGroups[material] = iMaterialGroup(material);
        }        

        for (auto &group : _materialGroups)
        {
            group.second.clear();
        }

        con_endl("render objects " << _scene->getOctree()->getResult().size());
        for (void *ptr : _scene->getOctree()->getResult())
        {
            iNodeRenderPtr renderNode = static_cast<iNodeRenderPtr>(ptr);

            if (renderNode != nullptr &&
                renderNode->isVisible()                 )
            {

                // temp hack
                if(renderNode->getMaterial() == nullptr)
                {
                    renderNode->setMaterial(iMaterialResourceFactory::getInstance().getDefaultMaterial());
                }

                _materialGroups[renderNode->getMaterial()].addRenderNode(renderNode);
            }
        }

        for (const auto renderNode : _scene->getRenderables())
        {
            if (renderNode->isVisible() &&
                renderNode->getMaterial())
            {
                _materialGroups[renderNode->getMaterial()].addRenderNode(renderNode);
            }
        }
    }

    void iRenderEngine::drawColorIDs()
    {
        IGOR_PROFILER_SCOPED(col_id);

        iRenderer2::getInstance().setMaterial(iMaterialResourceFactory::getInstance().getColorIDMaterial());

        std::vector<iMaterialPtr> materials;
        iMaterialResourceFactory::getInstance().getMaterials(materials);

        for (auto material : materials)
        {
            iMaterialGroup &materialGroup = _materialGroups[material];

            if (iRenderStateValue::On == material->getRenderState(iRenderState::Instanced))
            {
                // TODO no color id for now
            }
            else
            {
                auto renderNodes = materialGroup.getRenderNodes();
                for (auto renderNode : renderNodes)
                {
                    iRenderer2::getInstance().setColorID(renderNode->getID());
                    renderNode->draw();
                }
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
            iRenderer2::getInstance().setLightAmbient(lightNum, light->getAmbient());
            iRenderer2::getInstance().setLightDiffuse(lightNum, light->getDiffuse());
            iRenderer2::getInstance().setLightSpecular(lightNum, light->getSpecular());

            if (iLightType::Directional == light->getType())
            {
                iRenderer2::getInstance().setLightPosition(lightNum, light->getDirection());
            }
            else
            {
                iRenderer2::getInstance().setLightPosition(lightNum, light->getPosition());
            }

            ++lightNum;
        }

        for (const auto &pair : _materialGroups)
        {
            const iMaterialGroup &materialGroup = pair.second;

            con_endl("group nodes " << materialGroup.getRenderNodes().size());

            if(!materialGroup.hasNodes())
            {
                continue;
            }

            if (materialGroup.isInstanced())
            {
                // todo we should not do that every frame
                const auto &instancedRenderNodes = materialGroup.getInstancedRenderNodes();
                iRenderer2::getInstance().setMaterial(materialGroup.getMaterial());

                for (const auto &pair : instancedRenderNodes)
                {
                    auto instancer = pair.second._instancer;
                    if (instancer->getInstanceCount() == 0)
                    {
                        continue;
                    }

                    iRenderer2::getInstance().drawMesh(pair.first, pair.second._targetMaterial, instancer);
                }
            }
            else
            {
                const auto &renderNodes = materialGroup.getRenderNodes();

                if (!renderNodes.empty())
                {
                    iRenderer2::getInstance().setMaterial(materialGroup.getMaterial());
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