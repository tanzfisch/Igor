#include "PhysicsExample.h"

#include <iaConsole.h>
#include <iaRandomNumberGenerator.h>
using namespace IgorAux;

#include <iMaterial.h>
#include <iNodeVisitorPrintTree.h>
#include <iTaskManager.h>
#include <iNodeSkyBox.h>
#include <iNodeCamera.h>
#include <iNodeModel.h> 
#include <iNodeTransform.h>
#include <iRenderer.h>
#include <iApplication.h>
#include <iSceneFactory.h>
#include <iScene.h>
#include <iNodeManager.h>
#include <iMouse.h>
#include <iTimer.h>
#include <iTextureFont.h>
#include <iNodeLight.h>
#include <iNodePhysics.h>
#include <iModelResourceFactory.h>
#include <iPhysics.h>
#include <iPhysicsJoint.h>
#include <iTaskFlushModels.h>
#include <iTaskFlushTextures.h>
#include <iMaterialResourceFactory.h>
#include <iProfiler.h>
using namespace Igor;

PhysicsExample::PhysicsExample()
{
    init();
}

PhysicsExample::~PhysicsExample()
{
    deinit();
}

void PhysicsExample::init()
{
    // register keyboard and mouse events
    iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownDelegate(this, &PhysicsExample::onKeyPressed));
    iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &PhysicsExample::onMouseMoved));
    iMouse::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &PhysicsExample::onMouseWheel));
    iApplication::getInstance().registerApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &PhysicsExample::onHandle));

    // setup view for scene
    _view.setClearColor(iaColor4f(0.5f, 0, 0.5f, 1));
    _view.setPerspective(45);
    _view.setClipPlanes(0.1f, 10000.f);

    // setup view for statistics
    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setOrthogonal(0, 1024, 768, 0);
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &PhysicsExample::onRenderOrtho));

    // init window
    _window.setTitle("Physics Example");
    _window.addView(&_view);
    _window.addView(&_viewOrtho);
    _window.setClientSize(1024, 768);
	_window.setCentered();
    _window.open();
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &PhysicsExample::onWindowClosed));

    // setting up the scene
    _scene = iSceneFactory::getInstance().createScene();
    _view.setScene(_scene);
    
    // set physics simulation rate to 60Hz
    iPhysics::getInstance().setSimulationRate(60);

    // create some collision boxes and combine them in one to represent the floor
    iaMatrixd offsetFloor;
    std::vector<iPhysicsCollision*> collisions;
    collisions.push_back(iPhysics::getInstance().createBox(10, 1, 50, offsetFloor.getData()));
    collisions.push_back(iPhysics::getInstance().createBox(50, 1, 10, offsetFloor.getData()));
    offsetFloor.translate(0, -5, 0);
    collisions.push_back(iPhysics::getInstance().createBox(50, 1, 50, offsetFloor.getData()));
    iPhysicsCollision* floorCollision = iPhysics::getInstance().createCompound(collisions);
    // make a body from the floor collision
    iPhysicsBody* floorBody = iPhysics::getInstance().createBody(floorCollision);
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
    iPhysicsCollision* boxCollision = iPhysics::getInstance().createBox(1, 1, 1, offsetBox.getData());

    // now create boxes in various patterns

    // some random positioned boxes
    // for that we need a random number generator
    iaRandomNumberGeneratoru rand;
    rand.setSeed(1337);

    for (int i = 0; i < 30; ++i)
    {
        // create the box body and giv him mass
        iPhysicsBody* boxBody = iPhysics::getInstance().createBody(boxCollision);
        boxBody->setMass(100);
        // register force ans torque callback
        boxBody->registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &PhysicsExample::onApplyForceAndTorque));
        // store body id
        _bodyIDs.push_back(boxBody->getID());

        // generate random position and orientation
        iNodeTransform* transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();
        transformNode->translate((rand.getNext() % 10) - 5.0f, (rand.getNext() % 10) + 20.0f, (rand.getNext() % 10) - 5.0f);
        transformNode->rotate(rand.getNext(), iaAxis::X);
        transformNode->rotate(rand.getNext(), iaAxis::Y);
        transformNode->rotate(rand.getNext(), iaAxis::Z);
        // load the crate model
        iNodeModel* crate = iNodeManager::getInstance().createNode<iNodeModel>();
        crate->setModel("crate.ompf");
        transformNode->insertNode(crate);
        // bind the scene model to the physics body
        iPhysics::getInstance().bindTransformNode(boxBody, transformNode);
        // add the scene model to the scene
        _scene->getRoot()->insertNode(transformNode);
    }

    // and some more boxes
    for (int z = -24; z < 25; z += 4)
    {
        for (int x = -24; x < 25; x += 4)
        {
            for (int i = 0; i < 5; ++i)
            {
                // same as above just using a different interface here
                iNodePhysics* nodePhysics = iNodeManager::getInstance().createNode<iNodePhysics>();
                nodePhysics->addBox(1, 1, 1, offsetBox);
                nodePhysics->finalizeCollision();
                nodePhysics->setMass(100);
                nodePhysics->setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &PhysicsExample::onApplyForceAndTorque));

                iNodeTransform* transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();
                transformNode->translate(x, i, z);

                iNodeModel* cube = iNodeManager::getInstance().createNode<iNodeModel>();
                cube->setModel("crate.ompf");

                // binds physics node and transform node implicitly
                transformNode->insertNode(cube);
                transformNode->insertNode(nodePhysics);
                _scene->getRoot()->insertNode(transformNode);
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
    _cameraHeading->setName("_cameraHeading");
    _cameraHeading->rotate(0.4, iaAxis::Y);
    _cameraPitch = iNodeManager::getInstance().createNode<iNodeTransform>();
    _cameraPitch->setName("_cameraPitch");
    _cameraPitch->rotate(-0.4, iaAxis::X);
    _cameraTranslation = iNodeManager::getInstance().createNode<iNodeTransform>();
    _cameraTranslation->setName("_cameraTranslation");
    _cameraTranslation->translate(0, 0, 80);
    iNodeCamera* camera = iNodeManager::getInstance().createNode<iNodeCamera>();
    camera->setName("camera");

    _scene->getRoot()->insertNode(_cameraHeading);
    _cameraHeading->insertNode(_cameraPitch);
    _cameraPitch->insertNode(_cameraTranslation);
    _cameraTranslation->insertNode(camera);

    // and finally we tell the view which camera shall be the current one. for this to work a camera must be part of a 
    // scene assiciated with the view wich we achived by adding all those nodes on to an other starting with the root node
    _view.setCurrentCamera(camera->getID());

    // default sky box
    _materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setName("SkyBox");
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setOrder(10);

    iNodeSkyBox* skyBoxNode = iNodeManager::getInstance().createNode<iNodeSkyBox>();
    skyBoxNode->setTextures(
        iTextureResourceFactory::getInstance().requestFile("skybox_default/front.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/back.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/left.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/right.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/top.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/bottom.png"));
    skyBoxNode->setMaterial(_materialSkyBox);
    _scene->getRoot()->insertNode(skyBoxNode);

    // load font for statistics
    _font = new iTextureFont("StandardFont.png");

    // prepare igor logo
    _igorLogo = iTextureResourceFactory::getInstance().loadFile("special/splash.png", iResourceCacheMode::Free, iTextureBuildMode::Normal);
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::Blend, iRenderStateValue::On);

    // init light
    iNodeTransform* directionalLightTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    directionalLightTransform->translate(100, 100, 100);

    iNodeLight* lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
    lightNode->setAmbient(iaColor4f(0.3f, 0.3f, 0.3f, 1.0f));
    lightNode->setDiffuse(iaColor4f(0.8f, 0.8f, 0.8f, 1.0f));
    lightNode->setSpecular(iaColor4f(1.0f, 01.0f, 1.0f, 1.0f));
    directionalLightTransform->insertNode(lightNode);
    _scene->getRoot()->insertNode(directionalLightTransform);

    // launch resource handlers
    _flushModelsTask = iTaskManager::getInstance().addTask(new iTaskFlushModels(&_window));
    _flushTexturesTask = iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));

    _profilerVisualizer.setVerbosity(iProfilerVerbosity::FPSAndMetrics);
}

void PhysicsExample::deinit()
{
    iMouse::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &PhysicsExample::onMouseWheel));
    iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &PhysicsExample::onMouseMoved));
    iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownDelegate(this, &PhysicsExample::onKeyPressed));
    _viewOrtho.unregisterRenderDelegate(RenderDelegate(this, &PhysicsExample::onRenderOrtho));
    iApplication::getInstance().unregisterApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &PhysicsExample::onHandle));

    // free some resources
    _igorLogo = nullptr;

    if (_font)
    {
        delete _font;
        _font = nullptr;
    }

    iSceneFactory::getInstance().destroyScene(_scene);

    for (auto bodyID : _bodyIDs)
    {
        iPhysics::getInstance().destroyBody(bodyID);
    }

    _bodyIDs.clear();

    iTaskManager::getInstance().abortTask(_flushModelsTask);
    iTaskManager::getInstance().abortTask(_flushTexturesTask);

    if (_window.isOpen())
    {
        _window.close();
        _window.removeView(&_view);
        _window.removeView(&_viewOrtho);
    }
}

void PhysicsExample::onApplyForceAndTorque(iPhysicsBody* body, float32 timestep)
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

void PhysicsExample::onMouseWheel(int32 d)
{
    if (d < 0)
    {
        _cameraTranslation->translate(0, 0, 10);
    }
    else
    {
        _cameraTranslation->translate(0, 0, -10);
    }
}

void PhysicsExample::onMouseMoved(const iaVector2i& from, const iaVector2i& to, iWindow* window)
{
    if (iMouse::getInstance().getLeftButton())
    {
        float32 heading = static_cast<float32>(from._x - to._x) * 0.001f;
        float32 pitch = static_cast<float32>(from._y - to._y) * 0.001f;

        iaMatrixd matrix;
        _cameraPitch->getMatrix(matrix);
        matrix.rotate(pitch, iaAxis::X);
        _cameraPitch->setMatrix(matrix);

        _cameraHeading->getMatrix(matrix);
        matrix.rotate(heading, iaAxis::Y);
        _cameraHeading->setMatrix(matrix);

        iMouse::getInstance().setCenter();
    }
}

void PhysicsExample::onWindowClosed()
{
    iApplication::getInstance().stop();
}

void PhysicsExample::onKeyPressed(iKeyCode key)
{
    switch (key)
    {
    case iKeyCode::ESC:
        iApplication::getInstance().stop();
        break;

	case iKeyCode::F8:
		_profilerVisualizer.cycleVerbosity();
		break;

	case iKeyCode::F9:
	{
		iNodeVisitorPrintTree printTree;
		if (_scene != nullptr)
		{
			printTree.printToConsole(_scene->getRoot());
		}
	}
	break;

	case iKeyCode::F10:
		_view.setWireframeVisible(!_view.isWireframeVisible());
		break;

	case iKeyCode::F11:
		_view.setOctreeVisible(!_view.isOctreeVisible());
		break;

	case iKeyCode::F12:
		_view.setBoundingBoxVisible(!_view.isBoundingBoxVisible());
		break;

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
        break;
    }
}

void PhysicsExample::onHandle()
{
    _scene->handle();
}

void PhysicsExample::onRenderOrtho()
{
    iaMatrixd viewMatrix;
    iRenderer::getInstance().setViewMatrix(viewMatrix);

    iaMatrixd modelMatrix;
    modelMatrix.translate(0, 0, -30);
    iRenderer::getInstance().setModelMatrix(modelMatrix);

    drawLogo();

    // draw frame rate in lower right corner
    _profilerVisualizer.draw(&_window, _font, iaColor4f(0, 1, 0, 1));
}

void PhysicsExample::drawLogo()
{
    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

    float32 width = static_cast<float32>(_igorLogo->getWidth());
    float32 height = static_cast<float32>(_igorLogo->getHeight());
    float32 x = static_cast<float32>(_window.getClientWidth()) - width;
    float32 y = static_cast<float32>(_window.getClientHeight()) - height;

    iRenderer::getInstance().drawTexture(x, y, width, height, _igorLogo);
}

void PhysicsExample::run()
{
    iApplication::getInstance().run();
}