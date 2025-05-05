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

#ifndef MICA_VIEWPORT_H
#define MICA_VIEWPORT_H

#include "overlay/EntityOverlay.h"
#include "camera/CameraArc.h"

/*! Mica's viewport dialog
 */
class Viewport : public iDialog
{
    friend class iWidgetManager;

public:
    /*! init
     */
    Viewport();

    /*! deinit
     */
    ~Viewport();

    /*! sets the manipulator mode on currently selected entity

    \param overlayMode the modifier mode to set
    */
    void setOverlayMode(OverlayMode overlayMode);

    /*! \returns the overlay mode
     */
    OverlayMode getOverlayMode() const;

private:
    /*! viewport to display scene
     */
    iWidgetViewportPtr _viewportScene = nullptr;

    /*! viewport to display ui overlay scene
     */
    iWidgetViewportPtr _viewportOverlay = nullptr;

    /*! cel shading material for selecting entities in the scene
     */
    iShaderPtr _materialCelShading;

    /*! material for bounding box display
     */
    iShaderPtr _materialBoundingBox;

    /*! material for orientation plane
     */
    iShaderPtr _materialOrientationPlane;

    /*! entity overlays
     */
    std::vector<EntityOverlayPtr> _entityOverlays;

    /*! last mouse position
     */
    iaVector2f _lastMousePos;

    /*! overlay mode
     */
    OverlayMode _overlayMode = OverlayMode::None;

    /*! the context menu
     */
    iDialogMenu _contextMenu;

    /*! camera arc controller
     */
    std::unique_ptr<CameraArc> _cameraArc;

    /*! if true render orientation plane
     */
    bool _renderOrientationPlane = true;

    /*! grid button
     */
    iWidgetButtonPtr _buttonGrid = nullptr;

    /*! bounds button
     */
    iWidgetButtonPtr _buttonBounds = nullptr;

    /*! handles incoming generic event

    \param event the event
    */
    bool onEvent(iEvent &event) override;

    /*! handles pressed key event

    \param key the pressed key
    */
    bool onKeyDown(iEventKeyDown &event) override;

    /*! handles mouse key up events

    \param event the mouse key up event
    \returns true: if event was consumed and therefore ignored by the parent
    */
    bool onMouseKeyUp(iEventMouseKeyUp &event) override;

    /*! handles incoming mouse key down events

    \param event mouse key down event
    \returns true: if event was consumed and therefore ignored by the parent
    */
    virtual bool onMouseKeyDown(iEventMouseKeyDown &event);

    /*! handles incoming mouse move events

    \param event mouse move event
    */
    void onMouseMove(iEventMouseMove &event) override;

    /*! handles incoming mouse wheel event

    \param event mouse wheel event
    \returns true: if event was consumed and therefore ignored by the parent
    */
    bool onMouseWheel(iEventMouseWheel &event) override;

    /*! draws the widget
     */
    void draw() override;

    /*! \returns entityID at given screen position
    \param x horizontal screen position
    \param y vertical screen position
    */
    iEntityID getEntityIDAt(int32 x, int32 y);

    /*! \returns node at given screen position
    \param x horizontal screen position
    \param y vertical screen position
    */
    // TODO getEntityAt // iNodePtr getNodeAt(int32 x, int32 y);

    /*! frame viewport on selected entities
     */
    void frameOnSelection();

    /*! render the scene
     */
    void renderScene();

    /*! render the overlay
     */
    void renderOverlay();

    /*! render the selection outline
     */
    void renderSelection();

    /*! render the orientation plane
     */
    void renderOrientationPlane();

    /*! checks overlays for candidates that accept current mode-entity combination
     */
    void updateOverlay();

    /*! deactivates all overlays
    */
    void clearOverlay();

    /*! drag move handle

    \param drag the drag data
    \param mousePos the current mouse pos
    */
    void onDragMove(iDrag &drag, const iaVector2f &mousePos) override;

    /*! drop handle

    \param drag the drag data
    \param mousePos the current mouse pos
    */
    void onDrop(const iDrag &drag, const iaVector2f &mousePos) override;

    /*! called when a resource was loaded
     */
    void onResourceLoaded(const iResourceID resourceID);

    /*! called when project was loaded
     */
    bool onProjectLoaded(iEventProjectLoaded &event);

    /*! called when project was unloaded
     */
    bool onProjectUnloaded(iEventProjectUnloaded &event);

    /*! handles context menu call

    \param source the event source
    */
    void onContextMenu(iWidgetPtr source);

    /*! change camera
     */
    void onChangeCamera(iWidgetPtr source);

    /*! on grid button click
     */
    void onGridClick(iWidgetPtr source);

    /*! on bounds button click
     */
    void onBoundsClick(iWidgetPtr source);

    /*! handle selection change
     */
    void onSelectionChanged(const iEntitySceneID &sceneID, const std::vector<iEntityID> &entities);
};

#endif // MICA_VIEWPORT_H