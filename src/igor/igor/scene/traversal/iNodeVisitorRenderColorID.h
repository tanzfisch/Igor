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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IGOR_NODEVISITORRENDERCOLORID__
#define __IGOR_NODEVISITORRENDERCOLORID__

#include <igor/scene/traversal/iNodeVisitor.h>
#include <igor/iDefines.h>
#include <igor/resources/material/iMaterial.h>

#include <iaux/data/iaString.h>
#include <iaux/math/iaMatrix.h>
using namespace iaux;

#include <sstream>

namespace igor
{

    /*! traveses node tree and renders bounding boxes
    */
    class IGOR_API iNodeVisitorRenderColorID : public iNodeVisitor
    {

    public:
        /*! does nothing
        */
        iNodeVisitorRenderColorID() = default;

        /*! does nothing
        */
        virtual ~iNodeVisitorRenderColorID() = default;

    protected:
        /*! initialisation
        */
        void preTraverse() override;

        /*! calculates transformations and renders objects with their color ID

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
