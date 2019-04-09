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
// (c) Copyright 2012-2019 by Martin Loga
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

#ifndef __IGOR_AUX_STATEMACHINE__
#define __IGOR_AUX_STATEMACHINE__

#include <iaDefines.h>
#include <iaState.h>
using namespace IgorAux;

#include <map>
using namespace std;

namespace IgorAux
{

    class iaState;
    class iaTransition;

    /*! generall state machine with only one specialisation "render()"

    Example:
    \ref StateMachine/src/StateMachineExample.cpp "StateMachine usage example"

    */
    class IgorAux_API iaStateMachine
    {

    public:

        /*! creates new state

        \returns new state id
        */
        uint32 createState();

        /*! sets the initial state by id

        \param id the specified state id
        */
        void setInitialState(uint32 id);

        /*! \returns current state id
        */
        uint32 getCurrentState();

        /*! register delegate to enter event of specified state

        \param stateID specified state id
        \param enterStateDelegate enter state delegate
        */
        void registerEnterStateDelegate(uint32 stateID, EnterStateDelegate enterStateDelegate);

        /*! register delegate to reenter event of specified state

        \param stateID specified state id
        \param reEnterStateDelegate reenter state delegate
        */
        void registerReEnterStateDelegate(uint32 stateID, ReEnterStateDelegate reEnterStateDelegate);

        /*! register delegate to leave event of specified state

        \param stateID specified state id
        \param leaveStateDelegate leave state delegate
        */
        void registerLeaveStateDelegate(uint32 stateID, LeaveStateDelegate leaveStateDelegate);

        /*! register delegate to handle event of specified state

        \param stateID specified state id
        \param handleStateDelegate handle state delegate
        */
        void registerHandleStateDelegate(uint32 stateID, HandleStateDelegate handleStateDelegate);

        /*! register delegate to render event of specified state

        \param stateID specified state id
        \param renderStateDelegate render state delegate
        */
        void registerRenderStateDelegate(uint32 stateID, RenderStateDelegate renderStateDelegate);

        /*! unregister delegate from enter event with specified state

        \param stateID specified state id
        \param enterStateDelegate enter state delegate
        */
        void unregisterEnterStateDelegate(uint32 stateID, EnterStateDelegate enterStateDelegate);
        
        /*! unregister delegate from reenter event with specified state

        \param stateID specified state id
        \param reEnterStateDelegate reenter state delegate
        */
        void unregisterReEnterStateDelegate(uint32 stateID, ReEnterStateDelegate reEnterStateDelegate);

        /*! unregister delegate from leave event with specified state

        \param stateID specified state id
        \param leaveStateDelegate leave state delegate
        */
        void unregisterLeaveStateDelegate(uint32 stateID, LeaveStateDelegate leaveStateDelegate);

        /*! unregister delegate from handle event with specified state

        \param stateID specified state id
        \param handleStateDelegate handle state delegate
        */
        void unregisterHandleStateDelegate(uint32 stateID, HandleStateDelegate runStateDelegate);

        /*! unregister delegate from render event with specified state

        \param stateID specified state id
        \param renderStateDelegate render state delegate
        */
        void unregisterRenderStateDelegate(uint32 stateID, RenderStateDelegate renderStateDelegate);

        /*! creates transition form source state to destination state

        \param from source state
        \param to destination state
        \returns transition id
        */
        uint32 createTransition(uint32 from, uint32 to);

        /*! create gate for specified transition

        \param transitionID specified transition id
        \returns new gate id
        */
        uint32 createGate(uint32 transitionID);

        /*! opens the specified gate

        if all gates are open the transition will be made

        \param transitionID specified transition id
        \param id gate id
        */
        void openGate(uint32 transitionID, uint32 id);

        /*! if there are no gates defined it will directly do the transition

        if there are gates defined it will only make the transition if all gates are open

        \param transitionID specified transition id
        */
        void doTransition(uint32 transitionID);

        /*! close specified gate of specified transition

        \param transitionID specified transition
        \param id specified gate id
        */
        void closeGate(uint32 transitionID, uint32 id);

        /*! reset all gates of specified transition

        \param transitionID specified transition id
        */
        void resetGates(uint32 transitionID);

        /*! calles the handle of the current state

        is ment to be registered to iApplicationHandleDelegate but can be called by anyone
        */
        void handle();

        /*! calles the render method of the current state

        is ment to be registered to RenderDelegate but can be called by anyone
        */
        void render();

        /*! finalizes the state machine.
        
        after this you can not manipulate the state mashine anymore
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
        iaState* _initState = nullptr;

        /*! pointer to current active state
        */
        iaState* _currentState = nullptr;

        /*! if true statemachine is allowed to change
        if false the state machine can not be changed anymore
        will be set to false by start()
        */
        bool _initialisation = true;

        /*! map of all created states
        */
        map<uint32, iaState*> _states;

        /*! map of all created transitions
        */
        map<uint32, iaTransition*> _transitions;

        /*! triggers the current states enter event
        */
        void enterCurrentState();

        /*! triggers the current states reenter event
        */
        void reEnterCurrentState();

        /*! triggers a transition
        */
        void transit(iaTransition* transition);

        /*! \returns pointer to specified state

        \param id specified state id
        */
        iaState* getState(uint32 id);

        /*! \returns pointer to specified transition

        \param id specified transition id
        */
        iaTransition* getTransition(uint32 id);

    };

    /*! \example StateMachine/src/StateMachineExample.cpp
    State machine usage example.

    \include StateMachine/src/StateMachineExample.h
    */


}

#endif