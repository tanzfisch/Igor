// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "ExampleSound.h"

#include <igor/system/iApplication.h>
#include <igor/system/iTimer.h>
#include <igor/resources/iResourceManager.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/scene/nodes/iNodeAudioListener.h>
#include <igor/scene/nodes/iNodeAudioSource.h>
#include <igor/scene/nodes/iNodeModel.h>
using namespace igor;

ExampleSound::ExampleSound()
    : ExampleBase(L"Sound")
{
}

void ExampleSound::init()
{
    // load a sound
    iResourceParameters parameters = {"beep.wav"};
    _sound = iResourceManager::getInstance().loadResource(parameters);

    // setup camera
    iNodeTransform *camTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    camTransform->setName("camera transform");
    camTransform->translate(0, 3, 0);
    camTransform->rotate(-0.3, iaAxis::X);

    iNodeCamera *camera = iNodeManager::getInstance().createNode<iNodeCamera>();
    camera->setName("camera");
    getScene()->getRoot()->insertNode(camTransform);
    camTransform->insertNode(camera);
    getView().setCurrentCamera(camera->getID());

    // attach audio listener to camera
    iNodeAudioListener *audioListener = iNodeManager::getInstance().createNode<iNodeAudioListener>();
    audioListener->setName("audio listener");
    camera->insertNode(audioListener);

    // setup audio source
    iNodeTransform *catCenter = iNodeManager::getInstance().createNode<iNodeTransform>();
    catCenter->setName("cat center");
    catCenter->translate(0, 0, -10);
    iNodeTransform *catRotate = iNodeManager::getInstance().createNode<iNodeTransform>();
    catRotate->setName("cat rotate");
    _rotate = catRotate->getID();
    iNodeTransform *catTranslate = iNodeManager::getInstance().createNode<iNodeTransform>();
    catTranslate->setName("cat translate");
    catTranslate->translate(0, 0, 5);
    catTranslate->rotate(M_PI * 0.5, iaAxis::Y);

    // load a model to visualize the audio source
    iNodeModel *catModel = iNodeManager::getInstance().createNode<iNodeModel>();
    catModel->setModel("cat.ompf");

    // add the source
    iNodeAudioSource *audioSource = iNodeManager::getInstance().createNode<iNodeAudioSource>();
    audioSource->setName("audio source");
    audioSource->setSound(_sound);
    audioSource->setLoop(true);
    audioSource->play();

    getScene()->getRoot()->insertNode(catCenter);
    catCenter->insertNode(catRotate);
    catRotate->insertNode(catTranslate);
    catTranslate->insertNode(catModel);
    catTranslate->insertNode(audioSource);

    // animation
    _timerHandle = new iTimerHandle(iTimerTickDelegate(this, &ExampleSound::onTimer), iaTime::fromMilliseconds(10));
    _timerHandle->start();
}

void ExampleSound::onTimer()
{
    iNodeTransform *transform = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_rotate));
    transform->rotate(0.025f, iaAxis::Y);
}

void ExampleSound::deinit()
{
    if (_timerHandle)
    {
        delete _timerHandle;
        _timerHandle = nullptr;
    }
}
