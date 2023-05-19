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

#ifndef __IAUX_STATEMACHINE__
#define __IAUX_STATEMACHINE__

#include <iaux/iaDefines.h>
#include <iaux/statemachine/iaTransition.h>

#include <map>
#include <vector>

namespace iaux
{

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

        /*! \returns current state id
        */
        iaStateID getCurrentState() const;

        /*! register delegate to enter event of specified state

        \param stateID specified state id
        \param enterStateDelegate enter state delegate
        */
        void registerEnterStateDelegate(iaStateID stateID, iaEnterStateDelegate enterStateDelegate);

        /*! register delegate to reenter event of specified state

        \param stateID specified state id
        \param reEnterStateDelegate reenter state delegate
        */
        void registerReEnterStateDelegate(iaStateID stateID, iaReEnterStateDelegate reEnterStateDelegate);

        /*! register delegate to leave event of specified state

        \param stateID specified state id
        \param leaveStateDelegate leave state delegate
        */
        void registerLeaveStateDelegate(iaStateID stateID, iaLeaveStateDelegate leaveStateDelegate);

        /*! register delegate to update event of specified state

        \param stateID specified state id
        \param updateStateDelegate update state delegate
        */
        void registerUpdateStateDelegate(iaStateID stateID, iaUpdateStateDelegate updateStateDelegate);

        /*! unregister delegate from enter event with specified state

        \param stateID specified state id
        \param enterStateDelegate enter state delegate
        */
        void unregisterEnterStateDelegate(iaStateID stateID, iaEnterStateDelegate enterStateDelegate);

        /*! unregister delegate from reenter event with specified state

        \param stateID specified state id
        \param reEnterStateDelegate reenter state delegate
        */
        void unregisterReEnterStateDelegate(iaStateID stateID, iaReEnterStateDelegate reEnterStateDelegate);

        /*! unregister delegate from leave event with specified state

        \param stateID specified state id
        \param leaveStateDelegate leave state delegate
        */
        void unregisterLeaveStateDelegate(iaStateID stateID, iaLeaveStateDelegate leaveStateDelegate);

        /*! unregister delegate from update event with specified state

        \param stateID specified state id
        \param updateStateDelegate update state delegate
        */
        void unregisterUpdateStateDelegate(iaStateID stateID, iaUpdateStateDelegate updateStateDelegate);

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

        /*! if there are no gates defined it will directly do the transition

        if there are gates defined it will only make the transition if all gates are open

        \param transitionID specified transition id
        */
        void doTransition(iaTransitionID transitionID);

        /*! calls the update of the current state
        */
        void update();

        /*! finalizes the state machine.
        
        after this you can not manipulate the state machine anymore
        */
        void start();

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

#endif