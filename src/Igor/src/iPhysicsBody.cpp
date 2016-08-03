// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysicsBody.h>

#include <iPhysics.h>
#include <iNodeTransform.h>
#include <iNodeFactory.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    uint64 iPhysicsBody::_nextBodyID = INVALID_PHYSICSBODY_ID + 1;

    iPhysicsBody::iPhysicsBody(void* newtonBody)
    {
        con_assert(newtonBody != nullptr, "zero pointer");

        _newtonBody = newtonBody;

        _mutex.lock();
        _id = _nextBodyID++;
        _mutex.unlock();
    }

    iPhysicsBody::~iPhysicsBody()
    {
        if (nullptr != _newtonBody)
        {
            iPhysics::getInstance().destroyNewtonBody(_newtonBody);
            _newtonBody = nullptr;
        }
    }

    void iPhysicsBody::setMaterial(int64 id)
    {
        _materialID = id;

        iPhysics::getInstance().setMaterial(this, _materialID);
    }

    int64 iPhysicsBody::getMaterial() const
    {
        return _materialID;
    }

    uint32 iPhysicsBody::getTransformNodeID() const
    {
        return _transformNodeID;
    }

    void iPhysicsBody::setUserData(const void* userData)
    {
        _userData = userData;
    }

    const void* iPhysicsBody::getUserData() const
    {
        return _userData;
    }

    void iPhysicsBody::applyForceAndTorque(float64 timestep, int threadIndex)
    {
        _applyForceAndTorque(this, timestep, threadIndex);
    }

    void iPhysicsBody::bindTransformNode(iNodeTransform* transformNode)
    {
        con_assert(transformNode != nullptr, "zero pointer");

        if (transformNode != nullptr)
        {
            _transformNodeID = transformNode->getID();
        }
    }

    void* iPhysicsBody::getNewtonBody()
    {
        return _newtonBody;
    }

    void iPhysicsBody::release()
    {
        // in this case the newton body was already deleted by Newton it self
        _newtonBody = nullptr;
    }

    void iPhysicsBody::setTransformNodeMatrix(const iaMatrixf& matrix)
    {
        if (_transformNodeID != iNode::INVALID_NODE_ID)
        {
            iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
            con_assert(transformNode != nullptr, "body " << _id << " is not bound to a node");

            if (transformNode != nullptr)
            {
                transformNode->setMatrix(matrix);
            }
        }
    }

    iaMatrixf iPhysicsBody::getTransformNodeMatrix() const
    {
        iaMatrixf result;

        if (_transformNodeID != iNode::INVALID_NODE_ID)
        {
            iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
            con_assert(transformNode != nullptr, "body is not bound to a node");

            if (transformNode != nullptr)
            {
                transformNode->getMatrix(result);
            }
        }

        return result;
    }

    void iPhysicsBody::setMatrix(const iaMatrixf& matrix)
    {
        iPhysics::getInstance().updateMatrix(_newtonBody, matrix);
    }

    const iaVector3f& iPhysicsBody::getForce() const
    {
        return _force;
    }

    void iPhysicsBody::setForce(const iaVector3f& force)
    {
        _force = force;
        iPhysics::getInstance().setForce(_newtonBody, _force);
    }

    const iaVector3f& iPhysicsBody::getTorque() const
    {
        return _torque;
    }

    void iPhysicsBody::setTorque(const iaVector3f& torque)
    {
        _torque = torque;
        iPhysics::getInstance().setTorque(_newtonBody, _torque);
    }

    iaVector3f iPhysicsBody::getVelocity() const
    {
        iaVector3f velocity;
        iPhysics::getInstance().getVelocity(_newtonBody, velocity);
        return velocity;
    }

    const iaVector3f& iPhysicsBody::getOmega() const
    {
        return _omega;
    }

    void iPhysicsBody::setOmega(const iaVector3f& omega)
    {
        _omega = omega;
    }

    uint64 iPhysicsBody::getID()
    {
        return _id;
    }

    void iPhysicsBody::setAngularDamping(const iaVector3f& damping)
    {
        _angularDamping = damping;
        iPhysics::getInstance().setAngularDamping(_newtonBody, _angularDamping);
    }

    const iaVector3f& iPhysicsBody::getAngularDamping() const
    {
        return _angularDamping;
    }

    void iPhysicsBody::setLinearDamping(float32 damping)
    {
        _linearDamping = damping;
        iPhysics::getInstance().setLinearDamping(_newtonBody, _linearDamping);
    }

    float32 iPhysicsBody::getLinearDamping() const
    {
        return _linearDamping;
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