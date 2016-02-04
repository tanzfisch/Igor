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
// (c) Copyright 2014-2015 by Martin Loga
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
    class iMesh;

    /*! wrapper for newton game dynamics

    Examples:
    \ref Physics/src/PhysicsExample.cpp "Physics usage example"

    \todo way to many friends
    */
    class Igor_API iPhysics : public iaSingleton<iPhysics>
    {
        friend void PhysicsNodeSetTransform(const void* body, const float* matrix, int threadIndex);
        friend class iaSingleton<iPhysics>;
        friend class iPhysicsCollision;
        friend class iTaskPhysics;
        friend class iPhysicsBody; // needed so newton can call dtor on bodys by it self
        friend class iPhysicsPlayer;

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

        /*! creates newton collision in shape of a box

        \param width width of box
        \param height height of box
        \param depth depth of box
        \param offset matrix as offset to origin
        \returns shared pointer to newton collision
        */
        iPhysicsCollision* createBox(float32 width, float32 height, float32 depth, const iaMatrixf& offset);

        /*! creates a collision shape based on a mesh
        */
        iPhysicsCollision* createMesh(shared_ptr<iMesh> mesh, int64 faceAttribute, const iaMatrixf& offset);

        /*! creates newton collision in shape of a sphere

        \param radius radius of sphere
        \param offset matrix as offset to origin
        \returns shared pointer to newton collision
        */
        iPhysicsCollision* createSphere(float32 radius, const iaMatrixf& offset);

        /*! creates newton collision in shape of a cone

        \param radius radius of cone
        \param height height of cone
        \param offset matrix as offset to origin
        \returns shared pointer to newton collision
        */
        iPhysicsCollision* createCone(float32 radius, float32 height, const iaMatrixf& offset);

        /*! creates newton collision in shape of a capsule

        \param radius radius of capsule
        \param height height of capsule
        \param offset matrix as offset to origin
        \returns shared pointer to newton collision
        */
        iPhysicsCollision* createCapsule(float32 radius, float32 height, const iaMatrixf& offset);

        /*! creates newton collision in shape of a cylinder

        \param radius radius of cylinder
        \param height height of cylinder
        \param offset matrix as offset to origin
        \returns shared pointer to newton collision
        */
        iPhysicsCollision* createCylinder(float32 radius, float32 height, const iaMatrixf& offset);

        /*! creates a collision compound from multiple collisions

        \param collisions list od collisions
        \returns a compound collision
        */
        iPhysicsCollision* createCompound(vector<iPhysicsCollision*>& collisions);

        /*! creates and return a physics scene. a group of collision objects that are meant to be static
        use this for optimization
        */
        iPhysicsCollision* createScene();

        /*! adds collision to scene

        \returns scene proxy ID
        */
        uint64 addToScene(iPhysicsCollision* scene, iPhysicsCollision* collision);

        void removeFromScene(iPhysicsCollision* scene, uint64 sceneProxyID);

        /*! \returns collision by id

        \param collisionID the collision id
        */
        iPhysicsCollision* getCollision(uint64 collisionID);

        /*! destroys physics collision

        \param collision the collision to destroy
        */
        void destroyCollision(iPhysicsCollision* collision);

        /*! creates a newton body and connects it with a newton collision

        \param body the collision to connect with
        \returns shared pointer to newton body
        */
        iPhysicsBody* createBody(iPhysicsCollision* collisionVolume);

        /*! creates up vector joint

        \param body the body to connect with
        \param upVector the up vector
        */
        iPhysicsJoint* createUpVectorJoint(iPhysicsBody* body, const iaVector3f& upVector);

        /*! \returns joint by id

        \param jointID the joint id
        */
        iPhysicsJoint* getJoint(uint64 jointID);

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

        /*! returns the mass matrix from given body

        \todo have not realy understood that yet

        \param newtonBody the newton body
        \param mass the returned mass
        \param Ixx ???
        \param Iyy ???
        \param Izz ???
        */
        void getMassMatrixFromBody(void* newtonBody, float32& mass, float32& Ixx, float32& Iyy, float32& Izz);

        /*! the mass matrix of newton body

        \todo have not realy understood that yet

        \param newtonBody handle to newton body
        \param mass mass of newton body
        \param Ixx ???
        \param Iyy ???
        \param Izz ???
        */
        void setMassMatrix(void* newtonBody, float32 mass, float32 Ixx, float32 Iyy, float32 Izz);

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

        void setAngularForce(void* newtonBody, const iaVector3f& force);

        /*! updates newton bodys matrix

        \param newtonBody handle to newton body
        \param matrix the matrix
        */
        void updateMatrix(void* newtonBody, const iaMatrixf& matrix);

        /*! binds transform node to newton body

        \param body handle to newton body
        \param transformNode transform node to bind
        */
        void bindTransformNode(iPhysicsBody* body, iNodeTransform* transformNode);

    private:

        /*! next collision id
        */
        uint64 _nextCollisionID = 1;

        /*! next body id
        */
        uint64 _nextBodyID = 1;

        /*! next joint id
        */
        uint64 _nextJointID = 1;

        /*! next scene proxy id
        */
        uint64 _nextSceneProxyID = 1;

        mutex listsMutex;

        /*! list of collisions
        */
        map<uint64, iPhysicsCollision*> _collisions;

        /*! list of bodys
        */
        map<uint64, iPhysicsBody*> _bodys;

        /*! list of joints
        */
        map<uint64, iPhysicsJoint*> _joints;

        /*! list of scene proxies
        */
        map<uint64, void*> _sceneProxies;

        /*! handle to newton world
        */
        void* _world;

        /*! last frame time
        */
        float64 _lastTime;

        /*! handle for application cyclic update
        */
        bool _handleRegistered = false;

        /*! current simulation rate in Hz
        */
        static float32 _simulationRate;

        /*! prepares a just created collision

        \param collision the newton collision
        \returns the physics collision
        */
        iPhysicsCollision* prepareCollision(void* collision);

        /*! releases newton body

        \param newtonBody handle to newton body
        */
        void destroyNewtonBody(void* newtonBody);

        /*! releases newton collision

        \param collision handle to newton collision
        */
        void destroyNewtonCollision(void* collision);

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
