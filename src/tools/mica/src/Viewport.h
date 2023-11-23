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

#ifndef __VIEWPORT__
#define __VIEWPORT__

#include "Workspace.h"

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

private:
    iWidgetViewportPtr _viewportScene = nullptr;
    iWidgetViewportPtr _viewportOverlay = nullptr;

    WorkspacePtr _workspace;

    /*! cel shading material for selecting nodes in the scene
     */
    iMaterialPtr _materialCelShading;

    /*! material for bounding box display
     */
    iMaterialPtr _materialBoundingBox;

    // TODO need to handle light differently
    iNodeTransform *_directionalLightTranslate = nullptr;
    iNodeTransform *_directionalLightRotate = nullptr;
    iNodeLight *_lightNode = nullptr;

    /*! last mouse position
     */
    iaVector2f _lastMousePos;

    /*! handles pressed key event

    \param key the pressed key
    */
    bool onKeyDown(iKeyCode key) override;

    /*! handles mouse key up events

    \param key the key that was pressed
    \returns true: if event was consumed and therefore ignored by the parent
    */
    bool onMouseKeyUp(iKeyCode key) override;

    /*! handles incoming mouse move events

    \param pos mouse position
    \param consumed if true mouse move was already consumed
    */
    void onMouseMove(const iaVector2f &pos, bool consumed) override;

    /*! handles incoming mouse wheel event

    \param d mouse wheel delta
    \returns true: if event was consumed and therefore ignored by the parent
    */
    bool onMouseWheel(int32 d) override;

    /*! draws the widget
    */
    void draw() override;

    /*! \returns node at given screen position
    \param x horizonral screen position
    \param y vertical screen position
    */
    iNodePtr getNodeAt(int32 x, int32 y);

    void initScene();
    void renderSelection();
    void frameOnSelection();
};

#endif // __VIEWPORT__
