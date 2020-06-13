//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/                     ) )     ((
//                                           (_(       \)
//    (c) Copyright 2014-2020 by Martin Loga
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
// contact: martinloga@gmx.de

#ifndef __EVALUATION__
#define __EVALUATION__

#include <igor/igor.h>
#include <igor/system/iWindow.h>
#include <igor/graphics/iView.h>
#include <igor/system/iTimerHandle.h>
#include <igor/resources/model/iModelResourceFactory.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/resources/profiler/iProfilerVisualizer.h>
#include <igor/scene/nodes/iNodeTransform.h>
using namespace igor;

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{
    class iScene;
    class iNodeTransform;
    class iNodeLight;
    class iNodeSwitch;
    class iTextureFont;
    class iTaskFlushModels;
    class iTaskFlushTextures;
    class iNodeLODTrigger;
    class iNodeLODSwitch;
    class iNodeModel;
    class iTexture;
} // namespace igor

class Evaluation
{

public:
    /*! init
    */
    Evaluation();

    /*! deinit
    */
    virtual ~Evaluation();

    /*! run example
    */
    void run();

private:
    /*! the window
    */
    iWindow _window;

    /*! displays profiler
    */
    iProfilerVisualizer _profilerVisualizer;

    /*! the view we render 3D to
    */
    iView _view;

    /*! the view we render 2D to
    */
    iView _viewOrtho;

    /*! the scene holding our 3d nodes
    */
    iScene *_scene = nullptr;

    /*! async loading of models
    */
    uint64 _taskFlushModels = iTask::INVALID_TASK_ID;

    /*! async loading of textures
    */
    uint64 _taskFlushTextures = iTask::INVALID_TASK_ID;

    /*! texture font we use to display the profiler
    */
    iTextureFont *_font = nullptr;

    /*! id to transform node used for manipulating the heading of the camera
    */
    uint64 _cameraHeading = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the pitch of the camera
    */
    uint64 _cameraPitch = iNode::INVALID_NODE_ID;

    /*! id to transform node used for manipulating the distance of the camera to the origin
    */
    uint64 _cameraTranslation = iNode::INVALID_NODE_ID;

    /*! material definition for the sky box
    */
    uint64 _materialSkyBox = iMaterial::INVALID_MATERIAL_ID;

    /*! material for igor logo
    */
    uint64 _materialWithTextureAndBlending = iMaterial::INVALID_MATERIAL_ID;

    /*! igor logo
    */
    iTexturePtr _igorLogo = nullptr;

    /*! called on key pressed event

    \param key the key code of the pressed key
    */
    void onKeyPressed(iKeyCode key);

    /*! called when window was closed
    */
    void onWindowClosed();

    /*! called when window was resized

    \param clientWidth the client rectangle width
    \param clientHeight the client rectangle height
    */
    void onWindowResized(int32 clientWidth, int32 clientHeight);

    /*! called when the mouse was moved

    \param from last mouse position
    \param to current mouse position
    \param window the window the coordinates are related to
    */
    void onMouseMoved(const iaVector2i &from, const iaVector2i &to, iWindow *window);

    /*! called when mouse wheel was turned

    \param d mouse wheel delta
    */
    void onMouseWheel(int32 d);

    /*! called by orthogonal view
    */
    void onRenderOrtho();

    /*! draw igor logo
    */
    void drawLogo();

    /*! creates camera in the scene
    */
    void createCamera();

    /*! creates skybox in the scene
    */
    void createSkybox();

    /*! deinit example
    */
    void deinit();

    /*! init example
    */
    void init();

    /*! setup the evaluation example
    */
    void setupEvaluation();

    /*! evaluation script

    \param transform the transform to manipulate
    \param t the time where we are at
    */
    void evalScript(iNodeTransformPtr transform, float64 t);

    /*! create light in the scene
    */
    void createDirectionalLight();
};

#endif
