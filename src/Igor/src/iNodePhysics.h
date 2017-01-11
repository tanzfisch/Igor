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

#ifndef __iNODEPHYSICS__
#define __iNODEPHYSICS__

#include <iNodeVolume.h>

#include <iTask.h>
#include <iPhysicsCollisionConfig.h>
#include <iaMatrix.h>
#include <iPhysicsBody.h>
#include <iMesh.h>

#include <iaVector3.h>
#include <iaMatrix.h>
using namespace IgorAux;

#include <memory>
using namespace std;

namespace Igor
{
    /*! node that handles physics collisions bound to one body

    \todo also need to remove things after adding them. maybe using IDs for that
    */
    class Igor_API iNodePhysics : public iNodeVolume
	{
		
		friend class iNodeVisitorUpdateTransform;
        friend class iNodeFactory;
        friend class iPhysicsManager;
        friend class iPhysicsManagerTask;

    public:

        /*! clears all lists and removes all primitives
        */
        void clear();

        /*! adds box

        \param width width of box
        \param height height of box
        \param depth depth of box
        \param offset relative position
        */
        void addBox(float32 width, float32 height, float32 depth, const iaMatrixd& offset);

        /*! adds sphere

        \param radius radius of sphere
        \param offset relative position
        */
        void addSphere(float32 radius, const iaMatrixd& offset);

        /*! adds cone

        \param radius radius of cone
        \param height height of cone
        \param offset relative position
        */
        void addCone(float32 radius, float32 height, const iaMatrixd& offset);

        /*! adds capsule

        \param radius radius of capsule
        \param height height of capsule
        \param offset relative position
        */
        void addCapsule(float32 radius, float32 height, const iaMatrixd& offset);

        /*! adds cylinder

        \param radius radius of cylinder
        \param height height of cylinder
        \param offset relative position
        */
        void addCylinder(float32 radius, float32 height, const iaMatrixd& offset);

        /*! set up mesh for physics mesh

        \param mesh the mesh to make a physics collision from
        \param faceAttribute the faceattribute for the whole mesh
        \param offset off the mesh collision object
        */
        void addMesh(shared_ptr<iMesh> mesh, int64 faceAttribute, const iaMatrixd& offset);

        /*! finalizes the collisions
        */
        void finalizeCollision(bool asynchronos = false);

        /*! \returns mass of body
        */
        float32 getMass() const;

        /*! sets mass of body

        if body is initialized the actual bodys mass will also be updated

        \param mass new mass
        */
        void setMass(float32 mass);

        /*! sets a delegate to handle the ApplyForceAndTorque event

        \param applyForceAndTorqueDelegate the delegate to register
        */
        void setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate applyForceAndTorqueDelegate);

        /*! resets the delegate to handle the ApplyForceAndTorque event
        */
        void resetForceAndTorqueDelegate();

        /*! sets material id

        \param materialID the material id
        */
        void setMaterial(int64 materialID);

        /*! \returns material id
        */
        int64 getMaterial() const;

        /*! \returns the ID of the body if exists
        */
        uint64 getBodyID() const;

        /*! sets user data

        \param userData pointer to user data
        */
        void setUserData(const void* userData);

        /*! \retruns pointer to user data
        */
        const void* getUserData() const;

        /*! \returns current torque
        */
        const iaVector3d& getTorque() const;

        /*! sets torque of the body

        \param torque the torque to set
        */
        void setTorque(const iaVector3d& torque);

        /*! \returns current velocity
        */
        iaVector3d getVelocity() const;

        /*! \returns current omega (or rotation speed)
        */
        iaVector3d getOmega() const;

        /*! sets angular damping

        \param damping damping values for all 3 axis
        */
        void setAngularDamping(const iaVector3d& damping);

        /*! \returns damping values for all 3 axis
        */
        const iaVector3d& getAngularDamping() const;

        /*! sets linear damping

        \param damping damping value for all 3 dimensions
        */
        void setLinearDamping(float64 damping);

        /*! \returns linear damping value
        */
        float64 getLinearDamping() const;

        /*! \returns current force
        */
        const iaVector3d& getForce() const;

        /*! sets force

        \param force the force that affect thos body
        */
        void setForce(const iaVector3d& force);

	private:

        /*! physics collision config ID
        */
        uint64 _physicsCollisionConfigID = iPhysicsCollisionConfig::INVALID_COLLISIONCONFIG_ID;

        /*! pending prepare collision task
        */
        uint64 _prepareCollisionTask = iTask::INVALID_TASK_ID;

        /*! pointer to user data
        */
        const void* _userData = nullptr;

        /*! the bodys mass
        */
        float64 _mass = 0;

        /*! material id
        */
        int64 _materialID = 0;

        /*! current angular damping
        */
        iaVector3d _angularDamping;

        /*! current linear damping
        */
        float32 _linearDamping;

        /*! current force
        */
        iaVector3d _force;

        /*! current tourque
        */
        iaVector3d _torque;

        /*! current velocity
        */
        iaVector3d _velocity;

        /*! handles ApplyForceAndTorque event
        */
        iApplyForceAndTorqueDelegate* _applyForceAndTorqueDelegate = nullptr;

        /*! true if the physics was initialized
        */
        bool _physicsInitialized = false;

        /*! physics body
        */
        uint64 _bodyID = iPhysicsBody::INVALID_PHYSICSBODY_ID;

        /*! called when a task was finished

        \param taskID if of task that was just finished
        */
        void onTaskFinished(uint64 taskID);

        /*! initialized physics
        */
        virtual void initPhysics();

        /*! this is called just before setScene and gives the class the chance to unregister from the current scene if set.
        */
        virtual void onPreSetScene();

        /*! this is called just after setScene and gives the class the chance to register it self to the new scene.
        */
        virtual void onPostSetScene();

        /*! set world matrix

        \param matrix matrix to set
        */
        virtual void onUpdateTransform(iaMatrixd& matrix);

        /*! checks if physics was created and forces tree to update

        \returns true update changed anything
        */
        virtual bool onUpdateData();

        /*! does nothing
        */
        void draw();

        /*! \returns true if physics was already initialized
        */
        bool isInitialized();

        /*! initializes member variables
        */
        iNodePhysics();

        /*! copy ctor
        */
        iNodePhysics(const iNodePhysics* node);

        /*! does nothing
        */
        virtual ~iNodePhysics();

	};

};

#endif

