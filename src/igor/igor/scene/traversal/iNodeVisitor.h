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

#ifndef __IGOR_NODEVISITOR__
#define __IGOR_NODEVISITOR__

#include <igor/iDefines.h>
#include <igor/scene/nodes/iNode.h>

#include <memory>

namespace igor
{

    /*! base class for traversing a node tree
    */
    class IGOR_API iNodeVisitor
    {

    public:
        /*! does nothing
        */
        iNodeVisitor() = default;

        /*! does nothing
        */
        virtual ~iNodeVisitor() = default;

        /*! setter if inactive children shall be traversed too

        \param traverseInactive if true inactive children will be traversed too
        */
        void setTraverseInactiveChildren(bool traverseInactive = true);

        /*! sets wether or not traversal will check for next sibling

        \param traverseSiblings if true traversal will call preOrderVisit with next sibling if available
        */
        void setTraverseSiblings(bool traverseSiblings = true);

        /*! traverses tree from given node on

        \param node node to start from
        */
        void traverseTree(iNodePtr node);

    private:
        /*! flag if inactive children shall be traversed too
        */
        bool _traverseInactiveChildren = false;

        /*! if true traversal will call preOrderVisit with next sibling if available
        */
        bool _traverseSiblings = false;

        /*! internal recursive traverse methods
        */
        void traverseTreeInternal(iNodePtr node, iNodePtr nextSibling);
        void traverseTreeInternalWithInactive(iNodePtr node, iNodePtr nextSibling);
        void traverseTreeInternal(iNodePtr node);
        void traverseTreeInternalWithInactive(iNodePtr node);

    protected:
        /*! is called before traversal

        has to be implemented by deriving class
        */
        virtual void preTraverse() = 0;

        /*! is called before every node visited

        has to be implemented by deriving class
        */
        virtual bool preOrderVisit(iNodePtr node, iNodePtr nextSibling) = 0;

        /*! is called after every node visited

        has to be implemented by deriving class
        */
        virtual void postOrderVisit(iNodePtr node) = 0;

        /*! is called after the whole traversal

        has to be implemented by deriving class
        */
        virtual void postTraverse() = 0;
    };

}; // namespace igor

#endif // __IGOR_NODEVISITOR__
