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

#ifndef __USERCONTROLPARTICLESYSTEM__
#define __USERCONTROLPARTICLESYSTEM__

#include <iNodeModel.h>
#include <iUserControl.h>
#include <iGradient.h>
using namespace Igor;

namespace Igor
{
    class iWidgetGrid;
    class iWidgetTextEdit;
    class iWidgetLabel;
    class iWidgetButton;
    class iUserControlColorChooser;
    class iWidgetSlider;
    class iWidgetNumberChooser;
    class iWidgetSelectBox;
    class iUserControlFileChooser;
    class iWidgetCheckBox;
    class iWidgetColorGradient;
    class iDialogColorGradient;
}

class UserControlParticleSystem : public iUserControl
{
public:

    UserControlParticleSystem();
    ~UserControlParticleSystem();

    iWidget* getWidget();

    void setNode(uint32 id);
    uint32 getNode();

private:

    iWidgetGrid* _grid = nullptr;

    iWidgetButton* _buttonStart = nullptr;
    iWidgetButton* _buttonStop = nullptr;
    iWidgetButton* _buttonReset = nullptr;

    iUserControlFileChooser* _textureChooser0 = nullptr;
    iUserControlFileChooser* _textureChooser1 = nullptr;
    iUserControlFileChooser* _textureChooser2 = nullptr;
    iUserControlFileChooser* _textureChooser3 = nullptr;

    iWidgetSelectBox* _emitterSelection = nullptr;
    iWidgetSelectBox* _materialSelection = nullptr;

    iWidgetCheckBox* _loopCheckBox = nullptr;
    iWidgetCheckBox* _velocityOrientedCheckBox = nullptr;
    iWidgetNumberChooser* _periodChooser = nullptr;
    iWidgetNumberChooser* _airDragChooser = nullptr;
    iWidgetNumberChooser* _vorticityConfinementChooser = nullptr;
    iWidgetNumberChooser* _vorticityAppearanceRateChooser = nullptr;
    iWidgetNumberChooser* _tilingVerticalChooser = nullptr;
    iWidgetNumberChooser* _tilingHorizontalChooser = nullptr;
    iWidgetColorGradient* _colorGradient = nullptr;

    iDialogColorGradient* _colorGradientDialog = nullptr;

    vector<iWidget*> _allWidgets;

    vector<iNode*> _emitters;

    vector<uint32*> _userDataMaterialID;

    uint32 _nodeId = 0;
    uint32 _loadTextureTexUnit = 0;

    bool _ignoreNodeUpdate = false;

    void onDoUpdateNode(iWidget* source);
    void onLoopChanged(iWidget* source);

    void onOpenColorGradientEditor(iWidget* source);
    void onCloseColorGradientEditor(bool ok, const iGradientColor4f& gradient);

    void onStart(iWidget* source);
    void onStop(iWidget* source);
    void onReset(iWidget* source);

    void updateGUI();
    void updateNode();

    void initGUI();
    void deinitGUI();

};

#endif