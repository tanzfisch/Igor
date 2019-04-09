// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysicsMaterial.h>

#include <iPhysics.h>

namespace Igor
{

    iPhysicsMaterial::iPhysicsMaterial(int64 id)
    {
        _id = id;
    }

    int64 iPhysicsMaterial::getID()
    {
        return _id;
    }

    void iPhysicsMaterial::setName(const iaString& name)
    {
        _name = name;
    }

    const iaString& iPhysicsMaterial::getName() const
    {
        return _name;
    }


}