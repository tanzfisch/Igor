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

#ifndef __EMITTER_OVERLAY__
#define __EMITTER_OVERLAY__

#include "NodeOverlay.h"

/*! makes emitter nodes visible when selected
*/
class EmitterOverlay : public NodeOverlay
{

public:
    /*! initialize node overlay

    \param view the view to use
    \param scene the scene to use
    \param workspace the mica workspace
    */
    EmitterOverlay(iViewPtr view, iScenePtr scene, WorkspacePtr workspace);

    /*! cleanup
     */
    ~EmitterOverlay();

    /*! sets the node to control by ID

    \param nodeID id of node to control
    */
    void setNodeID(uint64 nodeID) override;

    /*! sets node overlay active

    \param active true to set node overlay active
    */
    void setActive(bool active) override;

    /*! \returns true if mode in combination with node type can be handled by this node overlay

    \param mode the overlay mode
    \param nodeKind kind of node
    \param nodeType type of node
    */
    bool accepts(OverlayMode mode, iNodeKind nodeKind, iNodeType nodeType) override;

    /*! sets overlay mode

    \param mode the new overlay mode
    */
    void setOverlayMode(OverlayMode mode) override;    

private:
    /*! root transform of overlay
     */
    iNodeTransformPtr _rootTransform = nullptr;

    /*! scale transform of overlay
     */
    iNodeTransformPtr _scaleTransform = nullptr;

    /*! switch node
    */
    iNodeSwitchPtr _switchNode = nullptr;

    /*! flat material
     */
    iMaterialPtr _materialFlat;

    /*! volume material
     */
    iMaterialPtr _materialVolume;

    /*! disc mesh node
    */
    iNodeMeshPtr _discMeshNode = nullptr;

    /*! circle mesh node
    */
    iNodeMeshPtr _circleMeshNode = nullptr;

    /*! sphere mesh node
    */
    iNodeMeshPtr _sphereMeshNode = nullptr;

    /*! square mesh node
    */
    iNodeMeshPtr _squareMeshNode = nullptr;

    /*! cube mesh node
    */
    iNodeMeshPtr _cubeMeshNode = nullptr;

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

#endif // __EMITTER_OVERLAY__
