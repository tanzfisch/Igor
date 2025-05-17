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

#ifndef IGOR_ENTITY_TRANSFORM_TRAVERSER_H
#define IGOR_ENTITY_TRANSFORM_TRAVERSER_H

#include <igor/entities/traversal/iEntityTraverser.h>

#include <igor/entities/components/iTransformComponent.h>

namespace igor
{

    /*! base class for traversing an entity tree
     */
    class IGOR_API iEntityTransformTraverser : public iEntityTraverser
    {

    public:
        /*! does nothing
         */
        iEntityTransformTraverser() = default;

        /*! does nothing
         */
        ~iEntityTransformTraverser() = default;

    private:
        /*! holds a stack of matrices while traversal tree
         */
        std::vector<iaTransformd> _transformStack;

        /*! cached current matrix
         */
        iaTransformd _currentTransform;

        /*! if true scene has quadtree
         */
        bool _hasQuadtree = false;

        /*! if true scene has octree
         */
        bool _hasOctree = false;

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

        /*! updates quadtree for given entity

        \param the given entity
        */
        void updateQuadtree(iEntityPtr entity);

        /*! updates octree for given entity

        \param the given entity
        */
        void updateOctree(iEntityPtr entity);

        /*! update the transform component

        \param transformComponent the component to update
        \param[in/out] transform the transform used for traversal
        */
        bool updateTransformComponent(iTransformComponentPtr transformComponent, iaTransformd &transform);
    };

}; // namespace igor

#endif // IGOR_ENTITY_TRANSFORM_TRAVERSER_H