// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntityPositioned.h>

namespace Igor
{

    iEntityPositioned::iEntityPositioned()
    {

    }

    iEntityPositioned::~iEntityPositioned()
    {

    }

    iEntity* iEntityPositioned::createInstance()
    {
        return new iEntityPositioned();
    }

    bool iEntityPositioned::hasPosition() const
    {
        return true;
    }

}
