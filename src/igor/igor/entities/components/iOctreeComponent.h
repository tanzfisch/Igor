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

#ifndef IGOR_OCTREE_COMPONENT_H
#define IGOR_OCTREE_COMPONENT_H

#include <igor/entities/iEntity.h>

#include <igor/data/iOctree.h>

namespace igor
{
    /*! octree component
     */
    class iOctreeComponent : public iEntityComponent
    {
    public:
        /*! ctor

        \param name the name of this component
        */
        iOctreeComponent(iOctreed::ObjectPtr object = nullptr, const iaString &name = "body 2d");

        /*! octree object
         */
        iOctreed::ObjectPtr _object = nullptr;

    private:
        /*! callback to activate component
         */
        void onActivate(iEntityPtr entity) override;

        /*! callback to deactivate component
         */
        void onDeactivate(iEntityPtr entity) override;
    };

}

#endif // IGOR_OCTREE_COMPONENT_H