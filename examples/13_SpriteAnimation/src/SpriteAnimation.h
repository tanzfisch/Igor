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

#ifndef __EXAMPLESPRITEANIMATION_H__
#define __EXAMPLESPRITEANIMATION_H__

#include <ExampleBase.h>

#include <memory>

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

	\param window the given window
    */
	SpriteAnimation(iWindowPtr window);

	/*! does nothing
    */
	~SpriteAnimation() = default;

private:
	/*! walk animation atlas
    */
	iAtlasPtr _walk = nullptr;

	/*! tiles atlas
	*/
	iAtlasPtr _tiles = nullptr;

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
	iMaterialID_old _materialTerrain = iMaterial_old::INVALID_MATERIAL_ID;

	/*! transform of camera
	*/
	iNodeTransformPtr _cameraTransform;

	/*! switches beween ascii output or single key output
    */
	bool _outputSwitch = true;

	/*! handles timer tick

	\param time the time
	*/
	void onUpdate(const iaTime &time);

	/*! \returns the characters state as string for debugging

	\param state the character state to convert to string
	*/
	iaString getCharacterStateName(CharacterState state);

	/*! called before every frame
    */
	void onPreDraw() override;

	/*! called every frame 
    
    here we render everyting
    */
	void onRenderOrtho() override;

	/*! initializes the example
    */
	void onInit() override;

	/*! deinitializes the example
    */
	void onDeinit() override;

	/*! called on any other event
    */
	void onEvent(iEvent &event) override;

	/*! called when key was pressed

    \param event the event to handle
    */
	bool onKeyDown(iEventKeyDown &event);

	/*! called when key was released

    \param event the event to handle
    */
	bool onKeyUp(iEventKeyUp &event);
};

#endif // __EXAMPLESPRITEANIMATION_H__
