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
// (c) Copyright 2012-2022 by Martin Loga
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

#ifndef __IGOR_ENTITY_SECENE__
#define __IGOR_ENTITY_SECENE__

#include <igor/entities/iComponents.h>

#include <entt.h>

namespace igor
{
	class iEntityScene;
	typedef iEntityScene* iEntityScenePtr;

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
        virtual void update(iEntityScenePtr scene) {};
    };	

	typedef std::unique_ptr<iEntitySystem> iEntitySystemPtr;
	class iEntity;

    /*! entity id definition
    */
    typedef entt::entity iEntityID;

	/*! iunvalid entity id definition
	*/
	const entt::entity iInvalidEntityID = entt::null;

	/*! wrapper for entt registry
	*/
	class IGOR_API iEntityScene
	{
		friend class iEntity;

	public:
		/*! creates an entity
		 */
		iEntity createEntity(const iaString &name = "");

		/*! destroyes an entity

		\param entity the entity to destroy
		*/
		void destroyEntity(iEntity entity);

		/*! destroyes an entity by id

		\param entityID the entity ID
		*/
		void destroyEntity(iEntityID entityID);

		/*! registers a system to the scene

		\param system the system to register
		*/
		void registerSystem(iEntitySystemPtr system);

		/*! updates all systems in the order hey have been added to the scene
		*/
		void updateSystems();

		/*! clears the scene and the systems
		*/
		void clear();

		/*! \returns all entities with given components
		*/
		template<typename... Components>
		auto getEntities()
		{
			return _registry.view<Components...>();
		}		

	private:

		/*! the entt registry

		\todo we need a much more mighty registry wrapper in order to get rid of the header include
		*/
		entt::registry _registry;

		/*! currently registered systems
		*/
		std::vector<iEntitySystemPtr> _systems;
	};

} // igor

#endif // __IGOR_ENTITY_SECENE__