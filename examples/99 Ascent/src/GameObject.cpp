#include "GameObject.h"

iScene* GameObject::_scene = nullptr;

GameObject::GameObject(Fraction fraction, GameObjectKind kind)
{
    _fraction = fraction;
    _kind = kind;
}

GameObjectKind GameObject::getKind() const
{
    return _kind;
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
    _shield = 0;
    _health = 0;
}