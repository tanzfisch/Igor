// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "ExampleCharacterController.h"

#include <iaux/system/iaConsole.h>
#include <iaux/data/iaString.h>
using namespace iaux;

#include <igor/resources/material/iMaterial.h>
#include <igor/scene/traversal/iNodeVisitorPrintTree.h>
#include <igor/threading/iTaskManager.h>
#include <igor/scene/nodes/iNodeSkyBox.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/graphics/iRenderer.h>
#include <igor/system/iApplication.h>
#include <igor/scene/iSceneFactory.h>
#include <igor/scene/iScene.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/system/iMouse.h>
#include <igor/system/iKeyboard.h>
#include <igor/system/iTimer.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/scene/nodes/iNodeLight.h>
#include <igor/resources/model/iModelResourceFactory.h>
#include <igor/threading/tasks/iTaskFlushModels.h>
#include <igor/threading/tasks/iTaskFlushTextures.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/scene/nodes/iNodeSwitch.h>
#include <igor/scene/nodes/iNodeLODSwitch.h>
#include <igor/scene/nodes/iNodeLODTrigger.h>
#include <igor/physics/iPhysics.h>
#include <igor/physics/iPhysicsCollision.h>
#include <igor/physics/iPhysicsBody.h>
#include <igor/physics/iPhysicsJoint.h>
#include <igor/physics/iPhysicsMaterial.h>
#include <igor/physics/iPhysicsMaterialCombo.h>
#include <igor/scene/nodes/iNodePhysics.h>
using namespace igor;

ExampleCharacterController::ExampleCharacterController(iWindow *window)
    : ExampleBase(window, "Character Controller", true, false)
{
}

void ExampleCharacterController::createBox(const iaVector3d &pos)
{
    iNodePhysics *physicsBox = iNodeManager::getInstance().createNode<iNodePhysics>();
    physicsBox->addBox(1, 1, 1, iaMatrixd());
    physicsBox->setMass(10);
    physicsBox->setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &ExampleCharacterController::onApplyForceAndTorqueBox));
    physicsBox->setMaterial(_entityMaterialID);
    physicsBox->finalizeCollision();

    iNodeTransform *transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();
    transformNode->translate(pos);

    iNodeModel *crate = iNodeManager::getInstance().createNode<iNodeModel>();
    crate->setModel("cube.ompf");
    transformNode->insertNode(crate);
    transformNode->insertNode(physicsBox);

    getScene()->getRoot()->insertNode(transformNode);
}

void ExampleCharacterController::onInit()
{
    // setup some physics materials
    iPhysicsMaterial *materialTerrain = iPhysics::getInstance().createMaterial("terrain");
    _terrainMaterialID = materialTerrain->getID();

    iPhysicsMaterial *materialEntity = iPhysics::getInstance().createMaterial("entity");
    _entityMaterialID = materialEntity->getID();

    iPhysicsMaterial *materialBullet = iPhysics::getInstance().createMaterial("bullet");
    _bulletMaterialID = materialBullet->getID();

    // setup which physics materials interact with each other
    iPhysicsMaterialCombo *terrainEntity = new iPhysicsMaterialCombo(materialTerrain, materialEntity);
    terrainEntity->setName("terrain-entity");
    //terrainEntity->registerContactDelegate(iContactDelegate(this, &EntityManager::onContact));
    //terrainEntity->setElasticity(0.0);
    //terrainEntity->setFriction(0.5, 0.4);

    iPhysicsMaterialCombo *terrainBullet = new iPhysicsMaterialCombo(materialTerrain, materialBullet);
    terrainBullet->setName("terrain-bullet");
    //terrainBullet->registerContactDelegate(iContactDelegate(this, &EntityManager::onContactTerrainBullet));

    iPhysicsMaterialCombo *bulletEntity = new iPhysicsMaterialCombo(materialBullet, materialEntity);
    bulletEntity->setName("bullet-entity");
    //bulletEntity->registerContactDelegate(iContactDelegate(this, &EntityManager::onContact));

    iPhysicsMaterialCombo *entityEntity = new iPhysicsMaterialCombo(materialEntity, materialEntity);
    entityEntity->setName("entity-entity");
    //entityEntity->registerContactDelegate(iContactDelegate(this, &EntityManager::onContact));

    iPhysicsMaterialCombo *bulletBullet = new iPhysicsMaterialCombo(materialBullet, materialBullet);
    bulletBullet->setName("bullet-bullet");
    //bulletBullet->registerContactDelegate(iContactDelegate(this, &EntityManager::onContact));

    // setup floor model
    iNodeModel *levelModel = iNodeManager::getInstance().createNode<iNodeModel>();
    iModelDataInputParameter *param = new iModelDataInputParameter();
    param->_keepMesh = true;
    param->_modelSourceType = iModelSourceType::File;
    levelModel->setModel("level.ompf", iResourceCacheMode::Keep, param);
    levelModel->registerModelReadyDelegate(iModelReadyDelegate(this, &ExampleCharacterController::onModelReady));
    iNodeTransform *levelTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    levelTransform->insertNode(levelModel);
    getScene()->getRoot()->insertNode(levelTransform);

    // create some extra moving boxes
    createBox(iaVector3d(5, 180, -12));
    createBox(iaVector3d(3, 180, -10));
    createBox(iaVector3d(2, 180, -4));
    createBox(iaVector3d(-3, 180, -2));

    // setup character and attache camera to it
    iaMatrixd startMatrix;
    startMatrix.translate(0, 176, 0);
    _characterController = new CharacterController(getScene()->getRoot(), _entityMaterialID, startMatrix);

    // setup camera
    iNodeCamera *camera = iNodeManager::getInstance().createNode<iNodeCamera>();
    _characterController->getHeadTransform()->insertNode(camera);
    // and finally we tell the view which camera shall be the current one. for this to work a camera must be part of a
    // scene assiciated with the view wich we achived by adding all those nodes on to an other starting with the root node
    getView().setCurrentCamera(camera->getID());

    // setup gun
    iNodeTransform *gunTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    gunTransform->translate(0, 0, -0.4);
    gunTransform->rotate(M_PI, iaAxis::Y);
    gunTransform->scale(0.04, 0.04, 0.04);
    iNodeModel *gun = iNodeManager::getInstance().createNode<iNodeModel>();
    gun->setModel("M4A1-S.ompf");
    _characterController->getRightSholderTransform()->insertNode(gunTransform);
    gunTransform->insertNode(gun);

    // create a skybox
    iNodeSkyBox *skyBoxNode = iNodeManager::getInstance().createNode<iNodeSkyBox>();
    // set it up with the default skybox texture
    skyBoxNode->setTextures(
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/front.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/back.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/left.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/right.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/top.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/bottom.jpg"));
    // create a material for the sky box because the default material for all iNodeRender and deriving classes has no textures and uses depth test
    _materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial("Sky Box");
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setOrder(iMaterial::RENDER_ORDER_MIN);
    // set that material
    skyBoxNode->setMaterial(_materialSkyBox);
    // and add it to the scene
    getScene()->getRoot()->insertNode(skyBoxNode);

    // setup light
    iNodeTransform *directionalLightTranslate = iNodeManager::getInstance().createNode<iNodeTransform>();
    directionalLightTranslate->translate(100, 100, 100);
    // the light node
    iNodeLight *lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
    lightNode->setAmbient(iaColor4f(0.5f, 0.5f, 0.5f, 1.0f));
    lightNode->setDiffuse(iaColor4f(0.9f, 0.9f, 0.9f, 1.0f));
    lightNode->setSpecular(iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
    // insert light to scene
    getScene()->getRoot()->insertNode(directionalLightTranslate);
    directionalLightTranslate->insertNode(lightNode);

    // and start physics
    iPhysics::getInstance().start();
}

void ExampleCharacterController::onModelReady(uint64 modelNodeID)
{
    iNodePtr node = iNodeManager::getInstance().getNode(modelNodeID);
    makeCollisions(node);
}

void ExampleCharacterController::makeCollisions(iNodePtr node)
{
    if (node->getType() == iNodeType::iNodeMesh)
    {
        iNodeMesh *meshNode = static_cast<iNodeMesh *>(node);
        iaMatrixd matrix;
        meshNode->calcWorldTransformation(matrix);

        iNodePhysics *physicsNode = iNodeManager::getInstance().createNode<iNodePhysics>();
        physicsNode->addMesh(meshNode->getMesh(), 0, iaMatrixd());
        physicsNode->finalizeCollision();
        physicsNode->setMass(0);
        physicsNode->setMaterial(_terrainMaterialID);
        meshNode->insertNode(physicsNode);
    }

    for (auto child : node->getChildren())
    {
        makeCollisions(child);
    }
}

void ExampleCharacterController::onPreDraw()
{
    float64 movingForceOnFloor = 10000;
    float64 movingForceInAir = 6000;
    float64 jumpingForce = 100000;

    iaMatrixd matrix;
    iaVector3d resultingForce;

    iNodeTransform *transformationNode = _characterController->getHeadingTransform();
    auto state = _characterController->getState();
    transformationNode->getMatrix(matrix);

    float64 movingForce = (state == CharacterController::State::Air) ? movingForceInAir : movingForceOnFloor;

    if (_inputFlags._forward)
    {
        iaVector3d foreward = matrix._depth;
        foreward.negate();
        foreward.normalize();
        foreward *= movingForce;
        resultingForce += foreward;
    }

    if (_inputFlags._backward)
    {
        iaVector3d backward = matrix._depth;
        backward.normalize();
        backward *= movingForce;
        resultingForce += backward;
    }

    if (_inputFlags._left)
    {
        iaVector3d left = matrix._right;
        left.negate();
        left.normalize();
        left *= movingForce;
        resultingForce += left;
    }

    if (_inputFlags._right)
    {
        iaVector3d right = matrix._right;
        right.normalize();
        right *= movingForce;
        resultingForce += right;
    }

    /*if (_inputFlags._up)
    {
        iaVector3d up = matrix._top;
        up.normalize();
        up *= movingForce;
        resultingForce += up;
    }*/

    if (_inputFlags._down)
    {
        iaVector3d down = matrix._top;
        down.negate();
        down.normalize();
        down *= movingForce;
        resultingForce += down;
    }

    // cap the horizontal movement force
    if (resultingForce.length() > movingForce)
    {
        resultingForce.normalize();
        resultingForce *= movingForce;
    }

    // jump
    if (_inputFlags._jump)
    {
        iaVector3d up = matrix._top;
        up.normalize();
        up *= jumpingForce;
        resultingForce += up;
        _inputFlags._jump = false;
    }

    _characterController->setForce(resultingForce);
}

void ExampleCharacterController::onApplyForceAndTorqueBox(iPhysicsBody *body, float32 timestep)
{
    float64 Ixx;
    float64 Iyy;
    float64 Izz;
    float64 mass;
    iaVector3d force;

    iPhysics::getInstance().getMassMatrix(body->getNewtonBody(), mass, Ixx, Iyy, Izz);
    force.set(0.0f, -mass * static_cast<float32>(__IGOR_GRAVITY__), 0.0f);

    body->setForce(force);
}

void ExampleCharacterController::onDeinit()
{
    if (_characterController != nullptr)
    {
        delete _characterController;
        _characterController = nullptr;
    }

    iMaterialResourceFactory::getInstance().destroyMaterial(_materialSkyBox);
}

void ExampleCharacterController::onRenderOrtho()
{
    iaMatrixd viewMatrix;
    iRenderer::getInstance().setViewMatrix(viewMatrix);

    iaMatrixd modelMatrix;
    modelMatrix.translate(0, 0, -30);
    iRenderer::getInstance().setModelMatrix(modelMatrix);

    iRenderer::getInstance().setColor(1, 1, 1, 1);

    iRenderer::getInstance().setMaterial(getFontMaterial());
    iRenderer::getInstance().setFont(getFont());
    iRenderer::getInstance().setFontSize(15.0f);

    iaString statusString;

    switch (_characterController->getState())
    {
    case CharacterController::State::Air:
        statusString = "Air";
        break;
    case CharacterController::State::Floor:
        statusString = "Floor";
        break;
    case CharacterController::State::Jumped:
        statusString = "Jumped";
        break;
    }

    iRenderer::getInstance().drawString(10, static_cast<float32>(getWindow()->getClientHeight() - 10), statusString, iHorizontalAlignment::Left, iVerticalAlignment::Bottom);

    ExampleBase::onRenderOrtho();
}

void ExampleCharacterController::onEvent(iEvent &event)
{
    // first call example base
    ExampleBase::onEvent(event);

    event.dispatch<iEventMouseKeyDown>(IGOR_BIND_EVENT_FUNCTION(ExampleCharacterController::onMouseKeyDownEvent));
    event.dispatch<iEventMouseKeyUp>(IGOR_BIND_EVENT_FUNCTION(ExampleCharacterController::onMouseKeyUpEvent));
    event.dispatch<iEventMouseMove>(IGOR_BIND_EVENT_FUNCTION(ExampleCharacterController::onMouseMoveEvent));
    event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(ExampleCharacterController::onKeyDown));
    event.dispatch<iEventKeyUp>(IGOR_BIND_EVENT_FUNCTION(ExampleCharacterController::onKeyUp));
}

bool ExampleCharacterController::onMouseKeyDownEvent(iEventMouseKeyDown &event)
{
    switch (event.getKey())
    {
    case iKeyCode::MouseLeft:
        _inputFlags._shootPrimary = true;
        return true;
    }

    return false;
}

bool ExampleCharacterController::onMouseKeyUpEvent(iEventMouseKeyUp &event)
{
    switch (event.getKey())
    {
    case iKeyCode::MouseLeft:
        _inputFlags._shootPrimary = false;
        return true;
    }

    return false;
}

bool ExampleCharacterController::onMouseMoveEvent(iEventMouseMove &event)
{
    if (_captureMouse)
    {
        const auto from = event.getLastPosition();
        const auto to = event.getPosition();

        iNodeTransform *cameraPitch = _characterController->getPitchTransform();
        iNodeTransform *cameraHeading = _characterController->getHeadingTransform();

        if (cameraPitch != nullptr &&
            cameraHeading != nullptr)
        {
            cameraPitch->rotate((from._y - to._y) * 0.00025f, iaAxis::X);
            cameraHeading->rotate((from._x - to._x) * 0.00025f, iaAxis::Y);

            iMouse::getInstance().setCenter();
        }

        return true;
    }

    return false;
}

bool ExampleCharacterController::onKeyDown(iEventKeyDown &event)
{
    switch (event.getKey())
    {
    case iKeyCode::Alt:
        _captureMouse = !_captureMouse;
        iMouse::getInstance().showCursor(!_captureMouse);
        return true;

    case iKeyCode::A:
        _inputFlags._left = true;
        return true;

    case iKeyCode::D:
        _inputFlags._right = true;
        return true;

    case iKeyCode::W:
        _inputFlags._forward = true;
        return true;

    case iKeyCode::S:
        _inputFlags._backward = true;
        return true;

    case iKeyCode::Space:
        _inputFlags._jump = true;
        _inputFlags._up = true;
        return true;

    case iKeyCode::LControl:
        _inputFlags._down = true;
        _inputFlags._crouch = true;
        return true;
    }

    return false;
}

bool ExampleCharacterController::onKeyUp(iEventKeyUp &event)
{
    switch (event.getKey())
    {
    case iKeyCode::A:
        _inputFlags._left = false;
        return true;

    case iKeyCode::D:
        _inputFlags._right = false;
        return true;

    case iKeyCode::W:
        _inputFlags._forward = false;
        return true;

    case iKeyCode::S:
        _inputFlags._backward = false;
        return true;

    case iKeyCode::Space:
        _inputFlags._jump = false;
        _inputFlags._up = false;
        return true;

    case iKeyCode::LControl:
        _inputFlags._down = false;
        _inputFlags._crouch = false;
        return true;
    }

    return false;
}