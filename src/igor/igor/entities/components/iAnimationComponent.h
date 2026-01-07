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
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef IGOR_ANIMATION_COMPONENT_H
#define IGOR_ANIMATION_COMPONENT_H

#include <igor/entities/iEntity.h>
#include <igor/resources/animation/iClip.h>

#include <iaux/statemachine/iaStateMachine.h>
using namespace iaux;

#include <unordered_map>

namespace igor
{
    /*! animation component
     */
    class IGOR_API iAnimationComponent : public iEntityComponent
    {
        friend class iAnimationSystem;

    public:
        /*! ctor

        \param name the name of this component
        */
        iAnimationComponent();

        /*! creates instance of this component type
         */
        static iEntityComponentPtr createInstance();

        /*! \returns type name of component
         */
        static const iaString &getTypeName();

        /*! adds clip to animation
        */
        void addClip(iClipPtr clip);

    private:
        /*! state machine
         */
        iaStateMachine _stateMachine;

        /*! begin state
         */
        iaStateID _begin = IGOR_INVALID_ID;

        /*! start time of all animation contained in this component
        */
        iaTime _startTime;

        /*! offset time used for making animations look more random
        */
        iaTime _offsetTime;

        /*! state animation map
         */
        std::unordered_map<iaStateID, iClipPtr> _clips;

        /*! \returns a copy of this component
         */
        iEntityComponentPtr getCopy() override;
    };

    /*! animation component pointer definition
    */
    typedef iAnimationComponent* iAnimationComponentPtr;
}

#endif // IGOR_ANIMATION_COMPONENT_H