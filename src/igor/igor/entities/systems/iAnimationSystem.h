
//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef IGOR_ANIMATION_SYSTEM_H
#define IGOR_ANIMATION_SYSTEM_H

#include <igor/entities/iEntitySystem.h>

#include <igor/entities/components/iTransformComponent.h>
#include <igor/entities/components/iSpriteRenderComponent.h>
#include <igor/entities/components/iAnimationComponent.h>

namespace igor
{	

	/*! behaviour system
	*/
	class iAnimationSystem : public iEntitySystem
	{
	public:
		/*! init types
		*/
		iAnimationSystem();

        /*! creates instance of this system type
         */
        static iEntitySystemPtr createInstance();
		
        /*! \returns type name of system
         */
        static const iaString &getTypeName();

		/*! updates system

		\param context the update context
		 */
		void onUpdate(const iEntitySceneUpdateContext &context) override;

		/*! \returns processing stage this system want's to run in
		*/
		iEntitySystemStage getStage() const override;

	private: 

		/*! a view on entities with animated transform
		*/
		iEntityViewPtr _viewTransform;

		/*! a view on entities with animated sprite
		*/
		iEntityViewPtr _viewSprite;

		/*! update transform
		*/
		void onUpdateTransform(const iaTime &time, iAnimationComponentPtr animationComp, iTransformComponentPtr transformComp);

		/*! update sprite
		*/
		void onUpdateSprite(const iaTime &time, iAnimationComponentPtr animationComp, iSpriteRenderComponentPtr spriteRenderComp);

		float64 mapTime(const iaTime &time, iAnimationComponentPtr animationComp, iClipPtr clip, iaStateMachine::iaEvent lastEvent);
	};

} // igor

#endif // IGOR_ANIMATION_SYSTEM_H