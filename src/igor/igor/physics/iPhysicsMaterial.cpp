// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/physics/iPhysicsMaterial.h>

#include <igor/physics/iPhysics.h>

namespace igor
{

    iPhysicsMaterial::iPhysicsMaterial(iPhysicsMaterialID id)
    {
        _id = id;
    }

    iPhysicsMaterialID iPhysicsMaterial::getID()
    {
        return _id;
    }

    void iPhysicsMaterial::setName(const iaString &name)
    {
        _name = name;
    }

    const iaString &iPhysicsMaterial::getName() const
    {
        return _name;
    }

} // namespace igor