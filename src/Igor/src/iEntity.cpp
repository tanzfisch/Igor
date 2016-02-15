// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntity.h>

#include <iPhysicsBody.h>
#include <iPhysics.h>
#include <iNodeTransform.h>
using namespace Igor;

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    uint32 iEntity::getID()
    {
        return _id;
    }

}