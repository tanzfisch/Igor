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
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef MICA_LIGHT_OVERLAY_H
#define MICA_LIGHT_OVERLAY_H

#include "EntityOverlay.h"

class LightOverlay : public EntityOverlay
{

public:
    /*! initialize node overlay

    \param view the view to use
    */
    LightOverlay(iViewPtr view);

    /*! cleanup
     */
    ~LightOverlay();

    /*! sets overlay mode

    \param mode the new overlay mode
    */
    void setOverlayMode(OverlayMode mode) override;    

private:
    /*! red material
     */
    iMaterialPtr _red;

    /*! green material
     */
    iMaterialPtr _green;

    /*! blue material
     */
    iMaterialPtr _blue;

    /*! cyan material
     */
    iMaterialPtr _cyan;

    /*! light root
     */
    iEntityPtr _lightRoot = nullptr;

    /*! light modifier
     */
    iEntityPtr _lightModifier = nullptr;

    /*! light icon
     */
    iEntityPtr _lightIcon = nullptr;

    /*! light rays
     */
    iEntityPtr _lightRays = nullptr;

    /*! translate modifier
     */
    iEntityPtr _translateModifier = nullptr;

    /*! rotate modifier
     */
    iEntityPtr _rotateModifier = nullptr;

    /*! translate ids
     */
    std::vector<iEntityID> _translateIDs;

    /*! rotate ids
     */
    std::vector<iEntityID> _rotateIDs;

    /*! initialisation
     */
    void onInit();

    /*! update internal structure
     */
    void onUpdate();

    /*! on pre render callback
     */
    void onPreRender();

    /*! \returns a sun shaped mesh
     */
    iMeshPtr createSun();

    /*! \returns a rays shaped mesh
     */
    iMeshPtr createRays();

    /*! \returns true if mode in combination with a given entity can be handled by this node overlay

    \param mode the overlay mod
    \param entity the given entity
    */
    bool accepts(OverlayMode mode, iEntityPtr entity) override;

    /*! sets node overlay active

    \param active true to set node overlay active
    */
    void setActive(bool active) override;

    /*! create translate manipulator

    \param translateMesh translate mesh
    */
    void createTranslateModifier(iMeshPtr &translateMesh);

    /*! create rotate manipulator

    \param ringMesh ring mesh
    \param cylinder cylinder mesh
    */
    void createRotateModifier(iMeshPtr &ringMesh, iMeshPtr &cylinder);
};

#endif // MICA_LIGHT_OVERLAY_H
