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

#ifndef __WIDGET_EXAMPLE_H__
#define __WIDGET_EXAMPLE_H__

#include <ExampleBase.h>

#include <igor/ui/dialogs/iDialog.h>
#include <igor/ui/dialogs/iDialogColorChooser.h>
#include <igor/ui/dialogs/iDialogColorGradient.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/resources/profiler/iProfilerVisualizer.h>
#include <igor/resources/texture/iTexture.h>
#include <igor/ui/dialogs/iDialogMessageBox.h>
using namespace igor;

#include <iaux/data/iaGradient.h>
#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{
    class iWidgetDefaultTheme;
    class iWidgetLabel;
    class iWidgetColor;
    class iWidgetColorGradient;
} // namespace igor

/*! this example shows how to use Igor Widgets
*/
class WidgetsExample : public ExampleBase
{

public:
    /*! ctor initializes widgets
    */
    WidgetsExample();

    /*! does nothing
    */
    ~WidgetsExample() = default;

private:
    /*! using the default widget theme
    */
    iWidgetDefaultTheme *_widgetDefaultTheme = nullptr;

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
    void WidgetsExample::onCloseDialog(iDialogPtr dialog);

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

    /*! triggered by mouse move event

    \param pos position of mouse cursor
    */
    void onMouseMoved(const iaVector2i &pos) override;

    /*! initialize example
    */
    void init() override;

    /*! deinitialize example
    */
    void deinit() override;

    /*! called by orthogonal view
    */
    void onRenderOrtho() override;

    /*! called when window was resized

    \param clientWidth the client rectangle width
    \param clientHeight the client rectangle height
    */
    void onWindowResized(int32 clientWidth, int32 clientHeight) override;
};

#endif // __WIDGET_EXAMPLE_H__