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
#include <iProfilerVisualizer.h>
#include <iTexture.h>
#include <iTimerHandle.h>
using namespace Igor;

#include <iaMatrix.h>
#include <iaVector2.h>
#include <iaBSpline.h>
#include <iaRandomNumberGenerator.h>
using namespace IgorAux;

#include <memory>


namespace Igor
{
	class iScene;
    class iAtlas;
    class iTextureFont;
	class iNodeTransform;
}

/*! sprite animation 2d example
*/
class SpriteAnimation
{

	/*! animation state of character
	*/
	enum class CharacterState
	{
		WalkN,
		WalkNE,
		WalkE,
		WalkSE,
		WalkS,
		WalkSW,
		WalkW,
		WalkNW,
		RunN,
		RunNE,
		RunE,
		RunSE,
		RunS,
		RunSW,
		RunW,
		RunNW,
		Idle,
	};

public:

    /*! initializes the example
    */
	SpriteAnimation();

    /*! deinitializes the example
    */
    virtual ~SpriteAnimation();

    /*! run the example
    */
    void run();

private:

    /*! the window
    */
    iWindow _window;

    /*! visualizes statistics
    */
    iProfilerVisualizer _profilerVisualizer;

    /*! the view we want to render in

    basically contains information about where inside the window to render and projection information
    */
    iView _view;
	
    /*! walk animation atlas
    */
	iAtlas* _walk = nullptr;

	/*! tiles atlas
	*/
	iAtlas* _tiles = nullptr;
	
	/*! the scene
	*/
	iScene* _scene = nullptr;
	
	/*! flags to determine what the character is doing
	*/
	bool _flags[5];

	/*! current position of character to render
	*/
	iaVector2f _characterPosition{ 0, 0 };

	/*! character velocity
	*/
	iaVector2f _characterVelocity;

	/*! current character state
	*/
	CharacterState _characterState;

	/*! animation offset in frames
	*/
	uint32 _animationOffset;

	/*! animation index in frames
	*/
	uint32 _animationIndex;

	/*! timer to control animation speed
	*/
	iTimerHandle _animationTimer;

	/*! texture font
	*/
	iTextureFont* _font = nullptr;

    /*! Igor logo
    */
    iTexturePtr _igorLogo = nullptr;
    
    /*! material id of a textured material with alpha blending
    */
    uint64 _materialWithTextureAndBlending = iMaterial::INVALID_MATERIAL_ID;

	/*! terrain material
	*/
	uint64 _materialTerrain = iMaterial::INVALID_MATERIAL_ID;

	/*! transform of camera
	*/
	iNodeTransform* _cameraTransform;

	/*! handles timer tick
	*/
	void onAnimationTimerTick();

	/*! \returns the characters state as string for debugging

	\param state the character state to convert to string
	*/
	iaString getCharacterStateName(CharacterState state);

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

    /*! called when key was pressed
    */
	void onKeyDown(iKeyCode key);

	/*! called when key was released
	*/
	void onKeyUp(iKeyCode key);

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

    /*! initializes the example
    */
	void init();

    /*! deinitializes the example
    */
	void deinit();

};

#endif

