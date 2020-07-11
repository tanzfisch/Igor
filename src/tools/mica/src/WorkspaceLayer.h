
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

#ifndef __WORKSPACELAYER_H__
#define __WORKSPACELAYER_H__

#include "Workspace.h"
#include "CameraArc.h"

#include <memory>

class WorkspaceLayer : public iLayer
{

public:
    /*! init members
	*/
    WorkspaceLayer(iWindow *window, int32 zIndex, WorkspacePtr workspace);

    /*! deinit resources
	*/
    ~WorkspaceLayer();

    /*! \returns node at given screen position
	\param x horizonral screen position
	\param y vertical screen position
	*/
    iNodePtr getNodeAt(int32 x, int32 y);

    /*! frames the view on current selection
	*/
    void frameOnSelection();

private:
    /*! main scene view
	*/
    iView _view;

    /*! the mice workspace
    */
    WorkspacePtr _workspace;

    /*! the default camera
    */
    std::unique_ptr<CameraArc> _cameraArc;

    // TODO need to handle light differently
    iNodeTransform *_directionalLightTranslate = nullptr;
    iNodeTransform *_directionalLightRotate = nullptr;
    iNodeLight *_lightNode = nullptr;

    /*! cel shading material for selecting nodes in the scene
	*/
    iMaterialID _materialCelShading;

    /*! material for bounding box display 
	*/
    uint64 _materialBoundingBox;

    /*! render selection
    */
    void renderSelection();

    /*! clear resources
	*/
    void onDeinit() override;

    /*! init layer
    */
    void onInit() override;

    /*! called on any other event
    */
    void onEvent(iEvent &event) override;

    /*! handles mouse key down event

    \param event the mouse key down event
    \returns true if consumed
    */
    bool onMouseKeyDownEvent(iEventMouseKeyDown &event);

    /*! handles mouse key up event

    \param event the mouse key up event
    \returns true if consumed
    */
    bool onMouseKeyUpEvent(iMouseKeyUpEvent_TMP &event);

    /*! handles mouse move event

    \param event the mouse move event
    \returns true if consumed
    */
    bool onMouseMoveEvent(iEventMouseMove &event);

    /*! handles mouse wheel event

    \param event the mouse wheel event
    \returns true if consumed
    */
    bool onMouseWheelEvent(iMouseWheelEvent_TMP &event);

    /*! called when key was pressed

    \param event the event to handle
    */
    bool onKeyDown(iEventKeyDown &event);
};

#endif // __WORKSPACELAYER_H__
