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

    /*! sets node overlay active

    \param active true to set node overlay active
    */
    void setActive(bool active) override;

    /*! \returns true if mode in combination with a given entity can be handled by this node overlay

    \param mode the overlay mod
    \param entity the given entity
    */
    bool accepts(OverlayMode mode, iEntityPtr entity) override;

private:
    /*! cyan material
     */
    iMaterialPtr _cyan;

    /*! light overlay
     */
    iEntityPtr _lightRoot = nullptr;

    /*! initialisation
     */
    void onInit();

    /*! clean up
     */
    void onDeinit();

    /*! update internal structure
     */
    void onUpdate();

    /*! on pre render callback
     */
    void onPreRender();    

    /*! \returns a lightbulb shaped mesh
    */
    iMeshPtr createLightbulb();
};

#endif // MICA_LIGHT_OVERLAY_H
