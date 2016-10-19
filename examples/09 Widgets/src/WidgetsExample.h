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
#include <iWidgetDialog.h>
#include <iMaterial.h>
using namespace Igor;

#include <iaMatrix.h>
using namespace IgorAux;

namespace Igor
{
    class iTextureFont;
	class iWidgetDefaultTheme;
    class iDialogMessageBox;
    class iWidgetLabel;
    class iTexture;
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
	iWidgetDialog* _dialog = nullptr;

    /*! mouse position label
    */
	iWidgetLabel* _labelMousePos = nullptr;

    /*! instance of a message box
    */
    iDialogMessageBox* _messageBox = nullptr;

    /*! just put all widgets in here for a easy clean up later
    */
	vector<iWidget*> _allWidgets;

    /*! splash texture
    */
    shared_ptr<iTexture> _igorLogo = nullptr;

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
    void onMessageBox(iWidget* source);

    /*! triggered by mouse move event

    \param x horrizontal position of mouse cursor
    \param y vertical position of mouse cursor
    */
	void onMouseMove(int32 x, int32 y);

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
