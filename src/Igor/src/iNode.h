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

#ifndef __iNODE__
#define __iNODE__

#include <iDataUpdateQueue.h>
#include <iDefines.h>

#include <iaString.h>
#include <iaMatrix.h>
using namespace IgorAux;

#include <ostream>
#include <vector>
#include <memory>
#include <mutex>
#include <map>
using namespace std;

namespace Igor
{
	
    /*! types of nodes
    */
	enum class iNodeType : unsigned int
	{
		iNode,
		iNodeCamera,
		iCelestialNode,
		iNodeLight,
		iNodeMesh,
		iNodeModel,
		iNodeRender,
		iNodeSkyBox,
		iSkyLightNode,
		iNodeTransform,
        iNodeSwitch,
        iNodeLODSwitch,
        iNodeLODTrigger,
        iNodePhysics,
        iNodeParticleSystem,
        iNodeEmitter,
        iNodeWater,
		Undefined
	};

    /*! kinds of nodes

    multiple types of nodes can represent the same kind of node
    */
	enum class iNodeKind : unsigned int
	{
		Node,
		Renderable,
		Volume,
        Physics,
		Light,
		Camera,
		Transformation,
		Undefined
	};

	class iScene;

    /*! base node implementation

    Works basically as a group node and has no other specific features.

    Make changes to the scene only in main thread. If you want to create nodes multithreaded you can do so but adding them is only save within the main thread

    \todo we should not use pointers but IDs of nodes to reference them! all the time?
    */
	class Igor_API iNode
	{

		friend class iNodeFactory;
		friend class iScene;
        friend class iNodeVisitorUpdateTransform;
        friend class iDataUpdateQueue;

    public:

        /*! invalid node id definition
        */
        static const uint64 INVALID_NODE_ID = 0;

        /*! \returns kind (group) of the node
        */
        iNodeKind getKind() const;

        /*! \returns the type of the node
        */
        iNodeType getType() const;

        /*! \returns id of the node
        */
        uint64 getID() const;

        /*! \returns name of the node
        */
        iaString getName() const;

        /*! sets name of the node

        \param name new name of the node
        */
        void setName(iaString name);

        /*! inserts node as child

        \param node pointer to node to be inserted
        */
        void insertNode(iNode* node);

        /*! inserts a node as child asynchronosly

        \param node pointer to node to be inserted
        */
        void insertNodeAsync(iNode* node);

        /*! sets node active or inactive

        \param active the active flag
        */
        void setActive(bool active = true);

        /*! sets node active or inactive asynchronosly

        \param active the active flag
        */
        void setActiveAsync(bool active = true);

        /*! removes node that was a child before

        \param node pointer to node to be removed
        */
        void removeNode(iNode* node);

        /*! removes node that was a child before asynchronosly

        \param node pointer to node to be removed
        */
        void removeNodeAsync(iNode* node);

        /*! \returns true: if this node has a parent; false if it does not have a parent
        */
        bool isChild();

        /*! \returns true if node is child of node

        \param child the node to check if it is a child of this node
        */
        bool isChild(iNode* child);

        /*! \returns true: if node has children; false: if node has no children
        */
        bool hasChildren();

        /*! \returns list of children
        */
        vector<iNode*>& getChildren();

        /*! returns child by name

        attention names are not unique. It will return the first node with the correct name found

        \param name the name to search for
        \returns node with the name specified. returns zero in case of an error
        */
        iNode* getChild(iaString name);

        /*! returns child by id

        \param id the id of the child node
        \returns pointer to child object
        */
        iNode* getChild(uint64 id);

        /*! \returns list of inactive children
        */
        vector<iNode*>& getInactiveChildren();

        /*! \returns pointer to parent node
        */
        iNode* getParent();

        /*! \returns pointer to scene this node is in
        */
        iScene* getScene();

        /*! \returns true: if transformation flag is dirty; false: if not

        this flag is set if on the path form root node down to this node the transformations have changed,
        so it's position will get updated during next frame
        */
        bool isTransformationDirty();

        /*! sets the transformation flag dirty.

        also sets the transformation flag of all children and parents dirty

        \bug sometimes parents don't get dirty

        \param dirty true: this node, all parents and children will be set dirty; false: only this node will be set non dirty
        */
        void setTransformationDirty(bool dirty = true);

        /*! gets the world transformation for any node

        quite expensive. see if there is an other interface from specialized nodes like iNodeCamera::getWorldMatrix

        \param[out] returns world matrix of this node
        */
        void calcWorldTransformation(iaMatrixd& matrix);

        /*! \returns true: if node is active
        */
        bool isActive() const;

	protected:

        /*! true: node is active; false: node will be ignored for most operations
        */
        bool _active = true;

        /*! the type of this node
        */
        iNodeType _nodeType = iNodeType::iNode;

        /*! the kind of this node
        */
        iNodeKind _nodeKind = iNodeKind::Node;

        /*! the name of this node
        */
		iaString _name = L"iNode";

        /*! dirty flag to detect transformation changes
        */
		bool _transformationDirty = true;

        /*! pointer to parenting scene
        */
		iScene* _scene = nullptr;

        /*! pointer to parent node
        */
		iNode* _parent = nullptr;

        /*! list of pointers to child nodes
        */
		vector<iNode*> _children;

        /*! list of pointers to child nodes that are inactive
        */
        vector<iNode*> _inactiveChildren;

        /*! traveses child trees ans sets the transformation dirty flag
        */
		void setTransformationDirtyDown();

        /*! goes through all parent nodes and sets the transformation dirty flag
        */
        void setTransformationDirtyUp();

        /*! sets data dirty and puts node in update data queue
        */
        void setDataDirty();

        /*! \returns true if data is dirty and top be queued
        */
        bool isDataDirty();

        /*! sets the parent of this node

        \param parent pointer to parent node
        */
		void setParent(iNode* parent);

        /*! sets pointer to scene

        \param scene pointer to scene
        */
		void setScene(iScene* scene);

        /*! this is called just before setScene and gives the class the chance to unregister from the current scene if set.
        */
		virtual void onPreSetScene();

        /*! this is called just after setScene and gives the class the chance to register it self to the new scene.
        */
		virtual void onPostSetScene();

        /*! called by node visitor update transform

        \param[in, out] matrix current transformation matrix
        */
        virtual void onUpdateTransform(iaMatrixd& matrix);

        /*! called by update dirty data queue

        to end up in that queue call setDataDirty();
        */
        virtual bool onUpdateData();

        /*! called after a node was copied

        \param nodeIDMap map with old node ids to new node ids
        */
        virtual void onPostCopyLink(map<uint64, uint64>& nodeIDMap);

        /*! set's node id
        */
		iNode();

        /*! copy ctor
        */
        iNode(iNode* node);

        /*! destroys all childdren
        */
		virtual ~iNode();

    private:

        /*! flag if there is dirty data
        */
        bool _queueToDirtyData = false;

        /*! mutex to save unique id generation
        */
        static mutex _mutexID;

        /*! the next node id
        */
        static uint64 _nextID;

        /*! id of this node
        */
        uint64 _nodeID = iNode::INVALID_NODE_ID;

        /*! calculates world transformation of this node

        recursively climbing up the parents until there is no parent anymore

        \param currentNode the current node
        \param matrix the current accumulated transformation matrix
        */
        void calcWorldTransformation(iNode* currentNode, iaMatrixd& matrix);

	};

	/*! stream operator

	\param stream the destination
	\param nodeType the node type to stream
	\returns the resulting stream
	*/
    Igor_API wostream& operator<<(wostream& stream, const iNodeType& nodeType);

	/*! stream operator

	\param stream the destination
	\param nodeKind the node kind to stream
	\returns the resulting stream
	*/
    Igor_API wostream& operator<<(wostream& stream, const iNodeKind& nodeKind);

};

#endif
