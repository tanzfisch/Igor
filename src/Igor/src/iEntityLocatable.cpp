// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntityLocatable.h>

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