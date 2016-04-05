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

#ifndef __iPHYSICSBODY__
#define __iPHYSICSBODY__

#include <iDefines.h>
#include <iNode.h>

#include <iaEvent.h>
#include <iaMatrix.h>
using namespace IgorAux;

#include <memory>
#include <mutex>
using namespace std;

namespace Igor
{

    class iNodeTransform;
    class iPhysicsBody;

    /*! event called by newton to apply force and Torque
    */
    iaEVENT(iApplyForceAndTorqueEvent, iApplyForceAndTorqueDelegate, void, (iPhysicsBody* body, float32 timestep, int threadIndex), (body, timestep, threadIndex));

    /*! wrapper for the NewtonBody handle
    */
    class Igor_API iPhysicsBody
    {

        friend class iPhysics;
        friend void PhysicsNodeDestructor(const void* body);
        friend void PhysicsApplyForceAndTorque(const void* body, float64 timestep, int threadIndex);
        friend void PhysicsNodeSetTransform(const void* body, const float* matrix, int threadIndex);

    public:

        static const uint64 INVALID_BODY_ID = 0;

        /*! updates matrix of newton body

        \param matrix the matrix
        */
        void setMatrix(const iaMatrixf& matrix);

        /*! changes the mass of the newton body

        \param mass the mass of the newton body (if zero the body is considered to be a non moveable object).
        */
        void setMass(float32 mass);

        /*! \retruns mass of body
        */
        float32 getMass();

        void setAngularDamping(const iaVector3f& damping);
        const iaVector3f& getAngularDamping() const;

        void setLinearDamping(float32 damping);
        float32 getLinearDamping() const;

        const iaVector3f& getForce() const;
        void setForce(const iaVector3f& force);

        void setUserData(uint64 userID);
        uint64 getUserData() const;

        const iaVector3f& getTorque() const;
        void setTorque(const iaVector3f& torque);

        iaVector3f getVelocity() const;

        const iaVector3f& getOmega() const;
        
        const iaMatrixf& getTransformNodeMatrix() const;

        uint32 getTransformNode() const;

        void setMaterial(int64 id);
        int64 getMaterial() const;
        
        /*! \returns body ID
        */
        uint64 getID();

        /*! \returns pointer to newton body
        */
        void* getNewtonBody();

        /*! registers to apply force and tourque event

        \param applyForceAndTorqueDelegate the delegate to register
        */
        void registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate applyForceAndTorqueDelegate);

        /*! unregisters from apply force and tourque event

        \param applyForceAndTorqueDelegate the delegate to unregister
        */
        void unregisterForceAndTorqueDelegate(iApplyForceAndTorqueDelegate applyForceAndTorqueDelegate);

    protected:

        int64 _materialID = 0;

        /*! body id
        */
        uint64 _id = 0;

        uint64 _userData = 0;

        /*! the mass of this body
        */
        float32 _mass = 0;

        iaVector3f _angularDamping;
        float32 _linearDamping;

        iaVector3f _force;
        iaVector3f _torque;
        iaVector3f _velocity;
        iaVector3f _omega;

        iaMatrixf _matrix;

        /*! handle to newton body
        */
        void* _newtonBody = nullptr;

        /*! bound transform node
        */
        uint32 _transformNodeID = iNode::INVALID_NODE_ID;

        void setTransformNodeMatrix(const iaMatrixf& matrix);
        void setOmega(const iaVector3f& omega);

        /*!
        \todo implement an event for this
        */
        virtual void applyForceAndTorque(float64 timestep, int threadIndex);        

        /*! initializes newton body handle

        \param newtonBody handle to newton body
        */
        iPhysicsBody(void* newtonBody, uint64 bodyID);

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
        void bindTransformNode(iNodeTransform* transformNode);

    };

}

#endif
