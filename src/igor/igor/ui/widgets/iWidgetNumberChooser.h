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
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef __IGOR_WIDGETNUMBERCHOOSER__
#define __IGOR_WIDGETNUMBERCHOOSER__

#include <igor/ui/widgets/iWidget.h>

namespace igor
{

    class iTextureFont;
    class iWidgetManager;

    /*! number chooser widget
     */
    class IGOR_API iWidgetNumberChooser : public iWidget
    {

    public:
        /*! ctro initializes member variables

        \param parent optional parent
        */
        iWidgetNumberChooser(const iWidgetPtr parent = nullptr);

        /*! does nothing
         */
        virtual ~iWidgetNumberChooser() = default;

        /*! sets post fix that will be displayed right after the number

        \param text the post fix
        */
        void setPostFix(const iaString &text);

        /*! \returns the post fix text
         */
        const iaString &getPostFix() const;

        /*! \returns the number
         */
        float32 getValue();

        /*! sets the value

        \param value the value to set
        */
        void setValue(float32 value);

        /*! defines how many after points will be displayed

        \param afterPoint amount of digits after point
        */
        void setAfterPoint(int32 afterPoint);

        /*! \returns amount of after point
         */
        int32 getAfterPoint() const;

        /*! sets min max boundings for number

        \param min minimum bounding for number
        \param max maximum bounding for number
        */
        void setMinMaxNumber(float32 min, float32 max);

        /*! sets the stepping for mouse wheel

        \param up the increase value when mouse wheel get's up
        \param down the decrese value when mouse wheel get's down
        */
        void setSteppingWheel(float32 up, float32 down);

        /*! sets the stepping for up and down button

        \param up the increase value when up button get's clicked
        \param down the decrese value when down button get's clicked
        */
        void setStepping(float32 up, float32 down);

    private:
        /*! defines how many digits are displayed after the point
         */
        int32 _afterPoint = 0;

        /*! post fix that will be displayed right nixet to the number
         */
        iaString _postFix;

        /*! the NUMBER de dee deeee ;-)
         */
        float32 _value = 0;

        /*! min bounding for number
         */
        float32 _min = 0;

        /*! max bounding for number
         */
        float32 _max = 100;

        /*! button up stepping
         */
        float32 _stepUp = 1;

        /*! button down stepping
         */
        float32 _stepDown = 1;

        /*! mouse wheel up stepping
         */
        float32 _stepUpWheel = 5;

        /*! mouse wheel down stepping
         */
        float32 _stepDownWheel = 5;

        /*! flag is true if mouse is over the button down
         */
        bool _mouseOverButtonDown = false;

        /*! flag is true if mouse is over the button down
         */
        bool _mouseOverButtonUp = false;

        /*! appearance state of button up
         */
        iWidgetState _buttonUpAppearanceState = iWidgetState::Standby;

        /*! appearance state of button down
         */
        iWidgetState _buttonDownAppearanceState = iWidgetState::Standby;

        /*! button up rectangle
         */
        iaRectanglei _buttonUpRectangle;

        /*! button down rectangle
         */
        iaRectanglei _buttonDownRectangle;

        /*! culls number agains min max boundings
         */
        void cullBoundings();

        /*! handles incoming mouse wheel event

        \param event mouse wheel event
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool onMouseWheel(iEventMouseWheel &event) override;

        /*! handles incoming mouse key down events

        \param event mouse key down event
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool onMouseKeyDown(iEventMouseKeyDown &event) override;

        /*! handles mouse key up events

        \param event the mouse key up event
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool onMouseKeyUp(iEventMouseKeyUp &event) override;

        /*! handles incoming mouse move events

        \param event mouse move event
        */
        void onMouseMove(iEventMouseMove &event) override;

        /*! increase number by value

        \param value the value to increase by
        */
        void increaseNumber(float32 value);

        /*! decrease number by value

        \param value the value to decrease by
        */
        void decreaseNumber(float32 value);

        /*! updates size based on it's content
         */
        void calcMinSize() override;

        /*! updates widget alignment

        \param clientWidth maximum width this widget can align to
        \param clientHeight maximum height this widget can align to
        */
        void updateAlignment(int32 clientWidth, int32 clientHeight) override;

        /*! draws the widget
         */
        void draw() override;
    };

    /*! widget number chooser pointer definition
     */
    typedef iWidgetNumberChooser *iWidgetNumberChooserPtr;

} // namespace igor

#endif // __IGOR_WIDGETNUMBERCHOOSER__
