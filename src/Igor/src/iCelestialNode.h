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
// (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __iCELESTIALNODE__
#define __iCELESTIALNODE__

#include <iNodeRender.h>
#include <iTextureResourceFactory.h>

namespace Igor
{

    /*!
    \todo do we need this or can we remove it?
    \deprecated
    */
	class Igor_API iCelestialNode : public iNodeRender
	{

		friend class iNodeFactory;

	private:

		shared_ptr<iTexture> texture;


		iCelestialNode();
		virtual ~iCelestialNode();

	public:

        virtual void draw();

		/*! the position is defined by 
		*/
		void setPosition(iaVector3f pos);

		void setTexture(iaString textureFileName);
	};

}

#endif