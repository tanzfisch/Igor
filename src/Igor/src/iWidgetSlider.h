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

#ifndef __iWIDGETSLIDER__
#define __iWIDGETSLIDER__

#include <iWidget.h>

#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

    class iTexture;

    /*! button widget

    \todo non linear slider would be nice e.g. logarithmic for setting shininess in phong shading

    Example:
    \ref Widgets/src/WidgetsExample.cpp "Widgets usage example"

    */
	class Igor_API iWidgetSlider : public iWidget
	{

        /*! needs to be friend because it's the factory that creates this widget
        */
		friend class iWidgetManager;

	public:

        /*! sets the width of the widget

        \param width new width
        */
		void setWidth(int32 width);

        /*! sets the height of the widget

        \param height new height
        */
        void setHeight(int32 height);

        /*! sets the texture to use for the button

        \param texturePath path to texture
        */
        void setTexture(const iaString& texturePath);

        /*! \returns texture path
        */
        const iaString& getTexture() const;

        /*! sets the texture to use for the button

        \param texturePath path to texture
        */
        void setBackgroundTexture(const iaString& texturePath);

        /*! \returns texture path
        */
        const iaString& getBackgroundTexture() const;

        /*! sets value

        \param value new value
        */
        void setValue(float32 value);

        /*! \returns current value
        */
        float32 getValue();

        void setMinValue(float32 min);
        void setMaxValue(float32 max);

        float32 getMinValue();
        float32 getMaxValue();

        /*! sets the stepping for mouse wheel

        \param up the increase value when mouse wheel get's up
        \param down the decrese value when mouse wheel get's down
        */
        void setSteppingWheel(float32 up, float32 down);

	protected:

        float32 _min = 0.0f;
        float32 _max = 1.0f;
        float32 _value = 0.5f;
        float32 _wheelStepUp = 0.1f;
        float32 _wheelStepDown = 0.1f;

        /*! the texture
        */
        shared_ptr<iTexture> _texture = nullptr;

        /*! optional background texture
        */
        shared_ptr<iTexture> _backgroundTexture = nullptr;

        /*! texture path
        */
        iaString _texturePath;

        /*! background texture path
        */
        iaString _backgroundTexturePath;

        /*! increase number by value

        \param value the value to increase by
        */
        void increaseNumber(float32 value);

        /*! decrease number by value

        \param value the value to decrease by
        */
        void decreaseNumber(float32 value);

        /*! handles incomming mouse wheel event

        \param d mouse wheel delta
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool handleMouseWheel(int32 d);

        /*! handles incomming mouse key down events

        \param key the key that was pressed
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool handleMouseKeyDown(iKeyCode key);

        /*! handles incomming mouse move events

        \param x horrizontal mouse position
        \param y horrizontal mouse position
        */
        void handleMouseMove(int32 x, int32 y);

        void handleMouseInput(int32 mouseX);

        void cullBoundings();

        /*! updates dimensions of button
        */
		void update();

        /*! draws the button
        */
        void draw();

	private:

        /*! last known horrizontal mouse position
        */
        int32 _mouseX = 0;

        /*! last known vertical mouse position
        */
        int32 _mouseY = 0;

        /*! ctor initializes member variables
        */
        iWidgetSlider();

        /*! release texture
        */
		virtual ~iWidgetSlider();
	};
}

#endif
