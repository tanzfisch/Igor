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
// (c) Copyright 2012-2022 by Martin Loga
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

#ifndef __iPHYSICSJOINT__
#define __iPHYSICSJOINT__

#include <igor/iDefines.h>

#include <iaux/system/iaEvent.h>
#include <iaux/system/iaMutex.h>
using namespace iaux;

namespace igor
{

    class iPhysicsJoint;

    iaEVENT(iSubmitConstraintsEvent, iSubmitConstraintsDelegate, (iPhysicsJoint * joint, float32 timestep), (joint, timestep));

    /*! wrapper for the NewtonJoint handle
    */
    class IGOR_API iPhysicsJoint
    {

        friend class iPhysics;
        friend void SubmitConstraints(const void *const joint, float64 timestep, int threadIndex);

    public:
        /*! \returns pointer to newton joint
        */
        void *getNewtonJoint() const;

        /*! \returns joint ID
        */
        uint64 getID();

        uint64 getBody0ID();
        uint64 getBody1ID();

        void registerSubmitConstraintsDelegate(iSubmitConstraintsDelegate submitConstraintsDelegate);
        void unregisterSubmitConstraintsDelegate(iSubmitConstraintsDelegate submitConstraintsDelegate);

    private:
        /*! next joint id
        */
        static uint64 _nextJointID;

        static iaMutex _mutex;

        /*! id of joint object
        */
        uint64 _jointID = 0;

        uint64 _bodyID0 = 0;
        uint64 _bodyID1 = 0;

        /*! the handle to the newton joint
        */
        void *_joint = nullptr;

        iSubmitConstraintsEvent _submitConstraints;

        void submitConstraints(float64 timestep);

        /*! initializes members

        \param collision handle to newton collision
        */
        iPhysicsJoint(void *joint, uint64 body0, uint64 body1);

        /*! does nothing
        */
        virtual ~iPhysicsJoint() = default;
    };

} // namespace igor

#endif
