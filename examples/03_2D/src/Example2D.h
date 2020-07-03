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

#ifndef __EXAMPLE2D_H__
#define __EXAMPLE2D_H__

#include <ExampleBase.h>

#include <memory>

/*! rendering 2d example
*/
class Example2D : public ExampleBase
{

public:
    /*! initializes the example
    */
    Example2D();

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

    /*! particles
    */
    iParticleSystem2D _particleSystem;

    /*! texture used for the particles
    */
    iTexturePtr _particleTexture;

    /*! just increases over time and feeds a sinus function to change the orientation of the particle stream
    */
    float32 _particleAnimatioValue = 0.0f;

    /*! multicolor gradient used for coloring the particles
    */
    iaGradientColor4f _rainbow;

    /*! opengl logo
    */
    iAtlasPtr _openGLLogo = nullptr;

    /*! current position of renderer logo 
    */
    iaVector2f _logoPosition{200, 200};

    /*! background tileable texture
    */
    iTexturePtr _backgroundTexture = nullptr;

    /*! a dummy texture
    */
    iTexturePtr _dummyTexture = nullptr;

    /*! material id of a textured material
    */
    uint64 _materialWithTexture = iMaterial::INVALID_MATERIAL_ID;

    /*! material id of a textured material with alpha blending
    */
    uint64 _materialWithTextureAndBlending = iMaterial::INVALID_MATERIAL_ID;

    /*! material id of a non textured material
    */
    uint64 _materialWithoutDepthTest = iMaterial::INVALID_MATERIAL_ID;

    /*! a B-Spline
    */
    iaBSplinef _spline;

    /*! random number generator
    */
    iaRandomNumberGeneratoru _rand;

    /*! call the particles system handle to update particle positions

    also changes initiali velocity of particles for waving particle stream effect
    */
    void updateParticles();

    /*! called by orthogonal view
    */
    void onRenderOrtho() override;

    /*! called before every frame
    */
    void onPreDraw() override;

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

    /*! handles mouse move event

    \param event the mouse move event
    \returns true if consumed
    */
    bool onMouseMoveEvent(iMouseMoveEvent_TMP &event);
};

#endif // __EXAMPLE2D_H__
