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

#ifndef __USERCONTROLMESH__
#define __USERCONTROLMESH__

#include "UserControlNode.h"

class UserControlMesh : public UserControlNode
{
public:
    /*! user control

    \param nodeID the node to init with
    \param parent parent widget
    */
    UserControlMesh(iNodeID nodeID, const iWidgetPtr parent = nullptr);

    /*! does nothing
     */
    ~UserControlMesh() = default;

    /*! update ui with node data
     */
    void update() override;

    /*! update node with ui data
     */
    void updateNode() override;

    /*! init ui
     */
    void init() override;

private:
    /*! texture chooser 0
     */
    iUserControlFileChooser *_textureChooser0 = nullptr;

    /*! texture chooser 1
     */
    iUserControlFileChooser *_textureChooser1 = nullptr;

    /*! texture chooser 2
     */
    iUserControlFileChooser *_textureChooser2 = nullptr;

    /*! texture chooser 3
     */
    iUserControlFileChooser *_textureChooser3 = nullptr;

    /*! vertex count text field
     */
    iWidgetLineTextEdit *_textVertices = nullptr;

    /*! triangle count text field
     */
    iWidgetLineTextEdit *_textTriangles = nullptr;

    /*! indices count text field
     */
    iWidgetLineTextEdit *_textIndexes = nullptr;

    /*! shininess number chooser
     */
    iWidgetNumberChooser *_textShininess = nullptr;

    /*! shininess slider
     */
    iWidgetSlider *_sliderShininess = nullptr;

    iaColor4f _ambient; // TODO remove these
    iaColor4f _emissive;
    iaColor4f _diffuse;
    iaColor4f _specular;
    float32 _shininess;

    /*! ambient color chooser
     */
    iUserControlColorChooser *_ambientColorChooser = nullptr;

    /*! emissive color chooser
     */
    iUserControlColorChooser *_emissiveColorChooser = nullptr;

    /*! diffuse color chooser
     */
    iUserControlColorChooser *_diffuseColorChooser = nullptr;

    /*! specular color chooser
     */
    iUserControlColorChooser *_specularColorChooser = nullptr;

    /*! material selector
     */
    iWidgetSelectBox *_materialSelector = nullptr;

    /*! prevent endless update loop
     */
    bool _ignoreNodeUpdate = false;

    /*! on ambient color change

    \param color the new color
    */
    void onAmbientChange(const iaColor4f &color);

    /*! on diffuse color change

    \param color the new color
    */
    void onDiffuseChange(const iaColor4f &color);

    /*! on specular color change

    \param color the new color
    */
    void onSpecularChange(const iaColor4f &color);

    /*! on emissive color change

    \param color the new color
    */
    void onEmissiveChange(const iaColor4f &color);

    /*! time to update the node

    \param source the source widget for this update
    */
    void onDoUpdateNode(const iWidgetPtr source);

    /*! handle change of shininess

    \param source the source widget
    */
    void onTextChangedShininess(const iWidgetPtr source);

    /*! handle change of shininess

    \param source the source widget
    */
    void onSliderChangedShininess(const iWidgetPtr source);

    /*! handle change of material

    \param source the source widget
    */
    void onMaterialChanged(const iWidgetPtr source);
};

#endif // __USERCONTROLMESH__