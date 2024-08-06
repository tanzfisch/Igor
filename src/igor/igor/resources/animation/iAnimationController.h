
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

#ifndef IGOR_ANIMATION_CONTROLLER_H
#define IGOR_ANIMATION_CONTROLLER_H

#include <igor/resources/animation/iClip.h>

#include <iaux/statemachine/iaStateMachine.h>
using namespace iaux;

#include <unordered_map>

namespace igor
{

    /*! entity pointer definition
    */
    class iEntity;
    typedef iEntity *iEntityPtr;

    /*! animation controller
     */
    class IGOR_API iAnimationController
    {
        friend class iAnimationSystem;

    public:

        iAnimationController();

        void addClip(iClipPtr clip);

    private:

        /*! state machine
        */
        iaStateMachine _stateMachine;

        /*! begin state
        */
        iaStateID _begin = IGOR_INVALID_ID;

        iaTime _startTime;
        iaTime _offsetTime;

        /*! state animation map
        */
        std::unordered_map<iaStateID, iClipPtr> _clips;

        /*! update animation on given entity

        \param time the tick time
        \param entity the given entity
        */
        void update(const iaTime &time, iEntityPtr entity);

        void onEnterState(iaStateID stateID);
    };

    /*! animation controller pointer definition
     */
    typedef std::shared_ptr<iAnimationController> iAnimationControllerPtr;

} // namespace igor

#endif // IGOR_ANIMATION_CONTROLLER_H
