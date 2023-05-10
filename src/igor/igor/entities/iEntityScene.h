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
#include <igor/entities/systems/iVelocitySystem.h>

#include <memory>
#include <unordered_map>
#include <typeindex>

namespace igor
{
	class iEntity;

	/*! wrapper for entt registry
	 */
	class iRegistry;

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

		/*! destroys an entity by id

		\param entityID the entity ID
		*/
		void destroyEntity(iEntityID entityID);

		/*! destroys an entity

		\param entity the entity to destroy
		*/
		void destroyEntity(const iEntity &entity);

		/*! clears the scene
		 */
		void clear();

		/*! adds component to entity

		\param component the component to add
		*/
		template <typename T>
		T &addComponent(iEntityID entityID, const T &component);

		/*! adds custom component to entity

		this is meant for types unknown to Igor

		\param component the component to add
		*/
		template <typename T>
		T &addCustomComponent(iEntityID entityID, const T &component)
		{
			auto iter = _customComponents.find(typeid(T));
			if (iter == _customComponents.end())
			{
				_customComponents[typeid(T)] = std::make_shared<iComponentMap<T>>();
			}

			return std::static_pointer_cast<iComponentMap<T>>(_customComponents[typeid(T)])->add(entityID, component);
		}

		/*! \returns reference to component for given entity

		\param entityID the given entity
		*/
		template <typename T>
		T &getComponent(iEntityID entityID);

		/*! \returns reference to custom component of given entity

		\param component the component to add
		*/
		template <typename T>
		T &getCustomComponent(iEntityID entityID)
		{
			auto iter = _customComponents.find(typeid(T));
			con_assert(iter != _customComponents.end(), "component does not exist")

				return std::static_pointer_cast<iComponentMap<T>>(iter->second)
					->get(entityID);
		}

		/*! \returns pointer to component for given entity. nullptr if component does not exist

		\param entityID the given entity
		*/
		template <typename T>
		T *tryGetComponent(iEntityID entityID);

		/*! \returns pointer to custom component for given entity. nullptr if component does not exist

		\param entityID the given entity
		*/
		template <typename T>
		T *tryGetCustomComponent(iEntityID entityID)
		{
			auto iter = _customComponents.find(typeid(T));
			if (iter == _customComponents.end())
			{
				return nullptr;
			}

			return std::static_pointer_cast<iComponentMap<T>>(iter->second)->tryGet(entityID);
		}

		/*! removes component of given entity with given type
		 */
		template <typename T>
		void removeComponent(iEntityID entityID);

		/*! initialize quadtree

		\param box volume of the whole quadtree
		\param splitThreshold threshold count of objects on a node before splitting the node
		\param maxDepth the maximum depth of the tree
		*/
		void initializeQuadtree(const iaRectangled &box, const uint32 splitThreshold = 4, const uint32 maxDepth = 16);

		/*! \returns internal quadtree
		 */
		iQuadtreed &getQuadtree() const;

		/*! \returns entt registry
		 */
		void *getRegistry() const;

		/*! sets global bounds
		 */
		void setBounds(const iAABoxd &box);

		/*! \returns global bounds
		 */
		const iAABoxd &getBounds() const;

	private:
		/*! pimpl
		 */
		iRegistry *_registry = nullptr;

		/*! caching entity ID lists
		 */
		std::unordered_map<std::type_index, std::vector<iEntityID>> _entityIDCache;

		/*! quadtree
		 */
		iQuadtreed *_quadtree = nullptr;

		std::shared_ptr<iVelocitySystem> _velocitySystem;

		/*! systems to update
		 */
		std::vector<iEntitySystemPtr> _systems;

		/*! systems that render
		 */
		std::vector<iEntitySystemPtr> _renderingSystems;

		/*! storing custom component type data
		 */
		std::unordered_map<std::type_index, std::shared_ptr<void>> _customComponents;

		/*! entities set up for deletion
		 */
		std::deque<iEntityID> _deleteQueue;

		/*! destroys entities in the delete queue
		 */
		void destroyEntities();

		/*! updates all non rendering systems
		 */
		void onUpdate();

		/*! updates all rendering systems
		 */
		void onRender(float32 clientWidth, float32 clientHeight);

		/*! init systems
		 */
		iEntityScene();

		/*! cleanup
		 */
		~iEntityScene();
	};

} // igor

#endif // __IGOR_ENTITY_SCENE__