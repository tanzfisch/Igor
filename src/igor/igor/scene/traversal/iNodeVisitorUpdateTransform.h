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

#ifndef __IGOR_NODEVISITORUPDATETRANSFORM__
#define __IGOR_NODEVISITORUPDATETRANSFORM__

#include <igor/scene/traversal/iNodeVisitor.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

#include <vector>

namespace igor
{

    /*! scene visitor that updates transformations
     */
    class iNodeVisitorUpdateTransform : public iNodeVisitor
    {

    public:
        /*! does nothing
         */
        iNodeVisitorUpdateTransform() = default;

        /*! does nothing
         */
        virtual ~iNodeVisitorUpdateTransform() = default;

    protected:
        /*! called before starting traversal
         */
        void preTraverse() override;

        /*! called before decent to child nodes
         */
        bool preOrderVisit(iNodePtr node, iNodePtr nextSibling) override;

        /*! called after ascent from child nodes
         */
        void postOrderVisit(iNodePtr node) override;

        /*! called after traversal is finished
         */
        void postTraverse() override;

    private:
        /*! holds a stack of matrices while traversal tree
         */
        std::vector<iaMatrixd> _matrixStack;

        /*! current matrix that eventually gets pushed on stack or came popped from stack
         */
        iaMatrixd _currentMatrix;
    };

}; // namespace igor

#endif // __IGOR_NODEVISITORUPDATETRANSFORM__
