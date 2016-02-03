// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysicsBody.h>

#include <iPhysics.h>
#include <iNodeTransform.h>
#include <iNodeFactory.h>

#include <iaConsole.h>

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

    uint32 iPhysicsBody::getTransformNode() const
    {
        return _transformNodeID;
    }

    void iPhysicsBody::ApplyForceAndTorque(float32 timestep, int threadIndex)
    {
        _applyForceAndTorque(this, timestep, threadIndex);
    }

    void iPhysicsBody::setTransformNode(iNodeTransform* transformNode)
    {
        con_assert(transformNode != nullptr, "zero pointer");

        if (transformNode != nullptr)
        {
            _transformNodeID = transformNode->getID();
            iaMatrixf matrix;
            transformNode->getMatrix(matrix);
            updateMatrix(matrix);
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

    void iPhysicsBody::setMatrix(const iaMatrixf& matrix)
    {
        iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
        con_assert_sticky(transformNode != nullptr, "body is not bound to a node");

        if (transformNode != nullptr)
        {
            transformNode->setMatrix(matrix);
        }
    }

    const iaMatrixf& iPhysicsBody::getMatrix() const
    {
        iaMatrixf result;
        iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
        con_assert_sticky(transformNode != nullptr, "body is not bound to a node");

        if (transformNode != nullptr)
        {
            transformNode->getMatrix(result);
        }

        return result;
    }

    void iPhysicsBody::updateMatrix(const iaMatrixf& matrix)
    {
        iPhysics::getInstance().updateMatrix(_newtonBody, matrix);
    }

    const iaVector3f& iPhysicsBody::getVelocity() const
    {
        return _velocity;
    }

    const iaVector3f& iPhysicsBody::getAngularVelocity() const
    {
        return _angularVelocity;
    }

    void iPhysicsBody::setVelocity(const iaVector3f& velocity)
    {
        _velocity = velocity;
    }

    void iPhysicsBody::setAngularVelocity(const iaVector3f& angularVelocity)
    {
        _angularVelocity = angularVelocity;
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