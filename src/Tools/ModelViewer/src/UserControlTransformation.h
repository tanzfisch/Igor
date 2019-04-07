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
// (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __USERCONTROLTRANSFORMATION__
#define __USERCONTROLTRANSFORMATION__

#include <iaMatrix.h>
using namespace IgorAux;

#include <iUserControl.h>
using namespace Igor;

#include <vector>
using namespace std;

namespace Igor
{
    class iWidgetGrid;
    class iWidgetTextEdit;
    class iWidgetLabel;
    class iWidgetButton;
}

class UserControlTransformation : public iUserControl
{
public:

    UserControlTransformation();
    ~UserControlTransformation();

    void setNode(uint32 id);
    uint32 getNode();

    iWidget* getWidget();

private:

    iWidgetGrid* _grid = nullptr;

    iWidgetGrid* _gridMatrix = nullptr;
    vector<iWidgetTextEdit*> _matrixText;

    iWidgetGrid* _gridModifiers = nullptr;

    iWidgetButton* _buttonApplyTranlation = nullptr;
    iWidgetButton* _buttonApplyScale = nullptr;
    iWidgetButton* _buttonApplyID = nullptr;
    iWidgetButton* _buttonApplyRotateX = nullptr;
    iWidgetButton* _buttonApplyRotateY = nullptr;
    iWidgetButton* _buttonApplyRotateZ = nullptr;

    vector<iWidgetTextEdit*> _translateText;
    vector<iWidgetTextEdit*> _scaleText;
    vector<iWidgetTextEdit*> _rotateText;

    uint32 _nodeId = 0;

    vector<iWidget*> _allWidgets;

    void onTranslation(iWidget* source);
    void onScale(iWidget* source);
    void onRotationX(iWidget* source);
    void onRotationY(iWidget* source);
    void onRotationZ(iWidget* source);
    void onID(iWidget* source);
   
    void updateGUI();

    void initGUI();
    void deinitGUI();

};

#endif
