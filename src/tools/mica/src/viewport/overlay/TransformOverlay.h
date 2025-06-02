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

#ifndef MICA_TRANSFORM_OVERLAY_H
#define MICA_TRANSFORM_OVERLAY_H

#include "EntityOverlay.h"

/*! 3d overlay ui element for scene nodes

    to modify position, orientation and scale of objects in the scene
    to display additional information when nodes are selected
*/
class TransformOverlay : public EntityOverlay
{

public:
    /*! initialize node overlay

    \param view the view to use
    */
    TransformOverlay(iViewPtr view);

    /*! cleanup
     */
    ~TransformOverlay();

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

    /*! translate ids
     */
    std::vector<iEntityID> _translateIDs;

    /*! scale ids
     */
    std::vector<iEntityID> _scaleIDs;

    /*! rotate ids
     */
    std::vector<iEntityID> _rotateIDs;

    /*! root transform
     */
    iEntityPtr _rootTransform = nullptr;

    /*! translate modifier
     */
    iEntityPtr _translateModifier = nullptr;

    /*! rotate modifier
     */
    iEntityPtr _rotateModifier = nullptr;

    /*! scale modifier
     */
    iEntityPtr _scaleModifier = nullptr;

    /*! selected entity ID
     */
    iEntityID _selectionID = iEntityID::getInvalid();

    /*! translate given matrix by vector projected on selected axis

    \param vec vector to translate
    \param transformComp the transform component to manipulate
    */
    void translate(const iaVector3d &vec, iTransformComponentPtr transform);

    /*! scele given matrix by vector on selected axis

    \param vec scale vector
    \param matrix the matrix to manipulate
    */
    void scale(const iaVector3d &vec, iTransformComponentPtr transform);

    /*! roate matrix based screen space movement

    \param from from mouse position in world coordinates
    \param to to mouse position in world coordinates
    \param transform the transform to manipulate
    */
    void rotate(const iaVector3d &from, const iaVector3d &to, iTransformComponentPtr transform);

    /*! update internal structure
     */
    void onUpdate();

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

    /*! on pre render callback
     */
    void onPreRender();

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
    \param cylinder cylinder mesh
    */
    void createRotateModifier(iMeshPtr &ringMesh, iMeshPtr &cylinder);

    /*! \returns true if mode in combination with a given entity can be handled by this node overlay

    \param mode the overlay mod
    \param entity the given entity
    */
    bool accepts(OverlayMode mode, iEntityPtr entity) override;

    /*! sets node overlay active

    \param active true to set node overlay active
    */
    void setActive(bool active) override;
};

#endif // MICA_TRANSFORM_OVERLAY_H
