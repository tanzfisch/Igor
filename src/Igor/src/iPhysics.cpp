// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysics.h>

#include <iaConsole.h>
#include <iPhysicsWorld.h>
#include <iTimer.h>
#include <iRenderer.h>
#include <iApplication.h>
#include <iaVector3.h>
#include <iTaskManager.h>
#include <iPhysicsBody.h>
#include <iNodeManager.h>
#include <iTimer.h>
#include <iPhysicsCollision.h>
#include <iPhysicsJoint.h>
#include <iPhysicsMaterial.h>
#include <iPhysicsMaterialCombo.h>
#include <iPhysicsCollisionConfig.h>
#include <iNodeTransform.h>
#include <iPhysicsUserMeshCollisionHandler.h>
#include <iaConvert.h>

#include <newton/Newton.h>

namespace Igor
{

    /*! callback to allocate memory for newton

    \param sizeInBytes size of data in bytes
    \returns pointer to new allocated data
    */
    void* AllocMemory(int sizeInBytes)
    {
        con_assert(sizeInBytes != 0, "can not alloc nothing");
        return malloc(sizeInBytes);
    }

    /*! this is the callback for freeing Newton Memory

    \param ptr pointer to data
    \param sizeInBytes size if data in bytes
    */
    void FreeMemory(void *ptr, int sizeInBytes)
    {
        con_assert(ptr != nullptr, "can not free null pointer");
        con_assert(sizeInBytes != 0, "lenght can not be zero");
        free(ptr);
    }

    /*! callback for physics body destruction

    \param body pointer to body that got destroyed
    */
    void PhysicsNodeDestructor(const void* body)
    {
        iPhysicsBody* physicsBody = static_cast<iPhysicsBody*>(NewtonBodyGetUserData(static_cast<const NewtonBody*>(body)));
        if (nullptr != physicsBody)
        {
            physicsBody->release();
            iPhysics::getInstance().destroyBodyAsync(physicsBody);
        }
    }

    /*! \todo dirty workaround because of casting issues
    */
    void PhysicsNodeSetTransform(const void* body, const float64* matrix, int threadIndex)
    {
        iPhysicsBody* physicsBody = static_cast<iPhysicsBody*>(NewtonBodyGetUserData(static_cast<const NewtonBody*>(body)));
        if (nullptr != physicsBody)
        {
            iaMatrixd m;
            m.setData(matrix);
            iPhysics::getInstance().queueTransformation(physicsBody, m);
        }
    }

    /*! updates the scene graph node transformation

    \param body the body that changed it's position
    \param matrix the updated matrix from newton
    \param threadIndex 
    */
    void PhysicsNodeSetTransformation(const NewtonBody* const body, const dFloat* const matrix, int threadIndex)
    {
        PhysicsNodeSetTransform(static_cast<const void*>(body), matrix, threadIndex);
    }

    /*! redirects all apply force and torque calls to igor physics bodies

    \param body the newton body
    \param timestep time delta
    \param threadIndex ???
    */
    void PhysicsApplyForceAndTorque(const void* body, float64 timestep, int threadIndex)
    {
        iPhysicsBody* physicsBody = static_cast<iPhysicsBody*>(NewtonBodyGetUserData(static_cast<const NewtonBody*>(body)));
        if (nullptr != physicsBody)
        {
            physicsBody->applyForceAndTorque(timestep);
        }
    }

    /*!

    \param joint ???
    \param timestep time delta
    \param threadIndex ???
    */
    void SubmitConstraints(const void* const joint, float64 timestep, int threadIndex)
    {
        iPhysicsJoint* physicsJoint = static_cast<iPhysicsJoint*>(NewtonJointGetUserData(static_cast<const NewtonJoint*>(joint)));
        if (physicsJoint != nullptr)
        {
            physicsJoint->submitConstraints(timestep);
        }
    }

    /*! generic handle to handle contacts beween two bodies

    \param newtonContactJoint ???
    \param timestep time delta
    \param threadIndex ???
    */
    void GenericContactProcessCompatible(const void* const newtonContactJoint, float64 timestep, int threadIndex)
    {
        con_assert(newtonContactJoint != nullptr, "zero pointer");

        NewtonBody* body0 = NewtonJointGetBody0(static_cast<const NewtonJoint*>(newtonContactJoint));
        NewtonBody* body1 = NewtonJointGetBody1(static_cast<const NewtonJoint*>(newtonContactJoint));

        con_assert(body0 != nullptr && body1 != nullptr, "zero pointers");

        if (body0 != nullptr && body1 != nullptr)
        {
            iPhysicsBody* physicsBody0 = static_cast<iPhysicsBody*>(NewtonBodyGetUserData(static_cast<const NewtonBody*>(body0)));
            iPhysicsBody* physicsBody1 = static_cast<iPhysicsBody*>(NewtonBodyGetUserData(static_cast<const NewtonBody*>(body1)));

            //con_assert(physicsBody0 != nullptr && physicsBody1 != nullptr, "zero pointers");

            if (physicsBody0 != nullptr && physicsBody1 != nullptr)
            {
                void* contact = NewtonContactJointGetFirstContact(static_cast<const NewtonJoint*>(newtonContactJoint));
                NewtonMaterial* materialCombo = NewtonContactGetMaterial(contact);
                iPhysicsMaterialCombo* physicsMaterialCombo = static_cast<iPhysicsMaterialCombo*>(NewtonMaterialGetMaterialPairUserData(materialCombo));

                if (physicsMaterialCombo != nullptr)
                {
                    iPhysics::getInstance().queueContact(physicsMaterialCombo, physicsBody0, physicsBody1);
                }
            }
        }
    }

    // ugly workaround
    void GenericContactProcess(const NewtonJoint* const newtonContactJoint, dFloat timestep, int threadIndex)
    {
        GenericContactProcessCompatible(static_cast<const void*>(newtonContactJoint), timestep, threadIndex);
    }

    iPhysics::iPhysics()
    {
        NewtonSetMemorySystem(AllocMemory, FreeMemory);
        createDefaultWorld();
        createDefaultMaterial();
    }

    iPhysics::~iPhysics()
    {
        stop();

        if (!_bodys.empty())
        {
            con_err("possible mem leak! " << _bodys.size() << " physic bodys left");

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
            con_err("possible mem leak! " << _collisions.size() << " physic collisions left");

            auto iter = _collisions.begin();
            while (iter != _collisions.end())
            {
                destroyCollision((*iter).second);
                iter++;
            }
            _collisions.clear();
        }

        destroyMaterials();
        NewtonMaterialDestroyAllGroupID(static_cast<const NewtonWorld*>(_defaultWorld));
        
        // need a copy because destroyWorld would manipulate the list we are iterating
        auto worldsToDelete = _worlds;
        for (auto world : worldsToDelete)
        {
            destroyWorld(world.second);
        }

        // todo clear joints
    }

    struct PreFilterUserData
    {
        iRayPreFilterDelegate preFilterDelegate;
        void* userData = nullptr;
    };

    unsigned CommonRayPrefilterCallback(const NewtonBody* const newtonBody, const NewtonCollision* const collision, void* const userData) 
    {
        iPhysicsCollision* physicsCollision = static_cast<iPhysicsCollision*>(NewtonCollisionGetUserData(collision));
        if (physicsCollision != nullptr)
        {
            iPhysicsBody* physicsBody = static_cast<iPhysicsBody*>(NewtonBodyGetUserData(newtonBody));
            if (physicsBody != nullptr)
            {
                iPhysicsCollision* physicsCollision = static_cast<iPhysicsCollision*>(NewtonCollisionGetUserData(collision));

                con_assert(physicsCollision != nullptr, "zero pointer");

                PreFilterUserData* preFilterUserData = static_cast<PreFilterUserData*>(userData);
                return preFilterUserData->preFilterDelegate(physicsBody, physicsCollision, preFilterUserData->userData);
            }

            return 1;
        }
        else
        {
            return 0;
        }
    }

    void iPhysics::convexCast(const iaMatrixd& matrix, const iaVector3d& target, iPhysicsCollision* collisionVolume, iRayPreFilterDelegate preFilterDelegate, void* userData, std::vector<ConvexCastReturnInfo>& result, int32 maxContactCount)
    {
        con_assert(maxContactCount <= 16, "param out of range");

        NewtonWorldConvexCastReturnInfo info[16];
        float64 param = 1.2; // TODO ?
        PreFilterUserData preFilterUserData;
        preFilterUserData.preFilterDelegate = preFilterDelegate;
        preFilterUserData.userData = userData;

        int numberOfContacts = NewtonWorldConvexCast(static_cast<const NewtonWorld*>(_defaultWorld), matrix.getData(), target.getData(), 
            static_cast<const NewtonCollision*>(collisionVolume->_collision), &param, &preFilterUserData,
            reinterpret_cast<NewtonWorldRayPrefilterCallback>(CommonRayPrefilterCallback), &info[0], maxContactCount, 0);

        for (int i = 0; i < numberOfContacts; ++i)
        {
            ConvexCastReturnInfo returnInfo;
            returnInfo._contactID = info[i].m_contactID;
            returnInfo._hitBody = static_cast<iPhysicsBody*>(NewtonBodyGetUserData(info[i].m_hitBody));
            returnInfo._normal.set(info[i].m_normal[0], info[i].m_normal[1], info[i].m_normal[2], info[i].m_normal[3]);
            returnInfo._point.set(info[i].m_point[0], info[i].m_point[1], info[i].m_point[2], info[i].m_point[3]);
            returnInfo._penetration = info[i].m_penetration;
            result.push_back(returnInfo);
        }
    }

    void iPhysics::queueContact(iPhysicsMaterialCombo* material, iPhysicsBody* body1, iPhysicsBody* body2)
    {
        Contact contact;
        contact._material = material;
        contact._body1 = body1;
        contact._body2 = body2;

        _mutexBodyContacts.lock();
        _bodyContacts.push_back(contact);
        _mutexBodyContacts.unlock();
    }

    void iPhysics::queueTransformation(iPhysicsBody* body, const iaMatrixd& matrix)
    {
        _mutexBodiesToTransform.lock();
        _bodiesToTransform.push_back(std::pair<iPhysicsBody*, iaMatrixd>(body, matrix));
        _mutexBodiesToTransform.unlock();
    }

    uint64 iPhysics::getDefaultWorldID()
    {
        return _defaultWorldID;
    }

    void iPhysics::destroyBodyAsync(iPhysicsBody* body)
    {
        con_assert(body != nullptr, "zero pointer");

        if (body != nullptr)
        {
            _mutexBodiesToDelete.lock();
            _bodiesToDelete.push_back(body);
            _mutexBodiesToDelete.unlock();
        }
    }

    void iPhysics::createDefaultWorld()
    {
        iPhysicsWorld* world = createWorld();
        _defaultWorld = world->getNewtonWorld();
        _defaultWorldID = world->getID();

        iPhysicsWorld* shadow = createWorld();
        _shadowWorld = shadow->getNewtonWorld();
        _shadowWorldID = shadow->getID();
    }

    iPhysicsCollision* iPhysics::createBox(float64 width, float64 height, float64 depth, const iaMatrixd& offset)
    {
        return createBox(width, height, depth, offset, _shadowWorldID);
    }

    iPhysicsCollision* iPhysics::createMesh(iMeshPtr mesh, int64 faceAttribute, const iaMatrixd& offset)
    {
        return createMesh(mesh, faceAttribute, offset, _shadowWorldID);
    }

    iPhysicsCollision* iPhysics::createSphere(float64 radius, const iaMatrixd& offset)
    {
        return createSphere(radius, offset, _shadowWorldID);
    }

    iPhysicsCollision* iPhysics::createCone(float64 radius, float64 height, const iaMatrixd& offset)
    {
        return createCone(radius, height, offset, _shadowWorldID);
    }

    iPhysicsCollision* iPhysics::createCapsule(float64 radius0, float64 radius1, float64 height, const iaMatrixd& offset)
    {
        return createCapsule(radius0, radius1, height, offset, _shadowWorldID);
    }

    iPhysicsCollision* iPhysics::createCylinder(float64 radius0, float64 radius1, float64 height, const iaMatrixd& offset)
    {
        return createCylinder(radius0, radius1, height, offset, _shadowWorldID);
    }

    iPhysicsCollision* iPhysics::createCompound(std::vector<iPhysicsCollision*>& collisions)
    {
        return createCompound(collisions, _shadowWorldID);
    }

    iPhysicsCollisionConfig* iPhysics::createCollisionConfig()
    {
        iPhysicsCollisionConfig* result = new iPhysicsCollisionConfig();

        _collisionsConfigListMutex.lock();
        _collisionConfigs[result->getID()] = result;
        _collisionsConfigListMutex.unlock();

        return result;
    }

    bool iPhysics::isCollisionConfig(uint64 collisionConfigID)
    {
        bool result = false;
        _collisionsConfigListMutex.lock();
        auto iter = _collisionConfigs.find(collisionConfigID);
        if (iter != _collisionConfigs.end())
        {
            result = true;
        }
        _collisionsConfigListMutex.unlock();

        return result;
    }

    iPhysicsCollisionConfig* iPhysics::getCollisionConfig(uint64 collisionConfigID)
    {
        iPhysicsCollisionConfig* result = nullptr;

        _collisionsConfigListMutex.lock();
        auto iter = _collisionConfigs.find(collisionConfigID);
        if (iter != _collisionConfigs.end())
        {
            result = (*iter).second;
        }
        _collisionsConfigListMutex.unlock();

        return result;
    }

    void iPhysics::destroyCollisionConfig(uint64 collisionConfigID)
    {
        destroyCollisionConfig(getCollisionConfig(collisionConfigID));
    }

    void iPhysics::destroyCollisionConfig(iPhysicsCollisionConfig* physicsCollisionConfig)
    {
        con_assert(physicsCollisionConfig != nullptr, "zero pointer");

        if (physicsCollisionConfig != nullptr)
        {
            _collisionsConfigListMutex.lock();
            auto iter = _collisionConfigs.find(physicsCollisionConfig->getID());
            if (iter != _collisionConfigs.end())
            {
                _collisionConfigs.erase(iter);
            }
            else
            {
                con_err("collision config id " << physicsCollisionConfig->getID() << " not found");
            }
            _collisionsConfigListMutex.unlock();
        }
    }

    void iPhysics::setAngularDamping(void* newtonBody, const iaVector3d& angularDamp)
    {
        NewtonBodySetAngularDamping(static_cast<const NewtonBody*>(newtonBody), angularDamp.getData());
    }

    void iPhysics::setLinearDamping(void* newtonBody, float64 linearDamp)
    {
        NewtonBodySetLinearDamping(static_cast<const NewtonBody*>(newtonBody), linearDamp);
    }

    void iPhysics::getVelocity(void* newtonBody, iaVector3d& velocity)
    {
        NewtonBodyGetVelocity(static_cast<const NewtonBody*>(newtonBody), velocity.getData());
    }

    void iPhysics::setForce(void* newtonBody, const iaVector3d& force)
    {
        NewtonBodySetForce(static_cast<const NewtonBody*>(newtonBody), force.getData());
    }

    void iPhysics::setTorque(void* newtonBody, const iaVector3d& torque)
    {
        NewtonBodySetTorque(static_cast<const NewtonBody*>(newtonBody), torque.getData());
    }

    void iPhysics::getMassMatrix(void* newtonBody, float64& mass, float64& Ixx, float64& Iyy, float64& Izz)
    {
        NewtonBodyGetMass(static_cast<const NewtonBody*>(newtonBody), &mass, &Ixx, &Iyy, &Izz);
    }

    void* iPhysics::getUserDataFromBody(void* newtonBody)
    {
        void* result = NewtonBodyGetUserData(static_cast<const NewtonBody*>(newtonBody));
        return result;
    }

    void iPhysics::start()
    {
        if (!_running)
        {
            _lastTime = iTimer::getInstance().getSeconds();
            _running = true;
        }
    }

    void iPhysics::stop()
    {
        if (_running)
        {
            _running = false;
        }
    }

    void iPhysics::destroyNewtonCollision(void* collision, uint64 worldID)
    {
        const NewtonWorld* world = static_cast<const NewtonWorld*>(getWorld(worldID)->getNewtonWorld());
        if (world != nullptr)
        {
            NewtonWaitForUpdateToFinish(world);
            NewtonDestroyCollision(static_cast<const NewtonCollision*>(collision));
        }
    }

    void iPhysics::setCollisionCallback(iPhysicsMaterialCombo* materialCombo)
    {
        NewtonMaterialSetCollisionCallback(static_cast<const NewtonWorld*>(_defaultWorld), materialCombo->getMaterial0(), materialCombo->getMaterial1(), NULL, GenericContactProcess);
        NewtonMaterialSetCallbackUserData(static_cast<const NewtonWorld*>(_defaultWorld), materialCombo->getMaterial0(), materialCombo->getMaterial1(), materialCombo);
    }

    void iPhysics::setSoftness(iPhysicsMaterialCombo* materialCombo, float64 value)
    {
        NewtonMaterialSetDefaultSoftness(static_cast<const NewtonWorld*>(_defaultWorld), materialCombo->getMaterial0(), materialCombo->getMaterial1(), value);
    }

    void iPhysics::setElasticity(iPhysicsMaterialCombo* materialCombo, float64 elasticCoef)
    {
        NewtonMaterialSetDefaultElasticity(static_cast<const NewtonWorld*>(_defaultWorld), materialCombo->getMaterial0(), materialCombo->getMaterial1(), elasticCoef);
    }

    void iPhysics::setFriction(iPhysicsMaterialCombo* materialCombo, float64 staticFriction, float64 kineticFriction)
    {
        con_assert(staticFriction >= 0.0 && staticFriction <= 2.0, "out of range");
        con_assert(kineticFriction >= 0.0 && kineticFriction <= 2.0, "out of range");
        con_assert(kineticFriction <= staticFriction, "out of range");

        NewtonMaterialSetDefaultFriction(static_cast<const NewtonWorld*>(_defaultWorld), materialCombo->getMaterial0(), materialCombo->getMaterial1(), staticFriction, kineticFriction);
    }

    void iPhysics::handleQueues()
    {
        _mutexBodyContacts.lock();
        auto contacts = std::move(_bodyContacts);
        _mutexBodyContacts.unlock();

        for (auto iter : contacts)
        {
            iter._material->contact(iter._body1, iter._body2);
        }

        _mutexBodiesToTransform.lock();
        auto bodiesToTransform = std::move(_bodiesToTransform);
        _mutexBodiesToTransform.unlock();

        for (auto iter : bodiesToTransform)
        {
            iter.first->setTransformNodeMatrix(iter.second);
        }

        _mutexBodiesToDelete.lock();
        auto bodiesToDelete = std::move(_bodiesToDelete);
        _mutexBodiesToDelete.unlock();

        for (auto iter : bodiesToDelete)
        {
            destroyBody(iter);
        }
    }

    void iPhysics::handle()
    {
        if (_running)
        {
            handleQueues();

            const float64 timeDelta = 1.0 / _simulationRate;
            const uint32 maxUpdateCount = 2;

            uint32 updateCount = 0;
            float64 currentTime = iTimer::getInstance().getSeconds();

            while ((_lastTime + timeDelta < currentTime) &&
                (updateCount < maxUpdateCount))
            {
                // "if you call another NewtonUpdateAsync before anothe one is still running the the secund will wait act as a NewtonUpdate wating fo rteh first updateAsyn To complete." Julio Jerez
                NewtonUpdateAsync(static_cast<const NewtonWorld*>(_defaultWorld), timeDelta);
                _lastTime += timeDelta;
                updateCount++;
            };
        }
    }

    void iPhysics::setSimulationRate(float64 simulationRate)
    {
        _simulationRate = simulationRate;
    }

    float64 iPhysics::getSimulationRate()
    {
        return _simulationRate;
    }

    iPhysicsMaterial* iPhysics::createMaterial(const iaString& name)
    {
        iPhysicsMaterial* result = nullptr;
        result = new iPhysicsMaterial(NewtonMaterialCreateGroupID(static_cast<const NewtonWorld*>(_defaultWorld)));
        result->setName(name);

        _materialListMutex.lock();
        _materials[result->getID()] = result;
        _materialListMutex.unlock();

        return result;
    }

    int64 iPhysics::getMaterialID(const iaString& name)
    {
        int64 result = iPhysicsMaterial::INVALID_PHYSICSMATERIAL_ID;

        _materialListMutex.lock();
        auto iter = _materials.begin();
        while (iter != _materials.end())
        {
            if ((*iter).second->getName() == name)
            {
                result = (*iter).first;
                break;
            }
            iter++;
        }
        _materialListMutex.unlock();

        return result;
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
            con_err("material id " << id << " not found");
        }

        return result;
    }

    iPhysicsMaterial* iPhysics::getDefaultMaterial()
    {
        return getMaterial(_defaultMaterialID);
    }

    int64 iPhysics::getDefaultMaterialID()
    {
        return _defaultMaterialID;
    }

    void iPhysics::destroyMaterials()
    {
        destroyDefaultMaterial();

        _materialListMutex.lock();
        for (auto materialIter : _materials)
        {
            delete materialIter.second;
        }
        _materials.clear();
        _materialListMutex.unlock();
    }

	void iPhysics::destroyDefaultMaterial()
	{
        _materialListMutex.lock();
        auto materialIter = _materials.find(_defaultMaterialID);
        if (materialIter != _materials.end())
        {
            delete _materials[_defaultMaterialID];
            _materials.erase(materialIter);
        }
        _materialListMutex.unlock();
	}

    void iPhysics::createDefaultMaterial()
    {
        iPhysicsMaterial* defaultMaterial = nullptr;
        defaultMaterial = new iPhysicsMaterial(NewtonMaterialGetDefaultGroupID(static_cast<const NewtonWorld*>(_defaultWorld)));
        defaultMaterial->setName("default");
        _defaultMaterialID = defaultMaterial->getID();

        _materialListMutex.lock();
        _materials[_defaultMaterialID] = defaultMaterial;
        _materialListMutex.unlock();
    }

    iPhysicsBody* iPhysics::createBody(iPhysicsCollision* collisionVolume)
    {
        con_assert(collisionVolume != nullptr, "zero pointer");
        con_assert(collisionVolume->_collision != nullptr, "zero pointer");

        iPhysicsBody* result = nullptr;
        iaMatrixd matrix;

        if (collisionVolume != nullptr &&
            collisionVolume->_collision != nullptr)
        {
            NewtonWaitForUpdateToFinish(static_cast<const NewtonWorld*>(_defaultWorld));
            NewtonBody* newtonBody = NewtonCreateDynamicBody(static_cast<const NewtonWorld*>(_defaultWorld), static_cast<const NewtonCollision*>(collisionVolume->_collision), matrix.getData());

            // set callbacks
            NewtonBodySetDestructorCallback(newtonBody, reinterpret_cast<NewtonBodyDestructor>(PhysicsNodeDestructor));
            NewtonBodySetTransformCallback(newtonBody, reinterpret_cast<NewtonSetTransform>(PhysicsNodeSetTransformation));
            NewtonBodySetForceAndTorqueCallback(newtonBody, reinterpret_cast<NewtonApplyForceAndTorque>(PhysicsApplyForceAndTorque));

            NewtonBodySetMassMatrix(newtonBody, 0, 0, 0, 0);
            NewtonBodySetMatrix(newtonBody, matrix.getData());

            result = new iPhysicsBody(newtonBody);

            _bodyListMutex.lock();
            _bodys[result->getID()] = result;
            _bodyListMutex.unlock();
        }
        else
        {
            con_err("can't create body without collision");
        }

        return result;
    }

    void iPhysics::setUserJointAddAngularRow(iPhysicsJoint* joint, float64 relativeAngleError, const iaVector3d& pin)
    {
        NewtonUserJointAddAngularRow(static_cast<const NewtonJoint*>(joint->getNewtonJoint()), relativeAngleError, pin.getData());
    }

    void iPhysics::setUserJointSetRowMinimumFriction(iPhysicsJoint* joint, float64 friction)
    {
        NewtonUserJointSetRowMinimumFriction(static_cast<const NewtonJoint*>(joint->getNewtonJoint()), friction);
    }

    void iPhysics::setUserJointSetRowMaximumFriction(iPhysicsJoint* joint, float64 friction)
    {
        NewtonUserJointSetRowMaximumFriction(static_cast<const NewtonJoint*>(joint->getNewtonJoint()), friction);
    }

    iPhysicsJoint* iPhysics::createJoint(iPhysicsBody* body0, iPhysicsBody* body1, int maxDOF)
    {
        iPhysicsJoint* result = nullptr;
        con_assert(body0 != nullptr, "zero pointer");

        if (body0 != nullptr)
        {
            NewtonWaitForUpdateToFinish(static_cast<const NewtonWorld*>(_defaultWorld));

            NewtonJoint * joint = NewtonConstraintCreateUserJoint(static_cast<NewtonWorld*>(_defaultWorld), maxDOF,
                reinterpret_cast<NewtonUserBilateralCallback>(SubmitConstraints), nullptr,
                static_cast<NewtonBody*>(body0->getNewtonBody()),
                body1 != nullptr ? static_cast<NewtonBody*>(body1->getNewtonBody()) : nullptr);

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

        return result;
    }

    bool iPhysics::isJoint(uint64 jointID)
    {
        bool result = false;

        _jointListMutex.lock();
        auto iter = _joints.find(jointID);
        if (iter != _joints.end())
        {
            result = true;
        }
        _jointListMutex.lock();

        return result;
    }

    void iPhysics::bindTransformNode(iPhysicsBody* body, iNodeTransform* transformNode, bool sync)
    {
        con_assert(body != nullptr && transformNode != nullptr, "zero pointer");

        if (body != nullptr && transformNode != nullptr)
        {
            if (sync)
            {
                iaMatrixd matrix;
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

    bool iPhysics::isBody(uint64 bodyID)
    {
        bool result = false;

        _bodyListMutex.lock();
        auto iter = _bodys.find(bodyID);
        if (iter != _bodys.end())
        {
            result = true;
        }
        _bodyListMutex.unlock();

        return result;
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

        return result;
    }

    void iPhysics::destroyBody(uint64 bodyID)
    {
        iPhysicsBody* body = getBody(bodyID);
        if (body != nullptr)
        {
            destroyBody(body);
        }
    }

    void iPhysics::destroyBody(iPhysicsBody* body)
    {
        con_assert(body != nullptr, "zero pointer");

        if (body != nullptr)
        {
            _bodyListMutex.lock();
            auto iter = _bodys.find(body->_id);
            con_assert(iter != _bodys.end(), "corrupt data");
            if (iter != _bodys.end())
            {
                _bodys.erase(iter);
            }
            _bodyListMutex.unlock();

            if (body->_newtonBody != nullptr)
            {
                destroyNewtonBody(body->_newtonBody);
                body->_newtonBody = nullptr;
            }

            delete body;
        }
    }

    void iPhysics::destroyNewtonBody(void* newtonBody)
    {
        NewtonWaitForUpdateToFinish(static_cast<const NewtonWorld*>(_defaultWorld));
        NewtonBodySetUserData(static_cast<const NewtonBody*>(newtonBody), nullptr);
        NewtonDestroyBody(static_cast<const NewtonBody*>(newtonBody));
    }

    void iPhysics::destroyCollision(uint64 collisionID)
    {
        iPhysicsCollision* collision = getCollision(collisionID);

        if (collision != nullptr)
        {
            destroyCollision(collision);
        }
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
                    destroyNewtonCollision((*iter).second->getCollision(), (*iter).second->getWorldID());
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

    iPhysicsCollision* iPhysics::createCompound(std::vector<iPhysicsCollision*>& collisions, uint64 worldID)
    {
        iPhysicsCollision* result = nullptr;
        const NewtonWorld* world = static_cast<const NewtonWorld*>(getWorld(worldID)->getNewtonWorld());

        if (world != nullptr)
        {
            NewtonCollision* collision = NewtonCreateCompoundCollision(static_cast<const NewtonWorld*>(world), 0);
            result = new iPhysicsCollision(collision, worldID);

            _collisionsListMutex.lock();
            _collisions[result->getID()] = result;
            _collisionsListMutex.unlock();

            NewtonCompoundCollisionBeginAddRemove(collision);

            for (auto subcollision : collisions)
            {
                NewtonCompoundCollisionAddSubCollision(static_cast<NewtonCollision*>(collision), static_cast<NewtonCollision*>(subcollision->_collision));
            }

            NewtonCompoundCollisionEndAddRemove(collision);
        }

        return result;
    }

    iPhysicsWorld* iPhysics::createWorld()
    {
        iPhysicsWorld* result = nullptr;

        _worldListMutex.lock();

        NewtonWorld* world = NewtonCreate();
        NewtonSetSolverModel(static_cast<const NewtonWorld*>(world), 1);
        NewtonSetThreadsCount(static_cast<const NewtonWorld*>(world), 4);

        result = new iPhysicsWorld(world);
        _worlds[result->getID()] = result;

        _worldListMutex.unlock();

        return result;
    }

    iPhysicsWorld* iPhysics::getWorld(uint64 id)
    {
        iPhysicsWorld* result;

        _worldListMutex.lock();
        auto iter = _worlds.find(id);
        if (iter != _worlds.end())
        {
            result = (*iter).second;
        }
        _worldListMutex.unlock();

        return result;
    }

    void iPhysics::destroyWorld(uint64 id)
    {
        destroyWorld(getWorld(id));
    }

    void iPhysics::destroyWorld(iPhysicsWorld* world)
    {
        con_assert(world != nullptr, "zero pointer");

        if (world != nullptr)
        {
            _worldListMutex.lock();
            auto iter = _worlds.find(world->getID());
            if (iter != _worlds.end())
            {
                NewtonWaitForUpdateToFinish(static_cast<const NewtonWorld*>((*iter).second->getNewtonWorld()));
                NewtonDestroy(static_cast<const NewtonWorld*>((*iter).second->getNewtonWorld()));
                delete (*iter).second;
                _worlds.erase(iter);
            }
            else
            {
                con_err("world id " << world->getID() << " not found");
            }
            _worldListMutex.unlock();
        }
    }

    bool iPhysics::isWorld(uint64 id)
    {
        bool result = false;

        _worldListMutex.lock();
        auto iter = _worlds.find(id);
        if (iter != _worlds.end())
        {
            result = true;
        }
        _worldListMutex.unlock();

        return result;
    }

    iPhysicsCollision* iPhysics::createBox(float64 width, float64 height, float64 depth, const iaMatrixd& offset, uint64 worldID)
    {
        iPhysicsCollision* result = nullptr;
        const NewtonWorld* world = static_cast<const NewtonWorld*>(getWorld(worldID)->getNewtonWorld());

        if (world != nullptr)
        {
            NewtonCollision* collision = NewtonCreateBox(static_cast<const NewtonWorld*>(world), width, height, depth, 0, offset.getData());
            result = new iPhysicsCollision(collision, worldID);
            NewtonCollisionSetUserID(static_cast<const NewtonCollision*>(collision), result->getID());
            NewtonCollisionSetUserData(static_cast<const NewtonCollision*>(collision), static_cast<void *const>(result));

            _collisionsListMutex.lock();
            _collisions[result->getID()] = result;
            _collisionsListMutex.unlock();
        }

        return result;
    }

    void CollideCallback(NewtonUserMeshCollisionCollideDesc* const collideDesc, const void* const continueCollisionHandle)
    {
        iPhysicsUserMeshCollisionHandler* handler = static_cast<iPhysicsUserMeshCollisionHandler*>(collideDesc->m_userData);
        handler->collideCallback(collideDesc, continueCollisionHandle);
    }

    float64 RayHitCallback(NewtonUserMeshCollisionRayHitDesc* const rayDesc)
    {
        iPhysicsUserMeshCollisionHandler* handler = static_cast<iPhysicsUserMeshCollisionHandler*>(rayDesc->m_userData);
        return handler->rayHitCallback(rayDesc);
    }

    void DestroyCallback(void* userData)
    {
        iPhysicsUserMeshCollisionHandler* handler = static_cast<iPhysicsUserMeshCollisionHandler*>(userData);
        handler->destroyCallback();
    }

    void GetCollisionInfo(void* const userData, NewtonCollisionInfoRecord* const infoRecord)
    {
        iPhysicsUserMeshCollisionHandler* handler = static_cast<iPhysicsUserMeshCollisionHandler*>(userData);
        handler->getCollisionInfo(infoRecord);
    }

    int AABBOverlapTest(void* userData, const float64* const box0, const float64* const box1)
    {
        iPhysicsUserMeshCollisionHandler* handler = static_cast<iPhysicsUserMeshCollisionHandler*>(userData);
        return handler->testOverlapAABB(box0, box1);
    }

    int GetFacesInAABB(void* userData, const float64* p0, const float64* p1, const float64** vertexArray, int* vertexCount, int* vertexStrideInBytes, const int* indexList, int maxIndexCount, const int* userDataList)
    {
        iPhysicsUserMeshCollisionHandler* handler = static_cast<iPhysicsUserMeshCollisionHandler*>(userData);
        return handler->getFacesInAABB(p0, p1, vertexArray, vertexCount, vertexStrideInBytes, indexList, maxIndexCount, userDataList);
    }


    iPhysicsCollision* iPhysics::createUserMeshCollision(const iaVector3d& minBox, const iaVector3d& maxBox, iPhysicsUserMeshCollisionHandler* handler, uint64 worldID)
    {
        iPhysicsCollision* result = nullptr;
        const NewtonWorld* world = static_cast<const NewtonWorld*>(getWorld(worldID)->getNewtonWorld());

        if (world != nullptr)
        {
            NewtonCollision* collision = NewtonCreateUserMeshCollision(static_cast<const NewtonWorld*>(world), minBox.getData(), maxBox.getData(), handler,
                CollideCallback, reinterpret_cast<NewtonUserMeshCollisionRayHitCallback>(RayHitCallback), DestroyCallback,
                GetCollisionInfo, reinterpret_cast<NewtonUserMeshCollisionAABBTest>(AABBOverlapTest), GetFacesInAABB, nullptr, 0);

            result = new iPhysicsCollision(collision, worldID);
            NewtonCollisionSetUserID(static_cast<const NewtonCollision*>(collision), result->getID());
            NewtonCollisionSetUserData(static_cast<const NewtonCollision*>(collision), static_cast<void *const>(result));

            _collisionsListMutex.lock();
            _collisions[result->getID()] = result;
            _collisionsListMutex.unlock();
        }

        return result;
    }

    iPhysicsCollision* iPhysics::createSphere(float64 radius, const iaMatrixd& offset, uint64 worldID)
    {
        iPhysicsCollision* result = nullptr;
        const NewtonWorld* world = static_cast<const NewtonWorld*>(getWorld(worldID)->getNewtonWorld());

        if (world != nullptr)
        {
            NewtonCollision* collision = NewtonCreateSphere(static_cast<const NewtonWorld*>(world), radius, 0, offset.getData());
            result = new iPhysicsCollision(collision, worldID);
            NewtonCollisionSetUserID(static_cast<const NewtonCollision*>(collision), result->getID());
            NewtonCollisionSetUserData(static_cast<const NewtonCollision*>(collision), static_cast<void *const>(result));

            _collisionsListMutex.lock();
            _collisions[result->getID()] = result;
            _collisionsListMutex.unlock();
        }

        return result;
    }

    iPhysicsCollision* iPhysics::createCone(float64 radius, float64 height, const iaMatrixd& offset, uint64 worldID)
    {
        iPhysicsCollision* result = nullptr;
        const NewtonWorld* world = static_cast<const NewtonWorld*>(getWorld(worldID)->getNewtonWorld());

        if (world != nullptr)
        {
            NewtonCollision* collision = NewtonCreateCone(static_cast<const NewtonWorld*>(world), radius, height, 0, offset.getData());
            result = new iPhysicsCollision(collision, worldID);
            NewtonCollisionSetUserID(static_cast<const NewtonCollision*>(collision), result->getID());
            NewtonCollisionSetUserData(static_cast<const NewtonCollision*>(collision), static_cast<void *const>(result));

            _collisionsListMutex.lock();
            _collisions[result->getID()] = result;
            _collisionsListMutex.unlock();
        }

        return result;
    }

    iPhysicsCollision* iPhysics::createCapsule(float64 radius0, float64 radius1, float64 height, const iaMatrixd& offset, uint64 worldID)
    {
        iPhysicsCollision* result = nullptr;
        const NewtonWorld* world = static_cast<const NewtonWorld*>(getWorld(worldID)->getNewtonWorld());

        if (world != nullptr)
        {
            NewtonCollision* collision = NewtonCreateCapsule(static_cast<const NewtonWorld*>(world), radius0, radius1, height, 0, offset.getData());
            result = new iPhysicsCollision(collision, worldID);
            NewtonCollisionSetUserID(static_cast<const NewtonCollision*>(collision), result->getID());
            NewtonCollisionSetUserData(static_cast<const NewtonCollision*>(collision), static_cast<void *const>(result));

            _collisionsListMutex.lock();
            _collisions[result->getID()] = result;
            _collisionsListMutex.unlock();
        }

        return result;
    }

    iPhysicsCollision* iPhysics::createCylinder(float64 radius0, float64 radius1, float64 height, const iaMatrixd& offset, uint64 worldID)
    {
        iPhysicsCollision* result = nullptr;
        const NewtonWorld* world = static_cast<const NewtonWorld*>(getWorld(worldID)->getNewtonWorld());

        if (world != nullptr)
        {
            NewtonCollision* collision = NewtonCreateCylinder(static_cast<const NewtonWorld*>(world), radius1, radius0, height, 0, offset.getData());
            result = new iPhysicsCollision(collision, worldID);
            NewtonCollisionSetUserID(static_cast<const NewtonCollision*>(collision), result->getID());
            NewtonCollisionSetUserData(static_cast<const NewtonCollision*>(collision), static_cast<void *const>(result));

            _collisionsListMutex.lock();
            _collisions[result->getID()] = result;
            _collisionsListMutex.unlock();
        }

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

        return result;
    }

    void iPhysics::updateMatrix(void* newtonBody, const iaMatrixd& matrix)
    {
        NewtonBodySetMatrix(static_cast<const NewtonBody*>(newtonBody), matrix.getData());
    }

    void iPhysics::getMatrix(void* newtonBody, iaMatrixd& matrix)
    {
        NewtonBodyGetMatrix(static_cast<const NewtonBody*>(newtonBody), matrix.getData());
    }

    void iPhysics::setMassMatrix(void* newtonBody, float64 mass, float64 Ixx, float64 Iyy, float64 Izz)
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

    iPhysicsCollision* iPhysics::createMesh(iMeshPtr mesh, int64 faceAttribute, const iaMatrixd& offset, uint64 worldID)
    {
        con_assert(mesh != nullptr, "zero pointer");

        iPhysicsCollision* result = nullptr;
        if (mesh != nullptr)
        {
            const NewtonWorld* world = static_cast<const NewtonWorld*>(getWorld(worldID)->getNewtonWorld());

            if (world != nullptr)
            {
                NewtonCollision* collision = NewtonCreateTreeCollision(static_cast<const NewtonWorld*>(world), 0);

                NewtonTreeCollisionBeginBuild(collision);

                float64 temp[9];

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
                    temp[3] = vertexes[vertexPos++];
                    temp[4] = vertexes[vertexPos++];
                    temp[5] = vertexes[vertexPos++];

                    vertexPos = (indexes[i + 2] * vertexFloatCount);
                    temp[6] = vertexes[vertexPos++];
                    temp[7] = vertexes[vertexPos++];
                    temp[8] = vertexes[vertexPos++];

                    NewtonTreeCollisionAddFace(collision, 3, temp, sizeof(float64) * 3, faceAttribute);
                }

                NewtonTreeCollisionEndBuild(collision, 0);

                result = new iPhysicsCollision(collision, worldID);
                NewtonCollisionSetUserID(static_cast<const NewtonCollision*>(collision), result->getID());
                NewtonCollisionSetUserData(static_cast<const NewtonCollision*>(collision), static_cast<void *const>(result));

                _collisionsListMutex.lock();
                _collisions[result->getID()] = result;
                _collisionsListMutex.unlock();
            }
        }
        else
        {
            con_err("invalid parameters");
        }

        return result;
    }

}