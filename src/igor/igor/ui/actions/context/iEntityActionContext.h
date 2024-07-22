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

#ifndef IGOR_ENTITY_ACTION_CONTEXT_H
#define IGOR_ENTITY_ACTION_CONTEXT_H

#include <igor/ui/actions/context/iActionContext.h>
#include <igor/entities/iEntityScene.h>

namespace igor
{

    /*! entities action context
     */
    class IGOR_API iEntityActionContext : public iActionContext
    {

    public:
        /*! init context

        \param entitySceneID the scene those entities are in
        \param entityIDs list of entity IDs to run the action on
        */
        iEntityActionContext(const iEntitySceneID &entitySceneID, const std::vector<iEntityID> &entityIDs)
        : _entitySceneID(entitySceneID), _entityIDs(entityIDs)
        {
        }

        /*! returns the scene id
         */
        iEntitySceneID getSceneID() const
        {
            return _entitySceneID;
        }

        /*! returns the scene id
         */
        const std::vector<iEntityID> &getEntities() const
        {
            return _entityIDs;
        }

    private:
        /*! entity scene id
         */
        iEntitySceneID _entitySceneID;

        /*! IDs of entities
         */
        std::vector<iEntityID> _entityIDs;
    };

}

#endif // IGOR_ENTITY_ACTION_CONTEXT_H