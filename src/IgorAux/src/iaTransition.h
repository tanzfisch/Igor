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
// (c) Copyright 2012-2018 by Martin Loga
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

#ifndef __IGOR_AUX_TRANSITION__
#define __IGOR_AUX_TRANSITION__

#include <iaDefines.h>
#include <iaEvent.h>

#include <iaState.h>

#include <map>
using namespace std;

namespace IgorAux
{

    /*! transition beween two states

    both states are alowed to be the same. than reEnter() will be triggered

    Example:
    \ref StateMachine/src/StateMachineExample.cpp "StateMachine usage example"
    */
    class IgorAux_API iaTransition
    {

        friend class iaStateMachine;

    public:

        /*! definition of invalid transition id
        */
        static const uint32 INVALID_TRANSITION_ID = 0;

    private:

        /*! definition of invalid gate id
        */
        static const uint32 INVALID_GATE_ID = 0;

        /*! holds the next transition id
        */
        static uint32 _nextID;

        /*! transition id
        */
        uint32 _id = INVALID_TRANSITION_ID;

        /*! holds the next gate id
        */
        uint32 _nextGateID = INVALID_GATE_ID + 1;

        /*! source state id
        */
        uint32 _from = iaState::INVALID_STATE_ID;

        /*! destination state id
        */
        uint32 _to = iaState::INVALID_STATE_ID;

        /*! map of gate flags
        */
        map<uint32, bool> _gates;

        /*! creates a gate

        \returns id to new gate
        */
        uint32 createGate();

        /*! sets specified gate flag to true

        if all gates are open transition will be made
        */
        void openGate(uint32 id);

        /*! sets specified gate flag to false
        */
        void closeGate(uint32 id);

        /*! reset all gate flags.

        set all gate flags to false
        */
        void resetGates();

        /*! \returns true: if all gates are open; false: if not all gates are open
        */
        bool isOpen();

        /*! \returns transition is
        */
        uint32 getID();

        /*! \returns source state id
        */
        uint32 getFrom(); 

        /*! \returns destination state id
        */
        uint32 getTo();

        /*! creates a transition from source to destination

        \param from source state id
        \param to destination state id
        */
        iaTransition(uint32 from, uint32 to);

        /*! does nothing
        */
        ~iaTransition() = default;

    };

}

#endif