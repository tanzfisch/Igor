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
// (c) Copyright 2014-2016 by Martin Loga
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
// contact: martinloga@gmx.de  

#ifndef __iNODEVISITOR__
#define __iNODEVISITOR__

#include <iDefines.h>

#include <memory>
using namespace std;

namespace Igor
{

	class iNode;

    /*! base class for traversing a node tree
    */
	class Igor_API iNodeVisitor
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

        /*! traverses tree from given node on

        \param node node to start from
        */
		void traverseTree(iNode* node);

    private:

        /*! flag if inactive children shall be traversed too
        */
        bool _traverseInactiveChildren = false;

        /*! internal recursive traverse method

        \param node current node
        */
        void traverseTreeInternal(iNode* node);

	protected:

        /*! is called before traversal

        has to be implemented by deriving class
        */
		virtual void preTraverse() = 0;

        /*! is called before every node visited

        has to be implemented by deriving class
        */
        virtual bool preOrderVisit(iNode* node) = 0;

        /*! is called after every node visited

        has to be implemented by deriving class
        */
        virtual void postOrderVisit(iNode* node) = 0;
		
        /*! is called after the whole traversal

        has to be implemented by deriving class
        */
        virtual void postTraverse() = 0;

	};

};

#endif
