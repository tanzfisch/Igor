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

#ifndef IGOR_PREFAB_COMPONENT_H
#define IGOR_PREFAB_COMPONENT_H

#include <igor/entities/iEntity.h>

#include <igor/resources/prefab/iPrefab.h>

namespace igor
{
    /*! octree component
     */
    class iPrefabComponent : public iEntityComponent
    {
    public:
        /*! default ctor
        */
        iPrefabComponent() = default;

        /*! ctor

        \param prefab the prefab reference
        */
        iPrefabComponent(iPrefabPtr prefab);

        /*! creates instance of this component type
        */
        static iEntityComponent* createInstance();        

        /*! set prefab

        only valid before onLoad
        */
        void setPrefab(iPrefabPtr prefab);

        /*! \returns prefab
         */
        iPrefabPtr getPrefab() const;

    private:
        /*! referenced prefab data
         */
        iPrefabPtr _prefab;

        /*! callback for loading component

        \param entity the entity this component relates to
        \param[out] asyncLoad if true try again if unsuccessful
        \returns true when loading was successful
        */
        bool onLoad(iEntityPtr entity, bool &asyncLoad) override;

        /*! \returns a copy of this component
         */
        iEntityComponentPtr getCopy() override;
    };

}

#endif // #define IGOR_PREFAB_COMPONENT_H
