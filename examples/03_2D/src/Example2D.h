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

#ifndef IGOR_EXAMPLE2D_H
#define IGOR_EXAMPLE2D_H

#include <ExampleBase.h>

#include <memory>

/*! rendering 2d example
*/
class Example2D : public ExampleBase
{

public:
    /*! initializes the example

    \param window the given window
    */
    Example2D(iWindowPtr window);

    /*! does nothing
    */
    ~Example2D() = default;

private:
    /*! perlin noise generator 
    (default ctor initializes with 1337 as random seed)
    */
    iPerlinNoise _perlinNoise;

    /*! stores the last mouse position
    */
    iaVector2f _lastMousePos;

    /*! doughnuts
    */
    iSpritePtr _doughnuts = nullptr;

    /*! current transform of doughnuts
    */
    iaMatrixf _doughnutMatrix;

    /*! frame index of doughnuts
    */
    uint32 _doughnutsFrameIndex = 0;

    /*! timer to control frame switching
    */
    iaTime _doughnutsTime;

    /*! background tileable texture
    */
    iTexturePtr _backgroundTexture;

    /*! a dummy texture
    */
    iTexturePtr _dummyTexture;

    /*! a mandelbrot texture
    */
    iTexturePtr _mandelbrotTexture;    

    /*! a B-Spline
    */
    iaBSplinef _spline;

    /*! random number generator
    */
    iaRandomNumberGenerator _rand;

    /*! update timer
    */
    iTimerHandlePtr _updateTimingHandle;

    /*! particle system
    */
    iParticleSystem2D _particleSystem;

    /*! called by timer

    \param time the time
    */
    void onUpdate(const iaTime &time);

    /*! called by orthogonal view
    */
    void onRenderOrtho() override;

    /*! initializes the example
    */
    void onInit() override;

    /*! deinitializes the example
    */
    void onDeinit() override;

    /*! called on any other event

    \param event the event to handle
    */
    void onEvent(iEvent &event) override;

    /*! called when key was pressed

    \param event the event to handle
    */
    bool onKeyDown(iEventKeyDown &event);    

    /*! initialize particle system and emitter
    */
    void initParticleSystem();

    /*! initialize a generated texture
    */
    void initMandelbrotTexture();

    /*! handles mouse move event

    \param event the mouse move event
    \returns true if consumed
    */
    bool onMouseMoveEvent(iEventMouseMove &event);
};

#endif // IGOR_EXAMPLE2D_H
