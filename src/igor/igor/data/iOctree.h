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

#ifndef IGOR_OCTREE_H
#define IGOR_OCTREE_H

#include <igor/iDefines.h>
#include <igor/data/iIntersection.h>

#include <algorithm>
#include <memory>
#include <any>

namespace igor
{
    /*! octree implementation
     */
    template <typename F>
    class IGOR_API_EXPORT_ONLY iOctree
    {

    public:
        struct iOctreeNode;

        /*! octree object
         */
        struct iOctreeObject
        {
            iOctreeObject() = default;

            iOctreeObject(const iaSphere<F> &sphere, const std::any &userData)
                : _sphere(sphere), _userData(userData)
            {
            }

            /*! the sphere of this object
             */
            iaSphere<F> _sphere;

            /*! parent node
             */
            std::weak_ptr<iOctreeNode> _parent;

            /*! user data
             */
            std::any _userData;
        };

        /*! node defintion
         */
        struct iOctreeNode
        {
            /*! parent node
             */
            std::weak_ptr<iOctreeNode> _parent;

            /*! children of node
             */
            std::shared_ptr<iOctreeNode> _children[8];

            /*! node cube
             */
            iAACube<F> _cube;

            /*! octree objects
             */
            std::vector<std::shared_ptr<iOctreeObject>> _objects;
        };

        /*! creates the octree including the root node

        \todo why do we have a max depth????

        \param maxVolume volume of the whole octree
        \param splitThreshold threshold count of objects on a node before splitting the node
        \param maxDepth the maximum depth of the tree
        */
        iOctree(const iAACube<F> &maxVolume, const uint32 splitThreshold = 8, const uint32 maxDepth = 16);

        /*! dtor
         */
        virtual ~iOctree() = default;

        /*! insert object at given position

        \param userData the user data
        */
        void insert(const std::shared_ptr<iOctreeObject> object);

        /*! remove given object

        \param userData the user data
        */
        void remove(const std::shared_ptr<iOctreeObject> object);

        /*! updates position of given object

        \param object the object to update
        \param position the new position of the object
        */
        void update(const std::shared_ptr<iOctreeObject> object, const iaVector3<F> &position);

        /*! updates position and radius of given object

        \param object the object to update
        \param sphere the new position and radius of the object
        */
        void update(const std::shared_ptr<iOctreeObject> object, const iaSphere<F> &sphere);

        /*! \returns root of tree
         */
        const std::shared_ptr<iOctreeNode> &getRoot() const;

        /*! queries for objects within given sphere

        \param sphere the given sphere
        \param objects the resulting found objects
        */
        void query(const iaSphere<F> &sphere, std::vector<std::shared_ptr<iOctreeObject>> &objects);

        /*! queries for objects within given cube

        \param cube the given cube
        \param objects the resulting found objects
        */
        void query(const iAACube<F> &cube, std::vector<std::shared_ptr<iOctreeObject>> &objects);

        /*! queries for objects within given frustum

        \param frustum the given frustum
        \param objects the resulting found objects
        */
        void query(const iFrustumd &frustum, std::vector<std::shared_ptr<iOctreeObject>> &objects);

        /*! clears the tree
         */
        void clear();

        /*! \returns dimensions of octree
         */
        const iAACube<F> &getVolume() const;

        /*! \returns split threshold
        */
        uint32 getSplitThreshold() const;

        /*! \returns configured max depth of tree
        */
        uint32 getMaxDepth() const;

        using Object = iOctreeObject;
        using Node = iOctreeNode;
        using ObjectPtr = std::shared_ptr<iOctreeObject>;
        using NodePtr = std::shared_ptr<iOctreeNode>;
        using Objects = std::vector<ObjectPtr>;

    private:
        /*! root node
         */
        std::shared_ptr<iOctreeNode> _root;

        /*! max number of objects before splitting node
         */
        const uint32 _splitThreshold;

        /*! max depth of tree
         */
        const uint32 _maxDepth;

        /*! recursive insert new data implementation

        \param node the current node
        \param userData the user data
        \param depth variable to measure depth
        */
        void insertInternal(const std::shared_ptr<iOctreeNode> &node, const std::shared_ptr<iOctreeObject> object, uint32 &depth);

        /*! removes user data

        \param node the current node
        \param userData the user data to remove
        */
        bool removeInternal(const std::shared_ptr<iOctreeNode> &node, const std::shared_ptr<iOctreeObject> object);

        /*! queries for user data within given sphere

        \param node the current node
        \param sphere the given sphere
        \param objects the resulting found user data
        */
        void queryInternal(const std::shared_ptr<iOctreeNode> &node, const iaSphere<F> &sphere, std::vector<std::shared_ptr<iOctreeObject>> &objects);

        /*! queries for user data within given cube

        \param node the current node
        \param cube the given cube
        \param objects the resulting found user data
        */
        void queryInternal(const std::shared_ptr<iOctreeNode> &node, const iAACube<F> &cube, std::vector<std::shared_ptr<iOctreeObject>> &objects);

        /*! queries for user data within given frustum

        \param node the current node
        \param frustum the given frustum
        \param objects the resulting found user data
        */
        void queryInternal(const std::shared_ptr<iOctreeNode> &node, const iFrustumd &frustum, std::vector<std::shared_ptr<iOctreeObject>> &objects);

        /*! split given node

        \param node the node to split
        */
        void split(const std::shared_ptr<iOctreeNode> &node);

        /*! try to merge given node

        \param node the given node
        */
        bool tryMerge(const std::shared_ptr<iOctreeNode> &node);

        /*! \returns true if node has no children

        \param node the node to test
        */
        bool isLeaf(const std::shared_ptr<iOctreeNode> &node) const;
    };

#include <igor/data/iOctree.inl>

    typedef iOctree<float32> iOctreef;
    typedef iOctree<float64> iOctreed;

} // namespace igor

#endif // IGOR_OCTREE_H