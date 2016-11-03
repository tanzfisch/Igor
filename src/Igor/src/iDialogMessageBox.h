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

#ifndef __iDIALOGMESSAGEBOX__
#define __iDIALOGMESSAGEBOX__

#include <iWidgetUserDialog.h>

#include <iaEvent.h>
#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

    class iWidgetButton;
    class iWidgetGrid;
    class iWidgetLabel;
    class iWidget;
    class iWidgetSpacer;

    /*! message box return values
    */
    enum class iMessageBoxReturnValue
    {
        No = 0,
        Yes = 1,
        Ok = 1,
        Cancel = 2
    };

    /*! message box button configuration
    */
    enum class iMessageBoxButtons
    {
        Ok,
        CancelOk,
        YesNo,
        YesNoCancel
    };

    /*! message box close event

    \param value the return value of the message box
    */
    iaEVENT(iDialogMessageBoxCloseEvent, iDialogMessageBoxCloseDelegate, void, (iMessageBoxReturnValue value), (value));

    /*! the message box
    */
    class Igor_API iDialogMessageBox : public iWidgetUserDialog
    {
    public:

        /*! does nothing
        */
        iDialogMessageBox() = default;

        /*! deinitializes gui
        */
        ~iDialogMessageBox();

        /*! initializes gui and opens the message box dialog

        \param message the message to display
        \param buttons the button configuration to use
        */
        void show(iaString message, iMessageBoxButtons buttons = iMessageBoxButtons::Ok);

        /*! initializes gui and opens the message box dialog

        \param message the message to display
        \param closeDelegate the delegate to be called when closed
        \param buttons the button configuration to use
        */
        void show(iaString message, iDialogMessageBoxCloseDelegate closeDelegate, iMessageBoxButtons buttons = iMessageBoxButtons::Ok);

    private:

        /*! the close event
        */
        iDialogMessageBoxCloseEvent _messageBoxCloseEvent;

        /*! the return value of the message box
        */
        iMessageBoxReturnValue _messageBoxReturnValue = iMessageBoxReturnValue::Ok;

        /*! the ok button
        */
        iWidgetButton* _okButton = nullptr;

        /*! the yes button
        */
        iWidgetButton* _yesButton = nullptr;

        /*! the no button
        */
        iWidgetButton* _noButton = nullptr;

        /*! the cancel button
        */
        iWidgetButton* _cancelButton = nullptr;

        /*! a spacer line
        */
        iWidgetSpacer* _spacerLine = nullptr;

        /*! an other spacer
        */
        iWidgetSpacer* _spacerLittle = nullptr;

        /*! over all grid
        */
        iWidgetGrid* _grid = nullptr;
        
        /*! grid for the buttons
        */
        iWidgetGrid* _buttonGrid = nullptr;
        
        /*! label for the message text
        */
        iWidgetLabel* _messageLabel = nullptr;

        /*! collect all widgets here so we can clean up faster later
        */
        vector<iWidget*> _allWidgets;

        /*! handles ok button clicked event

        \param source the ok button it self
        */
        void onOK(iWidget* source);

        /*! handles cancel button clicked event

        \param source the cancel button it self
        */
        void onCancel(iWidget* source);

        /*! handles yes button clicked event

        \param source the yes button it self
        */
        void onYes(iWidget* source);

        /*! handles no button clicked event

        \param source the no button it self
        */
        void onNo(iWidget* source);

        /*! closes the dialog and sends closed event

        will be triggered by any button
        */
        void close();

        /*! initializes the gui

        \param message the message text
        \param buttons the button configuration
        */
        void initGUI(iaString message, iMessageBoxButtons buttons);

        /*! deinitializes the gui elements
        */
        void deinitGUI();

    };

}

#endif