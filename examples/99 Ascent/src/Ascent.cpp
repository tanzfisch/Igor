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
#include <iNodeLODTrigger.h>
#include <iNodeLODSwitch.h>
#include <iOctree.h>
#include <iPhysics.h>
#include <iPhysicsMaterialCombo.h>
#include <iPhysicsBody.h>
#include <iEntityManager.h>
using namespace Igor;

#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "BossEnemy.h"
#include "StaticEnemy.h"

uint64 Ascent::_terrainMaterialID = 0;
uint64 Ascent::_entityMaterialID = 0;
uint64 Ascent::_bulletMaterialID = 0;

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

    iApplication::getInstance().registerApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &Ascent::onHandle));
}

void Ascent::unregisterHandles()
{
    iApplication::getInstance().unregisterApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &Ascent::onHandle));

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

    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setClipPlanes(0.1, 100);
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &Ascent::onRenderOrtho));
    _viewOrtho.setName("ortho");

    _window.setTitle("Ascent");
    _window.addView(&_view);
    _window.addView(&_viewOrtho);
#if 1
    _window.setClientSize(1024, 768);
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
}

void Ascent::initPlayer()
{
    iaMatrixd matrix;
    matrix.translate(10000, 9400, 10000 - 100);
    Player* player = new Player(_scene, matrix);
    _playerID = player->getID();

    iaMatrixd enemyMatrix;
    enemyMatrix._pos.set(10000, 9400, 10000 - 200);
    BossEnemy* boss = new BossEnemy(_scene, enemyMatrix, _playerID);
    _bossID = boss->getID();
}

void Ascent::initPhysics()
{
    iPhysicsMaterial* materialTerrain = iPhysics::getInstance().createMaterial("terrain");
    _terrainMaterialID = materialTerrain->getID();

    iPhysicsMaterial* materialEntity = iPhysics::getInstance().createMaterial("entity");
    _entityMaterialID = materialEntity->getID();

    iPhysicsMaterial* materialBullet = iPhysics::getInstance().createMaterial("bullet");
    _bulletMaterialID = materialBullet->getID();

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

    iPhysics::getInstance().start();
}

void Ascent::onContactTerrainBullet(iPhysicsBody* body0, iPhysicsBody* body1)
{
    if (body0 != nullptr && body1 != nullptr)
    {
        if (body0->getUserData() != nullptr)
        {
            uint64 id0 = reinterpret_cast<uint64>(body0->getUserData());
            _hitListMutex.lock();
            _hitList.push_back(pair<uint64, uint64>(id0, 0));
            _hitListMutex.unlock();

        }
        else if (body1->getUserData() != nullptr)
        {
            uint64 id1 = reinterpret_cast<uint64>(body1->getUserData());
            _hitListMutex.lock();
            _hitList.push_back(pair<uint64, uint64>(id1, 0));
            _hitListMutex.unlock();
        }
    }
}

void Ascent::onContact(iPhysicsBody* body0, iPhysicsBody* body1)
{
    if (body0 != nullptr && body1 != nullptr &&
        body0->getUserData() != nullptr &&
        body1->getUserData() != nullptr)
    {
        uint64 id0 = reinterpret_cast<uint64>(body0->getUserData());
        uint64 id1 = reinterpret_cast<uint64>(body1->getUserData());
        _hitListMutex.lock();
        _hitList.push_back(pair<uint64, uint64>(id0, id1));
        _hitList.push_back(pair<uint64, uint64>(id1, id0));
        _hitListMutex.unlock();
    }
}

void Ascent::onVoxelDataGenerated(const iaVector3I& min, const iaVector3I& max)
{
    iaVector3I pos;
    iaVector3I diff;
    diff = max;
    diff -= min;

    srand(min._x + min._y + min._z);

    iaMatrixd enemyMatrix;
    Player* player = static_cast<Player*>(iEntityManager::getInstance().getEntity(_playerID));

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
    }/**/

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
                    iEntityManager::getInstance().getEntities(sphere, result);
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
    con(" -- OpenGL 3D Test --" << endl);

    initViews();
    initScene();

    initPhysics();
    initPlayer();
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

void Ascent::onKeyPressed(iKeyCode key)
{
    if (_activeControls)
    {
        Player* player = static_cast<Player*>(iEntityManager::getInstance().getEntity(_playerID));

        if (player != nullptr)
        {
            switch (key)
            {
            case iKeyCode::A:
                player->startLeft();
                break;

            case iKeyCode::D:
                player->startRight();
                break;

            case iKeyCode::W:
                player->startForward();
                break;

            case iKeyCode::S:
                player->startBackward();
                break;

            case iKeyCode::Q:
                player->startUp();
                break;

            case iKeyCode::E:
                player->startDown();
                break;

            case iKeyCode::LShift:
                player->startFastTurn();
                break;

            case iKeyCode::One:
                player->startRollLeft();
                break;

            case iKeyCode::Three:
                player->startRollRight();
                break;

            case iKeyCode::Space:
                player->dig(_toolSize, _toolDensity);
                break;
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
        Player* player = static_cast<Player*>(iEntityManager::getInstance().getEntity(_playerID));
        if (player != nullptr)
        {
            switch (key)
            {
            case iKeyCode::A:
                player->stopLeft();
                break;

            case iKeyCode::D:
                player->stopRight();
                break;

            case iKeyCode::W:
                player->stopForward();
                break;

            case iKeyCode::S:
                player->stopBackward();
                break;

            case iKeyCode::Q:
                player->stopUp();
                break;

            case iKeyCode::E:
                player->stopDown();
                break;

            case iKeyCode::LShift:
                player->stopFastTurn();
                break;

            case iKeyCode::One:
                player->stopRollLeft();
                break;

            case iKeyCode::Three:
                player->stopRollRight();
                break;
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
    if (_activeControls)
    {
        Player* player = static_cast<Player*>(iEntityManager::getInstance().getEntity(_playerID));
        if (player != nullptr)
        {
           /* if (key == iKeyCode::MouseRight)
            {
                iaVector3d updown(_weaponPos._x, _weaponPos._y, _weaponPos._z);
                player->shootSecondaryWeapon(_view, updown);
            }*/

            if (key == iKeyCode::MouseLeft)
            {
                iaVector3d updown(_weaponPos._x, _weaponPos._y, _weaponPos._z);
                player->shootPrimaryWeapon(_view, updown);
            }
        }
    }
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
    Player* player = static_cast<Player*>(iEntityManager::getInstance().getEntity(_playerID));
    if (player != nullptr)
    {
        VoxelTerrainGenerator::getInstance().setLODTrigger(player->getLODTriggerID());
    }
    VoxelTerrainGenerator::getInstance().registerVoxelDataGeneratedDelegate(VoxelDataGeneratedDelegate(this, &Ascent::onVoxelDataGenerated));
}

void Ascent::handleMouse() // TODO
{
    if (_activeControls)
    {
        Player* player = static_cast<Player*>(iEntityManager::getInstance().getEntity(_playerID));
        if (player != nullptr)
        {
            // TODO WTF? if I use set() it does not work in release mode here
            _weaponPos._x = static_cast<float32>(_window.getClientWidth()) * 0.5f;
            _weaponPos._y = static_cast<float32>(_window.getClientHeight()) * 0.5f;
            _weaponPos._z = 0.0f;
            //_weaponPos.set(static_cast<float32>(_window.getClientWidth()) * 0.5f, static_cast<float32>(_window.getClientHeight()) * 0.5f, 0);

            float32 headingDelta = _mouseDelta._x * 0.002;
            float32 pitchDelta = _mouseDelta._y * 0.002;
            player->rotate(-headingDelta, -pitchDelta);
        }
    }
}

void Ascent::onHandle()
{
    if (_loading)
    {
        if (iTimer::getInstance().getTimerTime() > 5000 &&
            iTaskManager::getInstance().getQueuedRegularTaskCount() < 4 &&
            iTaskManager::getInstance().getQueuedRenderContextTaskCount() < 4)
        {
            _loading = false;
            _activeControls = true;
            _mouseDelta.set(0, 0);
        }
    }
    else
    {
        BossEnemy* boss = static_cast<BossEnemy*>(iEntityManager::getInstance().getEntity(_bossID));
        if (boss == nullptr)
        {
            vector<uint64> ids;
            iEntityManager::getInstance().getEntities(ids);

            for (auto id : ids)
            {
                if (_playerID != id)
                {
                    GameObject* entity = static_cast<GameObject*>(iEntityManager::getInstance().getEntity(id));
                    if (entity != nullptr &&
                        entity->getType() == GameObjectType::Vehicle)
                    {
                        iEntityManager::getInstance().destroyEntity(entity->getID());
                    }
                }
            }
        }

        handleHitList();

        iEntityManager::getInstance().handle();
    }

    handleMouse();
}

void Ascent::onRender()
{
    // nothing to do
}

void Ascent::handleHitList()
{
    _hitListMutex.lock();
    auto hitList = std::move(_hitList);
    _hitListMutex.unlock();

    for (auto hit : hitList)
    {
        GameObject* go1 = static_cast<GameObject*>(iEntityManager::getInstance().getEntity(hit.first));
        if (go1 != nullptr)
        {
            go1->hitBy(hit.second);
        }
        GameObject* go2 = static_cast<GameObject*>(iEntityManager::getInstance().getEntity(hit.second));
        if (go2 != nullptr)
        {
            go2->hitBy(hit.first);
        }
    }
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

        iRenderer::getInstance().setColor(iaColor4f(0, 0, 1, 1));
        iRenderer::getInstance().setFontSize(40.0f);
        iRenderer::getInstance().drawString(_window.getClientWidth() * 0.5, _window.getClientHeight() * 0.5, "generating level ...", iHorizontalAlignment::Center, iVerticalAlignment::Center);
    }
    else
    {
        BossEnemy* boss = static_cast<BossEnemy*>(iEntityManager::getInstance().getEntity(_bossID));
        if (boss == nullptr)
        {
            iRenderer::getInstance().setColor(iaColor4f(0, 1, 0, 1));
            iRenderer::getInstance().setFontSize(40.0f);
            iRenderer::getInstance().drawString(_window.getClientWidth() * 0.5, _window.getClientHeight() * 0.5, "you win!", iHorizontalAlignment::Center, iVerticalAlignment::Center);            
        }

        Player* player = static_cast<Player*>(iEntityManager::getInstance().getEntity(_playerID));
        if (player != nullptr)
        {
            iaString healthText = iaString::ftoa(player->getHealth(), 0);
            iaString shieldText = iaString::ftoa(player->getShield(), 0);

            iRenderer::getInstance().setFontSize(15.0f);
            iRenderer::getInstance().setColor(iaColor4f(1, 0, 0, 1));
            iRenderer::getInstance().drawString(_window.getClientWidth() * 0.05, _window.getClientHeight() * 0.05, healthText);

            iRenderer::getInstance().setColor(iaColor4f(0, 0, 1, 1));
            iRenderer::getInstance().drawString(_window.getClientWidth() * 0.10, _window.getClientHeight() * 0.05, shieldText);

            player->drawReticle(_window);
        }
        else
        {
            iRenderer::getInstance().setColor(iaColor4f(1, 0, 0, 1));
            iRenderer::getInstance().setFontSize(40.0f);
            iRenderer::getInstance().drawString(_window.getClientWidth() * 0.5, _window.getClientHeight() * 0.5, "you are dead :-P", iHorizontalAlignment::Center, iVerticalAlignment::Center);
            _activeControls = false;
        }
    }

    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
}

void Ascent::run()
{
    iApplication::getInstance().run();
}


