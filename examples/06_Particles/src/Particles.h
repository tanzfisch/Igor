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
// contact: igorgameengine@protonmail.com

#ifndef __PARTICLES_EXAMPLE_H__
#define __PARTICLES_EXAMPLE_H__

#include <ExampleBase.h>

#include <igor/resources/material/iMaterial.h>
#include <igor/system/iTimerHandle.h>
using namespace igor;

/*! the particles example class
*/
class Particles : public ExampleBase
{

public:
    /*! init
    */
    Particles();

    /*! does nothing
    */
    ~Particles() = default;

private:
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
    uint64 _particlesMaterial = iMaterial::INVALID_MATERIAL_ID;

    /*! ids of particle systems
    */
    std::vector<uint64> _particleSystemIDs;

    /*! id of circle emitter transform id
    */
    uint64 _circleEmitterTransformID = iNode::INVALID_NODE_ID;

    /*! id of wave emitter transform id
    */
    uint64 _waveEmitterTransformID = iNode::INVALID_NODE_ID;

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

    /*! called by orthogonal view
    */
    void onRenderOrtho();

    /*! called by timer
    */
    void onTimer();

    /*! called when the mouse was moved

    \param from last mouse position
    \param to current mouse position
    \param window the window the coordinates are related to
    */
    void onMouseMovedFull(const iaVector2i &from, const iaVector2i &to, iWindow *window) override;

    /*! called when mouse wheel was turned

    \param d mouse wheel delta
    */
    void onMouseWheel(int32 d) override;

    /*! called on key pressed event

    \param key the key code of the pressed key
    */
    void onKeyDown(iKeyCode key) override;

    /*! deinit example
    */
    void deinit() override;

    /*! init example
    */
    void init() override;
};

#endif
