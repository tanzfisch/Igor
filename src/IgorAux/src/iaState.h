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

#ifndef __IGOR_AUX_STATE__
#define __IGOR_AUX_STATE__

#include <iaDefines.h>
#include <iaEvent.h>

#include <vector>


namespace IgorAux
{

    /*! enter state event
    */
    iaEVENT(EnterStateEvent, EnterStateDelegate, void, (), ());

    /*! re enter state event
    */
    iaEVENT(ReEnterStateEvent, ReEnterStateDelegate, void, (), ());

    /*! leave state event
    */
    iaEVENT(LeaveStateEvent, LeaveStateDelegate, void, (), ());

    /*! handle state event
    */
    iaEVENT(HandleStateEvent, HandleStateDelegate, void, (), ());

    /*! render state event
    */
    iaEVENT(RenderStateEvent, RenderStateDelegate, void, (), ());

    /*! state in state machine

    Example:
    \ref StateMachine/src/StateMachineExample.cpp "StateMachine usage example"
    */
    class iaState
    {

    public:

        static const uint32 INVALID_STATE_ID = 0;

        /*! \returns the id of this state
        */
        uint32 getID();

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
        void handle();

        /*! triggers render event
        */
        void render();

        /*! register delegate to enter event

        \param enterStateDelegate delegate to register
        */
        void registerEnterStateDelegate(EnterStateDelegate enterStateDelegate);

        /*! register delegate to reenter event

        \param reEnterStateDelegate delegate to register
        */
        void registerReEnterStateDelegate(ReEnterStateDelegate reEnterStateDelegate);

        /*! register delegate to leave event

        \param leaveStateDelegate delegate to register
        */
        void registerLeaveStateDelegate(LeaveStateDelegate leaveStateDelegate);

        /*! register delegate to handle event

        \param handleStateDelegate delegate to register
        */
        void registerHandleStateDelegate(HandleStateDelegate handleStateDelegate);

        /*! register delegate to render event

        \param renderStateDelegate delegate to register
        */
        void registerRenderStateDelegate(RenderStateDelegate renderStateDelegate);

        /*! unregister delegate from enter event

        \param enterStateDelegate enter state delegate
        */
        void unregisterEnterStateDelegate(EnterStateDelegate enterStateDelegate);

        /*! unregister delegate from reenter event

        \param reEnterStateDelegate reenter state delegate
        */
        void unregisterReEnterStateDelegate(ReEnterStateDelegate reEnterStateDelegate);

        /*! unregister delegate from leave event

        \param leaveStateDelegate leave state delegate
        */
        void unregisterLeaveStateDelegate(LeaveStateDelegate leaveStateDelegate);

        /*! unregister delegate from handle event

        \param handleStateDelegate handle state delegate
        */
        void unregisterHandleStateDelegate(HandleStateDelegate handleStateDelegate);

        /*! unregister delegate from render event

        \param renderStateDelegate render state delegate
        */
        void unregisterRenderStateDelegate(RenderStateDelegate renderStateDelegate);

        /*! init id
        */
        iaState();

        /*! does nothing
        */
        virtual ~iaState() = default;

    private:

        /*! id of the next state created
        */
        static uint32 _nextID;

        /*! id of this state
        */ 
        uint32 _id = INVALID_STATE_ID;

        /*! enter state event
        */
        EnterStateEvent _enterStateEvent;

        /*! reenter state event
        */
        ReEnterStateEvent _reEnterStateEvent;

        /*! leave state event
        */
        LeaveStateEvent _leaveStateEvent;

        /*! handle state event
        */
        HandleStateEvent _handleStateEvent;

        /*! render state event
        */
        RenderStateEvent _renderStateEvent; 

    };

}

#endif