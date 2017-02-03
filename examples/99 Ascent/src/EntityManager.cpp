#include "EntityManager.h"

#include <iPhysics.h>
#include <iPhysicsBody.h>
#include <iPhysicsMaterial.h>
#include <iPhysicsMaterialCombo.h>

EntityManager::EntityManager()
{
    _octree = new iOctree(iAACubed(iaVector3d(0, 0, 0), 40000.0), 50.0, 100, 20);

    initPhysics();
}

EntityManager::~EntityManager()
{
    delete _octree;
}

Entity* EntityManager::getEntity(uint64 entityID)
{
    Entity* result = nullptr;

    auto iter = _entities.find(entityID);
    if (iter != _entities.end())
    {
        result = (*iter).second;
    }

    return result;
}

void EntityManager::registerEntity(Entity* entity)
{
    _entities[entity->getID()] = entity;
    _octree->insert(entity->getID(), entity->getSphere());
}

int64 EntityManager::getTerrainMaterialID() const
{
    return _terrainMaterialID;
}

int64 EntityManager::getEntityMaterialID() const
{
    return _entityMaterialID;
}

int64 EntityManager::getBulletMaterialID() const
{
    return _bulletMaterialID;
}

void EntityManager::unregisterEntity(Entity* entity)
{
    auto iter = _entities.find(entity->getID());
    _octree->remove(entity->getID());
    _entities.erase(iter);
}

void EntityManager::getEntities(const iSphered& sphere, vector<uint64>& result)
{
    _octree->resetFilter();
    _octree->filter(sphere);
    _octree->getResult(result);
}

void EntityManager::getEntities(vector<uint64>& result)
{
    for (auto entity : _entities)
    {
        result.push_back(entity.second->getID());
    }
}

void EntityManager::initPhysics()
{
    iPhysicsMaterial* materialTerrain = iPhysics::getInstance().createMaterial("terrain");
    _terrainMaterialID = materialTerrain->getID();

    iPhysicsMaterial* materialEntity = iPhysics::getInstance().createMaterial("entity");
    _entityMaterialID = materialEntity->getID();

    iPhysicsMaterial* materialBullet = iPhysics::getInstance().createMaterial("bullet");
    _bulletMaterialID = materialBullet->getID();
    
    iPhysicsMaterialCombo* terrainEntity = new iPhysicsMaterialCombo(materialTerrain, materialEntity);
    terrainEntity->setName("terrain-entity");
    terrainEntity->registerContactDelegate(iContactDelegate(this, &EntityManager::onContact));
    terrainEntity->setElasticity(0.0);
    terrainEntity->setFriction(0.0, 0.0);

    iPhysicsMaterialCombo* terrainBullet = new iPhysicsMaterialCombo(materialTerrain, materialBullet);
    terrainBullet->setName("terrain-bullet");
    terrainBullet->registerContactDelegate(iContactDelegate(this, &EntityManager::onContactTerrainBullet));

    iPhysicsMaterialCombo* bulletEntity = new iPhysicsMaterialCombo(materialBullet, materialEntity);
    bulletEntity->setName("bullet-entity");
    bulletEntity->registerContactDelegate(iContactDelegate(this, &EntityManager::onContact));

    iPhysicsMaterialCombo* entityEntity = new iPhysicsMaterialCombo(materialEntity, materialEntity);
    entityEntity->setName("entity-entity");
    entityEntity->registerContactDelegate(iContactDelegate(this, &EntityManager::onContact));

    iPhysicsMaterialCombo* bulletBullet = new iPhysicsMaterialCombo(materialBullet, materialBullet);
    bulletBullet->setName("bullet-bullet");
    bulletBullet->registerContactDelegate(iContactDelegate(this, &EntityManager::onContact));
}

void EntityManager::onContactTerrainBullet(iPhysicsBody* body0, iPhysicsBody* body1)
{
    if (body0 != nullptr && body1 != nullptr)
    {
        if (body0->getUserData() != nullptr)
        {
            uint64 id0 = static_cast<uint64>(*static_cast<const uint64*>(body0->getUserData()));
            _hitListMutex.lock();
            _hitList.push_back(pair<uint64, uint64>(id0, 0));
            _hitListMutex.unlock();

        }
        else if (body1->getUserData() != nullptr)
        {
            uint64 id1 = static_cast<uint64>(*static_cast<const uint64*>(body1->getUserData()));
            _hitListMutex.lock();
            _hitList.push_back(pair<uint64, uint64>(id1, 0));
            _hitListMutex.unlock();
        }
    }
}

void EntityManager::onContact(iPhysicsBody* body0, iPhysicsBody* body1)
{
    if (body0 != nullptr && body1 != nullptr &&
        body0->getUserData() != nullptr &&
        body1->getUserData() != nullptr)
    {
        uint64 id0 = static_cast<uint64>(*static_cast<const uint64*>(body0->getUserData()));
        uint64 id1 = static_cast<uint64>(*static_cast<const uint64*>(body1->getUserData()));
        _hitListMutex.lock();
        _hitList.push_back(pair<uint64, uint64>(id0, id1));
        _hitList.push_back(pair<uint64, uint64>(id1, id0));
        _hitListMutex.unlock();
    }
}

void EntityManager::handle()
{
    _hitListMutex.lock();
    vector<pair<uint64, uint64>> hitList = _hitList;
    _hitList.clear();
    _hitListMutex.unlock();

    for (auto hit : hitList)
    {
        Entity* entity = getEntity(hit.first);
        if (entity != nullptr)
        {
            entity->hitBy(hit.second);
        }
    }

    for (auto entity : _entities)
    {
        entity.second->_sphere._center = entity.second->updatePos();
        _octree->update(entity.second->_id, entity.second->_sphere);
    }

    for (auto entity : _entities)
    {
        entity.second->handle();
    }

    vector<uint64> toDelete;
    for (auto entity : _entities)
    {
        if (entity.second->getHealth() <= 0.0f)
        {
            toDelete.push_back(entity.first);
        }
    }

    for (auto id : toDelete)
    {
        auto iter = _entities.find(id);
        if (iter != _entities.end())
        {
            delete (*iter).second;
        }
    }
}

