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
        void setHeaderEnabled(bool enable);

        /*! \returns true if header is enabled
         */
        bool hasHeader() const;

        /*! sets title displayed in header

        \param title the title to set
        */
        void setTitle(const iaString &title);

        /*! \returns title of dialog
         */
        const iaString &getTitle() const;

        /*! sets wether or not the dialog is resize-able

        \param enable if true dialog is becoming resize-able
        */
        void setResizeable(bool enable);

        /*! \returns true if dialog is resize-able
         */
        bool isResizeable() const;

        /*! sets wether or not the dialog is dock-able

        \param enable if true dialog is becoming dock-able
        */
        void setDockable(bool enable);

        /*! \returns true if dialog is dock-able
         */
        bool isDockable() const;

        /*! \returns true if dialog is docked

        This is currently just an alias for hasParent
         */
        bool isDocked() const;

        /*! \returns docking parent id
         */
        iWidgetID getDockingParent() const;

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
        \param modal open this dialog as modal
        */
        virtual void open(iDialogCloseDelegate dialogCloseDelegate = iDialogCloseDelegate(), bool modal = false);

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

        /*! set wether or not dialog accepts ESC to close the dialog

        \param acceptESC if true dialog accepts ESC to close
        */
        void setAcceptESCToClose(bool acceptESC);

        /*! \returns true if dialog accepts ESC to close
        */
        bool isAcceptingESCToClose() const;

    private:
        /*! if true dialog is open
         */
        bool _isOpen = false;

        /*! if true dialog accepts ESC to close
        */
        bool _acceptsESC = false;

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
        bool _isResizeable = true;

        /*! if true dialog is moveable
         */
        bool _isMoveable = true;

        /*! if true dialog is dock-able
         */
        bool _isDockable = false;

        /*! true if actually moving (or being dragged)
         */
        bool _moving = false;

        /*! saving mouse pos when last time pressed a button
         */
        iaVector2f _lastMousePos;

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

        /*! handles pressed key event

        \param event the key down event
        */
        virtual bool onKeyDown(iEventKeyDown &event) override;

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

        /*! draws the widget
         */
        void draw() override;

        /*! updates cursor based on motion state

        \param motionState the given motion state
        */
        void updateCursor(iDialogMotionState motionState);
    };
} // namespace igor

#endif // __IGOR_DIALOG__
