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
        void enableHeader(bool enable);

        /*! \returns true if header is enabled
        */
        bool isHeaderEnabled() const;

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

        \param x horizontal position
        \param y vertical position
        */
        void setPos(int32 x, int32 y);

        /*! set size of border

        \param border border size
        */
        void setBorder(int32 border);

        /*! \returns border size
         */
        int32 getBorder();

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

    private:
        /*! if true dialog is open
         */
        bool _isOpen = false;

        /*! the return state of the this dialog
         */
        iDialogReturnState _returnState = iDialogReturnState::Ok;

        /*! horizontal position relative to parent if horizontal alignment is absolute
         */
        int32 _offsetX = 0;

        /*! vertical position relative to parent if horizontal alignment is absolute
         */
        int32 _offsetY = 0;

        /*! size of border
         */
        int32 _border = 1;

        /*! if true header is enabled
        */
        bool _headerEnabled = true;

        /*! the delegate to call after the dialog was closed
         */
        iDialogCloseDelegate _dialogCloseDelegate;

        /*! updates size based on it's content
         */
        void calcMinSize() override;

        /*! updates dialog's alignment

        \param clientWidth maximum width this widget can align to
        \param clientHeight maximum height this widget can align to
        */
        void updateAlignment(int32 clientWidth, int32 clientHeight);

        /*! draws the children
         */
        void draw();
    };
} // namespace igor

#endif // __IGOR_DIALOG__
