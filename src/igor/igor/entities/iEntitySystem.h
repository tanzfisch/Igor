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

#ifndef __IGOR_ENTITY_SYSTEM__
#define __IGOR_ENTITY_SYSTEM__

#include <igor/entities/components/iComponentMap.h>

#include <memory>

namespace igor
{
	class iEntityScene;

	/*! entity scene pointer definition
	*/
	typedef std::shared_ptr<iEntityScene> iEntityScenePtr;

	class iEntitySystem
	{
	public:
		/*! does nothing
		 */
		iEntitySystem() = default;

		/*! does nothing
		 */
		virtual ~iEntitySystem() = default;

		/*! updates system
		 */
		virtual void update(const iaTime &time, iEntityScenePtr scene) = 0;

	};

	/*! entity system pointer definition
	 */
	typedef std::shared_ptr<iEntitySystem> iEntitySystemPtr;

	class iEntityRenderSystem
	{
	public:
		/*! does nothing
		 */
		iEntityRenderSystem() = default;

		/*! does nothing
		 */
		virtual ~iEntityRenderSystem() = default;

		/*! render system
		 */
		virtual void render(iEntityScenePtr scene) = 0;

	};

	/*! entity render system pointer definition
	*/
	typedef std::shared_ptr<iEntityRenderSystem> iEntityRenderSystemPtr;

} // igor

#endif // __IGOR_ENTITY_SYSTEM__