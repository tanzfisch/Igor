#include "GameCore.h"

#include <iApplication.h>
#include <iWindow.h>
#include <iKeyboard.h>

#include <iSceneFactory.h>
#include <iScene.h>

#include <iNodeModel.h>
#include <iModel.h>
#include <iNodeFactory.h>
#include <iNodeCamera.h>
#include <iNodeSkyBox.h>
#include <iNodeLight.h>
#include <iNodeTransform.h>

#include <iPhysics.h>
#include <iPhysicsBody.h>

#include <iRenderer.h>
#include <iRenderStateSet.h>
#include <iMaterial.h>
#include <iMaterialGroup.h>

#include <iMaterialResourceFactory.h>
using namespace Igor;

#include "Player.h"
#include "Enemy.h"
#include "EntityManager.h"

GameCore::GameCore(iWindow* window)
{
    _parentWindow = window;
    init();
}

GameCore::~GameCore()
{
    deinit();
}

void GameCore::init()
{
    // window & view
    _view.setPerspective(45.0f);
    _view.setClearColor(iaColor4f(1, 1, 0, 1));
    _parentWindow->addView(&_view);

    // scene
    _scene = iSceneFactory::getInstance().createScene();

    _view.setScene(_scene);

    // camera
    _cameraHeading = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraPitch = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraDistance = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));

    _cameraHeading->insertNode(_cameraPitch);
    _cameraPitch->insertNode(_cameraDistance);
    _cameraDistance->insertNode(_camera);

    _cameraPitch->rotate(-45.0f / 180.0f * M_PI, iaAxis::X);
    _cameraDistance->translate(0, 0, 7);

    _scene->getRoot()->insertNode(_cameraHeading);
    _camera->makeCurrent();

    // light
    _directionalLightRotate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _directionalLightTranslate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _directionalLightTranslate->translate(100, 100, 100);
    _directionalLightRotate->insertNode(_directionalLightTranslate);
    _lightNode = static_cast<iNodeLight*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLight));
    _lightNode->setAmbient(iaColor4f(0.6f, 0.6f, 0.6f, 1.0f));
    _lightNode->setDiffuse(iaColor4f(0.8f, 0.8f, 0.8f, 1.0f));
    _lightNode->setSpecular(iaColor4f(0.9f, 0.9f, 0.9f, 1.0f));

    _directionalLightTranslate->insertNode(_lightNode);
    _scene->getRoot()->insertNode(_directionalLightRotate);

    // skybox
    int32 materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial("SkyBox");
    iMaterialResourceFactory::getInstance().getMaterial(materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterialGroup(materialSkyBox)->setOrder(10);

    iNodeSkyBox* skyBoxNode = static_cast<iNodeSkyBox*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSkyBox));
    skyBoxNode->setTextures(
        "skybox_default/front.png",
        "skybox_default/back.png",
        "skybox_default/left.png",
        "skybox_default/right.png",
        "skybox_default/top.png",
        "skybox_default/bottom.png");
    skyBoxNode->setTextureScale(10);
    skyBoxNode->setMaterial(materialSkyBox);
    _scene->getRoot()->insertNode(skyBoxNode);

    // register handles
    iApplication::getInstance().registerApplicationHandleDelegate(iApplicationHandleDelegate(this, &GameCore::onHandle));
    iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownDelegate(this, &GameCore::onKeyPressed));
    iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &GameCore::onKeyReleased));

    initPlayer();

    _isRunning = true;
}

void GameCore::initPlayer()
{
    _player = new Player(_scene, iaVector3f(0, 0, 0));

    Enemy* enemy = new Enemy(_scene, iaVector3f(4, 0, 0));
    
    _enemies.push_back(enemy);
}

void GameCore::deinitPlayer()
{
    delete _player;
}

void GameCore::deinit()
{
    iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownDelegate(this, &GameCore::onKeyPressed));
    iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &GameCore::onKeyReleased));


    deinitPlayer();

    _parentWindow->removeView(&_view);
    _view.setScene(nullptr);

    iSceneFactory::getInstance().destroyScene(_scene);
    iApplication::getInstance().unregisterApplicationHandleDelegate(iApplicationHandleDelegate(this, &GameCore::onHandle));

    _isRunning = false;
}

void GameCore::onHandle()
{
    EntityManager::getInstance().handle();
    _scene->handle();
}

bool GameCore::isRunning()
{
    return _isRunning;
}

void GameCore::onKeyPressed(iKeyCode key)
{
    switch (key)
    {
    case iKeyCode::Left:
        _player->startLeft();
        break;

    case iKeyCode::Right:
        _player->startRight();
        break;

    case iKeyCode::Up:
        _player->startUp();
        break;

    case iKeyCode::Down:
        _player->startDown();
        break;
    }
}

void GameCore::onKeyReleased(iKeyCode key)
{
    switch (key)
    {
    case iKeyCode::Left:
        _player->stopLeft();
        break;

    case iKeyCode::Right:
        _player->stopRight();
        break;

    case iKeyCode::Up:
        _player->stopUp();
        break;

    case iKeyCode::Down:
        _player->stopDown();
        break;
    }
}