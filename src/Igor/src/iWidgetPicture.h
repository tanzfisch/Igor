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

#ifndef __iWIDGETPICTURE__
#define __iWIDGETPICTURE__

#include <iWidget.h>

#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

    class iTexture;

    /*! picture widget

    Example:
    \ref Widgets/src/WidgetsExample.cpp "Widgets usage example"

    */
	class Igor_API iWidgetPicture : public iWidget
	{

        /*! needs to be friend because it's the factory that creates this widget
        */
        friend class iWidgetManager;

	public:

        /*! sets the texture to use for the picture

        \param texturePath path to texture
        */
		void setTexture(const iaString& texturePath);

        /*! \returns texture path
        */
		const iaString& getTexture() const;

        /*! sets maximum display size of picture

        \param width max width
        \param height max height
        */
        void setMaxSize(int32 width, int32 height);

        /*! \returns maximum width
        */
        int32 getMaxWidth();

        /*! \returns maximum height
        */
        int32 getMaxHeight();

	private:

        /*! texture path
        */
        iaString _texturePath;

        /*! shared pointer to texture
        */
        shared_ptr<iTexture> _texture = nullptr;

        /*! maximum dispaly width 
        */
        int32 _maxWidth = MAXINT32;
        
        /*! maximum dispaly height
        */
        int32 _maxHeight = MAXINT32;

        /*! updates size based on it's content
        */
        void updateContentSize();

		/*! draws the button

		\param parentPosX parent absolute horrizontal position
		\param parentPosY parent absolute vertical position
		*/
		void draw(int32 parentPosX, int32 parentPosY);

        /*! ctor initializes member variables
        */
		iWidgetPicture();

        /*! release texture
        */
		virtual ~iWidgetPicture();
	};
}

#endif
