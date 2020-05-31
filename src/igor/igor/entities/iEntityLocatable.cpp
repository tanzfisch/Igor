// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntityLocatable.h>

namespace Igor
{
    iEntityLocatable::iEntityLocatable()
    {
        _type = iEntityType::Locatable;
    }

    iSphered iEntityLocatable::getSphere() const
    {
        return _sphere;
    }
}