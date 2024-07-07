// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/physics/iPhysicsMaterialCombo.h>

#include <igor/physics/iPhysicsMaterial.h>
#include <igor/physics/iPhysics.h>

namespace igor
{
    iPhysicsMaterialCombo::iPhysicsMaterialCombo(iPhysicsMaterial *material0, iPhysicsMaterial *material1)
    {
        _material0 = material0->getID();
        _material1 = material1->getID();

        iPhysics::getInstance().setCollisionCallback(this);
    }

    void iPhysicsMaterialCombo::setName(const iaString &name)
    {
        _name = name;
    }

    const iaString &iPhysicsMaterialCombo::getName() const
    {
        return _name;
    }

    void iPhysicsMaterialCombo::setSoftness(float32 value)
    {
        iPhysics::getInstance().setSoftness(this, value);
    }

    void iPhysicsMaterialCombo::setElasticity(float32 elasticCoef)
    {
        iPhysics::getInstance().setElasticity(this, elasticCoef);
    }

    void iPhysicsMaterialCombo::setFriction(float32 staticFriction, float32 kineticFriction)
    {
        iPhysics::getInstance().setFriction(this, staticFriction, kineticFriction);
    }

    void iPhysicsMaterialCombo::contact(iPhysicsBody *body0, iPhysicsBody *body1)
    {
        _contactEvent(body0, body1);
    }

    void iPhysicsMaterialCombo::registerContactDelegate(iContactDelegate contactDelegate)
    {
        _contactEvent.add(contactDelegate);
    }

    void iPhysicsMaterialCombo::unregisterContactDelegate(iContactDelegate contactDelegate)
    {
        _contactEvent.remove(contactDelegate);
    }

    int64 iPhysicsMaterialCombo::getMaterial0() const
    {
        return _material0;
    }

    int64 iPhysicsMaterialCombo::getMaterial1() const
    {
        return _material1;
    }

} // namespace igor