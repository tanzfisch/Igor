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

#ifndef __USERCONTROLMESH__
#define __USERCONTROLMESH__

#include <iNodeMesh.h>
#include <iaColor4.h>
using namespace IgorAux;

#include <iWidgetUserControl.h>
#include <iUserControlColorChooser.h>
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
}

class UserControlMesh : public iWidgetUserControl
{
public:

    UserControlMesh();
    ~UserControlMesh();

    iWidget* getWidget();

    void setNode(uint32 id);
    uint32 getNode();

private:

    iWidgetGrid* _grid = nullptr;

    iWidgetGrid* _detailsGrid = nullptr;
    iWidgetLabel* _labelVertexes = nullptr;
    iWidgetLabel* _labelTriangles = nullptr;
    iWidgetLabel* _labelIndexes = nullptr;
    iWidgetTextEdit* _textVertices = nullptr;
    iWidgetTextEdit* _textTriangles = nullptr;
    iWidgetTextEdit* _textIndexes = nullptr;

    iWidgetGrid* _gridShininess = nullptr;
    iWidgetNumberChooser* _textShininess = nullptr;
    iWidgetLabel* _labelShininess = nullptr;
    iWidgetLabel* _labelShininessShort = nullptr;
    iWidgetSlider* _sliderShininess = nullptr;

    vector<iWidget*> _allWidgets;

    iaColor4f _ambient;
    iaColor4f _emissive;
    iaColor4f _diffuse;
    iaColor4f _specular;
    float32 _shininess;

    iUserControlColorChooser* _ambientColorChooser = nullptr;
    iUserControlColorChooser* _emissiveColorChooser = nullptr;
    iUserControlColorChooser* _diffuseColorChooser = nullptr;
    iUserControlColorChooser* _specularColorChooser = nullptr;

    uint32 _nodeId = 0;

    void onAmbientChange(const iaColor4f& color);
    void onDiffuseChange(const iaColor4f& color);
    void onSpecularChange(const iaColor4f& color);
    void onEmissiveChange(const iaColor4f& color);

    void onTextChangedShininess(iWidget* source);
    void onSliderChangedShininess(iWidget* source);

    void updateGUI();
    void updateNode();

    void initGUI();
    void deinitGUI();

};

#endif