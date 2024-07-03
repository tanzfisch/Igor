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
// (c) Copyright 2012-2024 by Martin Loga
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
// contact: igorgameengine@protonmail.com

#ifndef __IGOR_PHYSICS__
#define __IGOR_PHYSICS__

#include <igor/physics/iPhysicsBody.h>
#include <igor/physics/iPhysicsWorld.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/resources/module/iModule.h>

#include <iaux/math/iaMatrix.h>
#include <iaux/system/iaTime.h>
#include <iaux/system/iaEvent.h>
using namespace iaux;

#include <memory>
#include <vector>
#include <map>

namespace igor
{

    class iNodeTransform;
    class iPhysicsCollision;
    class iPhysicsJoint;
    class iPhysicsMaterial;
    class iPhysicsMaterialCombo;
    class iPhysicsUserMeshCollisionHandler;
    class iPhysicsCollisionConfig;

    typedef iaDelegate<unsigned, iPhysicsBody *, iPhysicsCollision *, const void *> iRayPreFilterDelegate;

    /*! convex cast return info
    */
    struct IGOR_API iConvexCastReturnInfo
    {
        iaVector4d _point;
        iaVector4d _normal;
        int64 _contactID;
        iPhysicsBody *_hitBody;
        float64 _penetration;
    };

    /*! wrapper for newton game dynamics
    */
    class IGOR_API iPhysics : public iModule<iPhysics>
    {
        friend void PhysicsNodeDestructor(const void *body);
        friend void PhysicsNodeSetTransform(const void *body, const float64 *matrix, int threadIndex);
        friend void GenericContactProcessCompatible(const void *const newtonContactJoint, float64 timestep, int threadIndex);
        friend class iModule<iPhysics>;
        friend class iPhysicsCollision;
        friend class iTaskPhysics;
        friend class iPhysicsBody; // needed so newton can call dtor on bodys by it self
        friend class iPhysicsMaterialCombo;
        friend class iPhysicsCollisionConfig;
        friend class iApplication;

    public:
        /*! set's the simulation rate in Hz

        default is 120Hz

        \param simulationRate simulation rate in Hz
        */
        void setSimulationRate(float64 simulationRate);

        /*! \returns simulation rate
        */
        float64 getSimulationRate();

        /*! registers handle to application handle event
        */
        void start();

        /*! unregisters handle from application handle event
        */
        void stop();

        /*! creates a material

        \returns pointer to new material
        */
        iPhysicsMaterial *createMaterial(const iaString &name);

        /*! \returns collision by id

        \param collisionID the collision id
        */
        iPhysicsCollision *getCollision(uint64 collisionID);

        /*! destroys physics collision

        \param collision the collision to destroy
        */
        void destroyCollision(iPhysicsCollision *collision);

        /*! destroys collision by id

        \param collisionID the collisions id
        */
        void destroyCollision(uint64 collisionID);

        /*! creates a world

        \returns pointer to world
        */
        iPhysicsWorld *createWorld();

        /*! \returns world by id

        \param id world id
        */
        iPhysicsWorld *getWorld(uint64 id);

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
        void destroyWorld(iPhysicsWorld *world);

        /*! \returns true if id belongs to a world

        \param id id of world
        */
        bool isWorld(uint64 id);

        void convexCast(const iaMatrixd &matrix, const iaVector3d &target, iPhysicsCollision *collisionVolume, iRayPreFilterDelegate preFilterDelegate, void *userData, std::vector<iConvexCastReturnInfo> &result, int32 maxContactCount = 10);

        /*! creates a collision configuration

        \returns pointer to collision configuration
        */
        iPhysicsCollisionConfig *createCollisionConfig();

        /*! \returns collision configuration by id

        \param collisionConfigID collision configuration id
        */
        iPhysicsCollisionConfig *getCollisionConfig(uint64 collisionConfigID);

        /*! destroy collision configuration by id

        \param collisionConfigID collision configuration id
        */
        void destroyCollisionConfig(uint64 collisionConfigID);

        /*! destroy collision configuration

        \param physicsCollisionConfig the collision configuration to destroy
        */
        void destroyCollisionConfig(iPhysicsCollisionConfig *physicsCollisionConfig);

        /*! \returns true if id belongs to a collision configuration

        \param collisionConfigID id of collision configuration
        */
        bool isCollisionConfig(uint64 collisionConfigID);

        /*! creates a joint
        */
        iPhysicsJoint *createJoint(iPhysicsBody *body0, iPhysicsBody *body1 = nullptr, int maxDOF = 6);

        /*! \returns joint by id

        \param jointID the joint id
        */
        iPhysicsJoint *getJoint(uint64 jointID);

        /*! \returns true if id is a joint

        \param jointID the joint id
        */
        bool isJoint(uint64 jointID);

        /*! \returns physics material by ID

        \param id the id of the physics material
        */
        iPhysicsMaterial *getMaterial(int64 id);

        /*! \returns material id by name

        \param name the name of the material
        */
        int64 getMaterialID(const iaString &name);

        /*! configures material for body

        \param body the body to configure the material
        \param materialID the material's id to set
        */
        void setMaterial(iPhysicsBody *body, int64 materialID);

        /*! \returns the default material
        */
        iPhysicsMaterial *getDefaultShader();

        /*! \returns the default material id
        */
        int64 getDefaultMaterialID();

        /*! creates a newton body and connects it with a newton collision

        \param body the collision to connect with
        \returns shared pointer to newton body
        */
        iPhysicsBody *createBody(iPhysicsCollision *collisionVolume);

        /*! destroy physics body

        \param body the body to destroy
        */
        void destroyBody(iPhysicsBody *body);

        /*! destroy physics body (asynchronous)

        \param body the body to destroy
        */
        void destroyBodyAsync(iPhysicsBody *body);

        /*! destrox body by ID

        \param bodyID the ID of the body to be destroyed
        */
        void destroyBody(uint64 bodyID);

        /*! \returns body by given ID

        \pram bodyID the body's ID
        */
        iPhysicsBody *getBody(uint64 bodyID);

        /*! \returns true if id belongs to a body
        */
        bool isBody(uint64 bodyID);

        /*! creates newton collision in shape of a box

        \param width width of box
        \param height height of box
        \param depth depth of box
        \param offset matrix as offset to origin
        \returns physics collision
        */
        iPhysicsCollision *createBox(float64 width, float64 height, float64 depth, const iaMatrixd &offset);

        /*! creates a collision shape based on a mesh

        \param mesh the mesh
        \param faceAttribute an integer attribute associated with the faces
        \param offset local offset matrix
        \returns physics collision
        */
        iPhysicsCollision *createMesh(iMeshPtr mesh, int64 faceAttribute, const iaMatrixd &offset);

        /*! creates newton collision in shape of a sphere

        \param radius radius of sphere
        \param offset matrix as offset to origin
        \returns physics collision
        */
        iPhysicsCollision *createSphere(float64 radius, const iaMatrixd &offset);

        /*! creates newton collision in shape of a cone

        \param radius radius of cone
        \param height height of cone
        \param offset matrix as offset to origin
        \returns physics collision
        */
        iPhysicsCollision *createCone(float64 radius, float64 height, const iaMatrixd &offset);

        /*! creates newton collision in shape of a capsule

        \param radius radius of capsule
        \param height height of capsule
        \param offset matrix as offset to origin
        \returns physics collision
        */
        iPhysicsCollision *createCapsule(float64 radius0, float64 radius1, float64 height, const iaMatrixd &offset);

        /*! creates newton collision in shape of a cylinder

        \param radius radius of cylinder
        \param height height of cylinder
        \param offset matrix as offset to origin
        \returns physics collision
        */
        iPhysicsCollision *createCylinder(float64 radius0, float64 radius1, float64 height, const iaMatrixd &offset);

        /*! creates a collision compound from multiple collisions

        \param collisions list od collisions
        \returns a compound collision
        */
        iPhysicsCollision *createCompound(std::vector<iPhysicsCollision *> &collisions);

        /*! returns the mass matrix from given body

        \todo have not realy understood that yet

        \param newtonBody the newton body
        \param mass the returned mass
        \param Ixx ???
        \param Iyy ???
        \param Izz ???
        */
        void getMassMatrix(void *newtonBody, float64 &mass, float64 &Ixx, float64 &Iyy, float64 &Izz);

        /*! the mass matrix of newton body

        \todo have not realy understood that yet

        \param newtonBody handle to newton body
        \param mass mass of newton body
        \param Ixx ???
        \param Iyy ???
        \param Izz ???
        */
        void setMassMatrix(void *newtonBody, float64 mass, float64 Ixx, float64 Iyy, float64 Izz);

        /*!
        */
        void setUserJointAddAngularRow(iPhysicsJoint *joint, float64 relativeAngleError, const iaVector3d &pin);
        void setUserJointSetRowMinimumFriction(iPhysicsJoint *joint, float64 friction);
        void setUserJointSetRowMaximumFriction(iPhysicsJoint *joint, float64 friction);

        /*! binds transform node to newton body

        \param body handle to newton body
        \param transformNode transform node to bind
        \param sync if true the bodys matrix will be updated with the transform node one

        \todo bind does not work prior to the transform node ist not part of the scene
        */
        void bindTransformNode(iPhysicsBody *body, iNodeTransform *transformNode, bool sync = true);

    private:
        /*! mutex to protect world list
        */
        iaMutex _worldListMutex;

        /*! mutex to protect body list
        */
        iaMutex _bodyListMutex;

        /*! mutex to protect joint list
        */
        iaMutex _jointListMutex;

        /*! mutex to protect material list
        */
        iaMutex _materialListMutex;

        /*! mutex to protect collisions list
        */
        iaMutex _collisionsListMutex;

        /*! mutex to protect collision config list
        */
        iaMutex _collisionsConfigListMutex;

        /*! default world
        */
        void *_defaultWorld = nullptr;

        /*! default world ID
        */
        uint64 _defaultWorldID = iPhysicsWorld::INVALID_WORLD_ID;

        /*! shadow world
        */
        void *_shadowWorld = nullptr;

        /*! shadow world ID
        */
        uint64 _shadowWorldID = iPhysicsWorld::INVALID_WORLD_ID;

        /*! map of worlds
        */
        std::map<uint64, iPhysicsWorld *> _worlds;

        /*! map of collision configs
        */
        std::map<uint64, iPhysicsCollisionConfig *> _collisionConfigs;

        /*! list of collisions
        */
        std::map<uint64, iPhysicsCollision *> _collisions;

        /*! list of bodies
        */
        std::map<uint64, iPhysicsBody *> _bodies;

        /*! list of bodies to delete
        */
        std::vector<iPhysicsBody *> _bodiesToDelete;

        /*! mutex to protect bodies to delete
        */
        iaMutex _mutexBodiesToDelete;

        /*! list of bodies to transform
        */
        std::vector<std::pair<iPhysicsBody *, iaMatrixd>> _bodiesToTransform;

        /*! mutex to protect bodies to transform
        */
        iaMutex _mutexBodiesToTransform;

        struct Contact
        {
            iPhysicsMaterialCombo *_material;
            iPhysicsBody *_body1;
            iPhysicsBody *_body2;
        };

        /*! queue of contacts to trigger
        */
        std::vector<Contact> _bodyContacts;

        /*! mutex to protect contacts list
        */
        iaMutex _mutexBodyContacts;

        /*! list of joints
        */
        std::map<uint64, iPhysicsJoint *> _joints;

        /*! map of materials
        */
        std::map<int64, iPhysicsMaterial *> _materials;

        /*! id of default material
        */
        int64 _defaultMaterialID = iPhysicsMaterial::INVALID_PHYSICSMATERIAL_ID;

        /*! last frame time
        */
        iaTime _lastTime;

        /*! if true physics is running
        */
        bool _running = true;

        /*! current simulation rate in Hz
        */
        float64 _simulationRate = 120.0;

        /*! queues transformation on body

        \param body the body to transform
        \param matrix the transform matrix
        */
        void queueTransformation(iPhysicsBody *body, const iaMatrixd &matrix);

        /*! queues contacr

        \param material involved material combination
        \param body1 first involved body
        \param body2 second involved body
        */
        void queueContact(iPhysicsMaterialCombo *material, iPhysicsBody *body1, iPhysicsBody *body2);

        /*! creates newton collision in shape of a box

        \param width width of box
        \param height height of box
        \param depth depth of box
        \param offset matrix as offset to origin
        \param worldID the world's id this collision will be created with
        \returns physics collision
        */
        iPhysicsCollision *createBox(float64 width, float64 height, float64 depth, const iaMatrixd &offset, uint64 worldID);

        /*! creates a collision shape based on a mesh

        \param mesh the mesh 
        \param faceAttribute an integer attribute associated with the faces
        \param offset local offset matrix
        \param worldID the world's id this collision will be created with
        \returns physics collision
        */
        iPhysicsCollision *createMesh(iMeshPtr mesh, int64 faceAttribute, const iaMatrixd &offset, uint64 worldID);

        /*! creates a user mesh collision

        \todo this is not done yet

        \param worldID the world's id this collision will be created with
        \returns physics collision
        */
        iPhysicsCollision *createUserMeshCollision(const iaVector3d &minBox, const iaVector3d &maxBox, iPhysicsUserMeshCollisionHandler *handler, uint64 worldID);

        /*! creates newton collision in shape of a sphere

        \param radius radius of sphere
        \param offset matrix as offset to origin
        \param worldID the world's id this collision will be created with
        \returns physics collision
        */
        iPhysicsCollision *createSphere(float64 radius, const iaMatrixd &offset, uint64 worldID);

        /*! creates newton collision in shape of a cone

        \param radius radius of cone
        \param height height of cone
        \param offset matrix as offset to origin
        \param worldID the world's id this collision will be created with
        \returns physics collision
        */
        iPhysicsCollision *createCone(float64 radius, float64 height, const iaMatrixd &offset, uint64 worldID);

        /*! creates newton collision in shape of a capsule

        \param radius radius of capsule
        \param height height of capsule
        \param offset matrix as offset to origin
        \param worldID the world's id this collision will be created with
        \returns physics collision
        */
        iPhysicsCollision *createCapsule(float64 radius0, float64 radius1, float64 height, const iaMatrixd &offset, uint64 worldID);

        /*! creates newton collision in shape of a cylinder

        \param radius radius of cylinder
        \param height height of cylinder
        \param offset matrix as offset to origin
        \param worldID the world's id this collision will be created with
        \returns physics collision
        */
        iPhysicsCollision *createCylinder(float64 radius0, float64 radius1, float64 height, const iaMatrixd &offset, uint64 worldID);

        /*! creates a collision compound from multiple collisions

        \param collisions list od collisions
        \param worldID the world's id this collision will be created with
        \returns a compound collision
        */
        iPhysicsCollision *createCompound(std::vector<iPhysicsCollision *> &collisions, uint64 worldID);

        /*! setup generic collision callback for specified material combination

        \param materialCombo the material combination specified
        */
        void setCollisionCallback(iPhysicsMaterialCombo *materialCombo);

        /*! sets softness between two materials

        \param materialCombo the two materials
        \param value softness valueget
        */
        void setSoftness(iPhysicsMaterialCombo *materialCombo, float64 value);

        /*! sets elasticity between two materials

        \param materialCombo the two materials
        \param elasticCoef elastic coefficient
        */
        void setElasticity(iPhysicsMaterialCombo *materialCombo, float64 elasticCoef);

        /*! sets friction between two materials

        \param materialCombo the two materials
        \param staticFriction static friction
        \param kineticFriction kinetic friction
        */
        void setFriction(iPhysicsMaterialCombo *materialCombo, float64 staticFriction, float64 kineticFriction);

        /*! creates the default material
        */
        void createDefaultMaterial();

        /*! destroys default material
		*/
        void destroyDefaultMaterial();

        /*! destroys all materials
        */
        void destroyMaterials();

        /*! creates the default world
        */
        void createDefaultWorld();

        /*! releases newton body

        \param newtonBody handle to newton body
        */
        void destroyNewtonBody(void *newtonBody);

        /*! releases newton collision

        \param collision handle to newton collision
        */
        void destroyNewtonCollision(void *collision, uint64 worldID);

        /*! updates newton bodys matrix

        \param newtonBody handle to newton body
        \param matrix the matrix
        */
        void updateMatrix(void *newtonBody, const iaMatrixd &matrix);

        /*! returns body matrix

        \param newtonBody the newton body
        \param[in,out] matrix the returned matrix
        */
        void getMatrix(void *newtonBody, iaMatrixd &matrix);

        /*! \returns pointer to user data in given newton body

        \param newtonBody the nevton body
        */
        void *getUserDataFromBody(void *newtonBody);

        /*! returns velocity for given nevton body

        \param newtonBody given newvton body
        \param velocity out value velocity
        */
        void getVelocity(void *newtonBody, iaVector3d &velocity);

        /*! set or apply force on nevton body

        \param newtonBody the body to apply the force to
        \param force the force to apply
        */
        void setForce(void *newtonBody, const iaVector3d &force);

        /*! sets angular damping of specified newton body

        \param newtonBody specified newton body
        \param angularDamp the angular damping
        */
        void setAngularDamping(void *newtonBody, const iaVector3d &angularDamp);

        /*! sets linear damping of specified newton body

        \param newtonBody specified newton body
        \param linearDamp the linear damping
        */
        void setLinearDamping(void *newtonBody, float64 linearDamp);

        /*! sets torque of specified newton body

        \param newtonBody specified newton body
        \param torque the torque
        */
        void setTorque(void *newtonBody, const iaVector3d &torque);

        /*! update newton calculations
        */
        void onUpdate();

        /*! handles queues like deleting stuff moving stuff etc.
        */
        void onUpdateQueues();

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

} // namespace igor

#endif
