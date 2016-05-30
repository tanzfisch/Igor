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
#include <iWidgetUserControl.h>
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
}

class UserControlParticleSystem : public iWidgetUserControl
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

    vector<iWidget*> _allWidgets;

    vector<iNode*> _emitters;

    vector<uint32*> _userDataMaterialID;

    uint32 _nodeId = 0;
    uint32 _loadTextureTexUnit = 0;

    bool _ignoreNodeUpdate = false;

    void onDoUpdateNode(iWidget* source);
    void onTexture0Button(iWidget* source);
    void onTexture1Button(iWidget* source);
    void onTexture2Button(iWidget* source);
    void onTexture3Button(iWidget* source);

    void onStart(iWidget* source);
    void onStop(iWidget* source);
    void onReset(iWidget* source);

    void updateGUI();
    void updateNode();

    void initGUI();
    void deinitGUI();

};

#endif