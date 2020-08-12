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
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __iNODEVISITORPRINTTREE__
#define __iNODEVISITORPRINTTREE__

#include <iaux/data/iaString.h>
using namespace iaux;

#include <igor/scene/traversal/iNodeVisitor.h>
#include <igor/iDefines.h>
#include <sstream>

namespace igor
{

    /*! traveses node tree and prints basic information in to console
	*/
    class IGOR_API iNodeVisitorPrintTree : public iNodeVisitor
    {

    public:
        /*! initializes traversal config
		*/
        iNodeVisitorPrintTree();

        /*! does nothing
		*/
        virtual ~iNodeVisitorPrintTree() = default;

        /*! traverses the tree and prints the output in console
		*/
        void printToConsole(iNodePtr node);

    protected:
        /*! initialisation
		*/
        void preTraverse() override;

        /*! generates information string on every node passed by
		handles output indentation

		\param node current node
		*/
        bool preOrderVisit(iNodePtr node, iNodePtr nextSibling) override;

        void preLine();

        /*! handles output indentation
		*/
        void postOrderVisit(iNodePtr node) override;

        /*! some more output
		*/
        void postTraverse() override;

    private:
        /*! counter for all nodes passed by
		*/
        uint64 _nodeCount = 0;

        /*! current indentation string
		*/
        iaString _indenttation = "";

        /*! all the output ends up in this string
		*/
        std::wstringstream _stream;
    };

}; // namespace igor

#endif
