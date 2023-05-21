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
    };

    /*! state pointer definition
     */
    typedef iaState *iaStatePtr;

} // namespace iaux

#endif