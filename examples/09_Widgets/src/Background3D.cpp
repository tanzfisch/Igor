// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "Background3D.h"

Background3D::Background3D(iWindow *window)
    : ExampleBase(window, "3D Scene")
{
}

void Background3D::onInit()
{
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
    cameraTranslation->translate(0, 0, 10);
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
    // and finally we set the camera active. for this to work a camera must be part of a scene
    // wich we achived by adding all those nodes on to an other starting with the root node
    getView().setCurrentCamera(camera->getID());

    // create a single cat model
    iNodeTransform *justCatTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    justCatTransform->setName("just a cat transform");
    justCatTransform->translate(0, 1, 0);
    // create a cat model
    iNodeModel *justCatModel = iNodeManager::getInstance().createNode<iNodeModel>();
    // Node model names can be altered but they also are generated based on the file name
    justCatModel->setModel("cat.ompf");
    // building the created nodes together and insert them in the scene
    getScene()->getRoot()->insertNode(justCatTransform);
    justCatTransform->insertNode(justCatModel);

    // create a group of models that can be moved together due to being child to the same transform node
    // creating transformation node used for the heading of it's children
    iNodeTransform *allObjectsHeading = iNodeManager::getInstance().createNode<iNodeTransform>();
    allObjectsHeading->setName("all objects heading");
    _allObjectsHeading = allObjectsHeading->getID();
    // creating transformation node used for the pitch of it's children
    iNodeTransform *allObjectsPitch = iNodeManager::getInstance().createNode<iNodeTransform>();
    allObjectsPitch->setName("all objects pitch");
    _allObjectsPitch = allObjectsPitch->getID();
    // and add the nodes to the scene
    getScene()->getRoot()->insertNode(allObjectsHeading);
    allObjectsHeading->insertNode(allObjectsPitch);

    // next we create a couple of model nodes that will be connected to be alternative representations of each other controled by a switch node
    // create a cat model including a transform node to add later to the switch node
    iNodeTransform *catTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    catTransform->setName("cat transform");
    catTransform->translate(1, -1, 0);
    iNodeModel *catModel = iNodeManager::getInstance().createNode<iNodeModel>();
    catModel->setModel("cat.ompf");
    // add the cat to the cat transform
    catTransform->insertNode(catModel);
    // repead for the teapot
    iNodeTransform *teapotTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    teapotTransform->setName("teapot transform");
    teapotTransform->translate(1, -1, 0);
    teapotTransform->scale(0.33, 0.33, 0.33);
    iNodeModel *teapotModel = iNodeManager::getInstance().createNode<iNodeModel>();
    teapotModel->setModel("teapot.ompf");
    teapotTransform->insertNode(teapotModel);
    // repeat for a crate
    iNodeTransform *crateTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    crateTransform->setName("crate transform");
    crateTransform->translate(1, -0.5, 0);
    iNodeModel *crateModel = iNodeManager::getInstance().createNode<iNodeModel>();
    crateModel->setModel("crate.ompf");
    crateTransform->insertNode(crateModel);
    // creating the switch node
    iNodeSwitch *switchNode = iNodeManager::getInstance().createNode<iNodeSwitch>();
    _switchNode = switchNode->getID();
    // add the switch node to the all objects pitch / group
    allObjectsPitch->insertNode(switchNode);
    // than add the transform nodes to the switch node
    switchNode->insertNode(crateTransform);
    switchNode->insertNode(teapotTransform);
    switchNode->insertNode(catTransform);
    // set the current activated child
    switchNode->setActiveChild("crate transform");

    // now we basically do the same again. creating three models and tree transform nodes but this time we add them not to a switch node but to an level of detail node
    // the teapot as LOD0
    iNodeTransform *lod0Transform = iNodeManager::getInstance().createNode<iNodeTransform>();
    lod0Transform->setName("lod0 transform");
    lod0Transform->translate(-1, -1, 0);
    lod0Transform->scale(0.33, 0.33, 0.33);
    iNodeModel *lod0Model = iNodeManager::getInstance().createNode<iNodeModel>();
    lod0Model->setModel("teapot.ompf");
    lod0Transform->insertNode(lod0Model);
    // the cat as LOD1
    iNodeTransform *lod1Transform = iNodeManager::getInstance().createNode<iNodeTransform>();
    lod1Transform->setName("lod1 transform");
    lod1Transform->translate(-1, -1, 0);
    iNodeModel *lod1Model = iNodeManager::getInstance().createNode<iNodeModel>();
    lod1Model->setModel("cat.ompf");
    lod1Transform->insertNode(lod1Model);
    // the create as LOD2
    iNodeTransform *lod2Transform = iNodeManager::getInstance().createNode<iNodeTransform>();
    lod2Transform->setName("lod0 transform");
    lod2Transform->translate(-1, -0.5, 0);
    iNodeModel *lod2Model = iNodeManager::getInstance().createNode<iNodeModel>();
    lod2Model->setModel("crate.ompf");
    lod2Transform->insertNode(lod2Model);
    // creating the LOD switch node
    iNodeLODSwitch *lodswitch = iNodeManager::getInstance().createNode<iNodeLODSwitch>();
    // add it to the scene
    allObjectsPitch->insertNode(lodswitch);
    // and add the nodes to it
    lodswitch->insertNode(lod0Transform);
    lodswitch->insertNode(lod1Transform);
    lodswitch->insertNode(lod2Transform);
    // set up the nodes switch distances. the switch distances are allowed to have gaps or overlap
    lodswitch->setThresholds(lod0Transform, 0.0f, 5.0f);
    lodswitch->setThresholds(lod1Transform, 5.0f, 11.0f);
    lodswitch->setThresholds(lod2Transform, 11.0f, 22.0f);
    // in order to causing the LOD switch to change the active child node it needs to be triggered by a LOD trigger node
    // creating the lod trigger node
    iNodeLODTrigger *lodtrigger = iNodeManager::getInstance().createNode<iNodeLODTrigger>();
    // bind the lod switch node with the lod trigger node
    lodswitch->addTrigger(lodtrigger);
    // and add the lod trigger to the scene by attaching it to the camera. there can be multiple LOD triggers and any lod switch can react on any lod trigger
    camera->insertNode(lodtrigger);

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
    _animationTimingHandle = new iTimerHandle(iTimerTickDelegate(this, &Background3D::onTimer), iaTime::fromMilliseconds(10));
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

void Background3D::onEvent(iEvent &event)
{
    ExampleBase::onEvent(event);

    event.dispatch<iEventMouseKeyDown>(IGOR_BIND_EVENT_FUNCTION(Background3D::onMouseKeyDownEvent));
    event.dispatch<iMouseMoveEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(Background3D::onMouseMoveEvent));
    event.dispatch<iMouseWheelEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(Background3D::onMouseWheelEvent));
}

bool Background3D::onMouseKeyDownEvent(iEventMouseKeyDown &event)
{
    switch (event.getKey())
    {
    case iKeyCode::Space:
    {
        _activeNode++;
        if (_activeNode > 2)
        {
            _activeNode = 0;
        }

        iNodeSwitch *switchNode = static_cast<iNodeSwitch *>(iNodeManager::getInstance().getNode(_switchNode));
        if (switchNode != nullptr)
        {
            switch (_activeNode)
            {
            case 0:
                switchNode->setActiveChild("crate transform");
                break;
            case 1:
                switchNode->setActiveChild("cat transform");
                break;
            case 2:
                switchNode->setActiveChild("teapot transform");
                break;
            }
        }

        return true;
    }
    break;
    }

    return false;
}

bool Background3D::onMouseMoveEvent(iMouseMoveEvent_TMP &event)
{
    const auto from = event.getLastPosition();
    const auto to = event.getPosition();

    if (iMouse::getInstance().getRightButton())
    {
        iNodeTransform *allObjectsPitch = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_allObjectsPitch));
        iNodeTransform *allObjectsHeading = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_allObjectsHeading));

        if (allObjectsPitch != nullptr &&
            allObjectsHeading != nullptr)
        {
            allObjectsPitch->rotate((to._y - from._y) * 0.0005f, iaAxis::X);
            allObjectsHeading->rotate((to._x - from._x) * 0.0005f, iaAxis::Y);
        }
    }
    else if (iMouse::getInstance().getLeftButton())
    {
        iNodeTransform *cameraPitch = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraPitch));
        iNodeTransform *cameraHeading = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraHeading));

        if (cameraPitch != nullptr &&
            cameraHeading != nullptr)
        {
            cameraPitch->rotate((to._y - from._y) * 0.005f, iaAxis::X);
            cameraHeading->rotate((to._x - from._x) * 0.005f, iaAxis::Y);
        }
    }

    return true;
}

bool Background3D::onMouseWheelEvent(iMouseWheelEvent_TMP &event)
{
    iNodeTransform *camTranslation = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraTranslation));
    if (camTranslation != nullptr)
    {
        if (event.getWheelDelta() < 0)
        {
            camTranslation->translate(0, 0, 1);
        }
        else
        {
            camTranslation->translate(0, 0, -1);
        }
    }

    return true;
}

void Background3D::onTimer()
{
    iNodeTransform *directionalLightRotate = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_directionalLightRotate));
    directionalLightRotate->rotate(0.005f, iaAxis::Y);
}
