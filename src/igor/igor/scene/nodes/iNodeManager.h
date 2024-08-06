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

#ifndef __IGOR_NODEFACTORY__
#define __IGOR_NODEFACTORY__

#include <igor/scene/nodes/iNode.h>
#include <igor/resources/module/iModule.h>
#include <igor/resources/profiler/iProfiler.h>

#include <iaux/system/iaMutex.h>
using namespace iaux;

#include <unordered_map>

namespace igor
{

    /*! creates and destroys nodes
	*/
    class IGOR_API iNodeManager : public iModule<iNodeManager>
    {
        friend class iModule<iNodeManager>;
        friend class iApplication;
        friend class iScene;
        friend class iNode;

    public:
        /*! type of action to queue
		*/
        enum class IGOR_API iActionType
        {
            Undefined,
            Insert,
            Remove,
            Destroy,
            Activate,
            Deactivate
        };

        /*! struct used for queueing actions
		*/
        struct IGOR_API iAction
        {
            /*! node A to do an action with
			*/
            iNodeID _nodeA = iNode::INVALID_NODE_ID;

            /*! node B to do an action with
			*/
            iNodeID _nodeB = iNode::INVALID_NODE_ID;

            /*! the action to do with node A, B or both
			*/
            iActionType _action = iActionType::Undefined;
        };

        /*! get node by id

		\param id id of ndoe
		\returns pointer to node
		*/
        iNodePtr getNode(iNodeID id) const;

        /*! returns list of all nodes with given node type

        \param[out] nodes returned by this function
		\param nodeType the given node type
		*/
        void getNodes(std::vector<iNodeID> &nodes, iNodeType nodeType);

        /*! returns list of all nodes

        \param[out] all nodes
		*/
        void getNodes(std::vector<iNodeID> &nodes);

        /*! \returns true if node ID exists

		\param id the nodes ID
		*/
        bool isNode(iNodeID id) const;

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
        void destroyNodeAsync(iNodeID nodeID);

        /*! applys asynchrounous actions to nodes

		\param actionQueue list of actions to be executed
		*/
        void applyActionsAsync(const std::vector<iAction> &actionQueue);

        /*! creates a node
		\returns pointer to new node
		*/
        template <class T>
        T *createNode(const iaString &name = "");

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
        std::unordered_map<iNodeID, iNodePtr> _nodes;

        /*! mutex to protect node list
		*/
        static iaMutex _mutexNodes;

        /*! queue with actions
		*/
        std::vector<iAction> _actionQueue;

        /*! mutex to protect activities
		*/
        iaMutex _mutexQueue;

        /*! internal copy function for nodes

		\param node the source node
		\returns a copy of the source node
		*/
        iNodePtr createNodeCopy(iNodePtr node);

        /*! create copy of node

		\param node the node to copy
		\param recursiveDepth recursive depth
		*/
        iNodePtr createCopyInternal(iNodePtr node, std::map<iNodeID, iNodeID> &nodeIDMap, uint32 recursiveDepth);

        /*! destroys node and all its children

		\param node pointer to node to be destroyed
		*/
        void destroyNode(iNodePtr node);

        /*! destroys node and all its children

		if Id does not exist it does nothing

		\param nodeID id of node
		*/
        void destroyNode(iNodeID nodeID);

        /*! called once per frame by application
		*/
        void onUpdate();

        /*! does nothing
		*/
        iNodeManager() = default;

        /*! checks if all nodes where released
		*/
        ~iNodeManager();
    };

#include <igor/scene/nodes/iNodeManager.inl>

} // namespace igor

#endif // __IGOR_NODEFACTORY__
