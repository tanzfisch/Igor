// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "PhysicsExample.h"

PhysicsExample::PhysicsExample(iWindow *window)
    : ExampleBase(window, "Physics")
{
}

void PhysicsExample::onInit()
{
    // set physics simulation rate to 60Hz
    iPhysics::getInstance().setSimulationRate(60);

    // create some collision boxes and combine them in one to represent the floor
    iaMatrixd offsetFloor;
    std::vector<iPhysicsCollision *> collisions;
    collisions.push_back(iPhysics::getInstance().createBox(10, 1, 50, offsetFloor.getData()));
    collisions.push_back(iPhysics::getInstance().createBox(50, 1, 10, offsetFloor.getData()));
    offsetFloor.translate(0, -5, 0);
    collisions.push_back(iPhysics::getInstance().createBox(50, 1, 50, offsetFloor.getData()));
    iPhysicsCollision *floorCollision = iPhysics::getInstance().createCompound(collisions);
    // make a body from the floor collision
    iPhysicsBody *floorBody = iPhysics::getInstance().createBody(floorCollision);
    // zero mass turns the floor in to a static body
    floorBody->setMass(0);
    // position the floor
    iaMatrixd floorMatrix;
    floorMatrix.translate(0, -1, 0);
    floorBody->setMatrix(floorMatrix);
    // and add the body id to the list of bodys for later cleanup
    _bodyIDs.push_back(floorBody->getID());

    // create a box collision used by all boxes we create
    iaMatrixd offsetBox;
    iPhysicsCollision *boxCollision = iPhysics::getInstance().createBox(1, 1, 1, offsetBox.getData());

    // now create boxes in various patterns
    // some random positioned boxes
    // for that we need a random number generator
    iaRandomNumberGeneratoru rand;
    rand.setSeed(1337);

    for (int i = 0; i < 30; ++i)
    {
        // create the box body and giv him mass
        iPhysicsBody *boxBody = iPhysics::getInstance().createBody(boxCollision);
        boxBody->setMass(1000);
        // register force ans torque callback
        boxBody->registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &PhysicsExample::onApplyForceAndTorque));
        // store body id
        _bodyIDs.push_back(boxBody->getID());

        // generate random position and orientation
        iNodeTransform *transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();
        transformNode->translate((rand.getNext() % 10) - 5.0f, (rand.getNext() % 10) + 20.0f, (rand.getNext() % 10) - 5.0f);
        transformNode->rotate(rand.getNext(), iaAxis::X);
        transformNode->rotate(rand.getNext(), iaAxis::Y);
        transformNode->rotate(rand.getNext(), iaAxis::Z);
        // load the crate model
        iNodeModel *crate = iNodeManager::getInstance().createNode<iNodeModel>();
        crate->setModel("crate.ompf");
        transformNode->insertNode(crate);
        // bind the scene model to the physics body
        iPhysics::getInstance().bindTransformNode(boxBody, transformNode);
        // add the scene model to the scene
        getScene()->getRoot()->insertNode(transformNode);
    }

    // and some more boxes
    for (int z = -24; z < 25; z += 4)
    {
        for (int x = -24; x < 25; x += 4)
        {
            for (int i = 0; i < 5; ++i)
            {
                // same as above just using a different interface here
                iNodePhysics *nodePhysics = iNodeManager::getInstance().createNode<iNodePhysics>();
                nodePhysics->addBox(1, 1, 1, offsetBox);
                nodePhysics->finalizeCollision();
                nodePhysics->setMass(100);
                nodePhysics->setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &PhysicsExample::onApplyForceAndTorque));

                iNodeTransform *transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();
                transformNode->translate(x, i, z);

                iNodeModel *cube = iNodeManager::getInstance().createNode<iNodeModel>();
                cube->setModel("crate.ompf");

                // binds physics node and transform node implicitly
                transformNode->insertNode(cube);
                transformNode->insertNode(nodePhysics);
                getScene()->getRoot()->insertNode(transformNode);
            }
        }
    }

    // no need to keep the collisions after putting them in to a body
    iPhysics::getInstance().destroyCollision(floorCollision);
    iPhysics::getInstance().destroyCollision(boxCollision);

    // also delete the collisions that are part of a compound
    for (auto collision : collisions)
    {
        iPhysics::getInstance().destroyCollision(collision);
    }

    // initialize camamera
    _cameraHeading = iNodeManager::getInstance().createNode<iNodeTransform>();
    _cameraHeading->setName("cameraHeading");
    _cameraHeading->rotate(0.4, iaAxis::Y);
    _cameraPitch = iNodeManager::getInstance().createNode<iNodeTransform>();
    _cameraPitch->setName("cameraPitch");
    _cameraPitch->rotate(-0.4, iaAxis::X);
    _cameraTranslation = iNodeManager::getInstance().createNode<iNodeTransform>();
    _cameraTranslation->setName("cameraTranslation");
    _cameraTranslation->translate(0, 0, 80);
    iNodeCamera *camera = iNodeManager::getInstance().createNode<iNodeCamera>();
    camera->setName("camera");

    getScene()->getRoot()->insertNode(_cameraHeading);
    _cameraHeading->insertNode(_cameraPitch);
    _cameraPitch->insertNode(_cameraTranslation);
    _cameraTranslation->insertNode(camera);

    // and finally we tell the view which camera shall be the current one. for this to work a camera must be part of a
    // scene assiciated with the view wich we achived by adding all those nodes on to an other starting with the root node
    getView().setCurrentCamera(camera->getID());

    // init light
    iNodeTransform *directionalLightTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    directionalLightTransform->translate(100, 100, 100);

    iNodeLight *lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
    lightNode->setAmbient(iaColor4f(0.3f, 0.3f, 0.3f, 1.0f));
    lightNode->setDiffuse(iaColor4f(0.8f, 0.8f, 0.8f, 1.0f));
    lightNode->setSpecular(iaColor4f(1.0f, 01.0f, 1.0f, 1.0f));
    directionalLightTransform->insertNode(lightNode);
    getScene()->getRoot()->insertNode(directionalLightTransform);
}

void PhysicsExample::onDeinit()
{
    for (auto bodyID : _bodyIDs)
    {
        iPhysics::getInstance().destroyBody(bodyID);
    }

    _bodyIDs.clear();
}

void PhysicsExample::onApplyForceAndTorque(iPhysicsBody *body, float32 timestep)
{
    float64 Ixx;
    float64 Iyy;
    float64 Izz;
    float64 mass;
    iaVector3d force;

    // apply gravity on this body
    iPhysics::getInstance().getMassMatrix(body->getNewtonBody(), mass, Ixx, Iyy, Izz);
    force.set(0.0f, -mass * static_cast<float32>(__IGOR_GRAVITY__), 0.0f);
    body->setForce(force);
}

void PhysicsExample::onRenderOrtho()
{
    iaMatrixd viewMatrix;
    iRenderer::getInstance().setViewMatrix(viewMatrix);

    iaMatrixd modelMatrix;
    modelMatrix.translate(0, 0, -30);
    iRenderer::getInstance().setModelMatrix(modelMatrix);

    ExampleBase::onRenderOrtho();
}

void PhysicsExample::onEvent(iEvent &event)
{
    // first call example base
    ExampleBase::onEvent(event);

    event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(PhysicsExample::onKeyDown));
    event.dispatch<iEventMouseMove>(IGOR_BIND_EVENT_FUNCTION(PhysicsExample::onMouseMoveEvent));
    event.dispatch<iEventMouseWheel>(IGOR_BIND_EVENT_FUNCTION(PhysicsExample::onMouseWheelEvent));
}

bool PhysicsExample::onKeyDown(iEventKeyDown &event)
{
    switch (event.getKey())
    {
    case iKeyCode::Space:
        _running = !_running;
        if (_running)
        {
            iPhysics::getInstance().start();
        }
        else
        {
            iPhysics::getInstance().stop();
        }
        return true;
    }

    return false;
}

bool PhysicsExample::onMouseMoveEvent(iEventMouseMove &event)
{
    if (iMouse::getInstance().getLeftButton())
    {
        const auto from = event.getLastPosition();
        const auto to = event.getPosition();

        float32 heading = static_cast<float32>(from._x - to._x) * 0.0005f;
        float32 pitch = static_cast<float32>(from._y - to._y) * 0.0005f;

        iaMatrixd matrix;
        _cameraPitch->getMatrix(matrix);
        matrix.rotate(pitch, iaAxis::X);
        _cameraPitch->setMatrix(matrix);

        _cameraHeading->getMatrix(matrix);
        matrix.rotate(heading, iaAxis::Y);
        _cameraHeading->setMatrix(matrix);

        iMouse::getInstance().setCenter();

        return true;
    }

    return false;
}

bool PhysicsExample::onMouseWheelEvent(iEventMouseWheel &event)
{
    if (event.getWheelDelta() < 0)
    {
        _cameraTranslation->translate(0, 0, 10);
    }
    else
    {
        _cameraTranslation->translate(0, 0, -10);
    }

    return true;
}
