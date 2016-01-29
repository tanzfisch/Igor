// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysicsBody.h>

#include <iPhysics.h>

namespace Igor
{

    iPhysicsBody::iPhysicsBody(void* newtonBody, uint64 bodyID)
        : _newtonBody(newtonBody)
    {
        _id = bodyID;
    }

    iPhysicsBody::~iPhysicsBody()
    {
        if (nullptr != _newtonBody)
        {
            iPhysics::getInstance().destroyNewtonBody(_newtonBody);
            _newtonBody = nullptr;
        }
    }

    void iPhysicsBody::ApplyForceAndTorque(float32 timestep, int threadIndex)
    {
        _applyForceAndTorque(this, timestep, threadIndex);
    }

    void iPhysicsBody::setEntity(iEntity* entity)
    {
        _entity = entity;
    }

    void* iPhysicsBody::getNewtonBody()
    {
        return _newtonBody;
    }

    iEntity* iPhysicsBody::getEntity() const
    {
        return _entity;
    }

    void iPhysicsBody::release()
    {
        // in this case the newton body was already deleted by Newton it self
        _newtonBody = nullptr;
    }

    void iPhysicsBody::updateMatrix(const iaMatrixf& matrix)
    {
        iPhysics::getInstance().updateMatrix(_newtonBody, matrix);
    }

    uint64 iPhysicsBody::getID()
    {
        return _id;
    }

    void iPhysicsBody::setMass(float32 mass)
    {
        _mass = mass;
        iPhysics::getInstance().setMassMatrix(_newtonBody, mass, mass / 6.0f, mass / 6.0f, mass / 6.0f);
    }

    float32 iPhysicsBody::getMass()
    {
        return _mass;
    }

    void iPhysicsBody::registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate applyForceAndTorqueDelegate)
    {
        _applyForceAndTorque.append(applyForceAndTorqueDelegate);
    }

    void iPhysicsBody::unregisterForceAndTorqueDelegate(iApplyForceAndTorqueDelegate applyForceAndTorqueDelegate)
    {
        _applyForceAndTorque.remove(applyForceAndTorqueDelegate);
    }

}