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

#ifndef __IGOR_NODE__
#define __IGOR_NODE__

#include <igor/iDefines.h>

#include <iaux/system/iaEvent.h>
#include <iaux/data/iaString.h>
#include <iaux/math/iaMatrix.h>
#include <iaux/system/iaMutex.h>
#include <iaux/data/iaIDGenerator.h>
using namespace iaux;

#include <ostream>
#include <vector>
#include <memory>
#include <map>

namespace igor
{

    class iScene;
    class iNode;

    /*! scene pointer definition
	*/
    typedef iScene *iScenePtr;

    /*! node pointer definition
	*/
    typedef iNode *iNodePtr;

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
        iNodeAudioListener,
        iNodeAudioSource,
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
        Audio,
        Undefined
    };

    /*! transformation change event
	*/
    IGOR_EVENT_DEFINITION(iTransformationChange, iNodePtr);

    /*! node id definition
    */
    typedef iaID64 iNodeID;

    /*! base node implementation

    Works basically as a group node and has no other specific features.

    Make changes to the scene only in main thread. If you want to create nodes multithreaded you can do so but adding them is only save within the main thread
    */
    class IGOR_API iNode
    {

        friend class iNodeManager;
        friend class iScene;
        friend class iNodeVisitorUpdateTransform;
        friend class iDataUpdateQueue;

    public:
        /*! invalid node id definition
        */
        static const iNodeID INVALID_NODE_ID = IGOR_INVALID_ID;

        /*! \returns kind (group) of the node
        */
        iNodeKind getKind() const;

        /*! \returns name for given kind of node
		\param nodeType the given kind of node
		*/
        static iaString getKindName(iNodeKind kind);

        /*! \returns the type of the node
        */
        iNodeType getType() const;

        /*! \returns name for given node type
		\param nodeType the given node type
		*/
        static iaString getTypeName(iNodeType nodeType);

        /*! \returns id of the node
        */
        iNodeID getID() const;

        /*! \returns name of the node
        */
        const iaString& getName() const;

        /*! returns multiple lines of information about this node

		each entry in vector is one line of information

		\returns strings with the nodes information
        \param brief if true only one brief line of information will be returned
        */
        virtual std::vector<iaString> getInfo(bool brief = false) const;

        /*! sets name of the node

        \param name new name of the node
        */
        void setName(const iaString &name);

        /*! inserts node as child

        \param node pointer to node to be inserted
        */
        void insertNode(iNodePtr node);

        /*! inserts a node as child asynchronously

        \param node pointer to node to be inserted
        */
        void insertNodeAsync(iNodePtr node);

        /*! sets node active or inactive

        \param active the active flag
        */
        void setActive(bool active = true);

        /*! sets node active or inactive asynchronously

        \param active the active flag
        */
        void setActiveAsync(bool active = true);

        /*! removes node that was a child before

        \param node pointer to node to be removed
        */
        void removeNode(iNodePtr node);

        /*! removes node that was a child before asynchronously

        \param node pointer to node to be removed
        */
        void removeNodeAsync(iNodePtr node);

        /*! \returns true: if this node has a parent; false if it does not have a parent
        */
        bool isChild();

        /*! \returns true if given node is child of this node

        \param child the node to check if it is a child of this node
        */
        bool isChild(iNodePtr child);

        /*! \returns true: if node has children; false: if node has no children
        */
        bool hasChildren();

        /*! \returns list of active children
        */
        const std::vector<iNodePtr> &getChildren() const;

        /*! returns list of all children

        \param[out] children returned list with all children active and inactive
        */
        void getAllChildren(std::vector<iNodePtr> &children) const;

        /*! returns child by name

        attention names are not unique. It will return the first node with the correct name found

        \param name the name to search for
        \returns node with the name specified. returns zero in case of an error
        */
        iNodePtr getChild(const iaString &name);

        /*! returns child by id

        \param id the id of the child node
        \returns pointer to child object
        */
        iNodePtr getChild(iNodeID id);

        /*! \returns list of inactive children
        */
        const std::vector<iNodePtr> &getInactiveChildren() const;

        /*! \returns pointer to parent node
        */
        iNodePtr getParent();

        /*! \returns pointer to scene this node is in
        */
        iScenePtr getScene();

        /*! \returns true: if transformation flag is dirty; false: if not

        this flag is set if on the path form root node down to this node the transformations have changed,
        so it's position will get updated during next frame
        */
        bool isTransformationDirty();

        /*! sets the transformation flag dirty.

        also sets the transformation flag of all children and parents dirty

        \param dirty true: this node, all parents and children will be set dirty; false: only this node will be set non dirty
        */
        void setTransformationDirty(bool dirty = true);

        /*! gets the world transformation for any node

        this is a quite expensive routine. preferably use an other interface 
        from specialized nodes like iNodeCamera::getWorldMatrix

        \param[out] matrix world matrix of this node
        */
        void calcWorldTransformation(iaMatrixd &matrix);

        /*! \returns true: if node is active
        */
        bool isActive() const;

        /* \returns transformation change event
		*/
        iTransformationChangeEvent &getTransformationChangeEvent();

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
        iScenePtr _scene = nullptr;

        /*! pointer to parent node
        */
        iNodePtr _parent = nullptr;

        /*! list of pointers to child nodes
        */
        std::vector<iNodePtr> _children;

        /*! list of pointers to child nodes that are inactive
        */
        std::vector<iNodePtr> _inactiveChildren;

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
        void setParent(iNodePtr parent);

        /*! sets pointer to scene

        \param scene pointer to scene
        */
        void setScene(iScenePtr scene);

        /*! called by node visitor update transform

		\param[in, out] matrix current transformation matrix
		*/
        virtual void onUpdateTransform(iaMatrixd &matrix);

        /*! this is called just before setScene and gives the class the chance to unregister from the current scene if set.
        */
        virtual void onPreSetScene();

        /*! this is called just after setScene and gives the class the chance to register it self to the new scene.
        */
        virtual void onPostSetScene();

        /*! called by update dirty data queue

        to end up in that queue call setDataDirty();
        */
        virtual bool onUpdateData();

        /*! called after a node was copied

        \param nodeIDMap map with old node ids to new node ids
        */
        virtual void onPostCopyLink(std::map<iNodeID, iNodeID> &nodeIDMap);

        /*! call this when ever you change something on the node
		*/
        iTransformationChangeEvent _transformationChangeEvent;

        /*! set's node id
        */
        iNode();

        /*! copy ctor
        */
        iNode(iNodePtr node);

        /*! destroys all childdren
        */
        virtual ~iNode();

    private:
        /*! flag if there is dirty data
        */
        bool _queueToDirtyData = false;

        /*! the next node id
        */
        static iaIDGenerator64 _idGenerator;

        /*! id of this node
        */
        iNodeID _nodeID = iNode::INVALID_NODE_ID;

        /*! calculates world transformation of this node

        recursively climbing up the parents until there is no parent anymore

        \param currentNode the current node
        \param matrix the current accumulated transformation matrix
        */
        void calcWorldTransformation(iNodePtr currentNode, iaMatrixd &matrix);
    };

    /*! definition of node pointer
    */
    typedef iNodePtr iNodePtr;

    /*! stream operator

	\param stream the destination
	\param nodeType the node type to stream
	\returns the resulting stream
	*/
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iNodeType &nodeType);

    /*! stream operator

	\param stream the destination
	\param nodeKind the node kind to stream
	\returns the resulting stream
	*/
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iNodeKind &nodeKind);

}; // namespace igor

#endif // __IGOR_NODE__
