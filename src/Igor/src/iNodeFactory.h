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
// (c) Copyright 2012-2017 by Martin Loga
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
using namespace IgorAux;

#include <map>
using namespace std;

namespace Igor
{

    /*! creates and destroys nodes
    */
	class Igor_API iNodeFactory : public iaSingleton<iNodeFactory>
	{
		friend class iaSingleton<iNodeFactory>;
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
            uint32 _nodeA = iNode::INVALID_NODE_ID;

            /*! node B to do an action with
            */
            uint32 _nodeB = iNode::INVALID_NODE_ID;

            /*! the action to do with node A, B or both
            */
            iActionType _action = iActionType::None;
        };

        /*! get node by id

        \param id id of ndoe
        \returns pointer to node
        */
        iNode* getNode(uint32 id);

        /*! \returns list of all node IDs of a certain node type

        \param nodeType type of nodes
        */
        vector<uint32> getNodes(iNodeType nodeType);

        /*! \returns true if node ID exists

        \param id the nodes ID
        */
        bool isNode(uint32 id);

        /*! create copy of node

        \param node the node to copy
        */
        iNode* createCopy(iNode* node);

        /*! destroys node and all its children  (asynchronously)

        node get's destroyed in main thread in next frame

        \param node pointer to node to be destroyed
        */
        void destroyNodeAsync(iNode* node);

        /*! destroys node and all its children

        node get's destroyed in main thread in next frame

        \param nodeID id of node (asynchronously)
        */
        void destroyNodeAsync(uint32 nodeID);

        /*! applys asynchrounous actions to nodes

        \param actionQueue list of actions to be executed
        */
        void applyActionsAsync(const vector<iAction>& actionQueue);

        /*! creates a node

        \param nodeType determines which type of node shall be created
        \returns pointer to new node
        */
        iNode* createNode(iNodeType nodeType);

        /*! inserts one node as child to an other (asynchronously)

        the actual insertion happens in the main thread

        \param parent the future parent node
        \param child the child to be inserted
        */
        void insertNodeAsync(iNode* parent, iNode* child);

        /*! removes one node from an other (asynchronously)

        the actual removal happens in the main thread

        \param parent parent node
        \param child the child to be removed
        */
        void removeNodeAsync(iNode* parent, iNode* child);

        /*! sets node active/inactive (asynchronously)

        \param node pointer to node
        \param active the active/inactive flag
        */
        void setActiveAsync(iNode* node, bool active);

        /*! flushing queues and updating scenes

        \todo should be private
        */
        void flush();
            
    private:

        /*! mapping ids to nodes
        */
        map<uint32, iNode*> _nodes;

        /*! mutex to protect node list
        */
        mutex _mutexNodes;

        /*! queue with actions
        */
        vector<iAction> _actionQueue;

        /*! mutex to protect activities
        */
        mutex _mutexQueue;

        /*! internal copy function for nodes

        \param node the source node
        \returns a copy of the source node
        */
        iNode* createNodeCopy(iNode* node);

        /*! create copy of node

        \param node the node to copy
        \param recursiveDepth recursive depth
        */
        iNode* createCopyInternal(iNode* node, map<uint32, uint32>& nodeIDMap, uint32 recursiveDepth);

        /*! destroys node and all its children

        \param node pointer to node to be destroyed
        */
        void destroyNode(iNode* node);

        /*! destroys node and all its children

        if Id does not exist it does nothing

        \param nodeID id of node
        */
        void destroyNode(uint32 nodeID);

        /*! called once per frame by application
        */
        void handle();

        /*! does nothing
        */
		iNodeFactory();

        /*! checks if all nodes where released
        */
		~iNodeFactory();

	};
}

#endif

