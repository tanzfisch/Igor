// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysics.h>

#include <iaConsole.h>
#include <iTimer.h>
#include <iRenderer.h>
#include <iApplication.h>
#include <iaVector3.h>
#include <iTaskManager.h>
#include <iPhysicsBody.h>
#include <iNodeFactory.h>
#include <iTimer.h>
#include <iPhysicsPlayer.h>
#include <iPhysicsCollision.h>
#include <iPhysicsJoint.h>
#include <iPhysicsMaterial.h>
#include <iPhysicsMaterialCombo.h>
#include <iNodeTransform.h>
#include <iMesh.h>

#include <newton/Newton.h>

namespace Igor
{

    float32 iPhysics::_simulationRate = 120.0f;

    void* AllocMemory(int sizeInBytes)
    {
        con_assert(sizeInBytes != 0, "can not alloc nothing");
        return malloc(sizeInBytes);
    }

    // this is the callback for freeing Newton Memory
    void FreeMemory(void *ptr, int sizeInBytes)
    {
        con_assert(ptr != nullptr, "can not free null pointer");
        con_assert(sizeInBytes != 0, "lenght can not be zero");
        free(ptr);
    }

    // rigid body destructor
    void PhysicsNodeDestructor(const void* body)
    {
        iPhysicsBody* physicsBody = static_cast<iPhysicsBody*>(NewtonBodyGetUserData(static_cast<const NewtonBody*>(body)));
        if (nullptr != physicsBody)
        {
            physicsBody->release();
            delete physicsBody;
        }
    }

    // set the transformation of a rigid body
    void PhysicsNodeSetTransform(const void* body, const float* matrix, int threadIndex)
    {
        iPhysicsBody* physicsBody = static_cast<iPhysicsBody*>(NewtonBodyGetUserData(static_cast<const NewtonBody*>(body)));
        if (nullptr != physicsBody)
        {
            physicsBody->setTransformNodeMatrix(matrix);
        }
    }

    void PhysicsApplyForceAndTorque(const void* body, float64 timestep, int threadIndex)
    {
        iPhysicsBody* physicsBody = static_cast<iPhysicsBody*>(NewtonBodyGetUserData(static_cast<const NewtonBody*>(body)));
        if (nullptr != physicsBody)
        {
            physicsBody->applyForceAndTorque(timestep, threadIndex);
        }
    }

    void SubmitConstraints(const void* const joint, float64 timestep, int threadIndex)
    {
        iPhysicsJoint* physicsJoint = static_cast<iPhysicsJoint*>(NewtonJointGetUserData(static_cast<const NewtonJoint*>(joint)));
        if (physicsJoint != nullptr)
        {
            physicsJoint->submitConstraints(static_cast<float64>(timestep), threadIndex);
        }
    }

    void GenericContactProcessCompatible(const void* const newtonContactJoint, float64 timestep, int threadIndex)
    {
        con_assert(newtonContactJoint != nullptr, "zero pointer")

            NewtonBody* body0 = NewtonJointGetBody0(static_cast<const NewtonJoint*>(newtonContactJoint));
        NewtonBody* body1 = NewtonJointGetBody1(static_cast<const NewtonJoint*>(newtonContactJoint));

        con_assert(body0 != nullptr && body1 != nullptr, "zero pointers")

            if (body0 != nullptr && body1 != nullptr)
            {
                iPhysicsBody* physicsBody0 = static_cast<iPhysicsBody*>(NewtonBodyGetUserData(static_cast<const NewtonBody*>(body0)));
                iPhysicsBody* physicsBody1 = static_cast<iPhysicsBody*>(NewtonBodyGetUserData(static_cast<const NewtonBody*>(body1)));

                con_assert(physicsBody0 != nullptr && physicsBody1 != nullptr, "zero pointers");

                void* contact = NewtonContactJointGetFirstContact(static_cast<const NewtonJoint*>(newtonContactJoint));
                NewtonMaterial* materialCombo = NewtonContactGetMaterial(contact);
                iPhysicsMaterialCombo* physicsMaterialCombo = static_cast<iPhysicsMaterialCombo*>(NewtonMaterialGetMaterialPairUserData(materialCombo));

                if (physicsMaterialCombo != nullptr && physicsBody0 != nullptr && physicsBody1 != nullptr)
                {
                    physicsMaterialCombo->contact(physicsBody0, physicsBody1);
                }
            }
    }

    void GenericContactProcess(const NewtonJoint* const newtonContactJoint, dFloat timestep, int threadIndex)
    {
        GenericContactProcessCompatible(static_cast<const void*>(newtonContactJoint), timestep, threadIndex);
    }

    iPhysics::iPhysics()
    {
        NewtonSetMemorySystem(AllocMemory, FreeMemory);
        _world = NewtonCreate();
        NewtonSetSolverModel(static_cast<const NewtonWorld*>(_world), 1);
        NewtonSetThreadsCount(static_cast<const NewtonWorld*>(_world), 4);

        _shadowWorld = NewtonCreate();
        NewtonSetSolverModel(static_cast<const NewtonWorld*>(_shadowWorld), 1);
        NewtonSetThreadsCount(static_cast<const NewtonWorld*>(_shadowWorld), 1);

        createDefaultMaterial();
    }

    iPhysics::~iPhysics()
    {
        stop();

        if (!_bodys.empty())
        {
            con_err("possible mem leak! not all physic bodys released");

            auto iter = _bodys.begin();
            while (iter != _bodys.end())
            {
                destroyBody((*iter).second);
                iter++;
            }
            _bodys.clear();
        }

        if (!_collisions.empty())
        {
            con_err("possible mem leak! not all physic collisions released");

            auto iter = _collisions.begin();
            while (iter != _collisions.end())
            {
                destroyCollision((*iter).second);
                iter++;
            }
            _collisions.clear();
        }

        // todo clear joints

        NewtonWaitForUpdateToFinish(static_cast<const NewtonWorld*>(_world));
        NewtonDestroy(static_cast<const NewtonWorld*>(_world));
    }

    void iPhysics::setAngularDamping(void* newtonBody, const iaVector3f& angularDamp)
    {
        NewtonBodySetAngularDamping(static_cast<const NewtonBody*>(newtonBody), angularDamp.getData());
    }

    void iPhysics::getVelocity(void* newtonBody, iaVector3f& velocity)
    {
        NewtonBodyGetVelocity(static_cast<const NewtonBody*>(newtonBody), velocity.getData());
    }

    void iPhysics::setForce(void* newtonBody, const iaVector3f& force)
    {
        NewtonBodySetForce(static_cast<const NewtonBody*>(newtonBody), force.getData());
    }

    void iPhysics::setTorque(void* newtonBody, const iaVector3f& force)
    {
        NewtonBodySetTorque(static_cast<const NewtonBody*>(newtonBody), force.getData());
    }

    void iPhysics::getMassMatrix(void* newtonBody, float32& mass, float32& Ixx, float32& Iyy, float32& Izz)
    {
        NewtonBodyGetMassMatrix(static_cast<const NewtonBody*>(newtonBody), &mass, &Ixx, &Iyy, &Izz);
    }

    void* iPhysics::getUserDataFromBody(void* newtonBody)
    {
        void* result = NewtonBodyGetUserData(static_cast<const NewtonBody*>(newtonBody));
        return result;
    }

    void iPhysics::start()
    {
        if (!_handleRegistered)
        {
            iApplication::getInstance().registerApplicationHandleDelegate(iApplicationHandleDelegate(this, &iPhysics::onUpdate));
            _handleRegistered = true;
        }

        _lastTime = iTimer::getInstance().getSeconds();
    }

    void iPhysics::stop()
    {
        if (_handleRegistered)
        {
            iApplication::getInstance().unregisterApplicationHandleDelegate(iApplicationHandleDelegate(this, &iPhysics::onUpdate));
            _handleRegistered = false;
        }
    }

    void iPhysics::destroyNewtonCollision(void* collision)
    {
        NewtonWaitForUpdateToFinish(static_cast<const NewtonWorld*>(_shadowWorld));
        _createDestroyMutex.lock();
        NewtonDestroyCollision(static_cast<const NewtonCollision*>(collision));
        _createDestroyMutex.unlock();
    }

    void iPhysics::setCollisionCallback(iPhysicsMaterialCombo* materialCombo)
    {
        NewtonMaterialSetCollisionCallback(static_cast<const NewtonWorld*>(_world), materialCombo->getMaterial0(), materialCombo->getMaterial1(), NULL, GenericContactProcess);
        NewtonMaterialSetCallbackUserData(static_cast<const NewtonWorld*>(_world), materialCombo->getMaterial0(), materialCombo->getMaterial1(), materialCombo);
    }

    void iPhysics::onUpdate()
    {
        const float32 timeDelta = 1.0f / static_cast<float64>(_simulationRate);
        const uint32 maxUpdateCount = 2;

        uint32 updateCount = 0;
        float64 currentTime = iTimer::getInstance().getSeconds();

        while ((_lastTime + timeDelta < currentTime) &&
            (updateCount < maxUpdateCount))
        {
            NewtonUpdateAsync(static_cast<const NewtonWorld*>(_world), timeDelta);
            _lastTime += timeDelta;
            updateCount++;
        };
    }

    void iPhysics::setSimulationRate(uint32 simulationRate)
    {
        _simulationRate = simulationRate;
    }

    uint32 iPhysics::getSimulationRate()
    {
        return _simulationRate;
    }

    iPhysicsMaterial* iPhysics::getMaterial(int64 id)
    {
        iPhysicsMaterial* result = nullptr;

        _materialListMutex.lock();
        auto iter = _materials.find(id);
        if (iter != _materials.end())
        {
            result = (*iter).second;
        }
        _materialListMutex.unlock();

        if (result == nullptr)
        {
            con_err("material id " << id << "not found");
        }

        return result;
    }

    iPhysicsMaterial* iPhysics::getDefaultMaterial()
    {
        return getMaterial(_defaultMaterial);
    }

    iPhysicsMaterial* iPhysics::createDefaultMaterial()
    {
        iPhysicsMaterial* result = nullptr;
        result = new iPhysicsMaterial(NewtonMaterialGetDefaultGroupID(static_cast<const NewtonWorld*>(_world)));
        result->setName("default");

        _materialListMutex.lock();
        _materials[result->getID()] = result;
        _materialListMutex.unlock();

        _defaultMaterial = result->getID();

        return result;
    }

    iPhysicsMaterial* iPhysics::createMaterial(const iaString& name)
    {
        iPhysicsMaterial* result = nullptr;
        result = new iPhysicsMaterial(NewtonMaterialCreateGroupID(static_cast<const NewtonWorld*>(_world)));
        result->setName(name);

        _materialListMutex.lock();
        _materials[result->getID()] = result;
        _materialListMutex.unlock();

        return result;
    }

    iPhysicsBody* iPhysics::createBody(iPhysicsCollision* collisionVolume)
    {
        iaMatrixf matrix;

        NewtonWaitForUpdateToFinish(static_cast<const NewtonWorld*>(_world));
        _createDestroyMutex.lock();
        NewtonBody* newtonBody = NewtonCreateDynamicBody(static_cast<const NewtonWorld*>(_world), static_cast<const NewtonCollision*>(collisionVolume->_collision), matrix.getData());
        _createDestroyMutex.unlock();

        // set callbacks
        NewtonBodySetDestructorCallback(newtonBody, reinterpret_cast<NewtonBodyDestructor>(PhysicsNodeDestructor));
        NewtonBodySetTransformCallback(newtonBody, reinterpret_cast<NewtonSetTransform>(PhysicsNodeSetTransform));
        NewtonBodySetForceAndTorqueCallback(newtonBody, reinterpret_cast<NewtonApplyForceAndTorque>(PhysicsApplyForceAndTorque));

        NewtonBodySetMassMatrix(newtonBody, 0, 0, 0, 0);
        NewtonBodySetMatrix(newtonBody, matrix.getData());

        start();

        uint64 bodyID = generateBodyID();

        iPhysicsBody* result = new iPhysicsBody(newtonBody, bodyID);

        _bodyListMutex.lock();
        _bodys[bodyID] = result;
        _bodyListMutex.unlock();

        return result;
    }

    void iPhysics::setUserJointAddAngularRow(iPhysicsJoint* joint, float32 relativeAngleError, const iaVector3f& pin)
    {
        NewtonUserJointAddAngularRow(static_cast<const NewtonJoint*>(joint->getNewtonJoint()), relativeAngleError, pin.getData());
    }

    void iPhysics::setUserJointSetRowMinimumFriction(iPhysicsJoint* joint, float32 friction)
    {
        NewtonUserJointSetRowMinimumFriction(static_cast<const NewtonJoint*>(joint->getNewtonJoint()), friction);
    }

    void iPhysics::setUserJointSetRowMaximumFriction(iPhysicsJoint* joint, float32 friction)
    {
        NewtonUserJointSetRowMaximumFriction(static_cast<const NewtonJoint*>(joint->getNewtonJoint()), friction);
    }

    iPhysicsJoint* iPhysics::createJoint(iPhysicsBody* body0, iPhysicsBody* body1, int dof)
    {
        iPhysicsJoint* result = nullptr;
        con_assert(body0 != nullptr, "zero pointer");

        if (body0 != nullptr)
        {
            NewtonWaitForUpdateToFinish(static_cast<const NewtonWorld*>(_world));
            _createDestroyMutex.lock();
            NewtonJoint * joint = NewtonConstraintCreateUserJoint(static_cast<NewtonWorld*>(_world), dof, reinterpret_cast<NewtonUserBilateralCallback>(SubmitConstraints), nullptr, static_cast<NewtonBody*>(body0->getNewtonBody()), body1 != nullptr ? static_cast<NewtonBody*>(body1->getNewtonBody()) : nullptr);
            _createDestroyMutex.unlock();

            result = new iPhysicsJoint(joint, body0->getID(), body1 != nullptr ? body1->getID() : 0);

            _jointListMutex.lock();
            _joints[result->getID()] = result;
            _jointListMutex.unlock();

            NewtonJointSetUserData(joint, result);
        }

        return result;
    }

    iPhysicsJoint* iPhysics::getJoint(uint64 jointID)
    {
        iPhysicsJoint* result = nullptr;

        _jointListMutex.lock();
        auto iter = _joints.find(jointID);

        if (iter != _joints.end())
        {
            result = (*iter).second;
        }
        _jointListMutex.lock();

        if (result == nullptr)
        {
            con_err("joint id " << jointID << "not found");
        }

        return result;
    }

    void iPhysics::bindTransformNode(iPhysicsBody* body, iNodeTransform* transformNode, bool sync)
    {
        con_assert(body != nullptr && transformNode != nullptr, "zero pointer");

        if (body != nullptr && transformNode != nullptr)
        {
            if (sync)
            {
                iaMatrixf matrix;
                transformNode->getMatrix(matrix);
                body->setMatrix(matrix);
            }

            body->bindTransformNode(transformNode);
            NewtonBodySetUserData(static_cast<const NewtonBody*>(body->_newtonBody), body);
        }
    }

    void iPhysics::setMaterial(iPhysicsBody* body, int64 materialID)
    {
        con_assert(body != nullptr, "zero pointer");

        if (body != nullptr)
        {
            NewtonBodySetMaterialGroupID(static_cast<const NewtonBody*>(body->_newtonBody), materialID);
        }
    }

    iPhysicsBody* iPhysics::getBody(uint64 bodyID)
    {
        iPhysicsBody* result = nullptr;
        _bodyListMutex.lock();
        auto iter = _bodys.find(bodyID);

        if (iter != _bodys.end())
        {
            result = (*iter).second;
        }

        _bodyListMutex.unlock();

        if (result == nullptr)
        {
            con_err("body id " << bodyID << "not found");
        }

        return result;
    }

    void iPhysics::destroyBody(iPhysicsBody* body)
    {
        con_assert_sticky(body != nullptr, "zero pointer");

        if (body != nullptr)
        {
            if (body->_newtonBody != nullptr)
            {
                destroyNewtonBody(body->_newtonBody);
                body->_newtonBody = nullptr;
            }

            delete body;
        }
    }

    void iPhysics::destroyBody(uint64 bodyID)
    {
        destroyBody(getBody(bodyID));
    }

    void iPhysics::destroyNewtonBody(void* newtonBody)
    {
        NewtonWaitForUpdateToFinish(static_cast<const NewtonWorld*>(_world));
        NewtonBodySetUserData(static_cast<const NewtonBody*>(newtonBody), nullptr);
        _createDestroyMutex.lock();
        NewtonDestroyBody(static_cast<const NewtonBody*>(newtonBody));
        _createDestroyMutex.unlock();
    }

    void iPhysics::destroyCollision(iPhysicsCollision* collision)
    {
        con_assert(collision != nullptr, "zero pointer");

        if (collision != nullptr &&
            collision->_collision != nullptr)
        {
            _collisionsListMutex.lock();
            auto iter = _collisions.find(collision->getID());
            if (iter != _collisions.end())
            {
                if ((*iter).second->_collision != nullptr)
                {
                    destroyNewtonCollision((*iter).second->_collision);
                }

                delete (*iter).second;
                _collisions.erase(iter);
            }
            else
            {
                con_err("can't find collision with id " << collision->getID());
            }
            _collisionsListMutex.unlock();
        }
    }

    iPhysicsCollision* iPhysics::createCompound(vector<iPhysicsCollision*>& collisions)
    {
        uint64 collisionID = generateCollisionID();

        _createDestroyMutex.lock();
        NewtonCollision* collision = NewtonCreateCompoundCollision(static_cast<const NewtonWorld*>(_shadowWorld), collisionID);
        _createDestroyMutex.unlock();
        iPhysicsCollision* result = new iPhysicsCollision(collision, collisionID);

        _collisionsListMutex.lock();
        _collisions[collisionID] = result;
        _collisionsListMutex.unlock();

        _createDestroyMutex.lock();
        NewtonCompoundCollisionBeginAddRemove(collision);

        for (auto subcollision : collisions)
        {
            NewtonCompoundCollisionAddSubCollision(static_cast<NewtonCollision*>(collision), static_cast<NewtonCollision*>(subcollision->_collision));
        }

        NewtonCompoundCollisionEndAddRemove(collision);
        _createDestroyMutex.unlock();

        return result;
    }

    iPhysicsCollision* iPhysics::createBox(float32 width, float32 height, float32 depth, const iaMatrixf& offset)
    {
        _createDestroyMutex.lock();
        NewtonCollision* collision = NewtonCreateBox(static_cast<const NewtonWorld*>(_shadowWorld), width, height, depth, 0, offset.getData());
        _createDestroyMutex.unlock();
        return prepareCollision(collision, generateCollisionID());
    }

    iPhysicsCollision* iPhysics::createSphere(float32 radius, const iaMatrixf& offset)
    {
        _createDestroyMutex.lock();
        NewtonCollision* collision = NewtonCreateSphere(static_cast<const NewtonWorld*>(_shadowWorld), radius, 0, offset.getData());
        _createDestroyMutex.unlock();
        return prepareCollision(collision, generateCollisionID());
    }

    iPhysicsCollision* iPhysics::createCone(float32 radius, float32 height, const iaMatrixf& offset)
    {
        _createDestroyMutex.lock();
        NewtonCollision* collision = NewtonCreateCone(static_cast<const NewtonWorld*>(_shadowWorld), radius, height, 0, offset.getData());
        _createDestroyMutex.unlock();
        return prepareCollision(collision, generateCollisionID());
    }

    iPhysicsCollision* iPhysics::createCapsule(float32 radius, float32 height, const iaMatrixf& offset)
    {
        _createDestroyMutex.lock();
        NewtonCollision* collision = NewtonCreateCapsule(static_cast<const NewtonWorld*>(_shadowWorld), radius, height, 0, 0, offset.getData());
        _createDestroyMutex.unlock();
        return prepareCollision(collision, generateCollisionID());
    }

    iPhysicsCollision* iPhysics::createCylinder(float32 radius, float32 height, const iaMatrixf& offset)
    {
        _createDestroyMutex.lock();
        NewtonCollision* collision = NewtonCreateCylinder(static_cast<const NewtonWorld*>(_shadowWorld), radius, height, 0, 0, offset.getData());
        _createDestroyMutex.unlock();
        return prepareCollision(collision, generateCollisionID());
    }

    iPhysicsCollision* iPhysics::prepareCollision(void* collision, uint64 collisionID)
    {
        NewtonCollisionSetUserID(static_cast<const NewtonCollision*>(collision), collisionID);
        iPhysicsCollision* result = new iPhysicsCollision(collision, collisionID);

        _collisionsListMutex.lock();
        _collisions[collisionID] = result;
        _collisionsListMutex.unlock();

        return result;
    }

    uint64 iPhysics::generateCollisionID()
    {
        uint64 result;
        _idMutex.lock();
        result = _nextCollisionID++;
        _idMutex.unlock();
        return result;
    }

    uint64 iPhysics::generateBodyID()
    {
        uint64 result;
        _idMutex.lock();
        result = _nextBodyID++;
        _idMutex.unlock();
        return result;
    }

    iPhysicsCollision* iPhysics::getCollision(uint64 collisionID)
    {
        iPhysicsCollision* result;
        _collisionsListMutex.lock();
        auto iter = _collisions.find(collisionID);
        if (iter != _collisions.end())
        {
            result = (*iter).second;
        }

        _collisionsListMutex.unlock();

        if (result == nullptr)
        {
            con_err("collision id " << collisionID << "not found");
        }

        return result;
    }

    void iPhysics::updateMatrix(void* newtonBody, const iaMatrixf& matrix)
    {
        NewtonBodySetMatrix(static_cast<const NewtonBody*>(newtonBody), matrix.getData());
    }

    void iPhysics::getMatrix(void* newtonBody, iaMatrixf& matrix)
    {
        NewtonBodyGetMatrix(static_cast<const NewtonBody*>(newtonBody), matrix.getData());
    }

    void iPhysics::setMassMatrix(void* newtonBody, float32 mass, float32 Ixx, float32 Iyy, float32 Izz)
    {
        if (mass >= __IGOR_GRAMM__)
        {
            NewtonBodySetMassMatrix(static_cast<const NewtonBody*>(newtonBody), mass, Ixx, Iyy, Izz);
        }
        else
        {
            NewtonBodySetMassMatrix(static_cast<const NewtonBody*>(newtonBody), 0, 0, 0, 0);
        }
    }

    iPhysicsCollision* iPhysics::createMesh(shared_ptr<iMesh> mesh, int64 faceAttribute, const iaMatrixf& offset)
    {
        uint64 collisionID = generateCollisionID();

        _createDestroyMutex.lock();
        NewtonCollision* collision = NewtonCreateTreeCollision(static_cast<const NewtonWorld*>(_shadowWorld), collisionID);

        NewtonTreeCollisionBeginBuild(collision);

        float32 temp[12];
        temp[3] = 1.0f;
        temp[7] = 1.0f;
        temp[11] = 1.0f;

        uint32* indexes = mesh->getIndexData();
        float32* vertexes = mesh->getVertexData();

        uint32 vertexFloatCount = mesh->getVertexSize() / 4;
        uint32 vertexPos = 0;
        uint32 indexCount = mesh->getIndexesCount();

        for (int i = 0; i < indexCount; i += 3)
        {
            vertexPos = (indexes[i + 0] * vertexFloatCount);
            temp[0] = vertexes[vertexPos++];
            temp[1] = vertexes[vertexPos++];
            temp[2] = vertexes[vertexPos++];

            vertexPos = (indexes[i + 1] * vertexFloatCount);
            temp[4] = vertexes[vertexPos++];
            temp[5] = vertexes[vertexPos++];
            temp[6] = vertexes[vertexPos++];

            vertexPos = (indexes[i + 2] * vertexFloatCount);
            temp[8] = vertexes[vertexPos++];
            temp[9] = vertexes[vertexPos++];
            temp[10] = vertexes[vertexPos++];

            NewtonTreeCollisionAddFace(collision, 3, temp, sizeof(float32) * 4, faceAttribute);
        }

        NewtonTreeCollisionEndBuild(collision, 0);

        _createDestroyMutex.unlock();

        return prepareCollision(collision, collisionID);
    }

}