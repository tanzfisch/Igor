// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/physics/iPhysicsJoint.h>

#include <igor/physics/iPhysics.h>

namespace igor
{

    uint64 iPhysicsJoint::_nextJointID = 1;
    iaMutex iPhysicsJoint::_mutex;

    iPhysicsJoint::iPhysicsJoint(void *joint, uint64 body0, uint64 body1)
    {
        _bodyID0 = body0;
        _bodyID1 = body1;
        _joint = joint;
        _mutex.lock();
        _jointID = _nextJointID++;
        _mutex.unlock();
    }

    uint64 iPhysicsJoint::getBody0ID()
    {
        return _bodyID0;
    }

    uint64 iPhysicsJoint::getBody1ID()
    {
        return _bodyID1;
    }

    uint64 iPhysicsJoint::getID()
    {
        return _jointID;
    }

    void *iPhysicsJoint::getNewtonJoint() const
    {
        return _joint;
    }

    void iPhysicsJoint::submitConstraints(float64 timestep)
    {
        _submitConstraints(this, timestep);
    }

    void iPhysicsJoint::registerSubmitConstraintsDelegate(iSubmitConstraintsDelegate submitConstraintsDelegate)
    {
        _submitConstraints.add(submitConstraintsDelegate);
    }

    void iPhysicsJoint::unregisterSubmitConstraintsDelegate(iSubmitConstraintsDelegate submitConstraintsDelegate)
    {
        _submitConstraints.remove(submitConstraintsDelegate);
    }

} // namespace igor