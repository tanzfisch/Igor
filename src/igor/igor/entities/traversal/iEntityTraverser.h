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

#ifndef IGOR_ENTITY_TRAVERSER_H
#define IGOR_ENTITY_TRAVERSER_H

#include <igor/iDefines.h>
#include <igor/entities/iEntityScene.h>

namespace igor
{

    /*! base class for traversing an entity tree
     */
    class IGOR_API iEntityTraverser
    {

    public:
        /*! does nothing
         */
        iEntityTraverser() = default;

        /*! does nothing
         */
        virtual ~iEntityTraverser() = default;

        /*! setter if inactive children shall be traversed too

        \param ignoreInactive if true inactive children will be skipped
        */
        void setIgnoreInactive(bool ignoreInactive);

        /*! \returns true if it is ignoring inactive children
         */
        bool isIgnoringInactive() const;

        /*! traverses tree of given entity scene

        \param scene scene to traverse
        */
        void traverse(iEntityScenePtr scene);

        /*! traverses tree under given entity

        \param entity the start point to traverse the scene
        */
        void traverse(iEntityPtr entity);

    protected:
        /*! is called before traversal

        has to be implemented by deriving class
        */
        virtual void preTraverse() = 0;

        /*! is called before every entity visited

        has to be implemented by deriving class
        */
        virtual bool preOrderVisit(iEntityPtr entity) = 0;

        /*! is called after every entity visited

        has to be implemented by deriving class
        */
        virtual void postOrderVisit(iEntityPtr entity) = 0;

        /*! is called after the whole traversal

        has to be implemented by deriving class
        */
        virtual void postTraverse() = 0;

        /*! \returns the scene that is being traversed
        */
        iEntityScenePtr getScene() const;

    private:
        /*! entity scene
         */
        iEntityScenePtr _scene = nullptr;

        /*! flag if inactive children shall be traversed too
         */
        bool _ignoreInactive = true;

        /*! internal recursive traverse methods
         */
        void traverseInternal(iEntityPtr entity, bool useInactive);
    };

}; // namespace igor

#endif // IGOR_ENTITY_TRAVERSER_H
