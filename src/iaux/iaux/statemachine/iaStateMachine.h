//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
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

#ifndef IAUX_STATEMACHINE_H
#define IAUX_STATEMACHINE_H

#include <iaux/iaDefines.h>
#include <iaux/statemachine/iaTransition.h>

#include <map>
#include <vector>

namespace iaux
{

    /*! enter state event
     */
    IGOR_EVENT_DEFINITION(iaEnterState, iaStateID);

    /*! re enter state event
     */
    IGOR_EVENT_DEFINITION(iaReEnterState, iaStateID);

    /*! leave state event
     */
    IGOR_EVENT_DEFINITION(iaLeaveState, iaStateID);

    /*! handle state event
     */
    IGOR_EVENT_DEFINITION(iaUpdateState, iaStateID);

    /*! state machine
     */
    class IAUX_API iaStateMachine
    {

    public:
        /*! creates and adds a new state

        \param name optional name of state
        \returns new state id
        */
        iaStateID addState(const iaString &name = "");

        /*! sets the initial state by id

        \param id the specified state id
        */
        void setInitialState(iaStateID id);

        /*! \returns initial state
         */
        iaStateID getInitialState() const;

        /*! \returns current state id
         */
        iaStateID getCurrentState() const;

        /*! creates transition form source state to destination state

        \param from source state
        \param to destination state
        \returns transition id
        */
        iaTransitionID addTransition(iaStateID from, iaStateID to);

        /*! \returns outgoing transition for given state id

        \param stateID the given state id
        */
        std::vector<iaTransitionID> getOutgoingTransitions(iaStateID stateID);

        /*! activate a gate for given transition

        \param transitionID the given transition
        \param index the gate index to activate (0-15)
        */
        void activateGate(iaTransitionID transitionID, uint32 index);

        /*! deactivate a gate for given transition

        \param transitionID the given transition
        \param index the gate index to deactivate (0-15)
        */
        void deactivateGate(iaTransitionID transitionID, uint32 index);

        /*! open a gate for given transition

        \param transitionID the given transition
        \param index the gate index to open (0-15)
        */
        void openGate(iaTransitionID transitionID, uint32 index);

        /*! close a gate for given transition

        \param transitionID the given transition
        \param index the gate index to close (0-15)
        */
        void closeGate(iaTransitionID transitionID, uint32 index);

        /*! closes all gates on for given transition

        \param transitionID the given transition
        */
        void resetGates(iaTransitionID transitionID);

        /*! calls the update of the current state
         */
        void update();

        /*! finalizes the state machine.

        after this you can not manipulate the state machine anymore
        */
        void start();

        /*! register delegate to enter event of specified state

        \param enterStateDelegate enter state delegate
        */
        void registerEnterStateDelegate(iaEnterStateDelegate enterStateDelegate);

        /*! register delegate to reenter event of specified state

        \param reEnterStateDelegate reenter state delegate
        */
        void registerReEnterStateDelegate(iaReEnterStateDelegate reEnterStateDelegate);

        /*! register delegate to leave event of specified state

        \param leaveStateDelegate leave state delegate
        */
        void registerLeaveStateDelegate(iaLeaveStateDelegate leaveStateDelegate);

        /*! register delegate to update event of specified state

        \param updateStateDelegate update state delegate
        */
        void registerUpdateStateDelegate(iaUpdateStateDelegate updateStateDelegate);

        /*! unregister delegate from enter event with specified state

        \param enterStateDelegate enter state delegate
        */
        void unregisterEnterStateDelegate(iaEnterStateDelegate enterStateDelegate);

        /*! unregister delegate from reenter event with specified state

        \param reEnterStateDelegate reenter state delegate
        */
        void unregisterReEnterStateDelegate(iaReEnterStateDelegate reEnterStateDelegate);

        /*! unregister delegate from leave event with specified state

        \param leaveStateDelegate leave state delegate
        */
        void unregisterLeaveStateDelegate(iaLeaveStateDelegate leaveStateDelegate);

        /*! unregister delegate from update event with specified state

        \param updateStateDelegate update state delegate
        */
        void unregisterUpdateStateDelegate(iaUpdateStateDelegate updateStateDelegate);

        /*! does nothing
         */
        iaStateMachine() = default;

        /*! does nothing
         */
        ~iaStateMachine() = default;

    private:
        /*! pointer to initial state
         */
        iaStatePtr _initState = nullptr;

        /*! pointer to current active state
         */
        iaStatePtr _currentState = nullptr;

        /*! map of all states
         */
        std::map<iaStateID, iaStatePtr> _states;

        /*! map of all transitions
         */
        std::map<iaTransitionID, iaTransitionPtr> _transitions;

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

        /*! triggers the current states enter event
         */
        void enterCurrentState();

        /*! triggers the current states reenter event
         */
        void reEnterCurrentState();

        /*! triggers a transition
         */
        void transit(iaTransition *transition);

        /*! \returns pointer to specified state

        \param id specified state id
        */
        iaStatePtr getState(iaStateID id);

        /*! \returns pointer to specified transition

        \param id specified transition id
        */
        iaTransition *getTransition(uint32 id);
    };

    /*! \example StateMachine/src/StateMachineExample.cpp
    State machine usage example.

    \include StateMachine/src/StateMachineExample.h
    */

} // namespace iaux

#endif // IAUX_STATEMACHINE_H