//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IGOR_BEHAVIOUR_SYSTEM__
#define __IGOR_BEHAVIOUR_SYSTEM__

#include <igor/entities/iEntitySystem.h>

namespace igor
{

	/*! behaviour system

	processes any entity that contains the following components:
	- iBehaviourComponent
	*/
	class iBehaviourSystem : public iEntitySystem
	{
	public:
		/*! does nothing
		*/
		iBehaviourSystem() = default;

		/*! does nothing
		*/
		~iBehaviourSystem() = default;

		/*! updates system

		\param scene the scene used for this update
		 */
		void update(iEntityScenePtr scene) override;
	};

} // igor

#endif // __IGOR_TRANSFORM_HIERARCHY_SYSTEM__