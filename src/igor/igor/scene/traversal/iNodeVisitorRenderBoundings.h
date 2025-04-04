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
// (c) Copyright 2012-2025 by Martin Loga
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

#ifndef __IGOR_NODEVISITORRENDERBOUNDINGS__
#define __IGOR_NODEVISITORRENDERBOUNDINGS__

#include <igor/scene/traversal/iNodeVisitor.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{

    /*! traveses node tree and renders bounding boxes
    */
    class iNodeVisitorRenderBoundings : public iNodeVisitor
    {

    public:
        /*! nothing todo
        */
        iNodeVisitorRenderBoundings() = default;

        /*! does nothing
        */
        virtual ~iNodeVisitorRenderBoundings() = default;

    protected:
        /*! initialisation
        */
        void preTraverse() override;

        /*! calculates transformations and renders bounding boxes

        \param node current node
        */
        bool preOrderVisit(iNodePtr node, iNodePtr nextSibling) override;

        /*! restores transformations

        \param node current node
        */
        void postOrderVisit(iNodePtr node) override;

        /*! does nothing
        */
        void postTraverse() override;

    private:
        /*! holds a stack of matrices while traversion tree
        */
        std::vector<iaMatrixd> _matrixStack;

        /*! current matrix that eventually gets pushed on stack or came poped from stack
        */
        iaMatrixd _currentMatrix;
    };

}; // namespace igor

#endif
