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
// (c) Copyright 2014-2015 by Martin Loga
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

    \todo make thread save
    */
	class Igor_API iNodeFactory : public iaSingleton<iNodeFactory>
	{
		friend class iaSingleton<iNodeFactory>;

    public:

        /*! get node by id

        \param id id of ndoe
        \returns pointer to node
        */
        iNode* getNode(uint32 id);

        /*! creates a node

        \param nodeType determines which type of node shall be created
        \returns pointer to new node
        */
        iNode* createNode(iNodeType nodeType);

        /*! create copy of node

        \param node the node to copy
        \param recursiveDepth recursive depth
        */
        iNode* createCopy(iNode* node, uint32 recursiveDepth = UINT32_MAX);

        /*! destroys node and all its children

        \param node pointer to node to be destroyed
        */
        void destroyNode(iNode* node);

        /*! destroys node and all its children  (asynchronously)

        \param node pointer to node to be destroyed
        */
        void destroyNodeAsync(iNode* node);

        /*! destroys node and all its children

        \param nodeID id of node
        */
        void destroyNode(uint32 nodeID);

        /*! destroys node and all its children

        \param nodeID id of node (asynchronously)
        */
        void destroyNodeAsync(uint32 nodeID);

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
            
    private:

        /*! mutex to protect add queue
        */
        mutex _mutexQueueAdd;

        /*! mutex to protect remove queue
        */
        mutex _mutexQueueRemove;

        /*! mutex to protect delete queue
        */
        mutex _mutexQueueDelete;

        /*! mutex to protect node list
        */
        mutex _mutexNodes;

        /*! the queue for addin nodes to other nodes
        */
        vector<pair<uint32, uint32>> _queueAdd;

        /*! the queue to remove nodes from other nodes
        */
        vector<pair<uint32, uint32>> _queueRemove;

        /*! node IDs to be deleted
        */
        vector<uint32> _queueDelete;

        /*! mapping ids to nodes
        */
        map<uint32, iNode*> _nodes;

        /*!
        */
        iNode* createCopyInternal(iNode* node);

        /*! flushing queues
        */
        void flush();

        /*! does nothing
        */
		iNodeFactory();

        /*! checks if all nodes where released
        */
		~iNodeFactory();

	};
}

#endif

