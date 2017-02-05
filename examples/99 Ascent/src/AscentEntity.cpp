#include "Entity.h"

#include "EntityManager.h"

uint64 Entity::_nextID = 1;

Entity::Entity(Fraction fraction, EntityType type)
{
    _type = type;
    _fraction = fraction;
    _id = _nextID++;

    EntityManager::getInstance().registerEntity(this);
}

Entity::~Entity()
{
    EntityManager::getInstance().unregisterEntity(this);
}

EntityType Entity::getType() const
{
    return _type;
}

uint64 Entity::getID()
{
    return _id;
}

float32 Entity::getDamage() const
{
    return _damage;
}

void Entity::setDamage(float32 value)
{
    _damage = value;
}

float32 Entity::getShieldDamage() const
{
    return _shieldDamage;
}

void Entity::setShieldDamage(float32 value)
{
    _shieldDamage = value;
}

Fraction Entity::getFraction() const
{
    return _fraction;
}

const iSphered& Entity::getSphere() const
{
    return _sphere;
}

float32 Entity::getShield() const
{
    return _shield;
}

float32 Entity::getHealth() const
{
    return _health;
}

void Entity::setShield(float32 value)
{
    _shield = value;
}

void Entity::setHealth(float32 value)
{
    _health = value;
}

void Entity::kill()
{
    _shield = 0;
    _health = 0;
}
