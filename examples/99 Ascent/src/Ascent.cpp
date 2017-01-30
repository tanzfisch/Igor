#include "Ascent.h"

#include <iaConsole.h>
using namespace IgorAux;

#include <iMaterial.h>
#include <iMaterialGroup.h>
#include <iNodeVisitorPrintTree.h>
#include <iTaskManager.h>
#include <iNodeSkyBox.h>
#include <iNodeLight.h>
#include <iNodeCamera.h>
#include <iNodeModel.h> 
#include <iNodeTransform.h>
#include <iRenderer.h>
#include <iApplication.h>
#include <iSceneFactory.h>
#include <iScene.h>
#include <iNodeFactory.h>
#include <iMouse.h>
#include <iTimer.h>
#include <iTextureFont.h>
#include <iTaskFlushModels.h>
#include <iTaskFlushTextures.h>
#include <iMaterialResourceFactory.h>
#include <iVoxelData.h>
#include <iMeshBuilder.h>
#include <iaVector3.h>
#include <iContouringCubes.h>
#include <iTextureResourceFactory.h>
#include <iPixmap.h>
#include <iStatistics.h>
#include <iTargetMaterial.h>
#include <iPerlinNoise.h>
#include <iNodeTransformControl.h>
#include <iNodeLODTrigger.h>
#include <iNodeLODSwitch.h>
#include <iOctree.h>
#include <iPhysics.h>
#include <iPhysicsMaterialCombo.h>
#include <iNodePhysics.h>
#include <iNodeEmitter.h>
using namespace Igor;

Ascent::Ascent()
{
    init();
}

Ascent::~Ascent()
{
    deinit();
}

void Ascent::registerHandles()
{
    iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownDelegate(this, &Ascent::onKeyPressed));
    iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &Ascent::onKeyReleased));
    iMouse::getInstance().registerMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &Ascent::onMouseDown));
    iMouse::getInstance().registerMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &Ascent::onMouseUp));
    iMouse::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &Ascent::onMouseWheel));
    iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &Ascent::onMouseMoved));

    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &Ascent::onWindowResized));
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &Ascent::onWindowClosed));

    iApplication::getInstance().registerApplicationHandleDelegate(iApplicationHandleDelegate(this, &Ascent::onHandle));
}

void Ascent::unregisterHandles()
{
    iApplication::getInstance().unregisterApplicationHandleDelegate(iApplicationHandleDelegate(this, &Ascent::onHandle));

    _window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &Ascent::onWindowResized));
    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &Ascent::onWindowClosed));

    iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &Ascent::onMouseMoved));
    iMouse::getInstance().unregisterMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &Ascent::onMouseDown));
    iMouse::getInstance().unregisterMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &Ascent::onMouseUp));
    iMouse::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &Ascent::onMouseWheel));

    iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &Ascent::onKeyReleased));
    iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownDelegate(this, &Ascent::onKeyPressed));
}

void Ascent::initViews()
{
    _view.setClearColor(iaColor4f(0.0f, 0.0f, 0.0f, 1.0f));
    _view.setPerspective(60);
    _view.setClipPlanes(0.1f, 500.f);
    _view.registerRenderDelegate(RenderDelegate(this, &Ascent::onRender));
    _view.setName("3d");    
    _view.setVisible(false);

    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setClipPlanes(0.1, 100);
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &Ascent::onRenderOrtho));
    _viewOrtho.setName("ortho");

    _window.setTitle("Ascent");
    _window.addView(&_view);
    _window.addView(&_viewOrtho);
#if 1
    _window.setSize(1024, 768);
#else
    _window.setSizeByDesktop();
    _window.setFullscreen();
#endif
    _window.open();

    iMouse::getInstance().showCursor(false);

    _viewOrtho.setOrthogonal(0, _window.getClientWidth(), _window.getClientHeight(), 0);
}

void Ascent::initScene()
{
    _scene = iSceneFactory::getInstance().createScene();
    _view.setScene(_scene);

    // light
    _lightTranslate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _lightTranslate->translate(100, 100, 100);
    _lightRotate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _lightNode = static_cast<iNodeLight*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLight));
    _lightNode->setAmbient(iaColor4f(0.7f, 0.7f, 0.7f, 1.0f));
    _lightNode->setDiffuse(iaColor4f(1.0f, 0.9f, 0.8f, 1.0f));
    _lightNode->setSpecular(iaColor4f(1.0f, 0.9f, 0.87f, 1.0f));

    _lightRotate->insertNode(_lightTranslate);
    _lightTranslate->insertNode(_lightNode);
    _scene->getRoot()->insertNode(_lightRotate);

    // reate a sky box and add it to scene
    iNodeSkyBox* skyBoxNode = static_cast<iNodeSkyBox*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSkyBox));
    skyBoxNode->setTextures(
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/front.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/back.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/left.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/right.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/top.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/bottom.jpg"));
    skyBoxNode->setTextureScale(1);
    // create a sky box material
    _materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterialGroup(_materialSkyBox)->setOrder(10);
    iMaterialResourceFactory::getInstance().getMaterialGroup(_materialSkyBox)->getMaterial()->setName("SkyBox");
    // and set the sky box material
    skyBoxNode->setMaterial(_materialSkyBox);
    // insert sky box to scene
    _scene->getRoot()->insertNode(skyBoxNode);

    _materialSolid = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialSolid)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSolid)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
}

void Ascent::initPhysics()
{
    iPhysicsMaterial* materialTerrain = iPhysics::getInstance().createMaterial("terrain");
    iPhysicsMaterial* materialEntity = iPhysics::getInstance().createMaterial("entity");
    iPhysicsMaterial* materialBullet = iPhysics::getInstance().createMaterial("bullet");

    iPhysicsMaterialCombo* terrainEntity = new iPhysicsMaterialCombo(materialTerrain, materialEntity);
    terrainEntity->setName("terrain-entity");
    terrainEntity->registerContactDelegate(iContactDelegate(this, &Ascent::onContact));
    terrainEntity->setElasticity(0.0);
    terrainEntity->setFriction(0.0, 0.0);

    iPhysicsMaterialCombo* terrainBullet = new iPhysicsMaterialCombo(materialTerrain, materialBullet);
    terrainBullet->setName("terrain-bullet");
    terrainBullet->registerContactDelegate(iContactDelegate(this, &Ascent::onContactTerrainBullet));

    iPhysicsMaterialCombo* bulletEntity = new iPhysicsMaterialCombo(materialBullet, materialEntity);
    bulletEntity->setName("bullet-entity");
    bulletEntity->registerContactDelegate(iContactDelegate(this, &Ascent::onContact));

    iPhysicsMaterialCombo* entityEntity = new iPhysicsMaterialCombo(materialEntity, materialEntity);
    entityEntity->setName("entity-entity");
    entityEntity->registerContactDelegate(iContactDelegate(this, &Ascent::onContact));

    iPhysicsMaterialCombo* bulletBullet = new iPhysicsMaterialCombo(materialBullet, materialBullet);
    bulletBullet->setName("bullet-bullet");
    bulletBullet->registerContactDelegate(iContactDelegate(this, &Ascent::onContact));
}

void Ascent::onContactTerrainBullet(iPhysicsBody* body0, iPhysicsBody* body1)
{
    if (body0 != nullptr && body1 != nullptr)
    {
        if (body0->getUserData() != nullptr)
        {
            handleContact(reinterpret_cast<uint64>(body0->getUserData()), iEntity::INVALID_ENTITY_ID);
        }
        else if (body1->getUserData() != nullptr)
        {
            handleContact(reinterpret_cast<uint64>(body1->getUserData()), iEntity::INVALID_ENTITY_ID);
        }
    }
}

void Ascent::onContact(iPhysicsBody* body0, iPhysicsBody* body1)
{
    if (body0 != nullptr && body1 != nullptr &&
        body0->getUserData() != nullptr &&
        body1->getUserData() != nullptr)
    {
        const void* bla = body0->getUserData();

        uint64 id0 = reinterpret_cast<uint64>(body0->getUserData());
        uint64 id1 = reinterpret_cast<uint64>(body1->getUserData());

        handleContact(id0, id1);
    }
}

void Ascent::handleContact(uint64 entityID1, uint64 entityID2)
{

}

void Ascent::initPlayer()
{
    _playerID = _entityManager.createEntity();
    _entityManager.linkComponent(_playerID, &_componentAttributes);
    _entityManager.linkComponent(_playerID, &_componentInput);
    _entityManager.linkComponent(_playerID, &_componentTransform);
    _entityManager.linkComponent(_playerID, &_componentForceAndTorque);

    Attributes* attributes = static_cast<Attributes*>(_componentAttributes.getData(_playerID));
    attributes->_fraction = Fraction::Green;
    attributes->_health = 200;
    attributes->_shield = 300;
    attributes->_damage = 1.0;
    attributes->_shieldDamage = 1.0;

    uint32* transformNodeID = static_cast<uint32*>(_componentTransform.getData(_playerID));

    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformNode->translate(10000, 9400, 10000);
    *transformNodeID = transformNode->getID();

    iNodeTransform* transformCam = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    //_transformCamNodeID = transformCam->getID();

    iNodeCamera* camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));
    //_cameraNodeID = camera->getID();

    iaMatrixd offset;
    iNodePhysics* physicsNode = static_cast<iNodePhysics*>(iNodeFactory::getInstance().createNode(iNodeType::iNodePhysics));
    //_physicsNodeID = physicsNode->getID();
    physicsNode->addSphere(1, offset);
    physicsNode->finalizeCollision();
    physicsNode->setMass(10);
    physicsNode->setMaterial(iPhysics::getInstance().getMaterialID("entity"));
    physicsNode->setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &Ascent::onApplyForceAndTorquePlayer));
    physicsNode->setUserData(reinterpret_cast<const void*>(_playerID));
    physicsNode->setAngularDamping(iaVector3d(100000, 100000, 100000));
    physicsNode->setLinearDamping(500);

    iNodeTransform* transformRecoilLeftGun = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    //_transformRecoilLeftGun = transformRecoilLeftGun->getID();

    iNodeTransform* transformRecoilRightGun = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    //_transformRecoilRightGun = transformRecoilRightGun->getID();

    iNodeTransform* transformLeftGun = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformLeftGun->translate(-0.5, -0.4, -0.75);
    transformLeftGun->scale(0.1, 0.1, 1);
    iNodeModel* leftgun = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    leftgun->setModel("crate.ompf", nullptr);

    iNodeTransform* transformRightGun = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformRightGun->translate(0.5, -0.4, -0.75);
    transformRightGun->scale(0.1, 0.1, 1);
    iNodeModel* rightgun = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    rightgun->setModel("crate.ompf", nullptr);

    iNodeTransform* transformLeftGunEmitter = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformLeftGunEmitter->translate(-0.5, -0.4, -1.25);

    iNodeTransform* transformRightGunEmitter = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformRightGunEmitter->translate(0.5, -0.4, -1.25);

    iNodeEmitter* emitterLeftGun = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
    //_emitterLeftGunNodeID = emitterLeftGun->getID();
    emitterLeftGun->setType(iEmitterType::Point);

    iNodeEmitter* emitterRightGun = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
    //_emitterRightGunNodeID = emitterRightGun->getID();
    emitterRightGun->setType(iEmitterType::Point);

    iNodeLODTrigger* lodTrigger = static_cast<iNodeLODTrigger*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLODTrigger));
    _lodTriggerID = lodTrigger->getID();

    transformNode->insertNode(transformRecoilLeftGun);
    transformNode->insertNode(transformRecoilRightGun);

    transformRecoilLeftGun->insertNode(transformLeftGun);
    transformRecoilRightGun->insertNode(transformRightGun);

    transformRecoilLeftGun->insertNode(transformLeftGunEmitter);
    transformRecoilRightGun->insertNode(transformRightGunEmitter);

    transformLeftGun->insertNode(leftgun);
    transformRightGun->insertNode(rightgun);

    transformLeftGunEmitter->insertNode(emitterLeftGun);
    transformRightGunEmitter->insertNode(emitterRightGun);

    transformCam->insertNode(camera);
    transformNode->insertNode(transformCam);
    transformNode->insertNode(physicsNode);
    camera->insertNode(lodTrigger);
    _scene->getRoot()->insertNode(transformNode);

    camera->makeCurrent();
}

void Ascent::initBoss()
{
    /*BossEnemy* bossEnemy = static_cast<BossEnemy*>(EntityManager::getInstance().createEntity("BossEnemy"));
    bossEnemy->setPosition(player->getPosition() + iaVector3d(0, 0, -10));
    bossEnemy->setTargetID(_playerID);
    _bossID = bossEnemy->getID();*/
}

void Ascent::onVoxelDataGenerated(const iaVector3I& min, const iaVector3I& max)
{
    /*iaVector3I pos;
    iaVector3I diff;
    diff = max;
    diff -= min;

    srand(min._x + min._y + min._z);

    iaMatrixd enemyMatrix;
    Player* player = static_cast<Player*>(EntityManager::getInstance().getEntity(_playerID));

    int count = 0;

    iaVector3I center(10000, 9400, 10000 - 200);

    for (int i = 0; i < 300; ++i)
    {
        pos.set(rand() % diff._x, rand() % diff._y, rand() % diff._z);
        pos += min;

        if (center.distance(pos) < 60)
        {
            bool addEnemy = true;

            for (int x = -2; x < 3; x++)
            {
                for (int y = -2; y < 3; y++)
                {
                    for (int z = -2; z < 3; z++)
                    {
                        if (VoxelTerrainGenerator::getInstance().getVoxelDensity(iaVector3I(pos._x + x, pos._y + y, pos._z + z)) != 0)
                        {
                            addEnemy = false;
                            break;
                        }
                    }
                }
            }

            if (addEnemy)
            {
                enemyMatrix._pos.set(pos._x, pos._y, pos._z);
                Enemy* enemy = new Enemy(_scene, enemyMatrix, _playerID);
                count++;
            }

            if (count >= 20)
            {
                break;
            }
        }
    }

    count = 0;

    for (int i = 0; i < 800; ++i)
    {
        pos.set(rand() % diff._x, rand() % diff._y, rand() % diff._z);
        pos += min;

        if (center.distance(pos) < 60)
        {
            bool addEnemy = true;

            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    for (int z = -1; z < 2; z++)
                    {
                        if (VoxelTerrainGenerator::getInstance().getVoxelDensity(iaVector3I(pos._x + x, pos._y + y, pos._z + z)) != 0)
                        {
                            addEnemy = false;
                            break;
                        }
                    }
                }
            }

            if (addEnemy)
            {
                iaVector3d from(pos._x, pos._y, pos._z);

                iaMatrixd matrix;

                switch (rand() % 6)
                {
                case 0:
                    // nothing
                    break;

                case 1:
                    matrix.rotate(M_PI * 0.5, iaAxis::Z);
                    break;

                case 2:
                    matrix.rotate(M_PI * -0.5, iaAxis::Z);
                    break;

                case 3:
                    matrix.rotate(M_PI, iaAxis::Z);
                    break;

                case 4:
                    matrix.rotate(M_PI * 0.5, iaAxis::X);
                    break;

                case 5:
                    matrix.rotate(M_PI * -0.5, iaAxis::X);
                    break;
                }

                iaVector3d to = from + matrix._top * -200;

                iaVector3I right(matrix._right._x, matrix._right._y, matrix._right._z);
                iaVector3I top(matrix._top._x, matrix._top._y, matrix._top._z);
                iaVector3I depth(matrix._depth._x, matrix._depth._y, matrix._depth._z);
                iaVector3I outside, inside;

                VoxelTerrainGenerator::getInstance().castRay(iaVector3I(from._x, from._y, from._z), iaVector3I(to._x, to._y, to._z), outside, inside);

                int rating = 0;

                if (outside.distance(pos) < 190)
                {
                    iSphered sphere(iaVector3d(outside._x, outside._y, outside._z), 5);
                    vector<uint64> result;
                    EntityManager::getInstance().getEntities(sphere, result);
                    if (result.empty())
                    {
                        if (VoxelTerrainGenerator::getInstance().getVoxelDensity(inside + right) != 0) rating++;
                        if (VoxelTerrainGenerator::getInstance().getVoxelDensity(inside - right) != 0) rating++;
                        if (VoxelTerrainGenerator::getInstance().getVoxelDensity(inside + right + depth) != 0) rating++;
                        if (VoxelTerrainGenerator::getInstance().getVoxelDensity(inside - right + depth) != 0) rating++;
                        if (VoxelTerrainGenerator::getInstance().getVoxelDensity(inside + right - depth) != 0) rating++;
                        if (VoxelTerrainGenerator::getInstance().getVoxelDensity(inside - right - depth) != 0) rating++;
                        if (VoxelTerrainGenerator::getInstance().getVoxelDensity(inside + depth) != 0) rating++;
                        if (VoxelTerrainGenerator::getInstance().getVoxelDensity(inside - depth) != 0) rating++;

                        if (VoxelTerrainGenerator::getInstance().getVoxelDensity(outside + right) < 50) rating++;
                        if (VoxelTerrainGenerator::getInstance().getVoxelDensity(outside - right) < 50) rating++;
                        if (VoxelTerrainGenerator::getInstance().getVoxelDensity(outside + right + depth) < 50) rating++;
                        if (VoxelTerrainGenerator::getInstance().getVoxelDensity(outside - right + depth) < 50) rating++;
                        if (VoxelTerrainGenerator::getInstance().getVoxelDensity(outside + right - depth) < 50) rating++;
                        if (VoxelTerrainGenerator::getInstance().getVoxelDensity(outside - right - depth) < 50) rating++;
                        if (VoxelTerrainGenerator::getInstance().getVoxelDensity(outside + depth) < 50) rating++;
                        if (VoxelTerrainGenerator::getInstance().getVoxelDensity(outside - depth) < 50) rating++;

                        if (rating > 10)
                        {
                            enemyMatrix.identity();
                            enemyMatrix = matrix;
                            enemyMatrix._pos.set(outside._x, outside._y, outside._z);
                            StaticEnemy* enemy = new StaticEnemy(_scene, enemyMatrix, _playerID);

                            count++;
                        }
                    }
                }
            }

            if (count >= 100)
            {
                break;
            }
        }
    }/**/
}

void Ascent::init()
{
    _startTime = iTimer::getInstance().getTime();

    initViews();
    initScene();

    initECS();
    initVoxelData();

    // set up octree debug rendering
    _octreeMaterial = iMaterialResourceFactory::getInstance().createMaterial("Octree");
    iMaterialResourceFactory::getInstance().getMaterial(_octreeMaterial)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_octreeMaterial)->getRenderStateSet().setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_octreeMaterial)->getRenderStateSet().setRenderState(iRenderState::Wireframe, iRenderStateValue::On);

    // set up statistics
    _font = new iTextureFont("StandardFont.png");
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial("TextureAndBlending");
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    _statisticsVisualizer.setVerbosity(iRenderStatisticsVerbosity::None);

    uint64 particlesMaterial = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(particlesMaterial)->setName("PMat");
    iMaterialResourceFactory::getInstance().getMaterial(particlesMaterial)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(particlesMaterial)->getRenderStateSet().setRenderState(iRenderState::CullFace, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(particlesMaterial)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(particlesMaterial)->getRenderStateSet().setRenderState(iRenderState::Texture2D1, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(particlesMaterial)->getRenderStateSet().setRenderState(iRenderState::Texture2D2, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(particlesMaterial)->getRenderStateSet().setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(particlesMaterial)->getRenderStateSet().setRenderState(iRenderState::BlendFuncSource, iRenderStateValue::SourceAlpha);
    iMaterialResourceFactory::getInstance().getMaterial(particlesMaterial)->getRenderStateSet().setRenderState(iRenderState::BlendFuncDestination, iRenderStateValue::OneMinusSourceAlpha);

    // launch resource handlers
    _taskFlushModels = iTaskManager::getInstance().addTask(new iTaskFlushModels(&_window));
    _taskFlushTextures = iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));

    registerHandles();

    iRenderer::getInstance().setWorldGridResolution(1000.0);

    initPhysics();
}

void Ascent::initECS()
{
    _entityManager.registerComponent(&_componentAttributes, "Attributes");
    _entityManager.registerComponent(&_componentForceAndTorque, "ForceAndTorque");
    _entityManager.registerComponent(&_componentInput, "Input");
    _entityManager.registerComponent(&_componentTransform, "Transform");

    _entityManager.registerSystem(&_systemPlayerInput, vector<uint64>(_componentInput.getID(), _componentForceAndTorque.getID()));

    initPlayer();
    initBoss();
}

void Ascent::deinit()
{
    unregisterHandles();

    if (_font)
    {
        delete _font;
        _font = nullptr;
    }

    iSceneFactory::getInstance().destroyScene(_scene);

    VoxelTerrainGenerator::getInstance().unregisterVoxelDataGeneratedDelegate(VoxelDataGeneratedDelegate(this, &Ascent::onVoxelDataGenerated));
    VoxelTerrainGenerator::getInstance().destroyInstance();

    iTaskManager::getInstance().abortTask(_taskFlushModels);
    iTaskManager::getInstance().abortTask(_taskFlushTextures);

    _view.unregisterRenderDelegate(RenderDelegate(this, &Ascent::onRender));
    _viewOrtho.unregisterRenderDelegate(RenderDelegate(this, &Ascent::onRenderOrtho));

    _window.close();
    _window.removeView(&_view);
    _window.removeView(&_viewOrtho);
}

void Ascent::onApplyForceAndTorquePlayer(iPhysicsBody* body, float32 timestep)
{
    ForceAndTorque* forceAndTorque = static_cast<ForceAndTorque*>(_componentForceAndTorque.getData(_playerID));

    body->setForce(forceAndTorque->_force);
    body->setTorque(forceAndTorque->_torque);
}

void Ascent::onKeyPressed(iKeyCode key)
{
    if (_activeControls)
    {
        InputFlags* inputFlags = static_cast<InputFlags*>(_componentInput.getData(_playerID));

        if (inputFlags != nullptr)
        {
            switch (key)
            {
            case iKeyCode::A:
                inputFlags->_left = true;
                break;

            case iKeyCode::D:
                inputFlags->_right = true;
                break;

            case iKeyCode::W:
                inputFlags->_forward = true;
                break;

            case iKeyCode::S:
                inputFlags->_backward = true;
                break;

            case iKeyCode::Q:
                inputFlags->_up = true;
                break;

            case iKeyCode::E:
                inputFlags->_down = true;
                break;

            case iKeyCode::LShift:
                inputFlags->_fastTurn = true;
                break;

            case iKeyCode::One:
                inputFlags->_rollLeft = true;
                break;

            case iKeyCode::Three:
                inputFlags->_rollRight = true;
                break;

            //case iKeyCode::Space:
              //  player->dig(_toolSize, _toolDensity);
//                break;
            }
        }
    }

    switch (key)
    {
    case iKeyCode::ESC:
        iApplication::getInstance().stop();
        break;

    case iKeyCode::F3:
    {
        iRenderStatisticsVerbosity level = _statisticsVisualizer.getVerbosity();

        if (level == iRenderStatisticsVerbosity::All)
        {
            level = iRenderStatisticsVerbosity::None;
        }
        else
        {
            int value = static_cast<int>(level);
            value++;
            level = static_cast<iRenderStatisticsVerbosity>(value);
        }

        _statisticsVisualizer.setVerbosity(level);
    }
    break;
    }
}

void Ascent::onKeyReleased(iKeyCode key)
{
    if (_activeControls)
    {
        InputFlags* inputFlags = static_cast<InputFlags*>(_componentInput.getData(_playerID));

        if (inputFlags != nullptr)
        {
            switch (key)
            {
            case iKeyCode::A:
                inputFlags->_left = false;
                break;

            case iKeyCode::D:
                inputFlags->_right = false;
                break;

            case iKeyCode::W:
                inputFlags->_forward = false;
                break;

            case iKeyCode::S:
                inputFlags->_backward = false;
                break;

            case iKeyCode::Q:
                inputFlags->_up = false;
                break;

            case iKeyCode::E:
                inputFlags->_down = false;
                break;

            case iKeyCode::LShift:
                inputFlags->_fastTurn = false;
                break;

            case iKeyCode::One:
                inputFlags->_rollLeft = false;
                break;

            case iKeyCode::Three:
                inputFlags->_rollRight = false;
                break;

                //case iKeyCode::Space:
                //  player->dig(_toolSize, _toolDensity);
                //                break;
            }
        }
    }
}

void Ascent::onMouseWheel(int d)
{
    if (_activeControls)
    {
        if (d > 0)
        {
            _toolSize += 1;
            if (_toolSize > 20)
            {
                _toolSize = 20;
            }
        }
        else
        {
            _toolSize -= 5;

            if (_toolSize < 5)
            {
                _toolSize = 5;
            }
        }
    }
}

void Ascent::onMouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* _window)
{
    if (_activeControls)
    {
        _mouseDelta.set(x2 - x1, y2 - y1);

        if (!iKeyboard::getInstance().getKey(iKeyCode::Space))
        {
            iMouse::getInstance().setCenter(true);
        }
    }
}

void Ascent::onMouseDown(iKeyCode key)
{
   /* if (_activeControls)
    {
        Player* player = static_cast<Player*>(EntityManager::getInstance().getEntity(_playerID));

        if (player != nullptr)
        {
            if (key == iKeyCode::MouseRight)
            {
                iaVector3d updown(_weaponPos._x, _weaponPos._y, _weaponPos._z);
                player->shootSecondaryWeapon(_view, updown);
            }

            if (key == iKeyCode::MouseLeft)
            {
                iaVector3d updown(_weaponPos._x, _weaponPos._y, _weaponPos._z);
                player->shootPrimaryWeapon(_view, updown);
            }
        }
    }*/
}

void Ascent::onMouseUp(iKeyCode key)
{

}

void Ascent::onWindowClosed()
{
    iApplication::getInstance().stop();
}

void Ascent::onWindowResized(int32 clientWidth, int32 clientHeight)
{
    _viewOrtho.setOrthogonal(0, static_cast<float32>(clientWidth), static_cast<float32>(clientHeight), 0);
}

void Ascent::initVoxelData()
{
    VoxelTerrainGenerator::getInstance().setScene(_scene);
    VoxelTerrainGenerator::getInstance().setLODTrigger(_lodTriggerID);
    VoxelTerrainGenerator::getInstance().registerVoxelDataGeneratedDelegate(VoxelDataGeneratedDelegate(this, &Ascent::onVoxelDataGenerated));
}

void Ascent::handleMouse()
{
    if (_activeControls)
    {
        /*Player* player = static_cast<Player*>(EntityManager::getInstance().getEntity(_playerID));
        if (player != nullptr)
        {
            _weaponPos.set(_window.getClientWidth() * 0.5, _window.getClientHeight() * 0.5, 0);

            float32 headingDelta = _mouseDelta._x * 0.002;
            float32 pitchDelta = _mouseDelta._y * 0.002;
            player->rotate(-headingDelta, -pitchDelta);
        }*/
    }
}

void Ascent::onHandle()
{
    if (_loading)
    {
        if (iTaskManager::getInstance().getQueuedRegularTaskCount() < 1 &&
            iTaskManager::getInstance().getQueuedRenderContextTaskCount() < 1 &&
            iTimer::getInstance().getTime() > (_startTime + (5 * __IGOR_SECOND__)))
        {
            _loading = false;
            _activeControls = true;
            _mouseDelta.set(0, 0);
            _view.setVisible(true);
        }
    }
    else
    {
     /*   BossEnemy* boss = static_cast<BossEnemy*>(EntityManager::getInstance().getEntity(_bossID));
        if (boss == nullptr)
        {
            vector<uint64> ids;
            EntityManager::getInstance().getEntities(ids);

            for (auto id : ids)
            {
                if (_playerID != id)
                {
                    GameObject* gameObject = static_cast<GameObject*>(EntityManager::getInstance().getEntity(id));
                    if (gameObject != nullptr &&
                        gameObject->getKind() == GameObjectKind::Vehicle)
                    {
                        gameObject->kill();
                    }
                }
            }
        }*/

        _entityManager.handle();
    }

    handleMouse();
}

void Ascent::onRender()
{
    // nothing to do
}

void Ascent::onRenderOrtho()
{
    _statisticsVisualizer.drawStatistics(&_window, _font, iaColor4f(0, 0, 0.8, 1));

    iaMatrixd matrix;
    iRenderer::getInstance().setViewMatrix(matrix);
    matrix.translate(0, 0, -30);
    iRenderer::getInstance().setModelMatrix(matrix);
    iMaterialResourceFactory::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setFont(_font);

    if (_loading)
    {
        iRenderer::getInstance().setColor(iaColor4f(0, 0, 0, 1));
        iRenderer::getInstance().drawRectangle(0, 0, _window.getClientWidth(), _window.getClientHeight());

        iRenderer::getInstance().setColor(iaColor4f(1, 0, 0, 1));
        iRenderer::getInstance().setFontSize(40.0f);
        iRenderer::getInstance().drawString(_window.getClientWidth() * 0.5, _window.getClientHeight() * 0.5, "loading ...", iHorizontalAlignment::Center, iVerticalAlignment::Center);
    }
    else
    {
/*        BossEnemy* boss = static_cast<BossEnemy*>(EntityManager::getInstance().getEntity(_bossID));
        if (boss == nullptr)
        {
            iRenderer::getInstance().setColor(iaColor4f(0, 1, 0, 1));
            iRenderer::getInstance().setFontSize(40.0f);
            iRenderer::getInstance().drawString(_window.getClientWidth() * 0.5, _window.getClientHeight() * 0.5, "you win!", iHorizontalAlignment::Center, iVerticalAlignment::Center);            
        }
        else
        {
            iaString healthText = iaString::ftoa(boss->getHealth(), 0);
            iaString shieldText = iaString::ftoa(boss->getShield(), 0);

            iRenderer::getInstance().setFontSize(15.0f);
            iRenderer::getInstance().setColor(iaColor4f(1, 0, 0, 1));
            iRenderer::getInstance().drawString(_window.getClientWidth() * 0.95, _window.getClientHeight() * 0.05, healthText);

            iRenderer::getInstance().setColor(iaColor4f(0, 0, 1, 1));
            iRenderer::getInstance().drawString(_window.getClientWidth() * 0.90, _window.getClientHeight() * 0.05, shieldText);

        }

        Player* player = static_cast<Player*>(EntityManager::getInstance().getEntity(_playerID));
        if (player != nullptr)
        {
            iaString healthText = iaString::ftoa(player->getHealth(), 0);
            iaString shieldText = iaString::ftoa(player->getShield(), 0);

            iRenderer::getInstance().setFontSize(15.0f);
            iRenderer::getInstance().setColor(iaColor4f(1, 0, 0, 1));
            iRenderer::getInstance().drawString(_window.getClientWidth() * 0.05, _window.getClientHeight() * 0.05, healthText);

            iRenderer::getInstance().setColor(iaColor4f(0, 0, 1, 1));
            iRenderer::getInstance().drawString(_window.getClientWidth() * 0.10, _window.getClientHeight() * 0.05, shieldText);

            drawReticle();
        }
        else
        {
            iRenderer::getInstance().setColor(iaColor4f(1, 0, 0, 1));
            iRenderer::getInstance().setFontSize(40.0f);
            iRenderer::getInstance().drawString(_window.getClientWidth() * 0.5, _window.getClientHeight() * 0.5, "you are dead :-P", iHorizontalAlignment::Center, iVerticalAlignment::Center);
            _activeControls = false;
        }*/
    }

    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
}

void Ascent::drawReticle()
{
    iaVector3f weaponPos(_window.getClientWidth() * 0.5, _window.getClientHeight() * 0.5, 0);

    float32 scale = 0.001 * _window.getClientWidth();

    iMaterialResourceFactory::getInstance().setMaterial(_materialSolid);
    iRenderer::getInstance().setLineWidth(1 * scale);

    iRenderer::getInstance().setColor(iaColor4f(1, 0, 0, 1));
    iRenderer::getInstance().drawLine(weaponPos + iaVector3f(-10 * scale, 0, 0), weaponPos + iaVector3f(10 * scale, 0, 0));
    iRenderer::getInstance().drawLine(weaponPos + iaVector3f(0, -10 * scale, 0), weaponPos + iaVector3f(0, 10 * scale, 0));
}

void Ascent::run()
{
    iApplication::getInstance().run();
}
