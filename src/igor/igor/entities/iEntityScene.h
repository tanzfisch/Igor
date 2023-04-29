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

#include <memory>
#include <unordered_map>
#include <typeindex>

namespace igor
{
	class iEntity;

	class iEntitySceneImpl;

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
		void destroyEntity(const iEntity &entity);

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
		const std::vector<iEntityID> &getEntitiesV2();

		template <typename... Components>
		auto getEntities()
		{
			return getRegistry().view<Components...>();
		}

		/*! \returns reference to component for given entity

		\param entityID the given entity
		*/
		template <typename T>
		T &getComponent(iEntityID entityID);

		/*! \returns pointer to component for given entity. nullptr if component does not exist

		\param entityID the given entity
		*/
		template <typename T>
		T *tryGetComponent(iEntityID entityID);

		/*! removes component of given entity with given type
		 */
		template <typename T>
		void removeComponent(iEntityID entityID);

        /*! adds transform component to given entity

        \param position the transform position
        \param orientation the transform orientation in rad
        \param scale the transform scale
        \param parent the parent of this transform
        \param worldMatrix world matrix of this transform

        \returns reference to newly created component
         */
        iTransformComponent &addTransformComponent(iEntityID entityID, const iaVector3d &position = iaVector3d(), const iaVector3d &orientation = iaVector3d(), const iaVector3d &scale = iaVector3d(), iEntityID parent = IGOR_INVALID_ENTITY_ID, const iaMatrixd &worldMatrix = iaMatrixd());

        /*! adds sprite render component to given entity

        \param texture texture to use for render
        \param color color to render with
        \param zIndex z index order

        \returns reference to newly created component
         */
        iSpriteRendererComponent &addSpriteRendererComponent(iEntityID entityID, iTexturePtr texture = nullptr, const iaColor4f &color = iaColor4f(), int32 zIndex = 0);

        /*! adds velocity component to given entity

        \param velocity velocity in 3 dimensions
        \param angularVelocity angular velocity in 3 dimensions

        \returns reference to newly created component
         */
        iVelocityComponent &addVelocityComponent(iEntityID entityID, const iaVector3d &velocity = iaVector3d(), const iaVector3d &angularVelocity = iaVector3d());

		/*! adds Quadtree component to given entity

		will only work if Quadtree on the scene was initialized before

		\param entityID the given entity
		\param size size of object (radius)
		 */
		void addToQuadtree(iEntityID entityID, float64 size = 1.0);

		/*! removes given entity form quadtree
		*/
		void removeFromQuadtree(iEntityID entityID);

		/*! \returns entt registry
		 */
		entt::registry &getRegistry() const;

		/*! initialize quadtree

		\param box volume of the whole quadtree
		\param splitThreshold threshold count of objects on a node before splitting the node
		\param maxDepth the maximum depth of the tree
		*/
		void initializeQuadtree(const iaRectangled &box, const uint32 splitThreshold = 4, const uint32 maxDepth = 16);

		/*! \returns internal quadtree
		 */
		iQuadtreed &getQuadtree() const;

	private:
		/*! pimpl
		 */
		iEntitySceneImpl *_impl = nullptr;

		/*! caching entity ID lists
		 */
		std::unordered_map<std::type_index, std::vector<iEntityID>> _entityIDCache;

		/*! updates all non rendering systems
		 */
		void onUpdate();

		/*! updates all rendering systems
		 */
		void onRender();

		/*! init systems
		 */
		iEntityScene();

		/*! cleanup
		 */
		~iEntityScene();
	};

} // igor

#endif // __IGOR_ENTITY_SCENE__