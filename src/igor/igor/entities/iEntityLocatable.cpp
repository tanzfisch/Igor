// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntityLocatable.h>

namespace igor
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