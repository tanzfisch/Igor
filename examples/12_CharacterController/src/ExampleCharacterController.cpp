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
#include <igor/resources/profiler/iProfiler.h>
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

ExampleCharacterController::ExampleCharacterController()
{
    init();
}

ExampleCharacterController::~ExampleCharacterController()
{
    deinit();
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

    _scene->getRoot()->insertNode(transformNode);
}

void ExampleCharacterController::init()
{
    con_endl(" -- Example character Controller --");
    iApplication::getInstance().registerApplicationPreDrawHandleDelegate(iPreDrawDelegate(this, &ExampleCharacterController::onHandle));

    // setup window
    _window.setTitle("Igor - 3D Example");
    _window.setSize(1024, 768);
    //_window.setSizeByDesktop();
    //_window.setFullscreen();
    _window.setCentered();
    _window.setVSync(false);
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &ExampleCharacterController::onWindowClosed));
    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &ExampleCharacterController::onWindowResized));

    // setup perspective view
    _view.setClearColor(iaColor4f(0.5f, 0, 0.5f, 1));
    _view.setPerspective(45);
    _view.setClipPlanes(0.01f, 1000.f);
    _window.addView(&_view);

    // setup orthogonal view
    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setOrthogonal(0.0f, static_cast<float32>(_window.getClientWidth()), static_cast<float32>(_window.getClientHeight()), 0.0f);
    _viewOrtho.registerRenderDelegate(iDrawDelegate(this, &ExampleCharacterController::onRenderOrtho));
    _window.addView(&_viewOrtho);
    // and open the window
    _window.open();

    // init scene
    _scene = iSceneFactory::getInstance().createScene();
    // bind scene to perspective view
    _view.setScene(_scene);

    // setup some physics materials
    iPhysicsMaterial *materialTerrain = iPhysics::getInstance().createMaterial("terrain");
    _terrainMaterialID = materialTerrain->getID();

    iPhysicsMaterial *materialEntity = iPhysics::getInstance().createMaterial("entity");
    _entityMaterialID = materialEntity->getID();

    iPhysicsMaterial *materialBullet = iPhysics::getInstance().createMaterial("bullet");
    _bulletMaterialID = materialBullet->getID();

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
    iNodeModel *floorModel = iNodeManager::getInstance().createNode<iNodeModel>();
    iModelDataInputParameter *param = new iModelDataInputParameter();
    param->_keepMesh = true;
    param->_modelSourceType = iModelSourceType::File;
    floorModel->setModel("level.ompf", iResourceCacheMode::Keep, param);
    floorModel->registerModelReadyDelegate(iModelReadyDelegate(this, &ExampleCharacterController::onModelReady));
    iNodeTransform *floorTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    floorTransform->insertNode(floorModel);
    _scene->getRoot()->insertNode(floorTransform);

    // create some moving boxes
    createBox(iaVector3d(5, 180, -12));
    createBox(iaVector3d(3, 180, -10));
    createBox(iaVector3d(2, 180, -4));
    createBox(iaVector3d(-3, 180, -2));

    // setup character and attache camera to it
    iaMatrixd startMatrix;
    startMatrix.translate(0, 176, 0);
    _characterController = new CharacterController(_scene->getRoot(), _entityMaterialID, startMatrix);

    // setup camera
    iNodeCamera *camera = iNodeManager::getInstance().createNode<iNodeCamera>();
    _characterController->getHeadTransform()->insertNode(camera);
    // and finally we tell the view which camera shall be the current one. for this to work a camera must be part of a
    // scene assiciated with the view wich we achived by adding all those nodes on to an other starting with the root node
    _view.setCurrentCamera(camera->getID());

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
    _materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setOrder(iMaterial::RENDER_ORDER_MIN);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setName("SkyBox");
    // set that material
    skyBoxNode->setMaterial(_materialSkyBox);
    // and add it to the scene
    _scene->getRoot()->insertNode(skyBoxNode);

    // setup light
    iNodeTransform *directionalLightTranslate = iNodeManager::getInstance().createNode<iNodeTransform>();
    directionalLightTranslate->translate(100, 100, 100);
    // the light node
    iNodeLight *lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
    lightNode->setAmbient(iaColor4f(0.5f, 0.5f, 0.5f, 1.0f));
    lightNode->setDiffuse(iaColor4f(0.9f, 0.9f, 0.9f, 1.0f));
    lightNode->setSpecular(iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
    // insert light to scene
    _scene->getRoot()->insertNode(directionalLightTranslate);
    directionalLightTranslate->insertNode(lightNode);

    // init font for render statistics
    _font = new iTextureFont("StandardFont.png");
    _profilerVisualizer.setVerbosity(iProfilerVerbosity::FPSAndMetrics);

    // prepare igor logo
    _igorLogo = iTextureResourceFactory::getInstance().loadFile("special/splash.png", iResourceCacheMode::Free, iTextureBuildMode::Normal);
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::Blend, iRenderStateValue::On);

    // start resource tasks
    _taskFlushModels = iTaskManager::getInstance().addTask(new iTaskFlushModels(&_window));
    _taskFlushTextures = iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));

    // register some callbacks
    iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownDelegate(this, &ExampleCharacterController::onKeyDown));
    iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &ExampleCharacterController::onKeyReleased));
    iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &ExampleCharacterController::onMouseMoved));
    iMouse::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &ExampleCharacterController::onMouseWheel));
    iMouse::getInstance().registerMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &ExampleCharacterController::onMouseKeyDown));
    iMouse::getInstance().registerMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &ExampleCharacterController::onMouseKeyUp));

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

void ExampleCharacterController::onKeyDown(iKeyCode key)
{
    switch (key)
    {
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

    case iKeyCode::Alt:
        _captureMouse = !_captureMouse;
        iMouse::getInstance().showCursor(!_captureMouse);
        break;

    case iKeyCode::A:
        _inputFlags._left = true;
        break;

    case iKeyCode::D:
        _inputFlags._right = true;
        break;

    case iKeyCode::W:
        _inputFlags._forward = true;
        break;

    case iKeyCode::S:
        _inputFlags._backward = true;
        break;

    case iKeyCode::Space:
        _inputFlags._jump = true;
        _inputFlags._up = true;
        break;

    case iKeyCode::LControl:
        _inputFlags._down = true;
        _inputFlags._crouch = true;
        break;

    case iKeyCode::ESC:
        iApplication::getInstance().stop();
        break;
    }
}

void ExampleCharacterController::onKeyReleased(iKeyCode key)
{
    switch (key)
    {
    case iKeyCode::A:
        _inputFlags._left = false;
        break;

    case iKeyCode::D:
        _inputFlags._right = false;
        break;

    case iKeyCode::W:
        _inputFlags._forward = false;
        break;

    case iKeyCode::S:
        _inputFlags._backward = false;
        break;

    case iKeyCode::Space:
        _inputFlags._jump = false;
        _inputFlags._up = false;
        break;

    case iKeyCode::LControl:
        _inputFlags._down = false;
        _inputFlags._crouch = false;
        break;
    }
}

void ExampleCharacterController::onHandle()
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

void ExampleCharacterController::deinit()
{
    if (_characterController != nullptr)
    {
        delete _characterController;
    }

    // unregister some callbacks
    iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownDelegate(this, &ExampleCharacterController::onKeyDown));
    iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &ExampleCharacterController::onKeyReleased));
    iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &ExampleCharacterController::onMouseMoved));
    iMouse::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &ExampleCharacterController::onMouseWheel));
    iMouse::getInstance().unregisterMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &ExampleCharacterController::onMouseKeyDown));
    iMouse::getInstance().unregisterMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &ExampleCharacterController::onMouseKeyUp));

    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &ExampleCharacterController::onWindowClosed));
    _window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &ExampleCharacterController::onWindowResized));
    _viewOrtho.unregisterRenderDelegate(iDrawDelegate(this, &ExampleCharacterController::onRenderOrtho));
    iApplication::getInstance().unregisterApplicationPreDrawHandleDelegate(iPreDrawDelegate(this, &ExampleCharacterController::onHandle));

    // deinit statistics
    if (_font != nullptr)
    {
        delete _font;
        _font = nullptr;
    }

    iSceneFactory::getInstance().destroyScene(_scene);
    _scene = nullptr;

    // abort resource tasks
    iTaskManager::getInstance().abortTask(_taskFlushModels);
    iTaskManager::getInstance().abortTask(_taskFlushTextures);

    iMaterialResourceFactory::getInstance().destroyMaterial(_materialSkyBox);

    if (_window.isOpen())
    {
        _window.close();
        _window.removeView(&_view);
        _window.removeView(&_viewOrtho);
    }
}

void ExampleCharacterController::onMouseKeyUp(iKeyCode keyCode)
{
    switch (keyCode)
    {
    case iKeyCode::MouseLeft:
        _inputFlags._shootPrimary = false;
        break;
    }
}

void ExampleCharacterController::onMouseKeyDown(iKeyCode keyCode)
{
    switch (keyCode)
    {
    case iKeyCode::MouseLeft:
        _inputFlags._shootPrimary = true;
        break;
    }
}

void ExampleCharacterController::onMouseWheel(int32 d)
{
}

void ExampleCharacterController::onMouseMoved(const iaVector2i &from, const iaVector2i &to, iWindow *_window)
{
    if (_captureMouse)
    {
        iNodeTransform *cameraPitch = _characterController->getPitchTransform();
        iNodeTransform *cameraHeading = _characterController->getHeadingTransform();

        if (cameraPitch != nullptr &&
            cameraHeading != nullptr)
        {
            cameraPitch->rotate((from._y - to._y) * 0.001f, iaAxis::X);
            cameraHeading->rotate((from._x - to._x) * 0.001f, iaAxis::Y);

            iMouse::getInstance().setCenter();
        }
    }
}

void ExampleCharacterController::onWindowClosed()
{
    iApplication::getInstance().stop();
}

void ExampleCharacterController::onWindowResized(int32 clientWidth, int32 clientHeight)
{
    _viewOrtho.setOrthogonal(0.0f, static_cast<float32>(clientWidth), static_cast<float32>(clientHeight), 0.0f);
}

void ExampleCharacterController::onRenderOrtho()
{
    iaMatrixd viewMatrix;
    iRenderer::getInstance().setViewMatrix(viewMatrix);

    iaMatrixd modelMatrix;
    modelMatrix.translate(0, 0, -30);
    iRenderer::getInstance().setModelMatrix(modelMatrix);

    drawLogo();

    // draw frame rate in lower right corner
    _profilerVisualizer.draw(&_window, _font, iaColor4f(0, 1, 0, 1));

    iRenderer::getInstance().setColor(1, 1, 1, 1);

    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setFont(_font);
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

    iRenderer::getInstance().drawString(10, static_cast<float32>(_window.getClientHeight() - 10), statusString, iHorizontalAlignment::Left, iVerticalAlignment::Bottom);
}

void ExampleCharacterController::drawLogo()
{
    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

    float32 width = static_cast<float32>(_igorLogo->getWidth());
    float32 height = static_cast<float32>(_igorLogo->getHeight());
    float32 x = static_cast<float32>(_window.getClientWidth()) - width;
    float32 y = static_cast<float32>(_window.getClientHeight()) - height;

    iRenderer::getInstance().drawTexture(x, y, width, height, _igorLogo);
}

void ExampleCharacterController::run()
{
    iApplication::getInstance().run();
}
