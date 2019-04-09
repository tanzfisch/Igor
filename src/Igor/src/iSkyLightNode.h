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

#ifndef __iSKYLIGHTNODE__
#define __iSKYLIGHTNODE__

#include <iNodeRender.h>
#include <iMeshBuilder.h>

namespace Igor
{

    /*! maybe replace with something more generall like SkyDomeNode or SkySphereNode and put all together in SkyBoxNode

    \deprecated
    */
	class Igor_API iSkyLightNode : public iNodeRender
	{

		friend class iNodeFactory;

	private:

        iaVector3f lightdirection = { 1, 0, 0 };
		shared_ptr<iMesh> _dome = 0;
		
		void initMesh();

		iSkyLightNode();
		virtual ~iSkyLightNode();

	public:

		void setLightDirection(iaVector3f lightdir);

        virtual void draw();

	};

}

#endif