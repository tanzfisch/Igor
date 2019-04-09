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
// (c) Copyright 2012-2019 by Martin Loga
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

#ifndef __iNODEVISITORRENDERCOLORID__
#define __iNODEVISITORRENDERCOLORID__

#include <iNodeVisitor.h>
#include <iDefines.h>
#include <iMaterial.h>

#include <iaString.h>
#include <iaMatrix.h>
using namespace IgorAux;

#include <sstream>
using namespace std;

namespace Igor
{

    /*! traveses node tree and renders bounding boxes
    */
	class Igor_API iNodeVisitorRenderColorID : public iNodeVisitor
	{

	public:

        /*! configures material
        */
        iNodeVisitorRenderColorID();

        /*! does nothing
        */
		virtual ~iNodeVisitorRenderColorID() = default;

	protected:

        /*! initialisation
        */
        virtual void preTraverse();

        /*! calculates transformations and renders objects with their color ID

        \param node current node
        */
        virtual bool preOrderVisit(iNodePtr node);

        /*! restores transformations

        \param node current node
        */
        virtual void postOrderVisit(iNodePtr node);

        /*! does nothing
        */
        virtual void postTraverse();

    private:

        /*! holds a stack of matrices while traversion tree
        */
        vector<iaMatrixd> _matrixStack;

        /*! current matrix that eventually gets pushed on stack or came poped from stack
        */
        iaMatrixd _currentMatrix;

        /*! color ID render material
        */
        uint64 _material = iMaterial::INVALID_MATERIAL_ID;

	};

};

#endif
