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

#ifndef __iOCTREE__
#define __iOCTREE__

#include <iAACube.h>
#include <iFrustum.h>

#include <memory>
#include <list>
#include <map>
#include <vector>
using namespace std;

namespace Igor
{

    class iNode;
    class iNodeVolume;

    /*! octree implementation

    \bug it does not work for bigger objects
    \todo does not merge efficiently
    */
    class iOctree
	{

        /*! represents an object within the octree
        */
        struct OctreeObject
        {
            /*! id of paranting octree node
            */
            uint32 _octreeNode;
        };

        /*! octree node of certain size

        can contain other nodes and objects
        */
        struct OctreeNode
        {
            /*! volume of octree node
            */
            iAACubef _box;

            /*! octree nodes child nodes
            */
            uint32* _children = nullptr;

            /*! the parenting octree node
            */
            uint32 _parent = 0;

            /*! list of objects within the volume of this octree node
            */
            vector<uint32> _objects;
        };

	private:

        /*! lookup table for faster split of octree node volumes
        */
        static const iaVector3f iOctree::_splitTable[8];

        /*! half of the minimum size a octree nodes volume has
        */
        float32 _halfMinResolution = 0;

        /*! minimum amount of objects in the child nodes of a node before merging them together
        */
        uint32 _objectCountMinThreashold = 0;

        /*! maximum amount of objects before splitting the parenting octree node
        */
        uint32 _objectCountMaxThreashold = 0;

        /*! the id of the next node created
        */
        uint32 _nextNodeID = 1;

        /*! lookup table for all objects within the octree
        */
        map<uint32, OctreeObject*> _objects;

        /*! lookup table for all nodes within the octree
        */
        map<uint32, OctreeNode*> _nodes;

        /*! id of the root node
        */
		uint32 _rootNode;

        /*! internal list for filtering
        */
		list<iNode*> _queryResult;

        /*! recursive function to insert a scene node to the octree

        if the right place is fount a octree object will be created the represents the scene node

        \param nodeID the current octree node in rucursion
        \param sceneNodeID the id of the scene node to bind to the octree object
        \param center center of scene node volume
        */
        void insert(uint32 nodeID, uint32 sceneNodeID, iaVector3f& center);

        /*! check if node has to be split and than split

        \param nodeID node to be checked for splitting
        */
        void trySplit(uint32 nodeID);

        /*! splits an octree node

        \param nodeID node to be split
        */
        void split(uint32 nodeID);

        /*! check if node should be merged with it's children and then merge

        \param nodeID node to be checked for merging
        */
        void tryMerge(uint32 nodeID);

        /*! merges node and it's children to one node

        \param nodeID node to be merged with it's children
        */
        void merge(uint32 nodeID);
        
        /*! recursive method to filter the octree with a frustum

        \param frustum the frustum
        \param nodeID current octree node to check for filtering
        */
        void filter(const iFrustumf& frustum, uint32 nodeID);

        void filter(const iSpheref& sphere, uint32 nodeID);

        /*! creates a node and returns the new node id

        \returns node id
        */
        uint32 createNode();

        /*! deletes a node by id

        \param nodeID node to be deleted
        */
        void deleteNode(uint32 nodeID);

        /*! creates an octree object

        \param sceneNodeID scene node to associate the octree object with
        \returns pointer to new octree object
        */
        OctreeObject* createObject(uint32 sceneNodeID);

        /*! deletes an octree object by scene node id

        \param sceneNodeID id of the corresponding scene node
        */
        void deleteObject(uint32 sceneNodeID);

        /*! recursive method the draw the octree structure

        only use for debugging!

        \param nodeID id of the current octree node
        */
        void draw(uint32 nodeID);

	public:

        /*! insert scene node to octree

        \param sceneNodeID id of the scene node
        */
		void insert(uint32 sceneNodeID);

        /*! remove scene node from octree

        \param sceneNodeID id of the scene node
        */
        void remove(uint32 sceneNodeID);

        /*! update scene node in octree

        this is called usually if the scene node changed it's position

        \param sceneNodeID id of the scene node
        */
        void update(uint32 sceneNodeID);

        /*! clears the filter results
        */
        void resetFilter();

        /*! filters everything out that is not within the frustum

        \param frustum cull frustum
        */
		void filter(const iFrustumf& frustum);

        /*! filters everything out that is not within the spehere

        \param sphere cull sphere
        */
        void filter(const iSpheref& sphere);

        /*! returns the result of filtering

        \param data in out parameter for the resulting data
        */
        void getResult(list<iNode*>& data);

        /*! creates the octree including the root node

        \param box volume of the whole octree
        \param halfMinResolution half of the minimum size a octree nodes volume has
        \param objectCountMaxThreashold maximum amount of objects before splitting the parenting octree node
        \param objectCountMinThreashold minimum amount of objects in the child nodes of a node before merging them together
        \todo it would be nice to have a octree with dynamic volume
        */
        iOctree(const iAACubef& box, float32 halfMinResolution = 1.0f, uint32 objectCountMaxThreashold = 5, uint32 objectCountMinThreashold = 2);

        /*! dtor
        \bug not implemented
        */
		virtual ~iOctree();

        /*! draws the octrees structure

        only use for debugging!
        */
        void Igor_API draw();

	};
}

#endif