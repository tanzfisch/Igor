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

#ifndef __iWIDGETCOLORVIEW__
#define __iWIDGETCOLORVIEW__

#include <iWidget.h>

#include <iaString.h>
#include <iaColor4.h>
using namespace IgorAux;

namespace Igor
{

    class iTexture;

    /*! picture widget

    Example:
    \ref Widgets/src/WidgetsExample.cpp "Widgets usage example"

    */
	class Igor_API iWidgetColorView : public iWidget
	{

        /*! needs to be friend because it's the factory that creates this widget
        */
        friend class iWidgetManager;

	public:

        void setWidth(int32 width);
        void setHeight(int32 height);
        void setColor(const iaColor4f& color);
        const iaColor4f& getColor() const;

	private:

        /*! the collor to present
        */
        iaColor4f _color;

        /*! shared pointer to texture
        */
        shared_ptr<iTexture> _texture = nullptr;

        void update();

        /*! draws widget
        */
        void draw();

        /*! ctor initializes member variables
        */
        iWidgetColorView();

        /*! release texture
        */
		virtual ~iWidgetColorView();
	};
}

#endif
