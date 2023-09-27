// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "Background3D.h"

Background3D::Background3D(iWindowPtr window)
    : ExampleBase(window, "3D Scene")
{
}

void Background3D::onInit()
{
    // setup camera
    iNodeTransform *cameraTransform = iNodeManager::getInstance().createNode<iNodeTransform>("camera_transform");
    cameraTransform->translate(0, 0.3, 2.6);
    iNodeCamera *camera = iNodeManager::getInstance().createNode<iNodeCamera>("camera");
    getScene()->getRoot()->insertNode(cameraTransform);
    cameraTransform->insertNode(camera);
    getView().setCurrentCamera(camera->getID());

    // create a cat
    iNodeTransform *catTransform = iNodeManager::getInstance().createNode<iNodeTransform>("cat_transform");
    _catTransformID = catTransform->getID();
    iNodeTransform *offsetTransform = iNodeManager::getInstance().createNode<iNodeTransform>("offset_transform");
    offsetTransform->translate(0,0,-0.35);
    iNodeModel *cat = iNodeManager::getInstance().createNode<iNodeModel>("cat_model");
    cat->setModel("example_model_cat");
    getScene()->getRoot()->insertNode(catTransform);
    catTransform->insertNode(offsetTransform);
    offsetTransform->insertNode(cat);

    // setup light
    iNodeTransform *directionalLightRotate = iNodeManager::getInstance().createNode<iNodeTransform>();
    iNodeTransform *directionalLightTranslate = iNodeManager::getInstance().createNode<iNodeTransform>();
    directionalLightTranslate->translate(100, 100, 100);
    iNodeLight *lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
    lightNode->setAmbient(iaColor3f(0.3f, 0.3f, 0.3f));
    lightNode->setDiffuse(iaColor3f(0.8f, 0.8f, 0.8f));
    lightNode->setSpecular(iaColor3f(1.0f, 1.0f, 1.0f));
    getScene()->getRoot()->insertNode(directionalLightRotate);
    directionalLightRotate->insertNode(directionalLightTranslate);
    directionalLightTranslate->insertNode(lightNode);

    // animation timing
    _animationTimingHandle = new iTimerHandle(iTimerTickDelegate(this, &Background3D::onUpdate), iaTime::fromMilliseconds(10));
    _animationTimingHandle->start();
}

void Background3D::onDeinit()
{
    // stop light animation
    if (_animationTimingHandle)
    {
        delete _animationTimingHandle;
        _animationTimingHandle = nullptr;
    }
}

void Background3D::onUpdate(const iaTime &time)
{
    iNodeTransform *transform = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_catTransformID));
    transform->rotate(0.005f, iaAxis::Y);
}
