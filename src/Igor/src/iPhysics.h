//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2014-2016 by Martin Loga
//
// This library is free software; you can redistribute it and or modify it   
// under the terms of the GNU Lesser General Public License as published by  
// the Free Software Foundation; either version 3 of the License, or (at   
// your option) any later version.                                           
// 
// This library is distributed in the hope that it will be useful,           
// but WITHOUT ANY WARRANTY; without even the implied warranty of            
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
// Lesser General Public License for more details.                           
// 
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
// 
// contact: martinloga@gmx.de  

#ifndef __iPHYSICS__
#define __iPHYSICS__

#include <iDefines.h>
#include <iaSingleton.h>
using namespace IgorAux;

#include <iPhysicsBody.h>
#include <iPhysicsWorld.h>
#include <iaMatrix.h>
using namespace Igor;

#include <memory>
#include <vector>
#include <map>
using namespace std;

namespace Igor
{

    class iNodeTransform;
    class iPhysicsCollision;
    class iPhysicsJoint;
    class iPhysicsMaterial;
    class iPhysicsMaterialCombo;
    class iPhysicsUserMeshCollisionHandler;
    class iPhysicsCollisionConfig;
    class iMesh;

    /*! wrapper for newton game dynamics

    Examples:
    \ref Physics/src/PhysicsExample.cpp "Physics usage example"

    \todo way to many friends
    \todo update Omega of bodys
    */
    class Igor_API iPhysics : public iaSingleton<iPhysics>
    {
        friend void PhysicsNodeSetTransform(const void* body, const float* matrix, int threadIndex);
        friend class iaSingleton<iPhysics>;
        friend class iPhysicsCollision;
        friend class iTaskPhysics;
        friend class iPhysicsBody; // needed so newton can call dtor on bodys by it self
        friend class iPhysicsPlayer;
        friend class iPhysicsMaterialCombo;
        friend class iPhysicsCollisionConfig;

    public:

        /*! set's the simulation rate in Hz

        default is 120Hz

        \param simulationRate simulation rate in Hz
        */
        static void setSimulationRate(uint32 simulationRate);

        /*! \returns simulation rate
        */
        static uint32 getSimulationRate();

        /*! registers handle to application handle event
        */
        void start();

        /*! unregisters handle from application handle event
        */
        void stop();

        /*! creates a material

        \returns pointer to new material
        */
        iPhysicsMaterial* createMaterial(const iaString& name);

        /*! \returns collision by id

        \param collisionID the collision id
        */
        iPhysicsCollision* getCollision(uint64 collisionID);

        /*! destroys physics collision

        \param collision the collision to destroy
        */
        void destroyCollision(iPhysicsCollision* collision);

        /*! destroys collision by id

        \param collisionID the collisions id
        */
        void destroyCollision(uint64 collisionID);

        /*! creates a world

        \returns pointer to world
        */
        iPhysicsWorld* createWorld();

        /*! \returns world by id

        \param id world id
        */
        iPhysicsWorld* getWorld(uint64 id);

        /*! \returns default world id
        */
        uint64 getDefaultWorldID();

        /*! destroy world by id

        \param id world id
        */
        void destroyWorld(uint64 id);

        /*! destroy world

        \param world the world to destroy
        */
        void destroyWorld(iPhysicsWorld* world);

        /*! \returns true if id belongs to a world

        \param id id of world
        */
        bool isWorld(uint64 id);

        /*! creates a collision configuration

        \returns pointer to collision configuration
        */
        iPhysicsCollisionConfig* createCollisionConfig();

        /*! \returns collision configuration by id

        \param collisionConfigID collision configuration id
        */
        iPhysicsCollisionConfig* getCollisionConfig(uint64 collisionConfigID);

        /*! destroy collision configuration by id

        \param collisionConfigID collision configuration id
        */
        void destroyCollisionConfig(uint64 collisionConfigID);

        /*! destroy collision configuration

        \param physicsCollisionConfig the collision configuration to destroy
        */
        void destroyCollisionConfig(iPhysicsCollisionConfig* physicsCollisionConfig);

        /*! \returns true if id belongs to a collision configuration

        \param collisionConfigID id of collision configuration
        */
        bool isCollisionConfig(uint64 collisionConfigID);

        /*! creates a newton body and connects it with a newton collision

        \param body the collision to connect with
        \returns shared pointer to newton body
        */
        iPhysicsBody* createBody(iPhysicsCollision* collisionVolume);

        /*! creates a joint
        */
        iPhysicsJoint* createJoint(iPhysicsBody* body0, iPhysicsBody* body1 = nullptr, int dof = 6);

        /*! \returns joint by id

        \param jointID the joint id
        */
        iPhysicsJoint* getJoint(uint64 jointID);

        /*! \returns true if id is a joint

        \param jointID the joint id
        */
        bool isJoint(uint64 jointID);

        /*! \returns physics material by ID

        \param id the id of the physics material
        */
        iPhysicsMaterial* getMaterial(int64 id);

        /*! configures material for body

        \param body the body to configure the material
        \param materialID the material's id to set
        */
        void setMaterial(iPhysicsBody* body, int64 materialID);

        /*! \returns the default material
        */
        iPhysicsMaterial* getDefaultMaterial();

        /*! \returns the default material id
        */
        int64 getDefaultMaterialID();

        /*! destroy physics body

        \param body the body to destroy
        */
        void destroyBody(iPhysicsBody* body);

        /*! destrox body by ID

        \param bodyID the ID of the body to be destroyed
        */
        void destroyBody(uint64 bodyID);

        /*! \returns body by given ID

        \pram bodyID the body's ID
        */
        iPhysicsBody* getBody(uint64 bodyID);

        /*! \returns true if id belongs to a body
        */
        bool isBody(uint64 bodyID);

        /*! returns the mass matrix from given body

        \todo have not realy understood that yet

        \param newtonBody the newton body
        \param mass the returned mass
        \param Ixx ???
        \param Iyy ???
        \param Izz ???
        */
        void getMassMatrix(void* newtonBody, float32& mass, float32& Ixx, float32& Iyy, float32& Izz);        

        /*! the mass matrix of newton body

        \todo have not realy understood that yet

        \param newtonBody handle to newton body
        \param mass mass of newton body
        \param Ixx ???
        \param Iyy ???
        \param Izz ???
        */
        void setMassMatrix(void* newtonBody, float32 mass, float32 Ixx, float32 Iyy, float32 Izz);

        /*!
        */
        void setUserJointAddAngularRow(iPhysicsJoint* joint, float32 relativeAngleError, const iaVector3f& pin);
        void setUserJointSetRowMinimumFriction(iPhysicsJoint* joint, float32 friction);
        void setUserJointSetRowMaximumFriction(iPhysicsJoint* joint, float32 friction);

        /*! binds transform node to newton body

        \param body handle to newton body
        \param transformNode transform node to bind
        \param sync if true the bodys matrix will be updated with the transform node one
        */
        void bindTransformNode(iPhysicsBody* body, iNodeTransform* transformNode, bool sync = true);

    private:

        /*! mutex to protect world list
        */
        mutex _worldListMutex;

        /*! mutex to protect body list
        */
        mutex _bodyListMutex;

        /*! mutex to protect joint list
        */
        mutex _jointListMutex;

        /*! mutex to protect material list
        */
        mutex _materialListMutex;

        /*! mutex to protect collisions list
        */
        mutex _collisionsListMutex;

        /*! mutex to protect collision config list
        */
        mutex _collisionsConfigListMutex;

        /*! mutex to protect destruction and creation of newton objects
        */
        mutex _createDestroyMutex;

        /*! default world
        */
        void* _defaultWorld = nullptr;

        /*! default world ID
        */
        uint64 _defaultWorldID = iPhysicsWorld::INVALID_WORLD_ID;

        /*! map of worlds
        */
        map<uint64, iPhysicsWorld*> _worlds;

        /*! map of collision configs
        */
        map<uint64, iPhysicsCollisionConfig*> _collisionConfigs;

        /*! list of collisions
        */
        map<uint64, iPhysicsCollision*> _collisions;

        /*! list of bodys
        */
        map<uint64, iPhysicsBody*> _bodys;

        /*! list of joints
        */
        map<uint64, iPhysicsJoint*> _joints;

        /*! map of materials
        */
        map<int64, iPhysicsMaterial*> _materials;

        /*! id of default material
        */
        int64 _defaultMaterialID = iPhysicsMaterial::INVALID_PHYSICSMATERIAL_ID;

        /*! last frame time
        */
        float64 _lastTime;

        /*! handle for application cyclic update
        */
        bool _handleRegistered = false;

        /*! current simulation rate in Hz
        */
        static float32 _simulationRate;

        /*! creates newton collision in shape of a box

        \param width width of box
        \param height height of box
        \param depth depth of box
        \param offset matrix as offset to origin
        \returns shared pointer to newton collision
        */
        iPhysicsCollision* createBox(float32 width, float32 height, float32 depth, const iaMatrixf& offset, uint64 worldID);

        /*! creates a collision shape based on a mesh

        \param mesh the mesh 
        \param faceAttribute an integer attribute associated with the faces
        \param offset local offset matrix
        */
        iPhysicsCollision* createMesh(shared_ptr<iMesh> mesh, int64 faceAttribute, const iaMatrixf& offset, uint64 worldID);

        /*! creates a user mesh collision

        \todo this is not done yet
        */
        iPhysicsCollision* createUserMeshCollision(const iaVector3f& minBox, const iaVector3f& maxBox, iPhysicsUserMeshCollisionHandler* handler, uint64 worldID);

        /*! creates newton collision in shape of a sphere

        \param radius radius of sphere
        \param offset matrix as offset to origin
        \returns shared pointer to newton collision
        */
        iPhysicsCollision* createSphere(float32 radius, const iaMatrixf& offset, uint64 worldID);

        /*! creates newton collision in shape of a cone

        \param radius radius of cone
        \param height height of cone
        \param offset matrix as offset to origin
        \returns shared pointer to newton collision
        */
        iPhysicsCollision* createCone(float32 radius, float32 height, const iaMatrixf& offset, uint64 worldID);

        /*! creates newton collision in shape of a capsule

        \param radius radius of capsule
        \param height height of capsule
        \param offset matrix as offset to origin
        \returns shared pointer to newton collision
        */
        iPhysicsCollision* createCapsule(float32 radius, float32 height, const iaMatrixf& offset, uint64 worldID);

        /*! creates newton collision in shape of a cylinder

        \param radius radius of cylinder
        \param height height of cylinder
        \param offset matrix as offset to origin
        \returns shared pointer to newton collision
        */
        iPhysicsCollision* createCylinder(float32 radius, float32 height, const iaMatrixf& offset, uint64 worldID);

        /*! creates a collision compound from multiple collisions

        \param collisions list od collisions
        \returns a compound collision
        */
        iPhysicsCollision* createCompound(vector<iPhysicsCollision*>& collisions, uint64 worldID);

        /*! setup generic collision callback for specified material combination

        \param materialCombo the material combination specified
        */
        void setCollisionCallback(iPhysicsMaterialCombo* materialCombo);

        /*! sets softness between two materials

        \param materialCombo the two materials
        \param value softness valueget
        */
        void setSoftness(iPhysicsMaterialCombo* materialCombo, float32 value);

        /*! sets elasticity between two materials

        \param materialCombo the two materials
        \param elasticCoef elastic coefficient
        */
        void setElasticity(iPhysicsMaterialCombo* materialCombo, float32 elasticCoef);

        /*! sets friction between two materials

        \param materialCombo the two materials
        \param staticFriction static friction
        \param kineticFriction kinetic friction
        */
        void setFriction(iPhysicsMaterialCombo* materialCombo, float32 staticFriction, float32 kineticFriction);

        /*! creates the default material
        */
        void createDefaultMaterial();

        /*! creates the default world
        */
        void createDefaultWorld();

        /*! releases newton body

        \param newtonBody handle to newton body
        */
        void destroyNewtonBody(void* newtonBody);

        /*! releases newton collision

        \param collision handle to newton collision
        */
        void destroyNewtonCollision(void* collision, uint64 worldID);

        /*! updates newton bodys matrix

        \param newtonBody handle to newton body
        \param matrix the matrix
        */
        void updateMatrix(void* newtonBody, const iaMatrixf& matrix);

        /*! returns body matrix

        \param newtonBody the newton body
        \param[in,out] matrix the returned matrix
        */
        void getMatrix(void* newtonBody, iaMatrixf& matrix);

        /*! \returns pointer to user data in given newton body

        \param newtonBody the nevton body
        */
        void* getUserDataFromBody(void* newtonBody);

        /*! returns velocity for given nevton body

        \param newtonBody given newvton body
        \param velocity out value velocity
        */
        void getVelocity(void* newtonBody, iaVector3f& velocity);

        /*! set or apply force on nevton body

        \param newtonBody the body to apply the force to
        \param force the force to apply
        */
        void setForce(void* newtonBody, const iaVector3f& force);

        /*! sets angular damping of specified newton body

        \param newtonBody specified newton body
        \param angularDamp the angular damping
        */
        void setAngularDamping(void* newtonBody, const iaVector3f& angularDamp);

        /*! sets linear damping of specified newton body

        \param newtonBody specified newton body
        \param linearDamp the linear damping
        */
        void setLinearDamping(void* newtonBody, float32 linearDamp);

        /*! sets torque of specified newton body

        \param newtonBody specified newton body
        \param torque the torque
        */
        void setTorque(void* newtonBody, const iaVector3f& torque);

        /*! update newton calculations
        */
        void onUpdate();

        /*! initializes newton world
        */
        iPhysics();

        /*! destroys newton world

        \todo how to handle bodys and collisions that we want to destroy earlier?
        */
        ~iPhysics();

    };

    /*! \example Physics/src/PhysicsExample.cpp
    Physics usage example.

    \include Physics/src/PhysicsExample.h
    */

}

#endif
