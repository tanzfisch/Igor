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
// (c) Copyright 2012-2026 by Martin A. Loga
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

    /*! state machine
     */
    class IAUX_API iaStateMachine
    {

    public:
        /*! event type definition
        */
        enum class iaEvent
        {
            Enter,      // entered state during update
            ReEnter,    // re entered the same state during update
            NoChange    // no change in state
        };

        /*! does nothing
         */
        iaStateMachine() = default;

        /*! does nothing
         */
        ~iaStateMachine() = default;        

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

        /*! processes the state machine
        \returns latest event
         */
        iaStateMachine::iaEvent update();

        /*! starts the state machine at it initial state
        */
        void start();

        /*! \returns true if state machine is well defined and ready to use
        */
        bool isValid() const;

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

        /*! last even triggered
        */
        iaStateMachine::iaEvent _lastEvent;

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