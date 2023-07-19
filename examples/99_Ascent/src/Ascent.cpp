// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "Ascent.h"

Ascent::Ascent(iWindowPtr window)
    : ExampleBase(window, "Ascent")
{
}

void Ascent::onInit()
{
    initScene();
    initPhysics();
}

void Ascent::onUpdate()
{
    // runs 100 times per second so we don't need to care about timing here

    iaMatrixd matrix;
    _cameraTransform->getMatrix(matrix);

    const float64 speed = 0.5;
    const float64 rollSpeed = 0.01;

    iaVector3d translate;
    if (_keyInput._front)
    {
        translate._z -= 1;
    }

    if (_keyInput._back)
    {
        translate._z += 1;
    }

    if (_keyInput._left)
    {
        translate._x -= 1;
    }
    
    if (_keyInput._right)
    {
        translate._x += 1;
    }

    if (_keyInput._up)
    {
        translate._y += 1;
    }
    
    if (_keyInput._down)
    {
        translate._y -= 1;
    }

    translate.normalize();
    translate *= speed;

    matrix.move(translate);

    float64 roll = 0.0;

    if (_keyInput._rollLeft)
    {
        roll += rollSpeed;
    }

    if (_keyInput._rollRight)
    {
        roll -= rollSpeed;
    }

    matrix.rotate(roll, iaAxis::Z);

    _cameraTransform->setMatrix(matrix);
}

void Ascent::onDeinit()
{
    // TODO
}

void Ascent::initScene()
{
    // create a directional light
    // transform node
    iNodeTransform *lightTranslate = iNodeManager::getInstance().createNode<iNodeTransform>();
    lightTranslate->translate(100, 100, 100);
    // and light node
    iNodeLight *lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
    lightNode->setAmbient(0.6f, 0.6f, 0.6f);
    lightNode->setDiffuse(0.9f, 0.7f, 0.6f);
    lightNode->setSpecular(1.0f, 0.9f, 0.87f);
    // and add it to the scene
    getScene()->getRoot()->insertNode(lightTranslate);
    lightTranslate->insertNode(lightNode);

    // create a skybox
    iNodeSkyBox *skyBoxNode = iNodeManager::getInstance().createNode<iNodeSkyBox>();
    // set it up with the default skybox texture
    skyBoxNode->setTexture(iResourceManager::getInstance().requestResource<iTexture>("skyboxes/stars.png"));
    // create a material for the sky box because the default material for all iNodeRender and deriving classes has no textures and uses depth test
    iMaterialPtr materialSkyBox = iMaterialResourceFactory::getInstance().loadMaterial("skybox.mat");
    materialSkyBox->setOrder(iMaterial::RENDER_ORDER_MIN);
    // set that material
    skyBoxNode->setMaterial(materialSkyBox);
    // and add it to the scene
    getScene()->getRoot()->insertNode(skyBoxNode);

    // create cam with LOD trigger
    _cameraTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    _cameraTransform->translate(100000, 100000, 100300);
    iNodeCamera *camera = iNodeManager::getInstance().createNode<iNodeCamera>();
    iNodeLODTrigger *lodtrigger = iNodeManager::getInstance().createNode<iNodeLODTrigger>();
    _cameraTransform->insertNode(camera);
    camera->insertNode(lodtrigger);
    getScene()->getRoot()->insertNode(_cameraTransform);
    getView().setCurrentCamera(camera->getID());

    initVoxelData(lodtrigger->getID());
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

void Ascent::initVoxelData(uint32 lodTriggerID)
{
    _voxelTerrain = std::unique_ptr<iVoxelTerrain>(
        new iVoxelTerrain(iVoxelTerrainGenerateDelegate(this, &Ascent::onGenerateVoxelData),
                          iVoxelTerrainPlacePropsDelegate(this, &Ascent::onVoxelDataGenerated), 7));

    // set up voxel mesh material
    _voxelMeshMaterial = iMaterialResourceFactory::getInstance().loadMaterial("voxel_terrain_directional_light.mat");
    _voxelTerrain->setMaterial(_voxelMeshMaterial);

    // set up voxel mesh target material
    iTargetMaterialPtr targetMaterial = _voxelTerrain->getTargetMaterial();
    targetMaterial->setTexture(iResourceManager::getInstance().requestResource<iTexture>("crates2.png"), 0);
    targetMaterial->setTexture(iResourceManager::getInstance().requestResource<iTexture>("crates2.png"), 1);
    targetMaterial->setTexture(iResourceManager::getInstance().requestResource<iTexture>("crates2.png"), 2);
    targetMaterial->setTexture(iResourceManager::getInstance().requestResource<iTexture>("detail.png"), 3);
    targetMaterial->setAmbient(iaColor3f(0.1f, 0.1f, 0.1f));
    targetMaterial->setDiffuse(iaColor3f(0.9f, 0.9f, 0.9f));
    targetMaterial->setSpecular(iaColor3f(1.0f, 1.0f, 1.0f));
    targetMaterial->setEmissive(iaColor3f(0.0f, 0.0f, 0.0f));
    targetMaterial->setShininess(1000.0f);

    oulineLevelStructure();

    _voxelTerrain->setPhysicsMaterialID(_terrainMaterialID);

    // add to scene
    _voxelTerrain->setScene(getScene());
    _voxelTerrain->setLODTrigger(lodTriggerID);
}

void Ascent::oulineLevelStructure()
{
    iaVector3d bossPosition(100000, 100000, 100000);

    // covering the boss
    _metaballs.push_back(iaSphered(iaVector3d(bossPosition._x + 20, bossPosition._y, bossPosition._z), 1.7));
    _metaballs.push_back(iaSphered(iaVector3d(bossPosition._x - 20, bossPosition._y, bossPosition._z), 1.7));
    _metaballs.push_back(iaSphered(iaVector3d(bossPosition._x, bossPosition._y + 20, bossPosition._z), 1.7));
    _metaballs.push_back(iaSphered(iaVector3d(bossPosition._x, bossPosition._y - 20, bossPosition._z), 1.7));
    _metaballs.push_back(iaSphered(iaVector3d(bossPosition._x, bossPosition._y, bossPosition._z + 20), 1.7));
    _metaballs.push_back(iaSphered(iaVector3d(bossPosition._x, bossPosition._y, bossPosition._z - 20), 1.7));

    // hole where the boss sits
    _holes.push_back(iaSphered(iaVector3d(bossPosition._x, bossPosition._y, bossPosition._z), 1.7));

    // body
    for (int i = 0; i < 15; ++i)
    {
        iaVector3d pos(static_cast<int32>(iaRandom::getNext() % 50) - 25,
                       static_cast<int32>(iaRandom::getNext() % 50) - 25,
                       static_cast<int32>(iaRandom::getNext() % 50) - 25);
        pos.normalize();
        pos *= 20 + (iaRandom::getNext() % 40);
        pos += bossPosition;

        _metaballs.push_back(iaSphered(pos, ((iaRandom::getNext() % 50 + 50) / 100.0) * 4.0));
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
        // TODO _hitListMutex.lock();
        // TODO _hitList.push_back(std::pair<uint64, uint64>(id0, 0));
        // TODO _hitListMutex.unlock();
    }
    else if (userData1.has_value())
    {
        uint64 id1 = std::any_cast<uint64>(userData1);
        // TODO _hitListMutex.lock();
        // TODO _hitList.push_back(std::pair<uint64, uint64>(id1, 0));
        // TODO _hitListMutex.unlock();
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
        // TODO _hitListMutex.lock();
        // TODO _hitList.push_back(std::pair<uint64, uint64>(id0, id1));
        // TODO _hitList.push_back(std::pair<uint64, uint64>(id1, id0));
        // TODO _hitListMutex.unlock();
    }
}

void Ascent::onVoxelDataGenerated(iVoxelBlockPropsInfo voxelBlockPropsInfo)
{
    /*iaVector3I diff;
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

            iaSphered sphere(creationPos, 5);
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
                iaSphered sphere(iaVector3d(outside._x, outside._y, outside._z), 5);
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
    }*/
}

bool Ascent::getTerrainIntersectionPoint(iaVector3I &intersection)
{
    iNodeCamera *camera = static_cast<iNodeCamera *>(iNodeManager::getInstance().getNode(getView().getCurrentCamera()));
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

    iaSphereI sphere;
    sphere._center = position;
    sphere._radius = toolSize;
    _voxelTerrain->modify(sphere, density);

    iaMatrixd effectMatrix;
    effectMatrix.translate(position._x, position._y, position._z);
    // TODO new DigEffect(_scene, effectMatrix);
}

void Ascent::onRender()
{
    // nothing to do
}

void Ascent::handleHitList()
{
    /*_hitListMutex.lock();
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
    }*/
}

void Ascent::onRenderOrtho()
{
    // TODO
}

void Ascent::onEvent(iEvent &event)
{
    event.dispatch<iEventMouseKeyDown>(IGOR_BIND_EVENT_FUNCTION(Ascent::onMouseKeyDownEvent));
    event.dispatch<iEventMouseMove>(IGOR_BIND_EVENT_FUNCTION(Ascent::onMouseMoveEvent));
    event.dispatch<iEventMouseWheel>(IGOR_BIND_EVENT_FUNCTION(Ascent::onMouseWheelEvent));
    event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(Ascent::onKeyDown));
    event.dispatch<iEventKeyUp>(IGOR_BIND_EVENT_FUNCTION(Ascent::onKeyUp));
}

bool Ascent::onMouseKeyDownEvent(iEventMouseKeyDown &event)
{
    /*if (_activeControls)
    {
        Player *player = static_cast<Player *>(iEntityManager::getInstance().getEntity(_playerID));
        if (player != nullptr)
        {
             if (key == iKeyCode::MouseRight)
             {
                 iaVector3d updown(_weaponPos._x, _weaponPos._y, _weaponPos._z);
                 player->shootSecondaryWeapon(_view, updown);
             }

            if (event.getKey() == iKeyCode::MouseLeft)
            {
                iaVector3d updown(_weaponPos._x, _weaponPos._y, _weaponPos._z);
                player->shootPrimaryWeapon(_view, updown);
                return true;
            }
        }
    }
*/
    return false;
}

bool Ascent::onMouseMoveEvent(iEventMouseMove &event)
{
    /*if (_activeControls)
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
*/
    return false;
}

bool Ascent::onMouseWheelEvent(iEventMouseWheel &event)
{
    /*if (_activeControls)
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
    }*/

    return false;
}

bool Ascent::onKeyDown(iEventKeyDown &event)
{
    switch (event.getKey())
    {
    case iKeyCode::A:
        _keyInput._left = true;
        return true;

    case iKeyCode::D:
        _keyInput._right = true;
        return true;

    case iKeyCode::W:
        _keyInput._front = true;
        return true;

    case iKeyCode::S:
        _keyInput._back = true;
        return true;

    case iKeyCode::Q:
        _keyInput._up = true;
        return true;

    case iKeyCode::E:
        _keyInput._down = true;
        return true;

    case iKeyCode::One:
        _keyInput._rollLeft = true;
        return true;

    case iKeyCode::Three:
        _keyInput._rollRight = true;
        return true;

    case iKeyCode::Space:
        // TODO
        return true;
    }

    return false;
}

bool Ascent::onKeyUp(iEventKeyUp &event)
{
    switch (event.getKey())
    {
    case iKeyCode::A:
        _keyInput._left = false;
        return true;

    case iKeyCode::D:
        _keyInput._right = false;
        return true;

    case iKeyCode::W:
        _keyInput._front = false;
        return true;

    case iKeyCode::S:
        _keyInput._back = false;
        return true;

    case iKeyCode::Q:
        _keyInput._up = false;
        return true;

    case iKeyCode::E:
        _keyInput._down = false;
        return true;

    case iKeyCode::One:
        _keyInput._rollLeft = false;
        return true;

    case iKeyCode::Three:
        _keyInput._rollRight = false;
        return true;
    }

    return ExampleBase::onKeyUp(event);
}
