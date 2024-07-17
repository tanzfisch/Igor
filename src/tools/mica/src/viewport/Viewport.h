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

#ifndef MICA_VIEWPORT_H
#define MICA_VIEWPORT_H

#include "../Workspace.h"
#include "overlay/NodeOverlay.h"

class Viewport : public iDialog
{
    friend class iWidgetManager;

public:
    /*! init
     */
    Viewport(WorkspacePtr workspace);

    /*! deinit
     */
    ~Viewport();

    void setCamera(iNodeID cameraID);
    iNodeID getCamera() const;

    /*! sets the manipulator mode on currently selected node
    but only if it is a transform node otherwise its set to none

    \param modifierMode the modifier mode to set
    */
    void setOverlayMode(OverlayMode overlayMode);

    /*! \returns the overlay mode
     */
    OverlayMode getOverlayMode() const;

private:
    /*! viewport to display workspace scene
     */
    iWidgetViewportPtr _viewportScene = nullptr;

    /*! viewport to display ui overlay scene
     */
    iWidgetViewportPtr _viewportOverlay = nullptr;

    /*! the workspace
     */
    WorkspacePtr _workspace;

    /*! cel shading material for selecting nodes in the scene
     */
    iShaderPtr _materialCelShading;

    /*! material for bounding box display
     */
    iShaderPtr _materialBoundingBox;

    /*! material for orientation plane
     */
    iShaderPtr _materialOrientationPlane;

    /*! the scene for the overlay
     */
    iScenePtr _overlayScene;

    /*! node overlays
     */
    std::vector<NodeOverlayPtr> _nodeOverlays;

    // TODO need to handle light differently
    iNodeTransform *_directionalLightTranslate = nullptr;
    iNodeTransform *_directionalLightRotate = nullptr;
    iNodeLight *_lightNode = nullptr;

    /*! last mouse position
     */
    iaVector2f _lastMousePos;

    /*! overlay mode
     */
    OverlayMode _overlayMode = OverlayMode::None;

    /*! selected node
     */
    iNodePtr _selectedNode = nullptr;

    // TODO 
    iResourceID _sceneResourceID;

    /*! handles incoming generic event

    \param event the event
    */
    bool onEvent(iEvent &event) override;

    /*! triggered when selection in scene changed

    \param event the event handle
    */
    bool onSceneSelectionChanged(iEventSceneSelectionChanged &event);

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

    /*! \returns node at given screen position
    \param x horizontal screen position
    \param y vertical screen position
    */
    iNodePtr getNodeAt(int32 x, int32 y);

    /*! initialize basic scene
     */
    void initScene();

    /*! frame viewport on selected nodes
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

    /*! checks overlays for candidates that accept current mode node combination
     */
    void updateAcceptance();

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
};

#endif // MICA_VIEWPORT_H