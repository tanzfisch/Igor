// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "GameObject.h"

#include <igor/entities/iEntityManager.h>
using namespace igor;

GameObject::GameObject(Fraction fraction, GameObjectType type)
{
    _type = type;
    _fraction = fraction;
}

GameObject::~GameObject()
{
}

GameObjectType GameObject::getType() const
{
    return _type;
}

float32 GameObject::getDamage() const
{
    return _damage;
}

void GameObject::setDamage(float32 value)
{
    _damage = value;
}

float32 GameObject::getShieldDamage() const
{
    return _shieldDamage;
}

void GameObject::setShieldDamage(float32 value)
{
    _shieldDamage = value;
}

Fraction GameObject::getFraction() const
{
    return _fraction;
}

float32 GameObject::getShield() const
{
    return _shield;
}

float32 GameObject::getHealth() const
{
    return _health;
}

void GameObject::setShield(float32 value)
{
    _shield = value;
}

void GameObject::setHealth(float32 value)
{
    _health = value;
}

void GameObject::kill()
{
    iEntityManager::getInstance().destroyEntity(getID());
    _shield = 0;
    _health = 0;
}