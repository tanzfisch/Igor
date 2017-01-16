// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntityPositioned.h>

namespace Igor
{

    bool iEntityPositioned::hasPosition() const
    {
        return true;
    }

    iaVector3d iEntityPositioned::getPosition() const
    {
        return _sphere._center;
    }

    iSphered iEntityPositioned::getSphere() const
    {
        return _sphere;
    }

}
