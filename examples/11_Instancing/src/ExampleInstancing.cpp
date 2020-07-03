// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "ExampleInstancing.h"

#include <igor/resources/material/iMaterial.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/iScene.h>
#include <igor/scene/nodes/iNodeLight.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/resources/model/iModelResourceFactory.h>
#include <igor/resources/material/iShader.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/system/iMouse.h>
using namespace igor;

ExampleInstancing::ExampleInstancing()
    : ExampleBase("Instacing")
{
}

void ExampleInstancing::onInit()
{
    const float32 spacing = 2.0f;
    const int32 amountPerDimension = 20;

    // setup camera
    // we want a camera which can be rotated arround the origin
    // we will acchive that with 3 transform nodes
    // one is for the heading
    iNodeTransform *cameraHeading = iNodeManager::getInstance().createNode<iNodeTransform>();
    // give the transform node a name. naming is optional and ist jus for helping to debug.
    // Names do not have to be unique but since it is possible to find nodes by name they better are
    cameraHeading->setName("camera heading");
    _cameraHeading = cameraHeading->getID();
    // one is for the pitch
    iNodeTransform *cameraPitch = iNodeManager::getInstance().createNode<iNodeTransform>();
    cameraPitch->setName("camera pitch");
    _cameraPitch = cameraPitch->getID();
    // and one is for translation or distance from the origin
    iNodeTransform *cameraTranslation = iNodeManager::getInstance().createNode<iNodeTransform>();
    cameraTranslation->setName("camera translation");
    // translate away from origin
    cameraTranslation->translate(0, 0, spacing * amountPerDimension * 0.5);
    _cameraTranslation = cameraTranslation->getID();
    // from all nodes that we want to control later we save the node ID
    // and last but not least we create a camera node
    iNodeCamera *camera = iNodeManager::getInstance().createNode<iNodeCamera>();
    camera->setName("camera");
    // and build everything together
    // first we add the heading to the root node
    getScene()->getRoot()->insertNode(cameraHeading);
    // than the pitch to the heading node
    cameraHeading->insertNode(cameraPitch);
    // then the translation to the pitch node
    cameraPitch->insertNode(cameraTranslation);
    // and than we add the camera to the translation
    cameraTranslation->insertNode(camera);
    // and finally we tell the view which camera shall be the current one. for this to work a camera must be part of a
    // scene assiciated with the view wich we achived by adding all those nodes on to an other starting with the root node
    getView().setCurrentCamera(camera->getID());

    // first we have to override the material which is stored within the model
    // to do that we create a new material using instancing
    _materialWithInstancing = iMaterialResourceFactory::getInstance().createMaterial();
    auto material = iMaterialResourceFactory::getInstance().getMaterial(_materialWithInstancing);
    material->setRenderState(iRenderState::Instanced, iRenderStateValue::On);
    material->setRenderState(iRenderState::InstancedFunc, iRenderStateValue::PositionOrientation);
    material->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    material->addShaderSource("igor/textured_ipo.vert", iShaderObjectType::Vertex);
    material->addShaderSource("igor/textured_ipo_directional_light.frag", iShaderObjectType::Fragment);
    material->compileShader();

    // now we can just put copies of that model in the scene
    iNodeTransform *transformGroup = iNodeManager::getInstance().createNode<iNodeTransform>();
    transformGroup->translate(-((amountPerDimension - 1) * spacing * 0.5), -((amountPerDimension - 1) * spacing * 0.5), -((amountPerDimension - 1) * spacing * 0.5));
    getScene()->getRoot()->insertNode(transformGroup);

    int count = 0;

    // create a bunch of models
    for (int z = 0; z < amountPerDimension; ++z)
    {
        for (int y = 0; y < amountPerDimension; ++y)
        {
            for (int x = 0; x < amountPerDimension; ++x)
            {
                iNodeTransform *transform = iNodeManager::getInstance().createNode<iNodeTransform>();
                transform->translate(x * spacing, y * spacing, z * spacing);
                transform->rotate(((rand() % 100) / 100.0) * M_PI * 2, iaAxis::X);
                transform->rotate(((rand() % 100) / 100.0) * M_PI * 2, iaAxis::Y);
                transform->rotate(((rand() % 100) / 100.0) * M_PI * 2, iaAxis::Z);

                iNodeModel *modelNode = iNodeManager::getInstance().createNode<iNodeModel>();
                // it is important to use the exact same parameters here as before when we direclty loaded the model
                // because here it will not load it again but get it from the cache where we is still the version with manipulated material
                modelNode->setModel("cat.ompf", iResourceCacheMode::Keep);
                modelNode->registerModelReadyDelegate(iModelReadyDelegate(this, &ExampleInstancing::onModelReady));

                // building the created nodes together and insert them in the scene
                transformGroup->insertNode(transform);
                transform->insertNode(modelNode);

                count++;
            }
        }
    }

    con_info("instance count: " << count);

    // setup light
    // transform node for the lights orientation
    iNodeTransform *directionalLightRotate = iNodeManager::getInstance().createNode<iNodeTransform>();
    // keep transform node id so we can manipulate the light's position later
    _directionalLightRotate = directionalLightRotate->getID();
    // transform node for the lights distance to the origin
    iNodeTransform *directionalLightTranslate = iNodeManager::getInstance().createNode<iNodeTransform>();
    directionalLightTranslate->translate(100, 100, 100);
    // the light node
    iNodeLight *lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
    lightNode->setAmbient(iaColor4f(0.3f, 0.3f, 0.3f, 1.0f));
    lightNode->setDiffuse(iaColor4f(0.8f, 0.8f, 0.8f, 1.0f));
    lightNode->setSpecular(iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
    // insert light to scene
    getScene()->getRoot()->insertNode(directionalLightRotate);
    directionalLightRotate->insertNode(directionalLightTranslate);
    directionalLightTranslate->insertNode(lightNode);

    // animation
    _animationTimingHandle = new iTimerHandle(iTimerTickDelegate(this, &ExampleInstancing::onTimer), iaTime::fromMilliseconds(10));
    _animationTimingHandle->start();
}

void ExampleInstancing::onDeinit()
{
    // stop light animation
    if (_animationTimingHandle)
    {
        delete _animationTimingHandle;
        _animationTimingHandle = nullptr;
    }

    iMaterialResourceFactory::getInstance().destroyMaterial(_materialWithInstancing);
    _materialWithInstancing = iMaterial::INVALID_MATERIAL_ID;
}

void ExampleInstancing::onEvent(iEvent &event)
{
    // first call example base
    ExampleBase::onEvent(event);

    event.dispatch<iMouseMoveEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(ExampleInstancing::onMouseMoveEvent));
    event.dispatch<iMouseWheelEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(ExampleInstancing::onMouseWheelEvent));
}

bool ExampleInstancing::onMouseWheelEvent(iMouseWheelEvent_TMP &event)
{
    iNodeTransform *camTranslation = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraTranslation));
    if (camTranslation != nullptr)
    {
        if (event.getWheelDelta() < 0)
        {
            camTranslation->translate(0, 0, 2);
        }
        else
        {
            camTranslation->translate(0, 0, -2);
        }
    }

    return true;
}

bool ExampleInstancing::onMouseMoveEvent(iMouseMoveEvent_TMP &event)
{
    if (iMouse::getInstance().getLeftButton())
    {
        const auto from = event.getLastPosition();
        const auto to = event.getPosition();

        iNodeTransform *cameraPitch = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraPitch));
        iNodeTransform *cameraHeading = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraHeading));

        if (cameraPitch != nullptr &&
            cameraHeading != nullptr)
        {
            cameraPitch->rotate((from._y - to._y) * 0.0005f, iaAxis::X);
            cameraHeading->rotate((from._x - to._x) * 0.0005f, iaAxis::Y);
            iMouse::getInstance().setCenter();
        }

        return true;
    }

    return false;
}

void ExampleInstancing::onTimer()
{
    iNodeTransform *directionalLightRotate = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_directionalLightRotate));
    directionalLightRotate->rotate(0.005f, iaAxis::Y);
}

void ExampleInstancing::onModelReady(uint64 modelNodeID)
{
    iNodeModel *modelNode = static_cast<iNodeModel *>(iNodeManager::getInstance().getNode(modelNodeID));
    if (modelNode != nullptr &&
        modelNode->isValid())
    {
        iNodeMesh *meshNode = static_cast<iNodeMesh *>(modelNode->getChild("cat"));
        if (meshNode != nullptr)
        {
            meshNode->setMaterial(_materialWithInstancing);
        }
    }
}
