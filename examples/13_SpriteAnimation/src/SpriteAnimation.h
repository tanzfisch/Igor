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

#ifndef __EXAMPLE_SPRITE_ANIMATION_H__
#define __EXAMPLE_SPRITE_ANIMATION_H__

#include <ExampleBase.h>

#include <igor/resources/material/iMaterial.h>
#include <igor/system/iTimerHandle.h>
using namespace igor;

#include <iaux/math/iaVector2.h>
using namespace iaux;

#include <memory>

namespace igor
{
	class iAtlas;
	class iNodeTransform;
} // namespace igor

/*! sprite animation 2d example
*/
class SpriteAnimation : public ExampleBase
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

	/*! does nothing
    */
	~SpriteAnimation() = default;

private:
	/*! walk animation atlas
    */
	iAtlas *_walk = nullptr;

	/*! tiles atlas
	*/
	iAtlas *_tiles = nullptr;

	/*! flags to determine what the character is doing
	*/
	bool _flags[5];

	/*! current position of character to render
	*/
	iaVector2f _characterPosition;

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

	/*! terrain material
	*/
	iMaterialID _materialTerrain = iMaterial::INVALID_MATERIAL_ID;

	/*! transform of camera
	*/
	iNodeTransform *_cameraTransform;

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
	void onMouseMoved(const iaVector2i &position) override;

	/*! called when key was pressed
    */
	void onKeyDown(iKeyCode key) override;

	/*! called when key was released
	*/
	void onKeyUp(iKeyCode key) override;

	/*! called before every frame
    */
	void onPreDraw() override;

	/*! called every frame 
    
    here we render everyting
    */
	void onRenderOrtho() override;

	/*! initializes the example
    */
	void init() override;

	/*! deinitializes the example
    */
	void deinit() override;
};

#endif // __EXAMPLE_SPRITE_ANIMATION_H__
