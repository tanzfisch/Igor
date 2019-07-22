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
//    (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __EXAMPLE2D__
#define __EXAMPLE2D__

#include <iWindow.h>
#include <iView.h>
#include <iParticleSystem2D.h>
#include <iKeyCodeMap.h>
#include <iaGradient.h>
#include <iPerlinNoise.h>
#include <iMaterial.h>
#include <iStatisticsVisualizer.h>
using namespace Igor;

#include <iaMatrix.h>
#include <iaVector2.h>
#include <iaBSpline.h>
#include <iaRandomNumberGenerator.h>
using namespace IgorAux;

#include <memory>
using namespace std;

namespace Igor
{
    class iSprite;
    class iTexture;
    class iTextureFont;
}

/*! rendering 2d example
*/
class Example2D
{

public:

    /*! initializes the example
    */
    Example2D();

    /*! deinitializes the example
    */
    virtual ~Example2D();

    /*! run the example
    */
    void run();

private:

    /*! the window
    */
    iWindow _window;

    /*! visualizes statistics
    */
    iStatisticsVisualizer _statisticsVisualizer;

    /*! the view we want to render in

    basically contains information about where inside the window to render and projection information
    */
    iView _view;

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
    shared_ptr<iTexture> _particleTexture = nullptr;

    /*! just increases over time and feeds a sinus function to change the orientation of the particle stream
    */
    float32 _particleAnimatioValue = 0.0f;

    /*! multicolor gradient used for coloring the particles
    */
    iaGradientColor4f _rainbow;
	
    /*! opengl logo
    */
	iSprite* _openGLLogo = nullptr;

    /*! Igor logo
    */
    shared_ptr<iTexture> _igorLogo = nullptr;

    /*! current position of renderer logo 
    */
    iaVector2f _logoPosition{200, 200};

    /*! rotation angle of logo in radians
    */
    float32 _logoRotationAngle = 0.0f;

    /*! texture font
    */
	iTextureFont* _font = nullptr;

    /*! background tileable texture
    */
    shared_ptr<iTexture> _backgroundTexture = nullptr;

    /*! a dummy texture
    */
	shared_ptr<iTexture> _dummyTexture = nullptr;

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
    iaBSpline _spline;

    /*! random number generator
    */
    iaRandomNumberGeneratoru _rand;

    /*! mouse move event with minimum data

    mouse coordinates have their origin in the upper left corner of the parenting window

    \param position last mouse position
    */
    void onMouseMove(const iaVector2i& position);

    /*! called when window was closed
    */
	void onWindowClosed();

    /*! called on window resize

    \param clientWidth width of client rectangle
    \param clientHeight height of client rectangle
    */
    void onWindowResize(int32 clientWidth, int32 clientHeight);

    /*! called when esc key was pressed
    */
	void onKeyESCPressed();

    /*! called before every frame
    */
	void onHandle();
	
    /*! called every frame 
    
    here we render everyting
    */
	void onRender();

    /*! draw Igor Logo
    */
    void drawLogo();

    /*! call the particles system handle to update particle positions

    also changes initiali velocity of particles for waving particle stream effect
    */
    void updateParticles();

    /*! initializes the example
    */
	void init();

    /*! deinitializes the example
    */
	void deinit();



};

#endif

