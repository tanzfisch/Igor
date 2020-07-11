
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

#ifndef __OVERLAYLAYER_H__
#define __OVERLAYLAYER_H__

#include "Workspace.h"

#include <igor/igor.h>
using namespace igor;

class OverlayLayer : public iLayer
{

public:
    /*! init members
	*/
    OverlayLayer(iWindow *window, int32 zIndex, WorkspacePtr workspace);

    /*! deinit resources
	*/
    ~OverlayLayer();

private:
    /*! the view of this layer
    */
    iView _view;

    /*! orthogonal projected view of this layer
    */
    iView _viewOrtho;

    /*! the scene of this layer
    */
    iScenePtr _scene;

    /*! material for orientation plane 
	*/
    iMaterialID _materialOrientationPlane;

    /*! the mica workspace
    */
    WorkspacePtr _workspace;

    /*! visualizer for profiler data
	*/
    iProfilerVisualizer _profilerVisualizer;

    /*! font for profiler
    */
    iTextureFontPtr _font;

    /*! manipulator
	*/
    //    Manipulator *_manipulator = nullptr;

    /*! pointer to active 3d widget
	*/
    // Widget3D *_widget3D = nullptr;

    /*! reset manipulator mode to none
	*/
    // void resetManipulatorMode();

    /*! sets the manipulator mode on currently selected node 
	but only if it is a transform node otherwise its set to none

	\param modifierMode the modifier mode to set
	*/
    // void setManipulatorMode(ManipulatorMode modifierMode);

    /*! render selection
    */
    void renderSelection();

    /*! render orientation plane
    */
    void renderOrientationPlane();

    /*! render overlay
    */
    void render();

    /*! render othogonal overlay
    */
    void renderOrtho();

    /*! clear resources
	*/
    void onDeinit() override;

    /*! init layer
    */
    void onInit() override;

    /*! called on any other event
    */
    void onEvent(iEvent &event) override;

    /*! called when key was pressed

    \param event the event to handle
    */
    bool onKeyDown(iEventKeyDown &event);

    /*! handle window resize event

    \param event the window resize event
    \returns true if consumed
    */
    bool onWindowResize(iEventWindowResize &event);
};

#endif // __OverlayLayer_H__
