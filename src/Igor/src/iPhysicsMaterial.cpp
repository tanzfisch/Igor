// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysicsJoint.h>

#include <iPhysics.h>

namespace Igor
{

    iPhysicsJoint::iPhysicsJoint(void* joint, uint64 jointID)
    {
        _joint = joint;
        _jointID = jointID;
    }

    uint64 iPhysicsJoint::getID()
    {
        return _jointID;
    }

    void* iPhysicsJoint::getJoint() const
    {
        return _joint;
    }

}