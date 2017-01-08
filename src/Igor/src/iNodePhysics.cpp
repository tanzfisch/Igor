// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodePhysics.h>

#include <iTaskManager.h>
#include <iTaskPrepareCollision.h>
#include <iNodeTransform.h>
#include <iScene.h>
#include <iPhysics.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iNodePhysics::iNodePhysics()
        : iNodeVolume()
    {
        _physicsCollisionConfigID = iPhysics::getInstance().createCollisionConfig()->getID();

        setName(L"iNodePhysics");
        _nodeType = iNodeType::iNodePhysics;
        _nodeKind = iNodeKind::Physics;

        setMaterial(iMaterial::INVALID_MATERIAL_ID); // physics should only render for debugging purposes        
    }

    iNodePhysics::iNodePhysics(const iNodePhysics* node)
        : iNodeVolume()
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = node->_nodeType;
        _nodeKind = node->_nodeKind;

        setName(node->getName());
        _bodyID = node->getBodyID();

        _physicsCollisionConfigID = node->_physicsCollisionConfigID; // todo create copy here

        _mass = node->_mass;
        _materialID = node->_materialID;

        if (node->_applyForceAndTorqueDelegate != nullptr)
        {
            _applyForceAndTorqueDelegate = new iApplyForceAndTorqueDelegate(*(node->_applyForceAndTorqueDelegate));
        }

        setDataDirty();
    }

    iNodePhysics::~iNodePhysics()
    {
        iTaskManager::getInstance().unregisterTaskFinishedDelegate(iTaskFinishedDelegate(this, &iNodePhysics::onTaskFinished));

		setScene(nullptr);

        if (iPhysics::getInstance().isCollisionConfig(_physicsCollisionConfigID))
        {
            iPhysics::getInstance().destroyCollisionConfig(_physicsCollisionConfigID);
        }

        if (iPhysics::getInstance().isBody(_bodyID))
        {
            iPhysics::getInstance().destroyBody(_bodyID);
        }
    }

    void iNodePhysics::setUserData(const void* userData)
    {
        _userData = userData;

        iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
        if (body != nullptr)
        {
            body->setUserData(_userData);
        }
    }

    const void* iNodePhysics::getUserData() const
    {
        return _userData;
    }
    const iaVector3d& iNodePhysics::getForce() const
    {
        return _force;
    }

    void iNodePhysics::setForce(const iaVector3d& force)
    {
        _force = force;

        iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
        if (body != nullptr)
        {
            body->setForce(_force);
        }
    }

    const iaVector3d& iNodePhysics::getTorque() const
    {
        return _torque;
    }

    void iNodePhysics::setTorque(const iaVector3d& torque)
    {
        _torque = torque;

        iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
        if (body != nullptr)
        {
            body->setTorque(_torque);
        }
    }

    iaVector3d iNodePhysics::getVelocity() const
    {
        iaVector3d velocity;

        iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
        if (body != nullptr)
        {
            velocity = body->getVelocity();
        }
        
        return velocity;
    }

    iaVector3d iNodePhysics::getOmega() const
    {
        iaVector3d omega;

        iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
        if (body != nullptr)
        {
            omega = body->getOmega();
        }

        return omega;
    }

    void iNodePhysics::setAngularDamping(const iaVector3d& damping)
    {
        _angularDamping = damping;

        iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
        if (body != nullptr)
        {
            body->setAngularDamping(_angularDamping);
        }
    }

    const iaVector3d& iNodePhysics::getAngularDamping() const
    {
        return _angularDamping;
    }

    void iNodePhysics::setLinearDamping(float64 damping)
    {
        _linearDamping = damping;

        iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
        if (body != nullptr)
        {
            body->setLinearDamping(_linearDamping);
        }
    }

    float64 iNodePhysics::getLinearDamping() const
    {
        return _linearDamping;
    }

    bool iNodePhysics::isInitialized()
    {
        return _physicsInitialized;
    }

    uint64 iNodePhysics::getBodyID() const
    {
        return _bodyID;
    }

    void iNodePhysics::onUpdateTransform(iaMatrixd& matrix)
    {
        iNodeVolume::onUpdateTransform(matrix);
    }

    void iNodePhysics::clear()
    {

    }

    void iNodePhysics::addBox(float32 width, float32 height, float32 depth, const iaMatrixf& offset)
    {
        iPhysicsCollisionConfig* physicsCollisionConfig = iPhysics::getInstance().getCollisionConfig(_physicsCollisionConfigID);
        if (physicsCollisionConfig)
        {
            physicsCollisionConfig->addBox(width, height, depth, offset);
        }
    }

    void iNodePhysics::addSphere(float32 radius, const iaMatrixf& offset)
    {
        iPhysicsCollisionConfig* physicsCollisionConfig = iPhysics::getInstance().getCollisionConfig(_physicsCollisionConfigID);
        if (physicsCollisionConfig)
        {
            physicsCollisionConfig->addSphere(radius, offset);
        }
    }

    void iNodePhysics::addCone(float32 radius, float32 height, const iaMatrixf& offset)
    {
        iPhysicsCollisionConfig* physicsCollisionConfig = iPhysics::getInstance().getCollisionConfig(_physicsCollisionConfigID);
        if (physicsCollisionConfig)
        {
            physicsCollisionConfig->addCone(radius, height, offset);
        }
    }

    void iNodePhysics::addCapsule(float32 radius, float32 height, const iaMatrixf& offset)
    {
        iPhysicsCollisionConfig* physicsCollisionConfig = iPhysics::getInstance().getCollisionConfig(_physicsCollisionConfigID);
        if (physicsCollisionConfig)
        {
            physicsCollisionConfig->addCapsule(radius, height, offset);
        }
    }

    void iNodePhysics::addCylinder(float32 radius, float32 height, const iaMatrixf& offset)
    {
        iPhysicsCollisionConfig* physicsCollisionConfig = iPhysics::getInstance().getCollisionConfig(_physicsCollisionConfigID);
        if (physicsCollisionConfig)
        {
            physicsCollisionConfig->addCylinder(radius, height, offset);
        }
    }

    void iNodePhysics::addMesh(shared_ptr<iMesh> mesh, int64 faceAttribute, const iaMatrixf& offset)
    {
        iPhysicsCollisionConfig* physicsCollisionConfig = iPhysics::getInstance().getCollisionConfig(_physicsCollisionConfigID);
        if (physicsCollisionConfig)
        {
            physicsCollisionConfig->addMesh(mesh, faceAttribute, offset);
        }
    }

    void iNodePhysics::finalizeCollision(bool asynchronos)
    {
        if (asynchronos)
        {
            iTaskManager::getInstance().registerTaskFinishedDelegate(iTaskFinishedDelegate(this, &iNodePhysics::onTaskFinished));
            _prepareCollisionTask = iTaskManager::getInstance().addTask(new iTaskPrepareCollision(_physicsCollisionConfigID));
        }
        else
        {
            iPhysicsCollisionConfig* collisionConfig = iPhysics::getInstance().getCollisionConfig(_physicsCollisionConfigID);
            if (collisionConfig != nullptr)
            {
                collisionConfig->finalize(iPhysics::getInstance().getDefaultWorldID());
                setDataDirty();
            }
        }
    }

    void iNodePhysics::onTaskFinished(uint64 taskID)
    {
        if (_prepareCollisionTask == taskID)
        {
            setDataDirty();
            iTaskManager::getInstance().unregisterTaskFinishedDelegate(iTaskFinishedDelegate(this, &iNodePhysics::onTaskFinished));

            _prepareCollisionTask = iTask::INVALID_TASK_ID;
        }
    }

    void iNodePhysics::setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate applyForceAndTorqueDelegate)
    {
        con_assert(!isInitialized(), "already initialized");

        if (!isInitialized())
        {
            if (_applyForceAndTorqueDelegate != nullptr)
            {
                delete _applyForceAndTorqueDelegate;
                _applyForceAndTorqueDelegate = nullptr;
            }

            _applyForceAndTorqueDelegate = new iApplyForceAndTorqueDelegate(applyForceAndTorqueDelegate);

            iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
            if (body != nullptr)
            {
                body->registerForceAndTorqueDelegate(*_applyForceAndTorqueDelegate);
            }
        }
    }

    void iNodePhysics::resetForceAndTorqueDelegate()
    {
        con_assert(!isInitialized(), "already initialized");

        if (!isInitialized())
        {
            if (_applyForceAndTorqueDelegate != nullptr)
            {
                iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
                if (body != nullptr)
                {
                    body->unregisterForceAndTorqueDelegate(*_applyForceAndTorqueDelegate);
                }

                delete _applyForceAndTorqueDelegate;
                _applyForceAndTorqueDelegate = nullptr;
            }
        }
    }

    void iNodePhysics::initPhysics()
    {
        if (!_physicsInitialized)
        {
            iPhysicsCollisionConfig* physicsCollisionConfig = iPhysics::getInstance().getCollisionConfig(_physicsCollisionConfigID);
            if (physicsCollisionConfig && physicsCollisionConfig->isFinalized())
            {
                iPhysicsCollision* collision = iPhysics::getInstance().getCollision(physicsCollisionConfig->getCollisionID());
                iPhysicsBody* body = iPhysics::getInstance().createBody(collision);
                body->setMass(_mass);
                body->setMaterial(_materialID);
                body->setUserData(getUserData());
                body->setLinearDamping(_linearDamping);
                body->setAngularDamping(_angularDamping);
                body->setForce(_force);
                body->setTorque(_torque);

                if (_applyForceAndTorqueDelegate != nullptr)
                {
                    body->registerForceAndTorqueDelegate(*_applyForceAndTorqueDelegate);
                    delete _applyForceAndTorqueDelegate;
                    _applyForceAndTorqueDelegate = nullptr;
                }

                _bodyID = body->getID();

                iPhysics::getInstance().destroyCollision(collision);
                iPhysics::getInstance().destroyCollisionConfig(_physicsCollisionConfigID);
                _physicsCollisionConfigID = iPhysicsCollisionConfig::INVALID_COLLISIONCONFIG_ID;

                _physicsInitialized = true;
            }
        }
    }

    void iNodePhysics::onPreSetScene()
    {
        iNodeVolume::onPreSetScene();
    }

    void iNodePhysics::onPostSetScene()
    {
        iNodeVolume::onPostSetScene();

        if (getScene())
        {
            if (isDataDirty())
            {
                getScene()->addToDataUpdateQueue(this);
            }
        }
    }

    float32 iNodePhysics::getMass() const
    {
        return _mass;
    }

    void iNodePhysics::setMass(float32 mass)
    {
        _mass = mass;

        if (_bodyID != iPhysicsBody::INVALID_PHYSICSBODY_ID)
        {
            iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
            if (body != nullptr)
            {
                body->setMass(_mass);
            }
        }
    }

    void iNodePhysics::setMaterial(int64 materialID)
    {
        _materialID = materialID;

        if (_bodyID != iPhysicsBody::INVALID_PHYSICSBODY_ID)
        {
            iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
            if (body != nullptr)
            {
                body->setMaterial(_materialID);
            }
        }
    }

    int64 iNodePhysics::getMaterial() const
    {
        return _materialID;
    }

    bool iNodePhysics::onUpdateData()
    {
        iNodeTransform* nextTransformNode = static_cast<iNodeTransform*>(_parent);
        while (nextTransformNode != nullptr && nextTransformNode->getType() != iNodeType::iNodeTransform)
        {
            nextTransformNode = static_cast<iNodeTransform*>(nextTransformNode->getParent());
        }

        if (nextTransformNode != nullptr &&
            nextTransformNode->getType() == iNodeType::iNodeTransform)
        {
            initPhysics();

            if (_bodyID != iPhysicsBody::INVALID_PHYSICSBODY_ID)
            {
                iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
                iPhysics::getInstance().bindTransformNode(body, nextTransformNode, true);
            }
            else
            {
                //   con_err("inconsistand data");
                return false;
            }
        }
        else
        {
            con_warn("need a transform node as ancester");
            return false;
        }

        setTransformationDirty();
        return true;
    }

    void iNodePhysics::draw()
    {
        // nothing to do here
    }

};
