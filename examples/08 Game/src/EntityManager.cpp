#include "EntityManager.h"


EntityManager::EntityManager()
{
    _octree = new iOctree(iAACubed(iaVector3d(0, 0, 0), 40000.0), 50.0, 8, 4);
}

EntityManager::~EntityManager()
{
    delete _octree;
}

uint64 EntityManager::createEntity()
{

}

void EntityManager::destroyEntity(uint64 entityID)
{

}