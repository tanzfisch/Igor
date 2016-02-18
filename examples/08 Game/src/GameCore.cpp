#include "GameCore.h"

#include <iApplication.h>
#include <iWindow.h>

#include <iSceneFactory.h>
#include <iScene.h>

#include <iNodeModel.h>
#include <iModel.h>
#include <iNodeFactory.h>
#include <iNodeCamera.h>
#include <iNodeSkyBox.h>
#include <iNodeLight.h>
#include <iNodeTransform.h>

#include <iRenderer.h>
#include <iRenderStateSet.h>
#include <iMaterial.h>
#include <iMaterialGroup.h>

#include <iSystemPosition.h>
#include <iSystemSceneTransformation.h>

#include <iPhysicsBody.h>
#include <iMaterialResourceFactory.h>
using namespace Igor;

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
    _cameraHeading->rotate(45.0f / 180.0f * M_PI, iaAxis::Y);
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

    // register handle
    iApplication::getInstance().registerApplicationHandleDelegate(iApplicationHandleDelegate(this, &GameCore::onHandle));

    initSystems();
    initPlayer();

    _isRunning = true;
}


void GameCore::initSystems()
{
    _systemPosition = new iSystemPosition();
    _systemSceneTransformation = new iSystemSceneTransformation(_systemPosition, _scene);
}

void GameCore::deinitSystems()
{
    delete _systemPosition;
    _systemPosition = nullptr;

    delete _systemSceneTransformation;
    _systemSceneTransformation = nullptr;
}

void GameCore::initPlayer()
{
    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    iNodeModel* playerModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    playerModel->setModel("crate.ompf");

    transformNode->insertNode(playerModel);
    _scene->getRoot()->insertNode(transformNode);

    _playerID = iEntityManager::getInstance().createEntity();
    _systemPosition->registerEntity(_playerID);
    _systemSceneTransformation->registerEntity(_playerID);
    _systemSceneTransformation->setTransformID(_playerID, transformNode->getID());

    _systemPosition->setPosition(_playerID, iaVector3f(0, 0, 0));
}

void GameCore::deinitPlayer()
{
    // get's unregistered from systems implicitly
    iEntityManager::getInstance().destroyEntity(_playerID);
}

void GameCore::initPlayerRepresentation()
{

}

void GameCore::deinitPlayerRepresentation()
{

}

void GameCore::deinit()
{
    deinitPlayer();
    deinitSystems();

    _parentWindow->removeView(&_view);
    _view.setScene(nullptr);

    iSceneFactory::getInstance().destroyScene(_scene);
    iApplication::getInstance().unregisterApplicationHandleDelegate(iApplicationHandleDelegate(this, &GameCore::onHandle));

    _isRunning = false;
}

void GameCore::onHandle()
{
    _scene->handle();
}

bool GameCore::isRunning()
{
    return _isRunning;
}