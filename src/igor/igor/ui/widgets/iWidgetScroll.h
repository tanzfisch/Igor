//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef __IGOR_WIDGETSCROLL__
#define __IGOR_WIDGETSCROLL__

#include <igor/ui/widgets/iWidget.h>
#include <igor/resources/texture/iTexture.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{

    class iUserControl;

    /*! scroll widget

    basically allows to have a child that is bigger an ads some srcoll bars then

    \todo need to use actual buttons for the scroll bars, or a scroll bar widget
    */
    class IGOR_API iWidgetScroll : public iWidget
    {

    public:
        /*! ctor initializes membervariables

        \param parent optional parent
        */
        iWidgetScroll(const iWidgetPtr parent = nullptr);

        /*! does nothing
         */
        ~iWidgetScroll() = default;

        /*! sets the width or thikness of the scrollbars
         */
        void setScollbarWidth(int32 width);

        /*! \returns scrollbar width
         */
        int32 getScollbarWidth();

        /*! sets horizontal scroll value

        \param value new value for horizontal scroll
        */
        void setHorizontalScroll(float32 value);

        /*! sets vertical scroll value

        \param value new value for vertical scroll
        */
        void setVerticalScroll(float32 value);

        /*! adds a child widget to this widget

        \param widget the child widget to be added
        */
        void addWidget(iWidgetPtr widget) override;

    private:
        struct ScrollButton
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

        /*! scroll bar width
         */
        int32 _scrollbarWidth = 15;

        /*! vertical scroll factor 0.0 - 1.0
         */
        float32 _vscroll = 0;

        /*! horizontal scroll factor 0.0 - 1.0
         */
        float32 _hscroll = 0;

        /*! vertical scrollbar active aka visible
         */
        bool _vscrollActive = false;

        /*! horizontal scrollbar active aka visible
         */
        bool _hscrollActive = false;

        /*! horizontal scroll button
         */
        ScrollButton _hscrollButton;

        /*! vertical scroll button
         */
        ScrollButton _vscrollButton;

        /*! up button
         */
        ScrollButton _upButton;

        /*! down button
         */
        ScrollButton _downButton;

        /*! left button
         */
        ScrollButton _leftButton;

        /*! right button
         */
        ScrollButton _rightButton;

        /*! rectangle of child frame
         */
        iaRectanglei _childFrame;

        /*! texture for button up
         */
        iTexturePtr _upTexture;

        /*! texture for button down
         */
        iTexturePtr _downTexture;

        /*! texture for button left
         */
        iTexturePtr _leftTexture;

        /*! texture for button uright
         */
        iTexturePtr _rightTexture;

        /*! handles incoming mouse wheel event

        \param event mouse wheel event
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool onMouseWheel(iEventMouseWheel &event) override;

        /*! handles incoming mouse move events

        \param event mouse move event
        */
        void onMouseMove(iEventMouseMove &event) override;

        /*! handles incoming mouse key down events

        \param event mouse key down event
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool onMouseKeyDown(iEventMouseKeyDown &event) override;

        /*! handles internal button clicks
        \return true if click was consumed
        */
        bool handleButtonClicks();

        /*! handles mouse key up events

        \param event the mouse key up event
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool onMouseKeyUp(iEventMouseKeyUp &event) override;

        /*! handles incoming double click

        \param event the mouse double click event
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool onMouseDoubleClick(iEventMouseKeyDoubleClick &event) override;

        /*! updates size based on it's content
         */
        void calcMinSize() override;

        /*! draws the widget
         */
        void draw() override;

        /*! calculate position and size of scroll buttons
         */
        void calcButtons();

        /*! \returns the space in which the vertical slider button can be moved
         */
        int32 calcVerticalScrollSpace() const;

        /*! \returns the space in which the horizontal slider button can be moved
         */
        int32 calcHorizontalScrollSpace() const;

        /*! calc child frame

        it's similar to the client area but it can vary if scroll bars are visible
        */
        void calcChildFrame();

        /*! updates widget alignment

        \param clientWidth maximum width this widget can align to
        \param clientHeight maximum height this widget can align to
        */
        void updateAlignment(int32 clientWidth, int32 clientHeight) override;

        /*! calculates childrens offsets relative to their parent

        \param offsets vector to be filled with childrens offsets
        */
        void calcChildOffsets(std::vector<iaRectanglef> &offsets) override;
    };

    /*! widget scroll pointer definition
     */
    typedef iWidgetScroll *iWidgetScrollPtr;
} // namespace igor

#endif // __IGOR_WIDGETSCROLL__
