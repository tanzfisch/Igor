#include "Entity.h"

#include "EntityManager.h"

uint64 Entity::_nextID = 1;

Entity::Entity(uint32 fraction)
{
    _fraction = fraction;
    _id = _nextID++;

    EntityManager::getInstance().registerEntity(this);
}

Entity::~Entity()
{
    EntityManager::getInstance().unregisterEntity(this);
}

void Entity::syncPosition()
{
    EntityManager::getInstance().updateEntityPos(this);
}

uint64 Entity::getID()
{
    return _id;
}

uint32 Entity::getFraction()
{
    return _fraction;
}

const iSpheref& Entity::getSphere() const
{
    return _sphere;
}