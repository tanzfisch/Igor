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

#ifndef __IGOR_ENTITY_SCENE__
#define __IGOR_ENTITY_SCENE__

#include <igor/entities/iEntitySystem.h>

#include <entt.h>

#include <memory>

namespace igor
{
	/*! entity id definition
	 */
	typedef entt::entity iEntityID;

	/*! invalid entity id definition
	 */
	const entt::entity INVALID_ENTITY_ID = entt::null;

	class iEntity;

	/*! entity scene
	 */
	class IGOR_API iEntityScene : public std::enable_shared_from_this<iEntityScene>
	{

		friend class iEntitySystemModule;
		friend class iEntitySceneDeleter;

	public:

		/*! creates an entity
		 */
		iEntity createEntity(const iaString &name = "", bool active = true);

		/*! destroys an entity

		\param entity the entity to destroy
		*/
		void destroyEntity(iEntity entity);

		/*! destroys an entity by id

		\param entityID the entity ID
		*/
		void destroyEntity(iEntityID entityID);

		/*! clears the scene and the systems
		 */
		void clear();

		/*! \returns all entities with given components
		 */
		template <typename... Components>
		auto getEntities()
		{
			return _registry.view<Components...>();
		}

		/*! \returns entt registry
		 */
		entt::registry &getRegistry();

	private:
		/*! the entt registry
		 */
		entt::registry _registry;

		/*! systems to update
		 */
		std::vector<iEntitySystemPtr> _systems;

		/*! systems that render
		 */
		std::vector<iEntitySystemPtr> _renderingSystems;

		/*! init systems
		*/
		iEntityScene();
	};

} // igor

#endif // __IGOR_ENTITY_SCENE__