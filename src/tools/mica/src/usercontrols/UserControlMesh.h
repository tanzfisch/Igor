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
// (c) Copyright 2014-2020 by Martin Loga
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

#ifndef __USERCONTROLMESH_H__
#define __USERCONTROLMESH_H__

#include <igor/igor.h>
using namespace iaux;
using namespace igor;

class UserControlMesh : public iUserControl
{

public:
    UserControlMesh();
    ~UserControlMesh();

    void setNode(uint32 id);
    uint32 getNode();

private:
    void draw() override;

    /*! if true ui is initialized
	*/
    bool _initialized = false;

    iUserControlFileChooser *_textureChooser0 = nullptr;
    iUserControlFileChooser *_textureChooser1 = nullptr;
    iUserControlFileChooser *_textureChooser2 = nullptr;
    iUserControlFileChooser *_textureChooser3 = nullptr;

    iWidgetLineTextEdit *_textVertices = nullptr;
    iWidgetLineTextEdit *_textTriangles = nullptr;
    iWidgetLineTextEdit *_textIndexes = nullptr;

    iWidgetNumberChooser *_textShininess = nullptr;
    iWidgetSlider *_sliderShininess = nullptr;

    iaColor4f _ambient;
    iaColor4f _emissive;
    iaColor4f _diffuse;
    iaColor4f _specular;
    float32 _shininess;

    iUserControlColorChooser *_ambientColorChooser = nullptr;
    iUserControlColorChooser *_emissiveColorChooser = nullptr;
    iUserControlColorChooser *_diffuseColorChooser = nullptr;
    iUserControlColorChooser *_specularColorChooser = nullptr;

    iWidgetSelectBox *_selectMaterial = nullptr;

    uint32 _nodeId = 0;
    bool _ignoreNodeUpdate = false;
    uint32 _loadTextureTexUnit = 0;

    void onAmbientChange(const iaColor4f &color);
    void onDiffuseChange(const iaColor4f &color);
    void onSpecularChange(const iaColor4f &color);
    void onEmissiveChange(const iaColor4f &color);

    void onDoUpdateNode(const iWidgetPtr source);
    void onBakeAction(const iWidgetPtr source);

    void onTextChangedShininess(const iWidgetPtr source);
    void onSliderChangedShininess(const iWidgetPtr source);

    void onMaterialChanged(const iWidgetPtr source);

    void updateGUI();
    void updateNode();

    void initGUI();
    void deinitGUI();
};

#endif // __USERCONTROLMESH_H__