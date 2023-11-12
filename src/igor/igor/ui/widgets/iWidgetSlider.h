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

#ifndef __IGOR_WIDGETSLIDER__
#define __IGOR_WIDGETSLIDER__

#include <igor/ui/widgets/iWidget.h>
#include <igor/resources/texture/iTexture.h>

#include <iaux/data/iaString.h>
using namespace iaux;

#include <memory>

namespace igor
{

    /*! button widget
	*/
    class IGOR_API iWidgetSlider : public iWidget
    {

    public:
        /*! ctor initializes member variables

		\param parent optional parent
		*/
        iWidgetSlider(const iWidgetPtr parent = nullptr);

        /*! release texture
		*/
        virtual ~iWidgetSlider();

        /*! sets the texture to use for the button

		\param texturePath path to texture
		*/
        void setTexture(const iaString &texturePath);

        /*! \returns texture path
		*/
        const iaString &getTexture() const;

        /*! sets the texture to use for the button

		\param texturePath path to texture
		*/
        void setBackgroundTexture(const iaString &texturePath);

        /*! \returns texture path
		*/
        const iaString &getBackgroundTexture() const;

        /*! sets value

		\param value new value
		*/
        void setValue(float32 value);

        /*! \returns current value
		*/
        float32 getValue();

        /*! sets minimum value of slider

		\param min minimum value
		*/
        void setMinValue(float32 min);

        /*! sets maximum value of slider

		\param max maximum value
		*/
        void setMaxValue(float32 max);

        /*! \returns minimum value
		*/
        float32 getMinValue();

        /*! \returns maximum value
		*/
        float32 getMaxValue();

        /*! sets the stepping for mouse wheel

		\param up the increase value when mouse wheel get's up
		\param down the decrese value when mouse wheel get's down
		*/
        void setSteppingWheel(float32 up, float32 down);

    private:
        struct SliderButton
        {
            /*! rectangle of button
			*/
            iaRectanglef _rectangle;

            /*! button apearance state
			*/
            iWidgetState _appearanceState = iWidgetState::Standby;

            /*! button mouse over state
			*/
            bool _mouseOver = false;

            /*! if true the button is kept pressed
			*/
            bool _mouseDown = false;
        };

        /*! minimum value
		*/
        float32 _min = 0.0f;

        /*! maximum value
		*/
        float32 _max = 1.0f;

        /*! current value
		*/
        float32 _value = 0.5f;

        /*! mouse wheel up value change stepping
		*/
        float32 _wheelStepUp = 0.1f;

        /*! mouse wheel down value change stepping
		*/
        float32 _wheelStepDown = 0.1f;

        /*! the texture
		*/
        iTexturePtr _texture = nullptr;

        /*! optional background texture
		*/
        iTexturePtr _backgroundTexture = nullptr;

        /*! texture path
		*/
        iaString _texturePath;

        /*! background texture path
		*/
        iaString _backgroundTexturePath;

        /*! the button of this slider
        */
        SliderButton _sliderButton;

        /*! increase number by value

		\param value the value to increase by
		*/
        void increaseNumber(float32 value);

        /*! decrease number by value

		\param value the value to decrease by
		*/
        void decreaseNumber(float32 value);

        /*! handles incoming mouse wheel event

		\param d mouse wheel delta
		\returns true: if event was consumed and therefore ignored by the parent
		*/
        bool onMouseWheel(int32 d);

        /*! handles incoming mouse key down events

		\param key the key that was pressed
		\returns true: if event was consumed and therefore ignored by the parent
		*/
        bool onMouseKeyDown(iKeyCode key);

        /*! handles mouse key up events

		\param key the key that was pressed
		\returns true: if event was consumed and therefore ignored by the parent
		*/
        bool onMouseKeyUp(iKeyCode key) override;

        /*! handles incoming mouse move events

		\param pos mouse position
		*/
        void onMouseMove(const iaVector2f &pos);

        /*! translates the horizontal mouse position in the a value
		*/
        void handleMouseInput(int32 mouseX);

        /*! updates size based on it's content
		*/
        void calcMinSize() override;

        /*! draws the widget
		*/
        void draw();
    };
} // namespace igor

#endif // __IGOR_WIDGETSLIDER__
