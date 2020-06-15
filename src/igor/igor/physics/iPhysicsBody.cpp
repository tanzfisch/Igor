// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/physics/iPhysicsBody.h>

#include <igor/physics/iPhysics.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodeManager.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    uint64 iPhysicsBody::_nextBodyID = INVALID_PHYSICSBODY_ID + 1;

    iPhysicsBody::iPhysicsBody(void *newtonBody)
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

    void iPhysicsBody::setUserData(const std::any &userData)
    {
        _userData = userData;
    }

    std::any iPhysicsBody::getUserData() const
    {
        return _userData;
    }

    void iPhysicsBody::applyForceAndTorque(float64 timestep)
    {
        _applyForceAndTorque(this, timestep);
    }

    void iPhysicsBody::bindTransformNode(iNodeTransform *transformNode)
    {
        con_assert(transformNode != nullptr, "zero pointer");

        if (transformNode != nullptr)
        {
            _transformNodeID = transformNode->getID();
        }
    }

    void *iPhysicsBody::getNewtonBody()
    {
        return _newtonBody;
    }

    void iPhysicsBody::release()
    {
        // in this case the newton body was already deleted by Newton it self we just set it's reference to zero
        _newtonBody = nullptr;
    }

    void iPhysicsBody::setTransformNodeMatrix(const iaMatrixd &matrix)
    {
        iNodeTransform *transformNode = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_transformNodeID));
        con_assert(transformNode != nullptr, "body " << _id << " is not bound to a node");

        if (transformNode != nullptr)
        {
            con_endl("setTransformNodeMatrix");
            transformNode->setMatrix(matrix);
        }
    }

    void iPhysicsBody::getTransformNodeMatrix(iaMatrixd &matrix) const
    {
        iNodeTransform *transformNode = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_transformNodeID));
        con_assert(transformNode != nullptr, "body is not bound to a node");

        if (transformNode != nullptr)
        {
            transformNode->getMatrix(matrix);
        }
    }

    void iPhysicsBody::getMatrix(iaMatrixd &matrix)
    {
        iPhysics::getInstance().getMatrix(_newtonBody, matrix);
    }

    void iPhysicsBody::setMatrix(const iaMatrixd &matrix)
    {
        iPhysics::getInstance().updateMatrix(_newtonBody, matrix);
    }

    const iaVector3d &iPhysicsBody::getForce() const
    {
        return _force;
    }

    void iPhysicsBody::setForce(const iaVector3d &force)
    {
        _force = force;
        iPhysics::getInstance().setForce(_newtonBody, _force);
    }

    const iaVector3d &iPhysicsBody::getTorque() const
    {
        return _torque;
    }

    void iPhysicsBody::setTorque(const iaVector3d &torque)
    {
        _torque = torque;
        iPhysics::getInstance().setTorque(_newtonBody, _torque);
    }

    iaVector3d iPhysicsBody::getVelocity() const
    {
        iaVector3d velocity;
        iPhysics::getInstance().getVelocity(_newtonBody, velocity);
        return velocity;
    }

    const iaVector3d &iPhysicsBody::getOmega() const
    {
        return _omega;
    }

    void iPhysicsBody::setOmega(const iaVector3d &omega)
    {
        _omega = omega;
    }

    uint64 iPhysicsBody::getID()
    {
        return _id;
    }

    void iPhysicsBody::setAngularDamping(const iaVector3d &damping)
    {
        _angularDamping = damping;
        iPhysics::getInstance().setAngularDamping(_newtonBody, _angularDamping);
    }

    const iaVector3d &iPhysicsBody::getAngularDamping() const
    {
        return _angularDamping;
    }

    void iPhysicsBody::setLinearDamping(float64 damping)
    {
        _linearDamping = damping;
        iPhysics::getInstance().setLinearDamping(_newtonBody, _linearDamping);
    }

    float64 iPhysicsBody::getLinearDamping() const
    {
        return _linearDamping;
    }

    void iPhysicsBody::setMass(float64 mass)
    {
        _mass = mass;
        iPhysics::getInstance().setMassMatrix(_newtonBody, mass, mass / 6.0f, mass / 6.0f, mass / 6.0f);
    }

    float64 iPhysicsBody::getMass()
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

} // namespace igor