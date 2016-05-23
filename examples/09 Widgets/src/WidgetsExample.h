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
#include <iTimerHandle.h>
#include <iModelResourceFactory.h>
#include <iKeyboard.h>
#include <iWidgetManager.h>
using namespace Igor;

#include <iaMatrix.h>
using namespace IgorAux;

namespace Igor
{
	class iScene;
	class iTransformNode;
	class iLightNode;
    class iTextureFont;
	class iTaskFlushResources;
	class iWidgetDialog;
	class iWidgetLabel;
	class iWidgetButton;
	class iWidgetGroupBox;
	class iWidgetGrid;
	class iWidgetDefaultTheme;
	class iWidgetCheckBox;
	class iWidgetNumberChooser;
	class iWidgetTextEdit;
    class iWidgetPicture;
	class iWidgetScroll;
    class iMessageBox;
    class iWidgetSelectBox;
}

class WidgetsExample
{
private:

	iWindow _window;
    iView _viewOrtho;
    iaMatrixf _modelViewOrtho;

    iTextureFont* _font = 0;
    uint32 _fontMaterial = 0;

	iWidgetDefaultTheme* _widgetDefaultTheme = nullptr;
	iWidgetDialog* _dialog1 = nullptr;
	iWidgetLabel* _label1 = nullptr;
	iWidgetLabel* _label2 = nullptr;
	iWidgetLabel* _label3 = nullptr;
	iWidgetLabel* _labelNum1 = nullptr;
	iWidgetLabel* _labelNum2 = nullptr;
	iWidgetLabel* _labelNum3 = nullptr;
	iWidgetLabel* _labelNum4 = nullptr;
	iWidgetGroupBox* _groupBox1 = nullptr;
	iWidgetGroupBox* _groupBox2 = nullptr;
    iWidgetSelectBox* _selectBox = nullptr;

	iWidgetButton* _button1 = nullptr;
	iWidgetButton* _button2 = nullptr;
	
	iWidgetGrid* _grid1 = nullptr;
	iWidgetGrid* _grid2 = nullptr;
	iWidgetCheckBox* _radio1 = nullptr;
	iWidgetCheckBox* _radio2 = nullptr;
	iWidgetCheckBox* _radio3 = nullptr;
	iWidgetCheckBox* _checkBox1 = nullptr;
	iWidgetNumberChooser* _numberChooser = nullptr;
	iWidgetTextEdit* _textEdit = nullptr;
    iWidgetPicture* _picture = nullptr;
	iWidgetScroll* _widgetScoll = nullptr;

    iMessageBox* _messageBox = nullptr;

	vector<iWidget*> _allWidgets;

    void render();
    void drawFPS();

    void initGUI();
    void deinitGUI();

    void onExitClick(iWidget* source);
    void onMessageBox(iWidget* source);

    void onWindowClosed();
    void onWindowResize(int32 clientWidth, int32 clientHeight);

    void init();
    void deinit();

public:

    WidgetsExample();
	virtual ~WidgetsExample();

	void run();

};

#endif
