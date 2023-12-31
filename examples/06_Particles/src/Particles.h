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

#ifndef IGOR_PARTICLES_EXAMPLE_H
#define IGOR_PARTICLES_EXAMPLE_H

#include <ExampleBase.h>

/*! the particles example class
*/
class Particles : public ExampleBase
{

public:
    /*! init

    \param window the given window
    */
    Particles(iWindowPtr window);

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
    iShaderMaterialPtr _particlesMaterial;

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

    /*! called by timer

    \param time the time
    */
    void onUpdate(const iaTime &time);

    /*! called on any other event

    \param event the event to handle
    */
    void onEvent(iEvent &event) override;

    /*! called when key was pressed

    \param event the event to handle
    */
    bool onKeyDown(iEventKeyDown &event);

    /*! handles mouse move event

    \param event the mouse move event
    \returns true if consumed
    */
    bool onMouseMoveEvent(iEventMouseMove &event);

    /*! handles mouse wheel event

    \param event the mouse wheel event
    \returns true if consumed
    */
    bool onMouseWheelEvent(iEventMouseWheel &event);

    /*! init example
    */
    void onInit() override;

    /*! deinit example
    */
    void onDeinit() override;

    /*! \returns help screen string
    */
    iaString getHelpString() override;      
};

#endif // IGOR_PARTICLES_EXAMPLE_H
