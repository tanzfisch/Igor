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
// (c) Copyright 2012-2017 by Martin Loga
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

#ifndef __iNODEVISITORSEARCHNAME__
#define __iNODEVISITORSEARCHNAME__

#include <iaString.h>
using namespace IgorAux;

#include <iNodeVisitor.h>

#include <vector>
using namespace std;

namespace Igor
{

    /*! traveses node tree and prints basic information in to console
    */
	class Igor_API iNodeVisitorSearchName : public iNodeVisitor
	{

	public:

        /*! initializes traversal config
        */
        iNodeVisitorSearchName();

        /*! does nothing
        */
		virtual ~iNodeVisitorSearchName() = default;

        /*! traverses the tree and returns all node id's using the specified name

        \param name the name to search for
        \returns list of found node ids
        */
		const vector<uint64>& find(iNode* node, const iaString& name);

	protected:

        /*! initialisation
        */
		virtual void preTraverse();

        /*! traversing tree and check if the current node's name matches the search string

        \param node the current node during traversal
        */
		virtual bool preOrderVisit(iNode* node);

        /*! not implemented
        */
        virtual void postOrderVisit(iNode* node);

        /*! not implemented
        */
        virtual void postTraverse();

	private:

        /*! resulting list of found node IDs
        */
        vector<uint64> _nodeIDs;

        /*! hold a copy of name to search for
        */
        iaString _name;

	};

};

#endif