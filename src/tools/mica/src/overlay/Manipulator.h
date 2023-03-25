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

#ifndef __MANIPULATOR_H__
#define __MANIPULATOR_H__

#include "NodeOverlay.h"

#include <memory>

/*! 3d overlay ui element for scene nodes

    to modify position, orientation and scale of objects in the scene
    to display additional information when nodes are selected
*/
class Manipulator : public NodeOverlay
{

public:
    Manipulator(iViewPtr view, iScenePtr scene, WorkspacePtr workspace);

    /*! cleanup
     */
    ~Manipulator();

    /*! \returns true the node overlay is selected
     */
    bool isSelected() const override;

    /*! sets node overlay visible

    \param visible true to set node overlay visible
    */
    void setVisible(bool visible) override;

    /*! sets the mode of the node overlay

    \param nodeOverlayMode the node overlay mode
    */
    void setOverlayMode(OverlayMode nodeOverlayMode) override;

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
    iMaterialPtr _material;

    /*! cel chader material for selection
     */
    iMaterialPtr _materialCelShading;

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

    /*! on mouse moved callback

    \param from the mouse moved from here
    \param to the mouse moved to here
    */
    virtual void onMouseMoved(const iaVector2i &from, const iaVector2i &to) override;

    /*! initialisation
     */
    void onInit();

    /*! clean up
     */
    void onDeinit();

    /*! on render callback
     */
    virtual void onRender();

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

    /*! selects node inside of node overlay if present
    
    \returns true if node overlay has control and can select given nodeID
    */
    bool select(iNodeID nodeID) override;

    /*! unselect what ever may be selected
    */
    void unselect() override;
};

/*! manipulator pointer definition
*/
typedef Manipulator* ManipulatorPtr;

#endif // __MANIPULATOR_H__
