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
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __iPHYSICSBODY__
#define __iPHYSICSBODY__

#include <igor/iDefines.h>
#include <igor/scene/nodes/iNode.h>
#include <igor/physics/iPhysicsMaterial.h>

#include <iaux/data/iaIDGenerator.h>
#include <iaux/system/iaEvent.h>
#include <iaux/math/iaMatrix.h>
#include <iaux/system/iaMutex.h>
using namespace iaux;

#include <memory>
#include <any>

namespace igor
{

    class iNodeTransform;
    class iPhysicsBody;

    /*! physics body id defintion
    */
    typedef uint64 iPhysicsBodyID;

    /*! event called by newton to apply force and Torque
    */
    iaEVENT(iApplyForceAndTorqueEvent, iApplyForceAndTorqueDelegate, void, (iPhysicsBody * body, float32 timestep), (body, timestep));

    /*! wrapper for the NewtonBody handle
    */
    class Igor_API iPhysicsBody
    {

        friend class iPhysics;
        friend void PhysicsNodeDestructor(const void *body);
        friend void PhysicsApplyForceAndTorque(const void *body, float64 timestep, int threadIndex);
        friend void PhysicsNodeSetTransform(const void *body, const float *matrix, int threadIndex);

    public:
        /*! defines the invalid body id
        */
        static const uint64 INVALID_PHYSICSBODY_ID = 0;

        /*! sets matrix of newton body

        \param matrix the matrix for the newton body
        */
        void setMatrix(const iaMatrixd &matrix);

        /*! returns current body matrix

		\param[out] matrix current body matrix
		*/
        void getMatrix(iaMatrixd &matrix);

        /*! returns the transform node materix

        the transform node matrix is affected by the bodys movement within the physics context (see setTransformNodeMatrix)
        returns identity materix if body is not bound to a transform node

        \param matrix[out] destination to put the matrix in
        */
        void getTransformNodeMatrix(iaMatrixd &matrix) const;

        /*! changes the mass of the newton body

        \param mass the mass of the newton body (if zero the body is considered to be a non moveable object).
        */
        void setMass(float64 mass);

        /*! \retruns mass of body
        */
        float64 getMass();

        /*! sets angular damping

        \param damping damping values for all 3 axis
        */
        void setAngularDamping(const iaVector3d &damping);

        /*! \returns damping values for all 3 axis
        */
        const iaVector3d &getAngularDamping() const;

        /*! sets linear damping

        \param damping damping value for all 3 dimensions
        */
        void setLinearDamping(float64 damping);

        /*! \returns linear damping value
        */
        float64 getLinearDamping() const;

        /*! \returns current force
        */
        const iaVector3d &getForce() const;

        /*! sets force

        \param force the force that affect thos body
        */
        void setForce(const iaVector3d &force);

        /*! sets user data

        \param userData pointer to user data
        */
        void setUserData(const std::any &userData);

        /*! \retruns pointer to user data
        */
        std::any getUserData() const;

        /*! \returns current torque
        */
        const iaVector3d &getTorque() const;

        /*! sets torque of the body

        \param torque the torque to set
        */
        void setTorque(const iaVector3d &torque);

        /*! \returns current velocity
        */
        iaVector3d getVelocity() const;

        /*! \returns current omega (or rotation speed)
        */
        const iaVector3d &getOmega() const;

        /*! \returns transform node id

        returns zero if body is not bound to a transform node
        */
        uint32 getTransformNodeID() const;

        /*! sets material id

        \param id physics material id
        */
        void setMaterial(int64 id);

        /*! \returns physics material id
        */
        int64 getMaterial() const;

        /*! \returns body ID
        */
        iPhysicsBodyID getID();

        /*! \returns pointer to newton body
        */
        void *getNewtonBody();

        /*! registers to apply force and tourque event

        \param applyForceAndTorqueDelegate the delegate to register
        */
        void registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate applyForceAndTorqueDelegate);

        /*! unregisters from apply force and tourque event

        \param applyForceAndTorqueDelegate the delegate to unregister
        */
        void unregisterForceAndTorqueDelegate(iApplyForceAndTorqueDelegate applyForceAndTorqueDelegate);

    protected:
        /*! material id of body
        */
        int64 _materialID = iPhysicsMaterial::INVALID_PHYSICSMATERIAL_ID;

        /*! body id
        */
        iPhysicsBodyID _id = INVALID_PHYSICSBODY_ID;

        /*! next body id
        */
        static iaIDGenerator64 _idGenerator;

        /*! user data
        */
        std::any _userData;

        /*! the mass of this body
        */
        float64 _mass = 0;

        /*! current angular damping
        */
        iaVector3d _angularDamping;

        /*! current linear damping
        */
        float64 _linearDamping;

        /*! current force
        */
        iaVector3d _force;

        /*! current tourque
        */
        iaVector3d _torque;

        /*! current velocity
        */
        iaVector3d _velocity;

        /*! current omega or angular speed
        */
        iaVector3d _omega;

        /*! handle to newton body
        */
        void *_newtonBody = nullptr;

        /*! bound transform node
        */
        uint32 _transformNodeID = iNode::INVALID_NODE_ID;

        /*! updates the transform node matrix by physics event

        is called by iPhysics after the body changed it's position

        \param matrix matrix to be set
        */
        void setTransformNodeMatrix(const iaMatrixd &matrix);

        /*! updates the omega by physics event

        is called by iPhysics after the body changed it's position

        \todo currently it's actually not called at all

        \param omega the omega to set
        */
        void setOmega(const iaVector3d &omega);

        /*! triggers callback for user implementation of applyForceAndTorque
        
        called by iPhysics

        \param timestep delta of time since last update
        */
        void applyForceAndTorque(float64 timestep);

        /*! initializes newton body handle

        \param newtonBody handle to newton body
        */
        iPhysicsBody(void *newtonBody);

        /*! dtor

		releases newton collision
		*/
        ~iPhysicsBody();

    private:
        /*! apply force and torque event
        */
        iApplyForceAndTorqueEvent _applyForceAndTorque;

        /*! set newton body handle to zero.

        this can only be called out of PhysicsNodeDestructor
        */
        void release();

        /*! binds transform node to physics body

        \param transformNode the transform node to bind to this body
        */
        void bindTransformNode(iNodeTransform *transformNode);
    };

} // namespace igor

#endif
