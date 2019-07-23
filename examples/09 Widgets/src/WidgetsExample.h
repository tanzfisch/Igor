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
#include <iMaterial.h>
#include <iStatisticsVisualizer.h>
#include <iTexture.h>
using namespace Igor;

#include <iaGradient.h>
#include <iaMatrix.h>
using namespace IgorAux;

namespace Igor
{
    class iTextureFont;
	class iWidgetDefaultTheme;
    class iDialogMessageBox;
    class iWidgetLabel;
    class iDialogColorChooser;
    class iWidgetColor;
    class iWidgetColorGradient;
    class iDialogColorGradient;
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
    iStatisticsVisualizer _statisticsVisualizer;

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
	iDialog* _dialog = nullptr;

    /*! mouse position label
    */
	iWidgetLabel* _labelMousePos = nullptr;

    /*! instance of a message box
    */
    iDialogMessageBox* _messageBox = nullptr;

    /*! color chooser dialog
    */
    iDialogColorChooser* _colorChooserDialog = nullptr;

    /*! color gradient editor dialog
    */
    iDialogColorGradient* _colorGradientDialog = nullptr;

    /*! color view to display the color selected in color chooser
    */
    iWidgetColor* _color = nullptr;

    /*! color gradient
    */
    iWidgetColorGradient* _colorGradient = nullptr;

    /*! just put all widgets in here for a easy clean up later
    */
	std::vector<iWidget*> _allWidgets;

    /*! splash texture
    */
    iTexturePtr _igorLogo = nullptr;

    /*! render function
    */
    void onRender();

    /*! draw Igor logo
    */
    void drawLogo();

    /*! initializes GUI
    */
    void initGUI();

    /*! deinitializes GUI
    */
    void deinitGUI();

    /*! triggered by exit button. will shut down application

    \param source the source widget of this event
    */
    void onExitClick(iWidget* source);

    /*! triggered by message box button. will open message box

    \param source the source widget of this event
    */
    void onOpenMessageBox(iWidget* source);

    /*! triggered by open color chooser button

    \param source the source widget
    */
    void onOpenColorChooser(iWidget* source);

    /*! triggered by cllicking on color gradient
    */
    void onOpenColorGradientEditor(iWidget* source);

    /*! triggered when closing color chooser dialog

    \param ok if true value ok, if false dialog was canceled
    \param color the resulting color
    */
    void onCloseColorChooser(bool ok, const iaColor4f& color);

    /*! triggered when color gradient editor was closed

    \param ok if true the ok button was closing the dialog
    \param gradient the resulting gradient
    */
    void onCloseColorGradient(bool ok, const iaGradientColor4f& gradient);

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
