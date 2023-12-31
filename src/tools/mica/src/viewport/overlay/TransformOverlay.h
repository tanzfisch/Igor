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

#ifndef __TRANSFORM_OVERLAY__
#define __TRANSFORM_OVERLAY__

#include "NodeOverlay.h"

/*! 3d overlay ui element for scene nodes

    to modify position, orientation and scale of objects in the scene
    to display additional information when nodes are selected
*/
class TransformOverlay : public NodeOverlay
{

public:
    /*! initialize node overlay

    \param view the view to use
    \param scene the scene to use
    \param workspace the mica workspace
    */
    TransformOverlay(iViewPtr view, iScenePtr scene, WorkspacePtr workspace);

    /*! cleanup
     */
    ~TransformOverlay();

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
    /*! id of selected manipulator node
     */
    iNodeID _selectedManipulatorNodeID = iNode::INVALID_NODE_ID;

    /*! root transform of manipulator
     */
    iNodeTransformPtr _rootTransform = nullptr;

    /*! switch node to switch between manipulator states
     */
    iNodeSwitchPtr _switchNode = nullptr;

    /*! translate node ids
     */
    std::vector<iNodeID> _translateIDs;

    /*! scale node ids
     */
    std::vector<iNodeID> _scaleIDs;

    /*! rotate node ids
     */
    std::vector<iNodeID> _rotateIDs;

    /*! rotate billboard transform
     */
    iNodeTransformPtr _rotateBillboardTransform = nullptr;

    /*! locator representation of manipulator
     */
    iNodePtr _locatorRepresentation = nullptr;

    /*! transform modifier representation of manipulator
     */
    iNodePtr _translateModifier = nullptr;

    /*! scale modifier representation of manipulator
     */
    iNodePtr _scaleModifier = nullptr;

    /*! rotate modifier representation of manipulator
     */
    iNodePtr _roateModifier = nullptr;

    /*! red target material
     */
    iTargetMaterialPtr _red = nullptr;

    /*! green target material
     */
    iTargetMaterialPtr _green = nullptr;

    /*! blue target material
     */
    iTargetMaterialPtr _blue = nullptr;

    /*! cyan target material
     */
    iTargetMaterialPtr _cyan = nullptr;

    /*! material to render manipulator
     */
    iShaderMaterialPtr _material;

    /*! cel chader material for selection
     */
    iShaderMaterialPtr _materialCelShading;

    /*! translate given matrix by vector projected on selected axis

    \param vec vector to translate
    \param matrix the matrix to manipulate
    */
    void translate(const iaVector3d &vec, iaMatrixd &matrix);

    /*! scele given matrix by vector on selected axis

    \param vec scale vector
    \param matrix the matrix to manipulate
    */
    void scale(const iaVector3d &vec, iaMatrixd &matrix);

    /*! roate matrix based screen space movement

    \param from from in screen space
    \param to to in screen space
    \param matrix the matrix to manipulate
    */
    void rotate(const iaVector2d &from, const iaVector2d &to, iaMatrixd &matrix);

    /*! update internal structure
     */
    void update();

    /*! handles mouse move event

    \param event the mouse move event
    */
    bool onMouseMoveEvent(iEventMouseMove &event) override;

    /*! handles mouse key down event

    \param event the mouse key down event
    \returns true if consumed
    */
    bool onMouseKeyDownEvent(iEventMouseKeyDown &event) override;

    /*! handles mouse key up event

    \param event the mouse key up event
    \returns true if consumed
    */
    virtual bool onMouseKeyUpEvent(iEventMouseKeyUp &event);    

    /*! initialisation
     */
    void onInit();

    /*! clean up
     */
    void onDeinit();

    /*! on render callback
     */
    void onRender();

    /*! create translate mesh
     */
    iMeshPtr createTranslateMesh();

    /*! create scale mesh
     */
    iMeshPtr createScaleMesh();

    /*! create cube mesh
     */
    iMeshPtr createCube();

    /*! create ring mesh
     */
    iMeshPtr createRingMesh();

    /*! create 2d ring mesh
     */
    iMeshPtr create2DRingMesh();

    /*! create cylinder mesh
     */
    iMeshPtr createCylinder();

    /*! create translate manipulator

    \param translateMesh translate mesh
    */
    void createTranslateModifier(iMeshPtr &translateMesh);

    /*! create scale manipulator

    \param scaleMesh scale mesh
    */
    void createScaleModifier(iMeshPtr &scaleMesh);

    /*! create rotate manipulator

    \param ringMesh ring mesh
    \param ringMesh2D ring mesh 2d
    \param cylinder cylinder mesh
    */
    void createRotateModifier(iMeshPtr &ringMesh, iMeshPtr &ringMesh2D, iMeshPtr &cylinder);

    /*! create locator representation

    \param cylinder cylinder mesh
    */
    void createLocatorRepresentation(iMeshPtr &cylinder);

    /*! highlight selected manipulator node
     */
    void renderHighlight();
};

#endif // __TRANSFORM_OVERLAY__
