//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/                     ) )     ((
//                                           (_(       \)
//    (c) Copyright 2014-2020 by Martin Loga
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

#ifndef __WIDGETEXAMPLE_H__
#define __WIDGETEXAMPLE_H__

#include <igor/igor.h>
using namespace igor;
using namespace iaux;

/*! this example shows how to use Igor Widgets

    we derive from iLayerWidgets so the handling of the widget manager is covered for us
*/
class WidgetsExample : public iLayerWidgets
{

public:
    /*! ctor initializes widgets

    \param window the given window
    */
    WidgetsExample(iWindow *window);

    /*! does nothing
    */
    ~WidgetsExample() = default;

private:
    /*! the main dialog
    */
    iDialogPtr _dialog = nullptr;

    /*! mouse position label
    */
    iWidgetLabel *_labelMousePos = nullptr;

    /*! instance of a message box
    */
    iDialogMessageBox *_messageBox = nullptr;

    /*! color chooser dialog
    */
    iDialogColorChooserPtr _colorChooserDialog = nullptr;

    /*! color gradient editor dialog
    */
    iDialogColorGradientPtr _colorGradientDialog = nullptr;

    /*! color view to display the color selected in color chooser
    */
    iWidgetColor *_color = nullptr;

    /*! color gradient
    */
    iWidgetColorGradient *_colorGradient = nullptr;

    /*! simple action that prints some text in the console
    */
    void onActionOne();

    /*! simple action that prints some text in the console
    */
    void onActionTwo();

    /*! simple action that prints some text in the console
    */
    void onActionThree();

    /*! triggered when main dialog got closed

    \param dialog source of the event
    */
    void onCloseDialog(iDialogPtr dialog);

    /*! initializes GUI
    */
    void initGUI();

    /*! triggered by exit button. will shut down application

    \param source the source widget of this event
    */
    void onExitClick(const iWidgetPtr source);

    /*! triggered by closing the message box

	\param dialog the dialog that was closed
	*/
    void onCloseMessageBox(iDialogPtr dialog);

    /*! triggered by message box button. will open message box

    \param source the source widget of this event
    */
    void onOpenMessageBox(const iWidgetPtr source);

    /*! triggered by open color chooser button

    \param source the source widget
    */
    void onOpenColorChooser(const iWidgetPtr source);

    /*! triggered by cllicking on color gradient
    */
    void onOpenColorGradientEditor(const iWidgetPtr source);

    /*! triggered when closing color chooser dialog

    \param ok if true value ok, if false dialog was canceled
    \param color the resulting color
    */
    void onCloseColorChooser(iDialogPtr dialog);

    /*! triggered when color gradient editor was closed

    \param ok if true the ok button was closing the dialog
    \param gradient the resulting gradient
    */
    void onCloseColorGradient(iDialogPtr dialog);

    /*! initialize example
    */
    void onInit() override;

    /*! deinitialize example
    */
    void onDeinit() override;

    /*! called on any other event

    \param event the event to handle
    */
    void onEvent(iEvent &event) override;

    /*! handles mouse move event

    \param event the mouse move event
    \returns true if consumed
    */
    bool onMouseMoveEvent(iMouseMoveEvent_TMP &event);
};

#endif // __WIDGETEXAMPLE_H__
