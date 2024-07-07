// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include "ExampleCharacterController.h"

ExampleCharacterController::ExampleCharacterController(iWindowPtr window)
    : ExampleBase(window, "Character Controller", true, "example_texture_skybox_stars")
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
    crate->setModel("example_model_cube");
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
    // terrainEntity->registerContactDelegate(iContactDelegate(this, &EntityManager::onContact));
    // terrainEntity->setElasticity(0.0);
    // terrainEntity->setFriction(0.5, 0.4);

    iPhysicsMaterialCombo *terrainBullet = new iPhysicsMaterialCombo(materialTerrain, materialBullet);
    terrainBullet->setName("terrain-bullet");
    // terrainBullet->registerContactDelegate(iContactDelegate(this, &EntityManager::onContactTerrainBullet));

    iPhysicsMaterialCombo *bulletEntity = new iPhysicsMaterialCombo(materialBullet, materialEntity);
    bulletEntity->setName("bullet-entity");
    // bulletEntity->registerContactDelegate(iContactDelegate(this, &EntityManager::onContact));

    iPhysicsMaterialCombo *entityEntity = new iPhysicsMaterialCombo(materialEntity, materialEntity);
    entityEntity->setName("entity-entity");
    // entityEntity->registerContactDelegate(iContactDelegate(this, &EntityManager::onContact));

    iPhysicsMaterialCombo *bulletBullet = new iPhysicsMaterialCombo(materialBullet, materialBullet);
    bulletBullet->setName("bullet-bullet");
    // bulletBullet->registerContactDelegate(iContactDelegate(this, &EntityManager::onContact));

    // load level
    iModelPtr level = iResourceManager::getInstance().requestResource<iModel>("model_terrain");

    // put it in scene
    iNodeModel *levelModel = iNodeManager::getInstance().createNode<iNodeModel>();
    levelModel->setModel(level);

    levelModel->registerModelReadyDelegate(iModelReadyDelegate(this, &ExampleCharacterController::onModelReady));
    iNodeTransform *levelTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    levelTransform->insertNode(levelModel);
    getScene()->getRoot()->insertNode(levelTransform);

    // create some extra moving boxes
    createBox(iaVector3d(105, 220, 90));
    createBox(iaVector3d(103, 220, 95));
    createBox(iaVector3d(102, 220, 85));
    createBox(iaVector3d(95, 220, 99));

    // setup character and attach camera to it
    iaMatrixd startMatrix;
    startMatrix.translate(100, 200, 100);
    _characterController = new CharacterController(getScene()->getRoot(), _entityMaterialID, startMatrix);

    // setup camera
    iNodeCamera *camera = iNodeManager::getInstance().createNode<iNodeCamera>();
    _characterController->getHeadTransform()->insertNode(camera);
    // and finally we tell the view which camera shall be the current one. for this to work a camera must be part of a
    // scene associated with the view which we achieved by adding all those nodes on to an other starting with the root node
    getView().setCamera(camera->getID());
    getView().setPerspective(60.0f);

    // setup gun
    /*iNodeTransform *gunTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    gunTransform->translate(0, 0, -0.4);
    gunTransform->rotate(M_PI, iaAxis::Y);
    gunTransform->scale(0.04, 0.04, 0.04);
    iNodeModel *gun = iNodeManager::getInstance().createNode<iNodeModel>();
    gun->setModel("example_model_M4A1-S");
    _characterController->getRightShoulderTransform()->insertNode(gunTransform);
    gunTransform->insertNode(gun);*/

    // setup light
    iNodeTransform *directionalLightTranslate = iNodeManager::getInstance().createNode<iNodeTransform>();
    directionalLightTranslate->translate(100, 100, 100);
    // the light node
    iNodeLight *lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
    lightNode->setAmbient(iaColor3f(0.5f, 0.5f, 0.5f));
    lightNode->setDiffuse(iaColor3f(0.9f, 0.9f, 0.9f));
    lightNode->setSpecular(iaColor3f(1.0f, 1.0f, 1.0f));
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

void ExampleCharacterController::onUpdate()
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

void ExampleCharacterController::onApplyForceAndTorqueBox(iPhysicsBody *body, float64 timestep)
{
    float64 Ixx;
    float64 Iyy;
    float64 Izz;
    float64 mass;
    iaVector3d force;

    iPhysics::getInstance().getMassMatrix(body->getNewtonBody(), mass, Ixx, Iyy, Izz);
    force.set(0.0f, -mass * static_cast<float32>(IGOR_EARTH_GRAVITY), 0.0f);

    body->setForce(force);
}

void ExampleCharacterController::onDeinit()
{
    if (_characterController != nullptr)
    {
        delete _characterController;
        _characterController = nullptr;
    }

    _materialSkyBox = nullptr;
}

void ExampleCharacterController::onRenderOrtho()
{
    iaMatrixd identity;
    iRenderer::getInstance().setViewMatrix(identity);

    iaMatrixd modelMatrix;
    modelMatrix.translate(0, 0, -1);
    iRenderer::getInstance().setModelMatrix(modelMatrix);

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
        iMouse::getInstance().hideCursor(_captureMouse);
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