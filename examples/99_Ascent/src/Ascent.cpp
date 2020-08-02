// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "Ascent.h"

#include "gameObjects/Player.h"
#include "gameObjects/Enemy.h"
#include "gameObjects/Bullet.h"
#include "gameObjects/BossEnemy.h"
#include "gameObjects/StaticEnemy.h"
#include "gameObjects/DigEffect.h"

#include <igor/resources/material/iMaterial.h>
#include <igor/scene/traversal/iNodeVisitorPrintTree.h>
#include <igor/threading/iTaskManager.h>
#include <igor/scene/nodes/iNodeSkyBox.h>
#include <igor/scene/nodes/iNodeLight.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/graphics/iRenderer.h>
#include <igor/system/iApplication.h>
#include <igor/scene/iSceneFactory.h>
#include <igor/scene/iScene.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/system/iMouse.h>
#include <igor/system/iTimer.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/threading/tasks/iTaskFlushModels.h>
#include <igor/threading/tasks/iTaskFlushTextures.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/terrain/data/iVoxelData.h>

#include <igor/resources/texture/iTextureResourceFactory.h>

#include <igor/resources/material/iTargetMaterial.h>
#include <igor/scene/nodes/iNodeLODTrigger.h>
#include <igor/scene/nodes/iNodeLODSwitch.h>
#include <igor/physics/iPhysics.h>
#include <igor/physics/iPhysicsMaterialCombo.h>
#include <igor/physics/iPhysicsBody.h>
#include <igor/entities/iEntityManager.h>
#include <igor/terrain/iVoxelTerrain.h>
using namespace igor;

using namespace iaux;

uint64 Ascent::_terrainMaterialID = 0;
uint64 Ascent::_entityMaterialID = 0;
uint64 Ascent::_bulletMaterialID = 0;

Ascent::Ascent(iWindow *window)
    : iLayer(window), _viewOrtho(iView(false))
{
}

void Ascent::initViews()
{
    _view.setClearColor(iaColor4f(0.0f, 0.0f, 0.0f, 1.0f));
    _view.setPerspective(60);
    _view.setClipPlanes(0.1f, 50000.f);
    _view.registerRenderDelegate(iDrawDelegate(this, &Ascent::onRender));
    _view.setName("3d");

    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setClipPlanes(0.1, 100);
    _viewOrtho.registerRenderDelegate(iDrawDelegate(this, &Ascent::onRenderOrtho));
    _viewOrtho.setName("ortho");

    getWindow()->setTitle("Ascent");
    getWindow()->addView(&_view);
    getWindow()->addView(&_viewOrtho);
#if 1
    getWindow()->setClientSize(1024, 768);
    getWindow()->setCentered();
#else
    getWindow()->setSizeByDesktop();
    getWindow()->setFullscreen();
#endif
    getWindow()->open();

    iMouse::getInstance().showCursor(false);

    _viewOrtho.setOrthogonal(0, getWindow()->getClientWidth(), getWindow()->getClientHeight(), 0);
}

void Ascent::initScene()
{
    _scene = iSceneFactory::getInstance().createScene();
    _view.setScene(_scene);

    // light
    _lightTranslate = iNodeManager::getInstance().createNode<iNodeTransform>();
    _lightTranslate->translate(100, 100, 100);
    _lightRotate = iNodeManager::getInstance().createNode<iNodeTransform>();
    _lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
    _lightNode->setAmbient(iaColor4f(0.7f, 0.7f, 0.7f, 1.0f));
    _lightNode->setDiffuse(iaColor4f(1.0f, 0.9f, 0.8f, 1.0f));
    _lightNode->setSpecular(iaColor4f(1.0f, 0.9f, 0.87f, 1.0f));

    _lightRotate->insertNode(_lightTranslate);
    _lightTranslate->insertNode(_lightNode);
    _scene->getRoot()->insertNode(_lightRotate);

    // reate a sky box and add it to scene
    iNodeSkyBox *skyBoxNode = iNodeManager::getInstance().createNode<iNodeSkyBox>();
    skyBoxNode->setTextures(
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/front.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/back.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/left.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/right.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/top.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/bottom.jpg"));
    skyBoxNode->setTextureScale(1);
    // create a sky box material
    _materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial("Sky Box");
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setOrder(10);
    // and set the sky box material
    skyBoxNode->setMaterial(_materialSkyBox);
    // insert sky box to scene
    _scene->getRoot()->insertNode(skyBoxNode);
}

void Ascent::initPlayer()
{
    iaMatrixd matrix;
    matrix.translate(10000, 10000, 10000);
    Player *player = new Player(_scene, &_view, matrix);
    _playerID = player->getID();

    matrix.translate(static_cast<int32>(rand.getNext() % 200) - 100, static_cast<int32>(rand.getNext() % 200) - 100, -200);
    BossEnemy *boss = new BossEnemy(_scene, _voxelTerrain, matrix, _playerID);
    _bossID = boss->getID();
}

void Ascent::initPhysics()
{
    iPhysicsMaterial *materialTerrain = iPhysics::getInstance().createMaterial("terrain");
    _terrainMaterialID = materialTerrain->getID();

    iPhysicsMaterial *materialEntity = iPhysics::getInstance().createMaterial("entity");
    _entityMaterialID = materialEntity->getID();

    iPhysicsMaterial *materialBullet = iPhysics::getInstance().createMaterial("bullet");
    _bulletMaterialID = materialBullet->getID();

    iPhysicsMaterialCombo *terrainEntity = new iPhysicsMaterialCombo(materialTerrain, materialEntity);
    terrainEntity->setName("terrain-entity");
    terrainEntity->registerContactDelegate(iContactDelegate(this, &Ascent::onContact));
    terrainEntity->setElasticity(0.0);
    terrainEntity->setFriction(0.0, 0.0);

    iPhysicsMaterialCombo *terrainBullet = new iPhysicsMaterialCombo(materialTerrain, materialBullet);
    terrainBullet->setName("terrain-bullet");
    terrainBullet->registerContactDelegate(iContactDelegate(this, &Ascent::onContactTerrainBullet));

    iPhysicsMaterialCombo *bulletEntity = new iPhysicsMaterialCombo(materialBullet, materialEntity);
    bulletEntity->setName("bullet-entity");
    bulletEntity->registerContactDelegate(iContactDelegate(this, &Ascent::onContact));

    iPhysicsMaterialCombo *entityEntity = new iPhysicsMaterialCombo(materialEntity, materialEntity);
    entityEntity->setName("entity-entity");
    entityEntity->registerContactDelegate(iContactDelegate(this, &Ascent::onContact));

    iPhysicsMaterialCombo *bulletBullet = new iPhysicsMaterialCombo(materialBullet, materialBullet);
    bulletBullet->setName("bullet-bullet");
    bulletBullet->registerContactDelegate(iContactDelegate(this, &Ascent::onContact));

    iPhysics::getInstance().start();
}

void Ascent::deinitVoxelData()
{
    if (_voxelTerrain != nullptr)
    {
        delete _voxelTerrain;
        _voxelTerrain = nullptr;
    }
}

void Ascent::initVoxelData()
{
    oulineLevelStructure();

    iTargetMaterial *targetMaterial = _voxelTerrain->getTargetMaterial();
    targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("crates2.png"), 0);
    targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("crates2.png"), 1);
    targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("crates2.png"), 2);
    targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("detail.png"), 3);
    targetMaterial->setAmbient(iaColor3f(0.1f, 0.1f, 0.1f));
    targetMaterial->setDiffuse(iaColor3f(0.9f, 0.9f, 0.9f));
    targetMaterial->setSpecular(iaColor3f(1.0f, 1.0f, 1.0f));
    targetMaterial->setEmissive(iaColor3f(0.0f, 0.0f, 0.0f));
    targetMaterial->setShininess(1000.0f);

    uint64 materialID = iMaterialResourceFactory::getInstance().createMaterial("TerrainMaterial");
    auto material = iMaterialResourceFactory::getInstance().getMaterial(materialID);
    material->addShaderSource("ascent/terrain.vert", iShaderObjectType::Vertex);
    material->addShaderSource("ascent/terrain_directional_light.frag", iShaderObjectType::Fragment);
    material->compileShader();

    _voxelTerrain->setMaterialID(materialID);
    _voxelTerrain->setPhysicsMaterialID(_terrainMaterialID);

    _voxelTerrain->setScene(_scene);
    Player *player = static_cast<Player *>(iEntityManager::getInstance().getEntity(_playerID));
    if (player != nullptr)
    {
        _voxelTerrain->setLODTrigger(player->getLODTriggerID());
    }
}

void Ascent::oulineLevelStructure()
{
    BossEnemy *boss = static_cast<BossEnemy *>(iEntityManager::getInstance().getEntity(_bossID));
    if (boss != nullptr)
    {
        iaVector3d bossPosition = boss->getCurrentPos();

        // covering the boss
        _metaballs.push_back(iSphered(iaVector3d(bossPosition._x + 20, bossPosition._y, bossPosition._z), 1.7));
        _metaballs.push_back(iSphered(iaVector3d(bossPosition._x - 20, bossPosition._y, bossPosition._z), 1.7));
        _metaballs.push_back(iSphered(iaVector3d(bossPosition._x, bossPosition._y + 20, bossPosition._z), 1.7));
        _metaballs.push_back(iSphered(iaVector3d(bossPosition._x, bossPosition._y - 20, bossPosition._z), 1.7));
        _metaballs.push_back(iSphered(iaVector3d(bossPosition._x, bossPosition._y, bossPosition._z + 20), 1.7));
        _metaballs.push_back(iSphered(iaVector3d(bossPosition._x, bossPosition._y, bossPosition._z - 20), 1.7));

        // hole where the boss sits
        _holes.push_back(iSphered(iaVector3d(bossPosition._x, bossPosition._y, bossPosition._z), 1.7));

        // body
        for (int i = 0; i < 15; ++i)
        {
            iaVector3d pos(static_cast<int32>(rand.getNext() % 50) - 25, static_cast<int32>(rand.getNext() % 50) - 25, static_cast<int32>(rand.getNext() % 50) - 25);
            pos.normalize();
            pos *= 20 + (rand.getNext() % 40);
            pos += bossPosition;

            _metaballs.push_back(iSphered(pos, ((rand.getNext() % 50 + 50) / 100.0) * 4.0));
        }
    }
}

__IGOR_INLINE__ float64 metaballFunction(const iaVector3d &metaballPos, const iaVector3d &checkPos)
{
    return 1.0 / ((checkPos._x - metaballPos._x) * (checkPos._x - metaballPos._x) + (checkPos._y - metaballPos._y) * (checkPos._y - metaballPos._y) + (checkPos._z - metaballPos._z) * (checkPos._z - metaballPos._z));
}

void Ascent::onGenerateVoxelData(iVoxelBlockInfo *voxelBlockInfo)
{
    uint32 lodFactor = static_cast<uint32>(pow(2, voxelBlockInfo->_lod)); // TODO move to engine

    iVoxelData *voxelData = voxelBlockInfo->_voxelData;
    iaVector3I &offset = voxelBlockInfo->_positionInLOD;
    offset *= (32 * lodFactor); // TODO move to engine
    iaVector3f &lodOffset = voxelBlockInfo->_lodOffset;
    uint64 size = voxelBlockInfo->_size;

    voxelData->setClearValue(0);
    voxelData->initData(size, size, size);

    const float64 from = 0.444;
    const float64 to = 0.45;
    float64 factor = 1.0 / (to - from);

    const float64 fromMeta = 0.017;
    const float64 toMeta = 0.0175;
    float64 factorMeta = 1.0 / (toMeta - fromMeta);

    for (int64 x = 0; x < voxelData->getWidth(); ++x)
    {
        for (int64 z = 0; z < voxelData->getDepth(); ++z)
        {
            for (int64 y = 0; y < voxelData->getHeight(); ++y)
            {
                iaVector3d pos(x * lodFactor + offset._x + lodOffset._x,
                               y * lodFactor + offset._y + lodOffset._y,
                               z * lodFactor + offset._z + lodOffset._z); // TODO move to engine

                float64 noise = (_perlinNoise.getValue(iaVector3d(pos._x * 0.01, pos._y * 0.01, pos._z * 0.01), 2) - 0.5) * 0.04;
                noise += (_perlinNoise.getValue(iaVector3d(pos._x * 0.1, pos._y * 0.1, pos._z * 0.1), 3) - 0.5) * 0.003;

                float32 density = 0;

                float64 distance = 0;
                for (auto metaball : _metaballs)
                {
                    distance += metaballFunction(metaball._center, pos) * metaball._radius;
                }

                distance += noise;

                if (distance >= toMeta)
                {
                    density = 1.0;
                }
                else
                {
                    if (distance >= fromMeta)
                    {
                        density = ((distance - fromMeta) * factorMeta);
                    }
                }

                distance = 0;
                for (auto hole : _holes)
                {
                    distance += metaballFunction(hole._center, pos) * hole._radius;
                }

                distance -= noise;

                if (distance >= toMeta)
                {
                    density = 0.0;
                }
                else
                {
                    if (distance >= fromMeta)
                    {
                        if (density > 0)
                        {
                            density = 1 - ((distance - fromMeta) * factorMeta);
                        }
                    }
                }

                float64 onoff = _perlinNoise.getValue(iaVector3d(pos._x * 0.04, pos._y * 0.04, pos._z * 0.04), 3, 0.5);

                if (onoff <= from)
                {
                    if (onoff >= to)
                    {
                        density = 1.0 - ((onoff - from) * factor);
                    }
                    else
                    {
                        density = 0.0;
                    }
                }

                if (density > 1.0)
                {
                    density = 1.0;
                }

                if (density < 0.0)
                {
                    density = 0.0;
                }

                if (density > 0.0)
                {
                    voxelData->setVoxelDensity(iaVector3I(x, y, z), (density * 254) + 1);
                    voxelBlockInfo->_transition = true;
                }
            }
        }
    }
}

void Ascent::onContactTerrainBullet(iPhysicsBody *body0, iPhysicsBody *body1)
{
    if (body0 == nullptr || body1 == nullptr)
    {
        return;
    }

    std::any userData0 = body0->getUserData();
    std::any userData1 = body1->getUserData();

    if (userData0.has_value())
    {
        uint64 id0 = std::any_cast<uint64>(userData0);
        _hitListMutex.lock();
        _hitList.push_back(std::pair<uint64, uint64>(id0, 0));
        _hitListMutex.unlock();
    }
    else if (userData1.has_value())
    {
        uint64 id1 = std::any_cast<uint64>(userData1);
        _hitListMutex.lock();
        _hitList.push_back(std::pair<uint64, uint64>(id1, 0));
        _hitListMutex.unlock();
    }
}

void Ascent::onContact(iPhysicsBody *body0, iPhysicsBody *body1)
{
    if (body0 == nullptr || body1 == nullptr)
    {
        return;
    }

    std::any userData0 = body0->getUserData();
    std::any userData1 = body1->getUserData();

    if (userData0.has_value() &&
        userData1.has_value())
    {
        uint64 id0 = std::any_cast<uint64>(userData0);
        uint64 id1 = std::any_cast<uint64>(userData1);
        _hitListMutex.lock();
        _hitList.push_back(std::pair<uint64, uint64>(id0, id1));
        _hitList.push_back(std::pair<uint64, uint64>(id1, id0));
        _hitListMutex.unlock();
    }
}

void Ascent::onVoxelDataGenerated(iVoxelBlockPropsInfo voxelBlockPropsInfo)
{
    iaVector3I diff;
    diff = voxelBlockPropsInfo._max;
    diff -= voxelBlockPropsInfo._min;

    iaVector3d offset = voxelBlockPropsInfo._min.convert<float64>();
    iaVector3d pos;
    int count = 0;

    rand.setSeed(offset._x + offset._y + offset._z);

    for (int i = 0; i < rand.getNext() % 10; ++i)
    {
        pos.set(rand.getNext() % diff._x, rand.getNext() % diff._y, rand.getNext() % diff._z);

        bool addEnemy = true;

        for (int x = -2; x < 3; x++)
        {
            for (int y = -2; y < 3; y++)
            {
                for (int z = -2; z < 3; z++)
                {
                    if (_voxelTerrain->getVoxelDensity(iaVector3I(pos._x + x, pos._y + y, pos._z + z)) != 0)
                    {
                        addEnemy = false;
                        break;
                    }
                }
            }
        }

        if (addEnemy)
        {
            iaVector3d creationPos = pos + offset;

            iSphered sphere(creationPos, 5);
            std::vector<uint64> result;
            iEntityManager::getInstance().getEntities(sphere, result);
            if (result.empty())
            {
                iaMatrixd matrix;
                matrix._pos = creationPos;
                new Enemy(_scene, _voxelTerrain, matrix, _playerID); // TODO this is ugly
                count++;
            }
        }

        if (count >= 2)
        {
            break;
        }
    }

    count = 0;

    for (int i = 0; i < 100; ++i)
    {
        pos.set(rand.getNext() % diff._x, rand.getNext() % diff._y, rand.getNext() % diff._z);

        if (_voxelTerrain->getVoxelDensity(iaVector3I(pos._x, pos._y, pos._z)) == 0)
        {
            iaVector3d from(pos._x, pos._y, pos._z);

            // orientation of enemy
            int orientation = rand.getNext() % 6;
            iaMatrixd matrix;
            switch (orientation)
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
            iaVector3I outside, inside;

            if (_voxelTerrain->castRay(iaVector3I(from._x, from._y, from._z), iaVector3I(to._x, to._y, to._z), outside, inside))
            {
                iSphered sphere(iaVector3d(outside._x, outside._y, outside._z), 5);
                std::vector<uint64> result;
                iEntityManager::getInstance().getEntities(sphere, result);
                if (result.empty())
                {
                    matrix._pos.set(outside._x, outside._y, outside._z);
                    StaticEnemy *enemy = new StaticEnemy(_scene, _voxelTerrain, matrix, _playerID);
                    count++;
                }
            }

            if (count >= 3)
            {
                break;
            }
        }
    }
}

void Ascent::onInit()
{
    con_endl(" -- OpenGL 3D Test --");

    initViews();
    initScene();

    initPhysics();

    // need to create voxel terrain before we create the boss enemy
    // using a lower LOD count because we don't create such huge structures anyway and the transition detection in details is better
    _voxelTerrain = new iVoxelTerrain(iVoxelTerrainGenerateDelegate(this, &Ascent::onGenerateVoxelData),
                                      iVoxelTerrainPlacePropsDelegate(this, &Ascent::onVoxelDataGenerated), 7);

    initPlayer();
    initVoxelData();

    // set up octree debug rendering
    _octreeMaterial = iMaterialResourceFactory::getInstance().createMaterial("Octree");
    iMaterialResourceFactory::getInstance().getMaterial(_octreeMaterial)->setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_octreeMaterial)->setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_octreeMaterial)->setRenderState(iRenderState::Wireframe, iRenderStateValue::On);

    // set up statistics
    _font = new iTextureFont("StandardFont.png");
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial("TextureAndBlending");
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

    uint64 particlesMaterial = iMaterialResourceFactory::getInstance().createMaterial("Particles");
    iMaterialResourceFactory::getInstance().getMaterial(particlesMaterial)->setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(particlesMaterial)->setRenderState(iRenderState::CullFace, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(particlesMaterial)->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(particlesMaterial)->setRenderState(iRenderState::Texture2D1, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(particlesMaterial)->setRenderState(iRenderState::Texture2D2, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(particlesMaterial)->setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(particlesMaterial)->setRenderState(iRenderState::BlendFuncSource, iRenderStateValue::SourceAlpha);
    iMaterialResourceFactory::getInstance().getMaterial(particlesMaterial)->setRenderState(iRenderState::BlendFuncDestination, iRenderStateValue::OneMinusSourceAlpha);

    // launch resource handlers
    _taskFlushModels = iTaskManager::getInstance().addTask(new iTaskFlushModels(getWindow()));
    _taskFlushTextures = iTaskManager::getInstance().addTask(new iTaskFlushTextures(getWindow()));
}

void Ascent::onDeinit()
{
    if (_font)
    {
        delete _font;
        _font = nullptr;
    }

    iSceneFactory::getInstance().destroyScene(_scene);

    deinitVoxelData();

    iTaskManager::getInstance().abortTask(_taskFlushModels);
    iTaskManager::getInstance().abortTask(_taskFlushTextures);

    _view.unregisterRenderDelegate(iDrawDelegate(this, &Ascent::onRender));
    _viewOrtho.unregisterRenderDelegate(iDrawDelegate(this, &Ascent::onRenderOrtho));

    getWindow()->close();
    getWindow()->removeView(&_view);
    getWindow()->removeView(&_viewOrtho);
}

bool Ascent::getTerrainIntersectionPoint(iaVector3I &intersection)
{
    iNodeCamera *camera = static_cast<iNodeCamera *>(iNodeManager::getInstance().getNode(_view.getCurrentCamera()));
    if (camera != nullptr)
    {
        iaMatrixd modelMatrix;
        camera->getWorldMatrix(modelMatrix);

        iaVector3d dir(modelMatrix._depth._x, modelMatrix._depth._y, modelMatrix._depth._z);
        dir.negate();
        dir *= 100.0f;
        iaVector3d from(modelMatrix._pos._x, modelMatrix._pos._y, modelMatrix._pos._z);
        iaVector3d to(from);
        to += dir;

        if (from._x > 0 &&
            from._y > 0 &&
            from._z > 0 &&
            to._x > 0 &&
            to._y > 0 &&
            to._z > 0)
        {
            iaVector3I f(static_cast<int64>(from._x + 0.5), static_cast<int64>(from._y + 0.5), static_cast<int64>(from._z + 0.5));
            iaVector3I t(static_cast<int64>(to._x + 0.5), static_cast<int64>(to._y + 0.5), static_cast<int64>(to._z + 0.5));

            iaVector3I inside;

            _voxelTerrain->castRay(f, t, intersection, inside);
            return true;
        }
    }

    return false;
}

void Ascent::dig(iaVector3I position, uint64 toolSize, uint8 density)
{
    /*iAABoxI box;
    box._center = position;
    box._halfWidths.set(toolSize, toolSize, toolSize);
    _voxelTerrain->modify(box, density);*/

    iSphereI sphere;
    sphere._center = position;
    sphere._radius = toolSize;
    _voxelTerrain->modify(sphere, density);

    iaMatrixd effectMatrix;
    effectMatrix.translate(position._x, position._y, position._z);
    new DigEffect(_scene, effectMatrix);
}

void Ascent::handleMouse() // TODO
{
    if (_activeControls)
    {
        Player *player = static_cast<Player *>(iEntityManager::getInstance().getEntity(_playerID));
        if (player != nullptr)
        {
            // TODO WTF? if I use set() it does not work in release mode here
            _weaponPos._x = static_cast<float32>(getWindow()->getClientWidth()) * 0.5f;
            _weaponPos._y = static_cast<float32>(getWindow()->getClientHeight()) * 0.5f;
            _weaponPos._z = 0.0f;
            //_weaponPos.set(static_cast<float32>(getWindow()->getClientWidth()) * 0.5f, static_cast<float32>(getWindow()->getClientHeight()) * 0.5f, 0);

            float32 headingDelta = _mouseDelta._x * 0.002;
            float32 pitchDelta = _mouseDelta._y * 0.002;
            player->rotate(-headingDelta, -pitchDelta);
        }
    }
}

void Ascent::onPreDraw()
{
    if (_loading)
    {
        if (iTimer::getInstance().getApplicationTime() > 5000 &&
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
        BossEnemy *boss = static_cast<BossEnemy *>(iEntityManager::getInstance().getEntity(_bossID));
        if (boss == nullptr)
        {
            std::vector<uint64> ids;
            iEntityManager::getInstance().getEntities(ids);

            for (auto id : ids)
            {
                if (_playerID != id)
                {
                    GameObject *entity = static_cast<GameObject *>(iEntityManager::getInstance().getEntity(id));
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
        GameObject *go1 = static_cast<GameObject *>(iEntityManager::getInstance().getEntity(hit.first));
        if (go1 != nullptr)
        {
            go1->hitBy(hit.second);
        }
        GameObject *go2 = static_cast<GameObject *>(iEntityManager::getInstance().getEntity(hit.second));
        if (go2 != nullptr)
        {
            go2->hitBy(hit.first);
        }
    }
}

void Ascent::onRenderOrtho()
{
    iaMatrixd matrix;
    iRenderer::getInstance().setViewMatrix(matrix);
    matrix.translate(0, 0, -30);
    iRenderer::getInstance().setModelMatrix(matrix);
    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setFont(_font);

    if (_loading)
    {
        iRenderer::getInstance().setColor(iaColor4f(0, 0, 0, 1));
        iRenderer::getInstance().drawRectangle(0, 0, getWindow()->getClientWidth(), getWindow()->getClientHeight());

        iRenderer::getInstance().setColor(iaColor4f(0, 0, 1, 1));
        iRenderer::getInstance().setFontSize(40.0f);
        iRenderer::getInstance().drawString(getWindow()->getClientWidth() * 0.5, getWindow()->getClientHeight() * 0.5, "generating level ...", iHorizontalAlignment::Center, iVerticalAlignment::Center);
    }
    else
    {
        BossEnemy *boss = static_cast<BossEnemy *>(iEntityManager::getInstance().getEntity(_bossID));
        if (boss == nullptr)
        {
            iRenderer::getInstance().setColor(iaColor4f(0, 1, 0, 1));
            iRenderer::getInstance().setFontSize(40.0f);
            iRenderer::getInstance().drawString(getWindow()->getClientWidth() * 0.5, getWindow()->getClientHeight() * 0.5, "you win!", iHorizontalAlignment::Center, iVerticalAlignment::Center);
        }

        Player *player = static_cast<Player *>(iEntityManager::getInstance().getEntity(_playerID));
        if (player != nullptr)
        {
            iaString healthText = iaString::toString(player->getHealth(), 0);
            iaString shieldText = iaString::toString(player->getShield(), 0);

            iRenderer::getInstance().setFontSize(15.0f);
            iRenderer::getInstance().setColor(iaColor4f(1, 0, 0, 1));
            iRenderer::getInstance().drawString(getWindow()->getClientWidth() * 0.05, getWindow()->getClientHeight() * 0.05, healthText);

            iRenderer::getInstance().setColor(iaColor4f(0, 0, 1, 1));
            iRenderer::getInstance().drawString(getWindow()->getClientWidth() * 0.10, getWindow()->getClientHeight() * 0.05, shieldText);

            player->drawReticle(getWindow());
        }
        else
        {
            iRenderer::getInstance().setColor(iaColor4f(1, 0, 0, 1));
            iRenderer::getInstance().setFontSize(40.0f);
            iRenderer::getInstance().drawString(getWindow()->getClientWidth() * 0.5, getWindow()->getClientHeight() * 0.5, "you are dead :-P", iHorizontalAlignment::Center, iVerticalAlignment::Center);
            _activeControls = false;
        }
    }

    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
}

void Ascent::onEvent(iEvent &event)
{
    event.dispatch<iEventWindowResize>(IGOR_BIND_EVENT_FUNCTION(Ascent::onWindowResize));
    event.dispatch<iEventMouseKeyDown>(IGOR_BIND_EVENT_FUNCTION(Ascent::onMouseKeyDownEvent));
    event.dispatch<iEventMouseMove>(IGOR_BIND_EVENT_FUNCTION(Ascent::onMouseMoveEvent));
    event.dispatch<iEventMouseWheel>(IGOR_BIND_EVENT_FUNCTION(Ascent::onMouseWheelEvent));
    event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(Ascent::onKeyDown));
    event.dispatch<iEventKeyUp>(IGOR_BIND_EVENT_FUNCTION(Ascent::onKeyUp));
}

bool Ascent::onMouseKeyDownEvent(iEventMouseKeyDown &event)
{
    if (_activeControls)
    {
        Player *player = static_cast<Player *>(iEntityManager::getInstance().getEntity(_playerID));
        if (player != nullptr)
        {
            /* if (key == iKeyCode::MouseRight)
             {
                 iaVector3d updown(_weaponPos._x, _weaponPos._y, _weaponPos._z);
                 player->shootSecondaryWeapon(_view, updown);
             }*/

            if (event.getKey() == iKeyCode::MouseLeft)
            {
                iaVector3d updown(_weaponPos._x, _weaponPos._y, _weaponPos._z);
                player->shootPrimaryWeapon(_view, updown);
                return true;
            }
        }
    }
    return false;
}

bool Ascent::onMouseMoveEvent(iEventMouseMove &event)
{
    if (_activeControls)
    {
        const auto from = event.getLastPosition();
        const auto to = event.getPosition();

        _mouseDelta.set(to._x - from._x, to._y - from._y);

        if (!iKeyboard::getInstance().getKey(iKeyCode::Space))
        {
            iMouse::getInstance().setCenter();
        }

        return true;
    }

    return false;
}

bool Ascent::onMouseWheelEvent(iEventMouseWheel &event)
{
    if (_activeControls)
    {
        if (event.getWheelDelta() > 0)
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

        return true;
    }

    return false;
}

bool Ascent::onKeyDown(iEventKeyDown &event)
{
    if (_activeControls)
    {
        Player *player = static_cast<Player *>(iEntityManager::getInstance().getEntity(_playerID));

        if (player != nullptr)
        {
            switch (event.getKey())
            {
            case iKeyCode::A:
                player->startLeft();
                return true;

            case iKeyCode::D:
                player->startRight();
                return true;

            case iKeyCode::W:
                player->startForward();
                return true;

            case iKeyCode::S:
                player->startBackward();
                return true;

            case iKeyCode::Q:
                player->startUp();
                return true;

            case iKeyCode::E:
                player->startDown();
                return true;

            case iKeyCode::LShift:
                player->startFastTurn();
                return true;

            case iKeyCode::One:
                player->startRollLeft();
                return true;

            case iKeyCode::Three:
                player->startRollRight();
                return true;

            case iKeyCode::Space:
                iaVector3I intersection;
                if (getTerrainIntersectionPoint(intersection))
                {
                    dig(intersection, _toolSize, _toolDensity);
                }
                return true;
            }
        }
    }

    switch (event.getKey())
    {
    case iKeyCode::ESC:
        iApplication::getInstance().stop();
        return true;
    }

    return false;
}

bool Ascent::onKeyUp(iEventKeyUp &event)
{
    if (_activeControls)
    {
        Player *player = static_cast<Player *>(iEntityManager::getInstance().getEntity(_playerID));
        if (player != nullptr)
        {
            switch (event.getKey())
            {
            case iKeyCode::A:
                player->stopLeft();
                return true;

            case iKeyCode::D:
                player->stopRight();
                return true;

            case iKeyCode::W:
                player->stopForward();
                return true;

            case iKeyCode::S:
                player->stopBackward();
                return true;

            case iKeyCode::Q:
                player->stopUp();
                return true;

            case iKeyCode::E:
                player->stopDown();
                return true;

            case iKeyCode::LShift:
                player->stopFastTurn();
                return true;

            case iKeyCode::One:
                player->stopRollLeft();
                return true;

            case iKeyCode::Three:
                player->stopRollRight();
                return true;
            }
        }
    }
    return false;
}

bool Ascent::onWindowResize(iEventWindowResize &event)
{
    _viewOrtho.setOrthogonal(0, static_cast<float32>(getWindow()->getClientWidth()), static_cast<float32>(getWindow()->getClientHeight()), 0);
    return true;
}