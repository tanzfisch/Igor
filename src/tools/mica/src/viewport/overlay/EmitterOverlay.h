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

#ifndef MICA_EMITTER_OVERLAY_H
#define MICA_EMITTER_OVERLAY_H

#include "EntityOverlay.h"

/*! makes emitter nodes visible when selected
*/
class EmitterOverlay : public EntityOverlay
{

public:
    /*! initialize node overlay

    \param view the view to use
    */
    EmitterOverlay(iViewPtr view);

    /*! cleanup
     */
    ~EmitterOverlay();

    /*! sets node overlay active

    \param active true to set node overlay active
    */
    void setActive(bool active) override;

    /*! \returns true if mode in combination with a given entity can be handled by this node overlay

    \param mode the overlay mod
    \param entity the given entity
    */
    bool accepts(OverlayMode mode, iEntityPtr entity) override;

    /*! sets overlay mode

    \param mode the new overlay mode
    */
    void setOverlayMode(OverlayMode mode) override;    

private:
    /*! flat material
     */
    iMaterialPtr _materialFlat;

    /*! volume material
     */
    iMaterialPtr _materialVolume;

    /*! update internal structure
     */
    void update();

    /*! initialisation
     */
    void onInit();

    /*! clean up
     */
    void onDeinit();

    /*! on render callback
     */
    void onRender();

    /*! creates a disc mesh
    */
    iMeshPtr createDisc();

    /*! creates a circle mesh
    */
    iMeshPtr createCircle();

    /*! creates a cube mesh
    */
    iMeshPtr createCube();

    /*! creates a sphere mesh
    */
    iMeshPtr createSphere();

    /*! creates a square mesh
    */
    iMeshPtr createSquare();
};

/*! emitter overlay pointer definition
 */
typedef EmitterOverlay *EmitterOverlayPtr;

#endif // MICA_EMITTER_OVERLAY_H
