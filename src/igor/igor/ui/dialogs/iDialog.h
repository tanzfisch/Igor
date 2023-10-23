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

#ifndef __IGOR_DIALOG__
#define __IGOR_DIALOG__

#include <igor/ui/widgets/iWidget.h>

namespace igor
{

    /*! dialog pointer definition
     */
    class iDialog;
    typedef iDialog *iDialogPtr;

    /*! dialog close event
     */
    typedef iaDelegate<void, iDialogPtr> iDialogCloseDelegate;

    /*! dialog return states
     */
    enum class iDialogReturnState
    {
        No = 0,
        Yes = 1,
        Ok = 1,
        Cancel = 2,
        Error = 3
    };

    /*! motion state of dialog
     */
    enum class iDialogMotionState
    {
        Moving,
        ResizeLeft,
        ResizeRight,
        ResizeTop,
        ResizeBottom,
        ResizeLeftTop,
        ResizeRightTop,
        ResizeLeftBottom,
        ResizeRightBottom,
        Static
    };

    /*! dialog widget
     */
    class IGOR_API iDialog : public iWidget
    {

        friend class iWidgetManager;

    public:
        /*! ctor initializes member variables and registers mouse events
         */
        iDialog(iWidgetType type = iWidgetType::iDialog, const iWidgetPtr parent = nullptr);

        /*! dtor unregisters mouse events
         */
        virtual ~iDialog();

        /*! enables/disables header

        \param enable if true header will be enabled
        */
        void setHeader(bool enable);

        /*! \returns true if header is enabled
         */
        bool hasHeader() const;

        /*! sets title displayed in header

        \param title the title to set
        */
        void setTitle(const iaString &title);

        /*! \returns title of dialog
        */
        const iaString& getTitle() const;

        /*! sets wether or not the dialog is resize-able

        \param enable if true dialog is becoming resize-able
        */
        void setResizeable(bool enable);

        /*! \returns true if dialog is resize-able
        */
        bool isResizeable() const;

        /*! sets wether or not the dialog is moveable

        \param enable if true dialog is becoming moveable
        */
        void setMoveable(bool enable);

        /*! \returns true if dialog is moveable
        */
        bool isMoveable() const;

        /*! sets horizontal position of dialog

        implicitly sets horizontal alignment to absolute

        \param x horizontal position
        */
        void setX(int32 x);

        /*! sets vertical position of dialog

        implicitly sets vertical alignment to absolute

        \param y vertical position
        */
        void setY(int32 y);

        /*! sets position of dialog

        implicitly sets alignment to absolute

        \param pos position of dialog
        */
        void setPos(const iaVector2f &pos);

        /*! \returns position of dialog
         */
        const iaVector2f &getPos() const;

        /*! shows the dialog on screen

        \param dialogCloseDelegate the delegate to call after the dialog was closed
        */
        virtual void open(iDialogCloseDelegate dialogCloseDelegate);

        /*! closes the dialog
         */
        virtual void close();

        /*! \returns true if dialog is open
         */
        bool isOpen() const;

        /*! \returns the return state of this dialog if one was set
         */
        iDialogReturnState getReturnState() const;

        /*! sets the return state of this dialog

        \param returnState the return state to be set
        */
        void setReturnState(iDialogReturnState returnState);

        /*! puts dialog in front of others
        */
        void putInFront();

    private:
        /*! if true dialog is open
         */
        bool _isOpen = false;

        /*! the return state of the this dialog
         */
        iDialogReturnState _returnState = iDialogReturnState::Ok;

        /*! position relative to parent in axis where alignment is absolute
         */
        iaVector2f _offset;

        /*! if true header is enabled
         */
        bool _headerEnabled = true;

        /*! if true dialog is resize-able
        */
        bool _resizeEnabled = true;
        
        /*! if true dialog is moveable
        */
        bool _isMoveable = true;

        /*! title of dialog
        */
        iaString _title;

        /*! motion state of dialog
         */
        iDialogMotionState _motionState = iDialogMotionState::Static;

        /*! the delegate to call after the dialog was closed
         */
        iDialogCloseDelegate _dialogCloseDelegate;

        /*! handles incoming mouse key down events

        \param key the key that was pressed
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool handleMouseKeyDown(iKeyCode key) override;

        /*! handles mouse key up events

        \param key the key that was pressed
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool handleMouseKeyUp(iKeyCode key) override;

        /*! handles incoming mouse move events

        \param pos mouse position
        */
        void handleMouseMove(const iaVector2f &pos) override;

        /*! updates size based on it's content
         */
        void calcMinSize() override;

        /*! updates dialog's alignment

        \param clientWidth maximum width this widget can align to
        \param clientHeight maximum height this widget can align to
        */
        void updateAlignment(int32 clientWidth, int32 clientHeight) override;

        /*! \returns motion state based on mouse position

        \param pos the mouse position
        */
        iDialogMotionState calcMotionState(const iaVector2f &pos);

        /*! draws the children
         */
        void draw();
    };
} // namespace igor

#endif // __IGOR_DIALOG__
