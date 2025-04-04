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

#ifndef IGOR_ENTITY_COPY_TRAVERSER_H
#define IGOR_ENTITY_COPY_TRAVERSER_H

#include <igor/entities/traversal/iEntityTraverser.h>

#include <igor/resources/prefab/iPrefab.h>

namespace igor
{

    /*! entity copy traverser
     */
    class iEntityCopyTraverser : public iEntityTraverser
    {

    public:
        /*! copy hierarchy of entities to given target entity

        \param prefab the prefab to copy from
        \param entity the given target entity
         */
        iEntityCopyTraverser(const iPrefabPtr &prefab, const iEntityPtr &targetEntity);

        /*! does nothing
         */
        ~iEntityCopyTraverser() = default;

    private:
        /*! the source prefab
        */
        iPrefabPtr _prefab;

        /*! the target entity id
        */
        iEntityPtr _targetEntity;

        /*! the target scene
        */
        iEntityScenePtr _targetScene = nullptr;

        /*! entity stack for quick access
        */
        std::vector<iEntityPtr> _entityStack;

        /*! the current target entity
        */
        iEntityPtr _currentEntity = nullptr;

        /*! is called before traversal

        has to be implemented by deriving class
        */
        void preTraverse() override;

        /*! is called before every entity visited

        has to be implemented by deriving class
        */
        bool preOrderVisit(iEntityPtr entity) override;

        /*! is called after every entity visited

        has to be implemented by deriving class
        */
        void postOrderVisit(iEntityPtr entity) override;

        /*! is called after the whole traversal

        has to be implemented by deriving class
        */
        void postTraverse() override;
    };

}; // namespace igor

#endif // IGOR_ENTITY_COPY_TRAVERSER_H