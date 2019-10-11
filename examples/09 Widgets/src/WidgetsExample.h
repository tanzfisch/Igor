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
//    (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __WIDGETEXAMPLE__
#define __WIDGETEXAMPLE__

#include <Igor.h>
#include <iWindow.h>
#include <iView.h>
#include <iDialog.h>
#include <iDialogColorChooser.h>
#include <iDialogColorGradient.h>
#include <iMaterial.h>
#include <iProfilerVisualizer.h>
#include <iTexture.h>
#include <iDialogMessageBox.h>
using namespace Igor;

#include <iaGradient.h>
#include <iaMatrix.h>
using namespace IgorAux;

namespace Igor
{
    class iTextureFont;
	class iWidgetDefaultTheme;
    class iWidgetLabel;
    class iWidgetColor;
    class iWidgetColorGradient;
}

/*! this example shows how to use Igor Widgets
*/
class WidgetsExample
{

public:

    /*! ctor initializes widgets
    */
    WidgetsExample();

    /*! dtor clean up
    */
    virtual ~WidgetsExample();

    /*! runs main loop
    */
    void run();

private:

    /*! the window it self
    */
	iWindow _window;

    /*! orthogonal view port
    */
    iView _viewOrtho;

    /*! visualize statistics
    */
    iProfilerVisualizer _profilerVisualizer;

    /*! font handle
    */
    iTextureFont* _font = nullptr;

    /*! material for drawing igor logo
    */
    uint64 _materialWithTextureAndBlending = iMaterial::INVALID_MATERIAL_ID;

    /*! using the default widget theme
    */
	iWidgetDefaultTheme* _widgetDefaultTheme = nullptr;

    /*! the main dialog
    */
	iDialogPtr _dialog = nullptr;

    /*! mouse position label
    */
	iWidgetLabel* _labelMousePos = nullptr;

    /*! instance of a message box
    */
    iDialogMessageBox* _messageBox = nullptr;

    /*! color chooser dialog
    */
    iDialogColorChooserPtr _colorChooserDialog = nullptr;

    /*! color gradient editor dialog
    */
    iDialogColorGradientPtr _colorGradientDialog = nullptr;

    /*! color view to display the color selected in color chooser
    */
    iWidgetColor* _color = nullptr;

    /*! color gradient
    */
    iWidgetColorGradient* _colorGradient = nullptr;
	
    /*! splash texture
    */
    iTexturePtr _igorLogo = nullptr;

    /*! simple action that prints some text in the console
    */
    void onActionOne();

    /*! simple action that prints some text in the console
    */
    void onActionTwo();

    /*! triggered when main dialog got closed

    \param dialog source of the event
    */
    void WidgetsExample::onCloseDialog(iDialogPtr dialog);

    /*! render function
    */
    void onRender();

    /*! draw Igor logo
    */
    void drawLogo();

    /*! initializes GUI
    */
    void initGUI();

    /*! triggered by exit button. will shut down application

    \param source the source widget of this event
    */
    void onExitClick(iWidgetPtr source);

	/*! triggered by closing the message box

	\param dialog the dialog that was closed
	*/
	void onCloseMessageBox(iDialogPtr dialog);

    /*! triggered by message box button. will open message box

    \param source the source widget of this event
    */
    void onOpenMessageBox(iWidgetPtr source);

    /*! triggered by open color chooser button

    \param source the source widget
    */
    void onOpenColorChooser(iWidgetPtr source);

    /*! triggered by cllicking on color gradient
    */
    void onOpenColorGradientEditor(iWidgetPtr source);

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
	void onMouseMove(const iaVector2i& pos);

    /*! triggred if window was closed
    */
    void onWindowClosed();

    /*! triggered if window was resized
    */
    void onWindowResize(int32 clientWidth, int32 clientHeight);

    /*! initialize example
    */
    void init();

    /*! deinitialize example
    */
    void deinit();

};

#endif
