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

#ifndef __PARTICLES__
#define __PARTICLES__

#include <igor/igor.h>
#include <igor/os/iWindow.h>
#include <igor/graphics/iView.h>
#include <igor/os/iTimerHandle.h>
#include <igor/resources/model/iModelResourceFactory.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/resources/profiler/iProfilerVisualizer.h>
using namespace Igor;

#include <iaux/math/iaMatrix.h>
using namespace IgorAux;

namespace Igor
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
} // namespace Igor

/*! the particles example class
*/
class Particles
{

public:
    /*! init
    */
    Particles();

    /*! deinit
    */
    virtual ~Particles();

    /*! run example
    */
    void run();

private:
    /*! the window
    */
    iWindow _window;

    /*! visualizes statistics
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

    /*! texture fon we use to render statistics
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

    /*! timer handle to control the movement of the light source over time
    */
    iTimerHandle *_animationTimingHandle = nullptr;

    /*! particles material
    */
    uint64 _particlesMaterial = 0;

    /*! id of task to load textures
    */
    uint64 _taskFlushTexturesID = iTask::INVALID_TASK_ID;

    /*! ids of particle systems
    */
    std::vector<uint64> _particleSystemIDs;

    /*! id of circle emitter transform id
    */
    uint64 _circleEmitterTransformID = iNode::INVALID_NODE_ID;

    /*! id of wave emitter transform id
    */
    uint64 _waveEmitterTransformID = iNode::INVALID_NODE_ID;

    /*! igor logo
    */
    iTexturePtr _igorLogo = nullptr;

    /*! igor logo material
    */
    uint64 _materialWithTextureAndBlending = iMaterial::INVALID_MATERIAL_ID;

    /*! creates the dot shaped particle system
    */
    void createDotParticleSystem();

    /*! creates smoke particle system
    */
    void createSmokeParticleSystem();

    /*! creates ring shaped particle system
    */
    void createRingParticleSystem();

    /*! crates a particle system with a wave shaped emitter
    */
    void createWaveParticleSystem();

    /*! crates a fontain look alike particle system
    */
    void createFontainParticleSystem();

    /*! crates a fire particle system
    */
    void createFireParticleSystem();

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

    /*! draws igor logo
    */
    void drawLogo();

    /*! called by timer
    */
    void onTimer();

    /*! deinit example
    */
    void deinit();

    /*! init example
    */
    void init();
};

#endif
