// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include "Entity.h"

#include "EntityManager.h"

uint64 Entity::_nextID = Entity::INVALID_ENTITY_ID + 1;
mutex Entity::_mutexID;

Entity::Entity()
{
    _mutexID.lock();
    _id = _nextID++;
    _mutexID.unlock();
}

void Entity::kill()
{
    _state = EntityState::Delete;
}

void Entity::setPosition(const iaVector3d& position)
{
    _sphere._center = position;
}

iaVector3d Entity::getPosition() const
{
    return _sphere._center;
}

iSphered Entity::getSphere() const
{
    return _sphere;
}

uint64 Entity::getID() const
{
    return _id;
}

void Entity::setRadius(float32 radius)
{
    _sphere._radius = radius;
}