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
// (c) Copyright 2012-2022 by Martin Loga
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

#ifndef __IGOR_OCTREE_H__
#define __IGOR_OCTREE_H__

#include <igor/data/iAACube.h>
#include <igor/data/iFrustum.h>

#include <iaux/data/iaSphere.h>

#include <memory>
#include <list>
#include <unordered_map>
#include <vector>

namespace igor
{

    /*! octree implementation

    \bug it does not work for bigger objects
    \todo does not merge efficiently
    \todo implement move functionality
    */
    class IGOR_API iOctree
    {

    public:
        /*! insert user data to octree

        \param userData pointer to user data
        */
        void insert(void* userData, const iSphered &sphere);

        /*! remove user data from octree

        \param userData pointer to user data
        */
        void remove(void* userData);

        /*! update user data in octree

        this is called usually if the user data changed it's position

        \param userData pointer to user data
        */
        void update(void* userData, const iSphered &sphere);

        /*! adds frustum to filter set

        \param frustum the frustum
        */
        void addFilter(const iFrustumd &frustum);

        /*! adds plane to filter set

        \param plane the plane to filter with
        */
        void addFilter(const iPlaned &plane);

        /*! adds sphere to filter set

        \param sphere the sphere to filter with
        */
        void addFilter(const iSphered &sphere);

        /*! clears filter set
         */
        void clearFilter();

        /*! filters the octree using the current filter set

        use getResult to get the result
        */
        void filter();

        /*! filters the octree with given frustum

        use getResult to get the result

        \param frustum the given frustum
        */
        void filter(const iFrustumd &frustum);

        /*! returns the result of filtering

        \returns the filtered user data
        */
        const std::vector<void*> &getResult() const;

        /*! creates the octree including the root node

        \param box volume of the whole octree
        \param halfMinResolution half of the minimum size a octree nodes volume has
        \param objectCountMaxThreashold maximum amount of objects before splitting the parenting octree node
        \param objectCountMinThreashold minimum amount of objects in the child nodes of a node before merging them together
        \todo it would be nice to have a octree with dynamic volume
        */
        iOctree(const iAACubed &box, float64 halfMinResolution = 1.0, uint64 objectCountMaxThreashold = 8, uint64 objectCountMinThreashold = 2);

        /*! dtor
        \bug not implemented
        */
        virtual ~iOctree();

        /*! draws the octrees structure

        only use for debugging!
        */
        void draw();

    private:
        /*! represents an object within the octree
         */
        struct OctreeObject
        {
            /*! id of paranting octree node
             */
            uint64 _octreeNode;

            /*! sphere of object
             */
            iSphered _sphere;
        };

        /*! octree node of certain size

        can contain other nodes and objects
        */
        struct OctreeNode
        {
            /*! volume of octree node
             */
            iAACubed _box;

            /*! octree nodes child nodes
             */
            uint64 *_children = nullptr;

            /*! the parenting octree node
             */
            uint64 _parent = 0;

            /*! list of user data
             */
            std::vector<void*> _objects;
        };

        /*! recursive method to filter the octree with a set of filters starting with specified node id

        \param nodeID current octree node to check for filtering
        */
        void filter(uint64 nodeID);

        /*! specialized version of filter function only filtering for given frustum

        \param nodeID current octree node to check for filtering
        \param frustum the given frustum
        */
        void filter(uint64 nodeID, const iFrustumd &frustum);

        /*! lookup table for faster split of octree node volumes
         */
        static const iaVector3d _splitTable[8];

        /*! half of the minimum size a octree nodes volume has
         */
        float64 _halfMinResolution = 0;

        /*! minimum amount of objects in the child nodes of a node before merging them together
         */
        uint64 _objectCountMinThreashold = 0;

        /*! maximum amount of objects before splitting the parenting octree node
         */
        uint64 _objectCountMaxThreashold = 0;

        /*! the id of the next node created
         */
        uint64 _nextNodeID = 1;

        /*! lookup table for all objects within the octree
         */
        std::unordered_map<void*, OctreeObject *> _objects;

        /*! lookup table for all nodes within the octree
         */
        std::unordered_map<uint64, OctreeNode *> _nodes;

        /*! id of the root node
         */
        uint64 _rootNode;

        /*! internal list for filtering
         */
        std::vector<void*> _queryResult;

        /*! spheres filter list
         */
        std::vector<iSphered> _spheresFilter;

        /*! planes filter list
         */
        std::vector<iPlaned> _planesFilter;

        /*! frustum filter list
         */
        std::vector<iFrustumd> _frustumFilter;

        /*! recursive function to insert a scene node to the octree

        if the right place is fount a octree object will be created the represents the scene node

        \param nodeID the current octree node in rucursion
        \param userData the id of the scene node to bind to the octree object
        \param position position of scene node volume
        */
        void insert(uint64 nodeID, void* userData, const iSphered &sphere);

        /*! check if node has to be split and than split

        \param nodeID node to be checked for splitting
        */
        void trySplit(uint64 nodeID);

        /*! splits an octree node

        \param nodeID node to be split
        */
        void split(uint64 nodeID);

        /*! check if node should be merged with it's children and then merge

        \param nodeID node to be checked for merging
        */
        void tryMerge(uint64 nodeID);

        /*! merges node and it's children to one node

        \param nodeID node to be merged with it's children
        */
        void merge(uint64 nodeID);

        /*! text sphere against filter

        \param sphere the sphere to filter
        \returns true if inside filter definition
        */
        bool testFilter(const iSphered &sphere);

        /*! text cube against filter

        \param cube the cube to filter
        \returns true if inside filter definition
        */
        bool testFilter(const iAACubed &cube);

        /*! creates a node and returns the new node id

        \returns node id
        */
        uint64 createNode();

        /*! deletes a node by id

        \param nodeID node to be deleted
        */
        void deleteNode(uint64 nodeID);

        /*! creates an octree object

        \param userData scene node to associate the octree object with
        \returns pointer to new octree object
        */
        OctreeObject *createObject(void* userData, const iSphered &sphere);

        /*! deletes an octree object by scene node id

        \param userData id of the corresponding scene node
        */
        void deleteObject(void* userData);

        /*! recursive method to draw the octree structure

        only use for debugging!

        \param nodeID id of the current octree node
        */
        float32 draw(uint64 nodeID);
    };

    /*! octree pointer definition
     */
    typedef iOctree *iOctreePtr;

} // namespace igor

#endif // __IGOR_OCTREE_H__
