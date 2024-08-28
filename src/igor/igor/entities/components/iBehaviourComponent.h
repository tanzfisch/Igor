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

#ifndef IGOR_BEHAVIOUR_COMPONENT_H
#define IGOR_BEHAVIOUR_COMPONENT_H

#include <igor/entities/iEntityComponent.h>

#include <iaux/system/iaEvent.h>
using namespace iaux;

#include <any>

namespace igor
{
    class iEntity;
    typedef iEntity* iEntityPtr;

    /*! behaviour function definition
     */
    typedef iaDelegate<void, iEntityPtr, std::any &> iBehaviourDelegate;

    /*! behaviour data
     */
    struct iBehaviourData
    {
        /*! delegate to be executed with given entity and user data
         */
        iBehaviourDelegate _delegate;

        /*! user data

        \todo this is not ideal maybe fix in #300
        */
        std::any _userData;

        /*! optional name of behaviour
         */
        iaString _name;
    };

    /*! behaviour component
     */
    class iBehaviourComponent : public iEntityComponent
    {
        friend class iEntity;
        friend class iBehaviourSystem;

    public:
        /*! ctor
         */
        iBehaviourComponent();

        /*! creates instance of this component type
         */
        static iEntityComponent *createInstance();

    private:
        /*! behaviors
         */
        std::vector<iBehaviourData> _behaviors;

        /*! \returns a copy of this component
         */
        iEntityComponentPtr getCopy() override;
    };
}

#endif // IGOR_BEHAVIOUR_COMPONENT_H