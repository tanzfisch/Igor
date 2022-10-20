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

#ifndef __IGOR_QUADTREE__
#define __IGOR_QUADTREE__

#include <igor/iDefines.h>
#include <igor/data/iIntersection.h>

#include <iaux/data/iaRectangle.h>
#include <iaux/data/iaCircle.h>

#include <algorithm>
#include <memory>

namespace igor
{

    template <typename F, typename T>
    struct iQuadtreeNode;

    /*! quadtree object
     */
    template <typename F, typename T>
    struct iQuadtreeObject
    {
        iQuadtreeObject() = default;

        iQuadtreeObject(const iaCircle<F> &circle, const T &userData)
            : _circle(circle), _userData(userData)
        {
        }

        /*! the circle of this object
         */
        iaCircle<F> _circle;

        /*! parent node
         */
        std::weak_ptr<iQuadtreeNode<F, T>> _parent;

        /*! user data
         */
        T _userData;
    };

    /*! node defintion
     */
    template <typename F, typename T>
    struct iQuadtreeNode
    {
        /*! parent node
         */
        std::weak_ptr<iQuadtreeNode<F, T>> _parent;

        /*! children of node
         */
        std::shared_ptr<iQuadtreeNode<F, T>> _children[4];

        /*! node box
         */
        iaRectangle<F> _box;

        /*! quadtree objects
         */
        std::vector<std::shared_ptr<iQuadtreeObject<F, T>>> _objects;
    };

    /*! quadtree implementation
     */
    template <typename F, typename T>
    class IGOR_API_TEMPLATE iQuadtree
    {

    public:
        /*! creates the quadtree including the root node

        \param box volume of the whole quadtree
        */
        iQuadtree(const iaRectangle<F> &box, const uint32 splitThreshold = 4, const uint32 maxDepth = 16);

        /*! dtor
         */
        virtual ~iQuadtree() = default;

        /*! insert user data at given position

        \param userData the user data
        */
        void insert(const std::shared_ptr<iQuadtreeObject<F, T>> object);

        /*! remove user data

        \param userData the user data
        */
        void remove(const std::shared_ptr<iQuadtreeObject<F, T>> object);

        /*! updates the user data

        \param userData the user data to update
        */
        void update(const std::shared_ptr<iQuadtreeObject<F, T>> object, const iaVector2<F> &newPosition);

        /*! \returns root of tree
         */
        const std::shared_ptr<iQuadtreeNode<F, T>> &getRoot() const;

        /*! queries for objects within given circle

        \param circle the given circle
        \param objects the resulting found objects
        */
        void query(const iaCircle<F> &circle, std::vector<std::shared_ptr<iQuadtreeObject<F, T>>> &objects);

        /*! queries for objects within given rectangle

        \param rectangle the given rectangle
        \param objects the resulting found objects
        */
        void query(const iaRectangle<F> &rectangle, std::vector<std::shared_ptr<iQuadtreeObject<F, T>>> &objects);

        /*! clears the tree
         */
        void clear();

        /*! \returns dimensions of quadtree
         */
        const iaRectangle<F> &getRootBox() const;

    private:
        /*! root node
         */
        std::shared_ptr<iQuadtreeNode<F, T>> _root;

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
        void insertInternal(const std::shared_ptr<iQuadtreeNode<F, T>> &node, const std::shared_ptr<iQuadtreeObject<F, T>> object, uint32 &depth);

        /*! removes user data

        \param node the current node
        \param userData the user data to remove
        */
        bool removeInternal(const std::shared_ptr<iQuadtreeNode<F, T>> &node, const std::shared_ptr<iQuadtreeObject<F, T>> object);

        /*! queries for user data within given circle

        \param node the current node
        \param circle the given circle
        \param objects the resulting found user data
        */
        void queryInternal(const std::shared_ptr<iQuadtreeNode<F, T>> &node, const iaCircle<F> &circle, std::vector<std::shared_ptr<iQuadtreeObject<F, T>>> &objects);

        /*! queries for user data within given rectangle

        \param node the current node
        \param circle the given rectangle
        \param objects the resulting found user data
        */
        void queryInternal(const std::shared_ptr<iQuadtreeNode<F, T>> &node, const iaRectangle<F> &rectangle, std::vector<std::shared_ptr<iQuadtreeObject<F, T>>> &objects);

        /*! split given node

        \param node the node to split
        */
        void split(const std::shared_ptr<iQuadtreeNode<F, T>> &node);

        /*! try to merge given node

        \param node the given node
        */
        bool tryMerge(const std::shared_ptr<iQuadtreeNode<F, T>> &node);

        /*! \returns true if node has no children

        \param node the node to test
        */
        bool isLeaf(const std::shared_ptr<iQuadtreeNode<F, T>> &node) const;
    };

#include <igor/data/iQuadtree.inl>

} // namespace igor

#endif // __IGOR_QUADTREE__
