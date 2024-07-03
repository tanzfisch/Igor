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
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef IGOR_ENTITY_SCENE_H
#define IGOR_ENTITY_SCENE_H

#include <igor/entities/iEntitySystem.h>
#include <igor/entities/iEntity.h>
#include <igor/data/iQuadtree.h>

#include <iaux/data/iaUUID.h>
#include <iaux/data/iaRectangle.h>
using namespace iaux;

#include <unordered_map>
#include <cstring>

namespace igor
{
	class iCameraSystem;

	/*! entity scene id
	 */
	typedef iaUUID iEntitySceneID;

	/*! entity scene
	 */
	class IGOR_API iEntityScene
	{
		friend class iEntitySystemModule;
		friend class iEntity;
		friend class iView;
		friend class iEntityTraverser;

	public:
		/*! sets name of scene

		\param name the name of the scene
		*/
		void setName(const iaString &name);

		/*! \returns name of the scene
		 */
		const iaString &getName() const;

		/*! creates an entity, adds it to this scene and returns it

		ownership stays with the scene

		\returns newly created entity
		*/
		iEntityPtr createEntity(const iaString &name = "");

		/*! \returns entity for given entity ID. zero if not found
		\param entityID the given entity ID
		*/
		iEntityPtr getEntity(iEntityID entityID) const;

		/*! \returns all entities with camera component

		only if iCameraSystem was added to this scene
		*/
		std::vector<iEntityPtr> getCameras() const;

		/*! \returns active camera
		 */
		iEntityPtr getActiveCamera() const;

		/*! destroys entity for given ID

		\param entityID the given entity id
		*/
		void destroyEntity(iEntityID entityID);

		/*! destroys given entity

		\param entity the given entity
		*/
		void destroyEntity(iEntityPtr entity);

		/*! \returns entity scene id
		 */
		const iEntitySceneID &getID() const;

		/*! initialize quadtree

		\param box volume of the whole quadtree
		\param splitThreshold threshold count of objects on a node before splitting the node
		\param maxDepth the maximum depth of the tree
		*/
		void initializeQuadtree(const iaRectangled &box, const uint32 splitThreshold = 4, const uint32 maxDepth = 16);

		/*! \returns internal quadtree
		 */
		iQuadtreed &getQuadtree() const;

		/*! \returns true if quadtree present
		 */
		bool hasQuadtree() const;

		/*! add system

		\param system the system to add
		*/
		void addSystem(iEntitySystemPtr system);

		/*! remove system

		\param system the system to remove
		*/
		void removeSystem(iEntitySystemPtr system);		

	private:
		/*! entity scene id
		 */
		iEntitySceneID _id;

		/*! name of the scene
		 */
		iaString _name;

		/*! map of entities
		 */
		std::unordered_map<iEntityID, iEntityPtr> _entities;

		/*! entity delete queue
		 */
		std::vector<iEntityID> _deleteQueue;

		/*! keep one specialized root entity for tree traversal
		 */
		iEntityPtr _root = nullptr;

		/*! list of systems
		 */
		std::array<std::vector<iEntitySystemPtr>, (int)iEntitySystemStage::StageCount> _systems;

		/*! quadtree
		 */
		iQuadtreed *_quadtree = nullptr;

		/*! pointer to camera system if it was added
		 */
		iCameraSystem *_cameraSystem = nullptr;

		/*! the render engine to use in render update stage
		*/
		iRenderEnginePtr _renderEngine = nullptr;

		/*! ctor

		\param name the name of the scene
		*/
		iEntityScene(const iaString &name);

		/*! dtor clean up
		 */
		~iEntityScene();

		/*! sets render engine

		\param renderEngine the render engine to set
		*/
		void setRenderEngine(iRenderEnginePtr renderEngine);

		/*! flush entity delete queue
		 */
		void flushQueues();

		/*! updates systems

		\param time simulation frame time
		\param stage what stage to update
		 */
		void onUpdate(const iaTime &time, iEntitySystemStage stage);

		/*! callback to handle added component

		\param entity pointer of entity
		\param typeID type of added component
		*/
		void onComponentAdded(iEntityPtr entity, const std::type_index &typeID);

		/*! callback to handle removed component

		\param entity pointer of entity
		\param typeID type of removed component
		*/
		void onComponentRemoved(iEntityPtr entity, const std::type_index &typeID);

		/*! callback to handle component to be removed

		\param entity pointer of entity
		\param typeID type of component to be removed
		*/
		void onComponentToRemove(iEntityPtr entity, const std::type_index &typeID);

		/*! called after a bunch of components been added/removed

		\param entity the entity that has changed it's components
		*/
		void onEntityChanged(iEntityPtr entity);
	};

} // igor

#endif // IGOR_ENTITY_SCENE_H