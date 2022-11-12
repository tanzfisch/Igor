// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include "ExampleSound.h"

ExampleSound::ExampleSound(iWindowPtr window)
    : ExampleBase(window, L"Sound")
{
}

void ExampleSound::onInit()
{
    // load some sounds
    iResourceParameters parametersBeep = {"beep.wav"};
    _soundBeep = iResourceManager::getInstance().loadResource(parametersBeep);
    iResourceParameters parametersLeftRight = {"left_right.wav"};
    _soundLeftRight = iResourceManager::getInstance().loadResource(parametersLeftRight);

    // setup camera
    iNodeTransform *camTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    camTransform->setName("camera transform");
    camTransform->translate(0, 3, 0);
    camTransform->rotate(-0.4, iaAxis::X);

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
    catCenter->translate(0, 0, -8);
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
    audioSource->setName("audio source beep");
    audioSource->setSound(_soundBeep);
    audioSource->setLoop(true);
    audioSource->play();

    // add an extra stereo audio source
    iNodeAudioSource *audioSourceStereo = iNodeManager::getInstance().createNode<iNodeAudioSource>();
    audioSourceStereo->setName("audio source leftright");
    audioSourceStereo->setSound(_soundLeftRight);
    audioSourceStereo->setLoop(true);
    audioSourceStereo->setGain(0.3);
    audioSourceStereo->play();

    // add eversthing to the scene
    getScene()->getRoot()->insertNode(catCenter);
    catCenter->insertNode(catRotate);
    catRotate->insertNode(catTranslate);
    catTranslate->insertNode(catModel);
    catTranslate->insertNode(audioSource);

    getScene()->getRoot()->insertNode(audioSourceStereo);

    // animation
    _timerHandle = new iTimerHandle(iTimerTickDelegate(this, &ExampleSound::onUpdate), iaTime::fromMilliseconds(10));
    _timerHandle->start();
}

void ExampleSound::onUpdate(const iaTime &time)
{
    iNodeTransform *transform = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_rotate));
    transform->rotate(0.03f, iaAxis::Y);
}

void ExampleSound::onDeinit()
{
    // destroy timer handle
    if (_timerHandle)
    {
        delete _timerHandle;
        _timerHandle = nullptr;
    }

    // free sounds
    _soundBeep = nullptr;
    _soundLeftRight = nullptr;
}
