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

#ifndef IGOR_GLOBAL_BOUNDARY_COMPONENT_H
#define IGOR_GLOBAL_BOUNDARY_COMPONENT_H

#include <igor/entities/iEntityComponent.h>

#include <iaux/system/iaEvent.h>
using namespace iaux;

#include <any>

namespace igor
{
    /*! global boundary contraint type
     */
    enum class iGlobalBoundaryType
    {
        None, //! no constraint
        Normal, //! do not allow entity to leave boundaries
        Repeat //! if the entity leaves the bounds it reappears at the other end of the boundaries
    };

    /*! keeps entities within some global boundaries

    check the scene interface for configuring the global boundaries
    */
    class iGlobalBoundaryComponent : public iEntityComponent
    {
    public:
        /*! ctor

        \param type the type to set
        */
        iGlobalBoundaryComponent(iGlobalBoundaryType type = iGlobalBoundaryType::Normal);

        /*! creates instance of this component type
         */
        static iEntityComponent *createInstance();

        /*! \returns global boundary type
         */
        iGlobalBoundaryType getType() const;

        /*! sets global boundary type

        \param type the type to set
        */
        void setType(iGlobalBoundaryType type);

    private:
        /*! global boundary type
         */
        iGlobalBoundaryType _type = iGlobalBoundaryType::None;

        /*! \returns a copy of this component
         */
        iEntityComponentPtr getCopy() override;
    };
}

#endif // IGOR_GLOBAL_BOUNDARY_COMPONENT_H