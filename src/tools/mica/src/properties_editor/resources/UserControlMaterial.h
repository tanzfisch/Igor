//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef MICA_USERCONTROL_MATERIAL_H
#define MICA_USERCONTROL_MATERIAL_H

#include "UserControlResource.h"

class UserControlMaterial : public UserControlResource
{
public:
    /*! init user control

    \param resourceID the resource id to use
    \param parent the optional parent widget
    */
    UserControlMaterial(iResourceID resourceID, const iWidgetPtr parent = nullptr);

    /*! does nothing
     */
    ~UserControlMaterial() = default;

    /*! init ui
     */
    virtual void init();

    /*! update ui with node data
     */
    virtual void update();

    /*! update node with ui data
     */
    virtual void updateResource();

private:
    /*! diffuse color chooser
     */
    iUserControlColor *_diffuseColorChooser = nullptr;

    /*! ambient color chooser
     */
    iUserControlColor *_ambientColorChooser = nullptr;

    /*! specular color chooser
     */
    iUserControlColor *_specularColorChooser = nullptr;

    /*! emissive color chooser
     */
    iUserControlColor *_emissiveColorChooser = nullptr;

    /*! shininess number chooser
     */
    iWidgetNumberChooserPtr _numberChooserShininess = nullptr;

    /*! texture choosers
     */
    iUserControlTexturePtr _textureChooser[4] = {nullptr, nullptr, nullptr, nullptr};

    /*! shader material chooser
    */
    iUserControlShaderMaterialPtr _shaderMaterialChooser = nullptr;

    /*! shininess slider
     */
    iWidgetSliderPtr _sliderShininess = nullptr;

    /*! flag to prevent endless update loop
     */
    bool _ignoreUpdate = false;

    /*! triggers material update

    \param source the source widget of this event
    */
    void onDoUpdateMaterial(const iWidgetPtr source);

    /*! triggers material update

    \param source the source widget of this event
    */
    void onDoUpdateShininess(const iWidgetPtr source);
};

#endif // MICA_USERCONTROL_MATERIAL_H