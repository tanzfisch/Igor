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
// (c) Copyright 2012-2019 by Martin Loga
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
// contact: martinloga@gmx.de  

#ifndef __iNODEFACTORY__
#define __iNODEFACTORY__

#include <iNode.h>

#include <iaSingleton.h>
#include <iaMutex.h>
using namespace IgorAux;

#include <unordered_map>

namespace Igor
{

	/*! creates and destroys nodes
	*/
	class Igor_API iNodeManager : public iaSingleton<iNodeManager>
	{
		friend class iaSingleton<iNodeManager>;
		friend class iApplication;
		friend class iScene;
		friend class iNode;

	public:

		/*! type of action to queue
		*/
		enum class Igor_API iActionType
		{
			None,
			Insert,
			Remove,
			Destroy,
			Activate,
			Deactivate
		};

		/*! struct used for queueing actions
		*/
		struct Igor_API iAction
		{
			/*! node A to do an action with
			*/
			uint64 _nodeA = iNode::INVALID_NODE_ID;

			/*! node B to do an action with
			*/
			uint64 _nodeB = iNode::INVALID_NODE_ID;

			/*! the action to do with node A, B or both
			*/
			iActionType _action = iActionType::None;
		};

		/*! get node by id

		\param id id of ndoe
		\returns pointer to node
		*/
		iNodePtr getNode(uint64 id) const;

		/*! \returns list of all node IDs of a certain node type

		\param nodeType type of nodes
		*/
		std::vector<uint64> getNodes(iNodeType nodeType);

		/*! \returns true if node ID exists

		\param id the nodes ID
		*/
		bool isNode(uint64 id) const;

		/*! create copy of node

		\param node the node to copy
		*/
		iNodePtr createCopy(iNodePtr node);

		/*! destroys node and all its children  (asynchronously)

		node get's destroyed in main thread in next frame

		\param node pointer to node to be destroyed
		*/
		void destroyNodeAsync(iNodePtr node);

		/*! destroys node and all its children

		node get's destroyed in main thread in next frame

		\param nodeID id of node (asynchronously)
		*/
		void destroyNodeAsync(uint64 nodeID);

		/*! applys asynchrounous actions to nodes

		\param actionQueue list of actions to be executed
		*/
		void applyActionsAsync(const std::vector<iAction>& actionQueue);

		/*! creates a node
		\returns pointer to new node
		*/
		template<class T>
		T* createNode();

		/*! inserts one node as child to an other (asynchronously)

		the actual insertion happens in the main thread

		\param parent the future parent node
		\param child the child to be inserted
		*/
		void insertNodeAsync(iNodePtr parent, iNodePtr child);

		/*! removes one node from an other (asynchronously)

		the actual removal happens in the main thread

		\param parent parent node
		\param child the child to be removed
		*/
		void removeNodeAsync(iNodePtr parent, iNodePtr child);

		/*! sets node active/inactive (asynchronously)

		\param node pointer to node
		\param active the active/inactive flag
		*/
		void setActiveAsync(iNodePtr node, bool active);

		/*! flushing queues and updating scenes

		\todo should be private
		*/
		void flush();

	private:

		/*! mapping ids to nodes
		*/
		std::unordered_map<uint64, iNodePtr> _nodes;

		/*! mutex to protect node list
		*/
		static iaMutex _mutexNodes;

		/*! queue with actions
		*/
		std::vector<iAction> _actionQueue;

		/*! mutex to protect activities
		*/
		iaMutex _mutexQueue;

		/*! last chance for the instance to clean up before shut down
		*/
		virtual void onPreDestroyInstance();

		/*! internal copy function for nodes

		\param node the source node
		\returns a copy of the source node
		*/
		iNodePtr createNodeCopy(iNodePtr node);

		/*! create copy of node

		\param node the node to copy
		\param recursiveDepth recursive depth
		*/
		iNodePtr createCopyInternal(iNodePtr node, std::map<uint64, uint64>& nodeIDMap, uint32 recursiveDepth);

		/*! destroys node and all its children

		\param node pointer to node to be destroyed
		*/
		void destroyNode(iNodePtr node);

		/*! destroys node and all its children

		if Id does not exist it does nothing

		\param nodeID id of node
		*/
		void destroyNode(uint64 nodeID);

		/*! called once per frame by application
		*/
		void handle();

		/*! does nothing
		*/
		iNodeManager() = default;

		/*! checks if all nodes where released
		*/
		~iNodeManager();

	};

	template<class T>
	T* iNodeManager::createNode()
	{
		T* result = new T();

		_mutexNodes.lock();
		_nodes[static_cast<iNode*>(result)->getID()] = static_cast<iNode*>(result);
		_mutexNodes.unlock();

		return result;
	}


#include <iNodeManager.inl>

}

#endif

