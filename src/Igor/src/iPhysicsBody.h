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

#ifndef __iPHYSICSBODY__
#define __iPHYSICSBODY__

#include <iDefines.h>

#include <iaEvent.h>
#include <iaMatrix.h>
using namespace IgorAux;

#include <memory>
#include <mutex>
using namespace std;

namespace Igor
{

    class iEntity;
    class iPhysicsBody;

    /*! event called by newton to apply force and Torque
    */
    iaEVENT(iApplyForceAndTorqueEvent, iApplyForceAndTorqueDelegate, void, (iPhysicsBody* body, float32 timestep, int threadIndex), (body, timestep, threadIndex));

    /*! wrapper for the NewtonBody handle
    */
    class Igor_API iPhysicsBody
    {

        friend class iPhysics;
        friend class iEntity;
        friend void PhysicsNodeDestructor(const void* body);
        friend void PhysicsApplyForceAndTorque(const void* body, float32 timestep, int threadIndex);
        friend void PhysicsNodeSetTransform(const void* body, const float* matrix, int threadIndex);

    public:

        /*! updates matrix of newton body

        \param matrix the matrix
        */
        void updateMatrix(const iaMatrixf& matrix);

        /*! changes the mass of the newton body

        \param mass the mass of the newton body (if zero the body is considered to be a non moveable object).
        */
        void setMass(float32 mass);

        /*! \retruns mass of body
        */
        float32 getMass();

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

        /*! body id
        */
        uint64 _id = 0;

        /*! the mass of this body
        */
        float32 _mass = 0;

        /*! handle to newton body
        */
        void* _newtonBody = nullptr;

        /*! weak pointer to corresponding entity
        */
        iEntity* _entity = nullptr;

        /*!
        \todo implement an event for this
        */
        virtual void ApplyForceAndTorque(float32 timestep, int threadIndex);        

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

        /*! set entity

        \param entity pointer to entity under physic control
        */
        void setEntity(iEntity* entity);

        /*! \returns entity
        */
        iEntity* getEntity() const;

    };

}

#endif