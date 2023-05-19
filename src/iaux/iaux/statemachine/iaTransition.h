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

#ifndef __IAUX_TRANSITION__
#define __IAUX_TRANSITION__

#include <iaux/iaDefines.h>

#include <iaux/statemachine/iaState.h>

#include <bitset>

namespace iaux
{

    /*! transition ID definition
     */
    typedef iaID32 iaTransitionID;

    /*! transition between two states

    both states are allowed to be the same. than reEnter() will be triggered

    \todo this is a bit over engineered. Maybe turn it in to a struct and move it to iaStateMachine
    */
    class IAUX_API iaTransition
    {

        friend class iaStateMachine;

    private:
        /*! transition id
         */
        iaTransitionID _id = IGOR_INVALID_ID;

        /*! the id generator
         */
        static iaIDGenerator32 _idGenerator;

        /*! source state id
         */
        iaStateID _from = IGOR_INVALID_ID;

        /*! destination state id
         */
        iaStateID _to = IGOR_INVALID_ID;

        /*! gate flags
         */
        std::bitset<16> _gates;

        /*! flags of active gates
         */
        std::bitset<16> _activeGates;

        /*! activate gate with given index

        \param index the index to activate (0-15)
         */
        void activateGate(uint32 index);

        /*! deactivate gate with given index

        \param index the index to deactivate (0-15)
         */
        void deactivateGate(uint32 index);

        /*! open gate with given index

        \param index the index to open (0-15)
         */
        void openGate(uint32 index);

        /*! close gate with given index

        \param index the index to close (0-15)
         */
        void closeGate(uint32 index);

        /*! closes all gates
         */
        void resetGates();

        /*! \returns true: if all gates are open; false: if not all gates are open
         */
        bool isOpen() const;

        /*! \returns transition is
         */
        iaTransitionID getID() const;

        /*! \returns source state id
         */
        iaStateID getFrom() const;

        /*! \returns destination state id
         */
        iaStateID getTo() const;

        /*! creates a transition from source to destination

        \param from source state id
        \param to destination state id
        */
        iaTransition(iaStateID from, iaStateID to);

        /*! does nothing
         */
        ~iaTransition() = default;
    };

    /*! transition pointer definition
     */
    typedef iaTransition *iaTransitionPtr;

} // namespace iaux

#endif