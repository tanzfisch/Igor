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

#include <iPhysics.h>
#include <iPhysicsBody.h>

#include <iRenderer.h>
#include <iRenderStateSet.h>
#include <iMaterial.h>
#include <iMaterialGroup.h>

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

    initPlayer();

    _isRunning = true;
}

void GameCore::onApplyForceAndTorquePlayer(iPhysicsBody* body, float32 timestep, int threadIndex)
{
    iaVector3f force;
    float32 Ixx;
    float32 Iyy;
    float32 Izz;
    float32 mass;

    //iPhysics::getInstance().getMassMatrixFromBody(static_cast<void*>(body->getNewtonBody()), mass, Ixx, Iyy, Izz);
    //force.set(0.0f, -mass * static_cast<float32>(__IGOR_GRAVITY__), 0.0f);
    force = body->getForce();

/*    iaVector3<float32> velocity;
    iPhysics::getInstance().getVelocity(body->getNewtonBody(), velocity);
    velocity.negate();
    force += (velocity / (1.0 / iPhysics::getSimulationRate())) * 0.5;*/

    iPhysics::getInstance().setForce(static_cast<void*>(body->getNewtonBody()), force);
}

void GameCore::initPlayer()
{
    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));

    iNodeModel* playerModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    playerModel->setModel("crate.ompf");

    transformNode->insertNode(playerModel);
    _scene->getRoot()->insertNode(transformNode);

    iaMatrixf offset;
    iPhysicsCollision* collisionBox = iPhysics::getInstance().createBox(1, 1, 1, offset);
    iPhysicsBody* body = iPhysics::getInstance().createBody(collisionBox);
    iPhysics::getInstance().destroyCollision(collisionBox);
    body->registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &GameCore::onApplyForceAndTorquePlayer));
    iPhysics::getInstance().bindTransformNode(body, transformNode);

    // TODO
}

void GameCore::deinitPlayer()
{
    // TODO
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

void GameCore::onKeyPressed(iKeyCode key)
{
    switch (key)
    {
    case iKeyCode::Left:
        break;

    case iKeyCode::Right:
        break;

    case iKeyCode::Up:
        break;

    case iKeyCode::Down:
        break;
    }
}

void GameCore::onKeyReleased(iKeyCode key)
{
    switch (key)
    {
    case iKeyCode::Left:
        break;

    case iKeyCode::Right:
        break;

    case iKeyCode::Up:
        break;

    case iKeyCode::Down:
        break;
    }
}