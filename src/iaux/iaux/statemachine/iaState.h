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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IAUX_STATE__
#define __IAUX_STATE__

#include <iaux/system/iaEvent.h>
#include <iaux/data/iaIDGenerator.h>
#include <iaux/data/iaString.h>

#include <vector>

namespace iaux
{

    /*! enter state event
     */
    IGOR_EVENT_DEFINITION(iaEnterState, void);

    /*! re enter state event
     */
    IGOR_EVENT_DEFINITION(iaReEnterState, void);

    /*! leave state event
     */
    IGOR_EVENT_DEFINITION(iaLeaveState, void);

    /*! handle state event
     */
    IGOR_EVENT_DEFINITION(iaUpdateState, void);

    /*! state ID definition
     */
    typedef iaID32 iaStateID;

    /*! state in state machine
     */
    class IAUX_API iaState
    {

    public:
        /*! \returns the id of this state
         */
        iaStateID getID() const;

        /*! \returns name of state
        */
        const iaString &getName() const;

        /*! triggers leave event
         */
        void leave();

        /*! triggers enter event
         */
        void enter();

        /*! triggers reenter event
         */
        void reEnter();

        /*! triggers handle event
         */
        void update();

        /*! register delegate to enter event

        \param enterStateDelegate delegate to register
        */
        void registerEnterStateDelegate(iaEnterStateDelegate enterStateDelegate);

        /*! register delegate to reenter event

        \param reEnterStateDelegate delegate to register
        */
        void registerReEnterStateDelegate(iaReEnterStateDelegate reEnterStateDelegate);

        /*! register delegate to leave event

        \param leaveStateDelegate delegate to register
        */
        void registerLeaveStateDelegate(iaLeaveStateDelegate leaveStateDelegate);

        /*! register delegate to handle event

        \param updateStateDelegate delegate to register
        */
        void registerUpdateStateDelegate(iaUpdateStateDelegate updateStateDelegate);

        /*! unregister delegate from enter event

        \param enterStateDelegate enter state delegate
        */
        void unregisterEnterStateDelegate(iaEnterStateDelegate enterStateDelegate);

        /*! unregister delegate from reenter event

        \param reEnterStateDelegate reenter state delegate
        */
        void unregisterReEnterStateDelegate(iaReEnterStateDelegate reEnterStateDelegate);

        /*! unregister delegate from leave event

        \param leaveStateDelegate leave state delegate
        */
        void unregisterLeaveStateDelegate(iaLeaveStateDelegate leaveStateDelegate);

        /*! unregister delegate from handle event

        \param updateStateDelegate handle state delegate
        */
        void unregisterUpdateStateDelegate(iaUpdateStateDelegate updateStateDelegate);

        /*! init id
         */
        iaState(const iaString &name);

        /*! does nothing
         */
        virtual ~iaState() = default;

    private:
        /*! name of state
         */
        iaString _name;

        /*! id of this state
         */
        iaStateID _id;

        /*! the id generator
         */
        static iaIDGenerator32 _idGenerator;

        /*! enter state event
         */
        iaEnterStateEvent _enterStateEvent;

        /*! reenter state event
         */
        iaReEnterStateEvent _reEnterStateEvent;

        /*! leave state event
         */
        iaLeaveStateEvent _leaveStateEvent;

        /*! update state event
         */
        iaUpdateStateEvent _updateStateEvent;
    };

    /*! state pointer definition
    */
    typedef iaState* iaStatePtr;

} // namespace iaux

#endif