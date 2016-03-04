// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysicsMaterialCombo.h>

#include <iPhysicsMaterial.h>
#include <iPhysics.h>

namespace Igor
{
    iPhysicsMaterialCombo::iPhysicsMaterialCombo(iPhysicsMaterial* material0, iPhysicsMaterial* material1)
    {
        _material0 = material0->getID();
        _material1 = material1->getID();

        iPhysics::getInstance().setCollisionCallback(this);
    }

    iPhysicsMaterialCombo::~iPhysicsMaterialCombo()
    {
        // TODO iPhysics::getInstance().UN?setCollisionCallback(this);
    }

    void iPhysicsMaterialCombo::setName(const iaString& name)
    {
        _name = name;
    }

    const iaString& iPhysicsMaterialCombo::getName() const
    {
        return _name;
    }

    void iPhysicsMaterialCombo::contact(iPhysicsBody* body0, iPhysicsBody* body1)
    {
        _contactEvent(body0, body1);
    }

    void iPhysicsMaterialCombo::registerContactDelegate(iContactDelegate contactDelegate)
    {
        _contactEvent.append(contactDelegate);
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


}