#include "EntityManager.h"

EntityManager::EntityManager()
{
    _octree = new iOctree(iAACubed(iaVector3d(0, 0, 0), 40000.0), 50.0, 8, 4);
}

EntityManager::~EntityManager()
{
    delete _octree;
}

void EntityManager::updateEntityPos(Entity* entity)
{
    _octree->update(entity->getID(), entity->getSphere());
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

void EntityManager::handle()
{
    for (auto entity : _entities)
    {
        entity.second->handle();
    }
}
