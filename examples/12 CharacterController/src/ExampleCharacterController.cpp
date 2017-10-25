#include "ExampleCharacterController.h"

#include <iaConsole.h>
#include <iaString.h>
using namespace IgorAux;

#include <iMaterial.h>
#include <iMaterialGroup.h>
#include <iNodeVisitorPrintTree.h>
#include <iTaskManager.h>
#include <iNodeSkyBox.h>
#include <iNodeCamera.h>
#include <iNodeModel.h> 
#include <iNodeMesh.h>
#include <iNodeTransform.h>
#include <iRenderer.h>
#include <iApplication.h>
#include <iSceneFactory.h>
#include <iScene.h>
#include <iNodeFactory.h>
#include <iMouse.h>
#include <iKeyboard.h>
#include <iTimer.h>
#include <iTextureFont.h>
#include <iNodeLight.h>
#include <iModelResourceFactory.h>
#include <iTaskFlushModels.h>
#include <iTaskFlushTextures.h>
#include <iaString.h>
#include <iMaterialResourceFactory.h>
#include <iStatistics.h>
#include <iNodeSwitch.h>
#include <iNodeLODSwitch.h>
#include <iNodeLODTrigger.h>
#include <iPhysics.h>
#include <iPhysicsCollision.h>
#include <iPhysicsBody.h>
#include <iPhysicsJoint.h>
#include <iPhysicsMaterial.h>
#include <iPhysicsMaterialCombo.h>
using namespace Igor;

ExampleCharacterController::ExampleCharacterController()
{
    init();
}

ExampleCharacterController::~ExampleCharacterController()
{
    deinit();
}

void ExampleCharacterController::init()
{
    con(" -- Example character Controller --" << endl);
    iApplication::getInstance().registerApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &ExampleCharacterController::onHandle));

    // setup window
    _window.setTitle("Igor - 3D Example");
    _window.setClientSize(1024, 768);
    _window.setCentered();
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
    _viewOrtho.setOrthogonal(0, _window.getClientWidth(), _window.getClientHeight(), 0);
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &ExampleCharacterController::onRenderOrtho));
    _window.addView(&_viewOrtho);
    // and open the window
    _window.open();

    // init scene
    _scene = iSceneFactory::getInstance().createScene();
    // bind scene to perspective view
    _view.setScene(_scene);

    // setup some physics materials
    iPhysicsMaterial* materialTerrain = iPhysics::getInstance().createMaterial("terrain");
    _terrainMaterialID = materialTerrain->getID();

    iPhysicsMaterial* materialEntity = iPhysics::getInstance().createMaterial("entity");
    _entityMaterialID = materialEntity->getID();

    iPhysicsMaterial* materialBullet = iPhysics::getInstance().createMaterial("bullet");
    _bulletMaterialID = materialBullet->getID();

    iPhysicsMaterialCombo* terrainEntity = new iPhysicsMaterialCombo(materialTerrain, materialEntity);
    terrainEntity->setName("terrain-entity");
    //terrainEntity->registerContactDelegate(iContactDelegate(this, &EntityManager::onContact));
    //terrainEntity->setElasticity(0.0);
    //terrainEntity->setFriction(0.5, 0.4);

    iPhysicsMaterialCombo* terrainBullet = new iPhysicsMaterialCombo(materialTerrain, materialBullet);
    terrainBullet->setName("terrain-bullet");
    //terrainBullet->registerContactDelegate(iContactDelegate(this, &EntityManager::onContactTerrainBullet));

    iPhysicsMaterialCombo* bulletEntity = new iPhysicsMaterialCombo(materialBullet, materialEntity);
    bulletEntity->setName("bullet-entity");
    //bulletEntity->registerContactDelegate(iContactDelegate(this, &EntityManager::onContact));

    iPhysicsMaterialCombo* entityEntity = new iPhysicsMaterialCombo(materialEntity, materialEntity);
    entityEntity->setName("entity-entity");
    //entityEntity->registerContactDelegate(iContactDelegate(this, &EntityManager::onContact));

    iPhysicsMaterialCombo* bulletBullet = new iPhysicsMaterialCombo(materialBullet, materialBullet);
    bulletBullet->setName("bullet-bullet");
    //bulletBullet->registerContactDelegate(iContactDelegate(this, &EntityManager::onContact));

    // setup floor model
    iNodeModel* floorModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    iModelDataInputParameter* param = new iModelDataInputParameter();
    param->_keepMesh = true;
    param->_modelSourceType = iModelSourceType::File;
    floorModel->setModel("level.ompf", iResourceCacheMode::Keep, param);
    floorModel->registerModelReadyDelegate(iModelReadyDelegate(this, &ExampleCharacterController::onModelReady));
    iNodeTransform* floorTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    floorTransform->insertNode(floorModel);
    _scene->getRoot()->insertNode(floorTransform);

    iPhysicsCollision* floorCollision = iPhysics::getInstance().createBox(3, 1, 3, iaMatrixd());
    iPhysicsBody* floorBody = iPhysics::getInstance().createBody(floorCollision);
    floorBody->setMass(10);
    iaMatrixd floorM;
    floorM.translate(5, 5, 0);
    floorBody->setMatrix(floorM);
    floorBody->registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &ExampleCharacterController::onApplyForceAndTorqueBox));
    floorBody->setMaterial(_terrainMaterialID);

    // create a box that drops on floor
    {
        iPhysicsCollision* boxCollision = iPhysics::getInstance().createBox(1, 1, 1, iaMatrixd());
        iPhysicsBody* boxBody = iPhysics::getInstance().createBody(boxCollision);
        boxBody->setMass(10);
        boxBody->registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &ExampleCharacterController::onApplyForceAndTorqueBox));
        boxBody->setMaterial(_entityMaterialID);

        iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
        transformNode->translate(1, 10, 0);

        iNodeModel* crate = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
        crate->setModel("crate.ompf");
        transformNode->insertNode(crate);

        iPhysics::getInstance().bindTransformNode(boxBody, transformNode);
        _scene->getRoot()->insertNode(transformNode);
    }

    // create another box that drops on floor
    {
        iPhysicsCollision* boxCollision = iPhysics::getInstance().createBox(1, 1, 1, iaMatrixd());
        iPhysicsBody* boxBody = iPhysics::getInstance().createBody(boxCollision);
        boxBody->setMass(10);
        boxBody->registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &ExampleCharacterController::onApplyForceAndTorqueBox));
        boxBody->setMaterial(_entityMaterialID);

        iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
        transformNode->translate(10, 10, 0);

        iNodeModel* crate = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
        crate->setModel("crate.ompf");
        transformNode->insertNode(crate);

        iPhysics::getInstance().bindTransformNode(boxBody, transformNode);
        _scene->getRoot()->insertNode(transformNode);
    }

    // setup character and attache camera to it
    iaMatrixd startMatrix;
    startMatrix.translate(15,2,0);
    _characterController = new CharacterController(_scene->getRoot(), _entityMaterialID, startMatrix);

    // setup camera
    iNodeCamera* camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));
    _characterController->getHeadTransform()->insertNode(camera);
    // and finally we tell the view which camera shall be the current one. for this to work a camera must be part of a 
    // scene assiciated with the view wich we achived by adding all those nodes on to an other starting with the root node
    _view.setCurrentCamera(camera->getID());

    // setup gun
    iNodeTransform* gunTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    gunTransform->translate(0, 0, 0);
    iNodeTransform* gunScaleTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    gunScaleTransform->scale(0.1,0.1,1);
    iNodeModel* crate = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    crate->setModel("crate.ompf");
    _characterController->getRightSholderTransform()->insertNode(gunTransform);
    gunTransform->insertNode(gunScaleTransform);
    gunScaleTransform->insertNode(crate);

    // create a skybox
    iNodeSkyBox* skyBoxNode = static_cast<iNodeSkyBox*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSkyBox));
    // set it up with the default skybox texture
    skyBoxNode->setTextures(
        iTextureResourceFactory::getInstance().requestFile("skybox_default/front.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/back.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/left.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/right.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/top.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/bottom.png"));
    // manipulate the texture scale so we can see the repeating pattern of the textures
    skyBoxNode->setTextureScale(10);
    // create a material for the sky box because the default material for all iNodeRender and deriving classes has no textures and uses depth test
    _materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterialGroup(_materialSkyBox)->setOrder(iMaterial::RENDER_ORDER_MIN);
    iMaterialResourceFactory::getInstance().getMaterialGroup(_materialSkyBox)->getMaterial()->setName("SkyBox");
    // set that material
    skyBoxNode->setMaterial(_materialSkyBox);
    // and add it to the scene
    _scene->getRoot()->insertNode(skyBoxNode);

    // setup light
    iNodeTransform* directionalLightTranslate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    directionalLightTranslate->translate(100, 100, 100);
    // the light node
    iNodeLight* lightNode = static_cast<iNodeLight*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLight));
    lightNode->setAmbient(iaColor4f(0.3f, 0.3f, 0.3f, 1.0f));
    lightNode->setDiffuse(iaColor4f(0.8f, 0.8f, 0.8f, 1.0f));
    lightNode->setSpecular(iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
    // insert light to scene
    _scene->getRoot()->insertNode(directionalLightTranslate);
    directionalLightTranslate->insertNode(lightNode);

    // init font for render statistics
    _font = new iTextureFont("StandardFont.png");
    _statisticsVisualizer.setVerbosity(iRenderStatisticsVerbosity::FPSAndMetrics);

    // prepare igor logo
    _igorLogo = iTextureResourceFactory::getInstance().loadFile("special/splash.png", iResourceCacheMode::Free, iTextureBuildMode::Normal);
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);

    // start resource tasks
    _taskFlushModels = iTaskManager::getInstance().addTask(new iTaskFlushModels(&_window));
    _taskFlushTextures = iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));

    // register some callbacks
    iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownDelegate(this, &ExampleCharacterController::onKeyPressed));
    iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &ExampleCharacterController::onKeyReleased));
    iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &ExampleCharacterController::onMouseMoved));
    iMouse::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &ExampleCharacterController::onMouseWheel));

    // and start physics
    iPhysics::getInstance().start();
}

void ExampleCharacterController::onModelReady(uint64 modelNodeID)
{
    iNode* node = iNodeFactory::getInstance().getNode(modelNodeID);
    makeCollisions(node);
}

void ExampleCharacterController::makeCollisions(iNode* node)
{
    if (node->getType() == iNodeType::iNodeMesh)
    {
        iNodeMesh* meshNode = static_cast<iNodeMesh*>(node);
        iPhysicsCollision* collision = iPhysics::getInstance().createMesh(meshNode->getMesh(), 0, iaMatrixd());
        iPhysicsBody* body = iPhysics::getInstance().createBody(collision);
        body->setMass(0);
        body->setMatrix(iaMatrixd());
        body->setMaterial(_terrainMaterialID);
    }

    for (auto child : node->getChildren())
    {
        makeCollisions(child);
    }
}

void ExampleCharacterController::onKeyPressed(iKeyCode key)
{
    switch (key)
    {
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
        //_inputFlags._up = true;
        break;

    case iKeyCode::LControl:
        _inputFlags._down = true;
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
        _inputFlags._up = false;
        break;

    case iKeyCode::LControl:
        _inputFlags._down = false;
        break;
    }
}

void ExampleCharacterController::onHandle()
{
    float64 movingForce =  20000;
    float64 jumpingForce = 100000;

    iaMatrixd matrix;
    iaVector3d resultingForce;

    

    iNodeTransform* transformationNode = _characterController->getHeadingTransform();
    transformationNode->getMatrix(matrix);

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

    if (_inputFlags._up)
    {
        iaVector3d up = matrix._top;
        up.normalize();
        up *= movingForce;
        resultingForce += up;
    }

    if (_inputFlags._down)
    {
        iaVector3d down = matrix._top;
        down.negate();
        down.normalize();
        down *= movingForce;
        resultingForce += down;
    }

    if (resultingForce.length() > movingForce)
    {
        resultingForce.normalize();
        resultingForce *= movingForce;
    }

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

void ExampleCharacterController::onApplyForceAndTorqueBox(iPhysicsBody* body, float32 timestep)
{
    float64 Ixx;
    float64 Iyy;
    float64 Izz;
    float64 mass;
    iaVector3d force;

    iPhysics::getInstance().getMassMatrix(static_cast<void*>(body->getNewtonBody()), mass, Ixx, Iyy, Izz);
    force.set(0.0f, -mass * static_cast<float32>(__IGOR_GRAVITY__), 0.0f);

    body->setForce(force);
}

void ExampleCharacterController::deinit()
{
    // unregister some callbacks
    iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownDelegate(this, &ExampleCharacterController::onKeyPressed));
    iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &ExampleCharacterController::onKeyReleased));
    iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &ExampleCharacterController::onMouseMoved));
    iMouse::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &ExampleCharacterController::onMouseWheel));
    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &ExampleCharacterController::onWindowClosed));
    _window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &ExampleCharacterController::onWindowResized));
    _viewOrtho.unregisterRenderDelegate(RenderDelegate(this, &ExampleCharacterController::onRenderOrtho));
    iApplication::getInstance().unregisterApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &ExampleCharacterController::onHandle));

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

void ExampleCharacterController::onMouseWheel(int32 d)
{

}

void ExampleCharacterController::onMouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* _window)
{
    if (iMouse::getInstance().getLeftButton())
    {
        iNodeTransform* cameraPitch = _characterController->getPitchTransform();
        iNodeTransform* cameraHeading = _characterController->getHeadingTransform();

        if (cameraPitch != nullptr &&
            cameraHeading != nullptr)
        {
            cameraPitch->rotate((y1 - y2) * 0.005f, iaAxis::X);
            cameraHeading->rotate((x1 - x2) * 0.005f, iaAxis::Y);

            iMouse::getInstance().setCenter(true);
        }
    }
}

void ExampleCharacterController::onWindowClosed()
{
    iApplication::getInstance().stop();
}

void ExampleCharacterController::onWindowResized(int32 clientWidth, int32 clientHeight)
{
    _viewOrtho.setOrthogonal(0, clientWidth, clientHeight, 0);
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
    _statisticsVisualizer.drawStatistics(&_window, _font, iaColor4f(0, 1, 0, 1));
}

void ExampleCharacterController::drawLogo()
{
    iMaterialResourceFactory::getInstance().setMaterial(_materialWithTextureAndBlending);
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
