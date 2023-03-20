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
// (c) Copyright 2012-2023 by Martin Loga
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
#include <any>

namespace igor
{
    /*! quadtree implementation
     */
    template <typename F>
    class IGOR_API_EXPORT_ONLY iQuadtree
    {

    public:
        struct iQuadtreeNode;

        /*! quadtree object
         */
        struct iQuadtreeObject
        {
            iQuadtreeObject() = default;

            iQuadtreeObject(const iaCircle<F> &circle, const std::any &userData)
                : _circle(circle), _userData(userData)
            {
            }

            /*! the circle of this object
             */
            iaCircle<F> _circle;

            /*! parent node
             */
            std::weak_ptr<iQuadtreeNode> _parent;

            /*! user data
             */
            std::any _userData;
        };

        /*! node defintion
         */
        struct iQuadtreeNode
        {
            /*! parent node
             */
            std::weak_ptr<iQuadtreeNode> _parent;

            /*! children of node
             */
            std::shared_ptr<iQuadtreeNode> _children[4];

            /*! node box
             */
            iaRectangle<F> _box;

            /*! quadtree objects
             */
            std::vector<std::shared_ptr<iQuadtreeObject>> _objects;
        };

        /*! creates the quadtree including the root node

        \param box volume of the whole quadtree
        */
        iQuadtree(const iaRectangle<F> &box, const uint32 splitThreshold = 4, const uint32 maxDepth = 16);

        /*! dtor
         */
        virtual ~iQuadtree() = default;

        /*! insert object at given position

        \param userData the user data
        */
        void insert(const std::shared_ptr<iQuadtreeObject> object);

        /*! remove given object

        \param userData the user data
        */
        void remove(const std::shared_ptr<iQuadtreeObject> object);

        /*! updates position of given object

        \param object the object to update
        \param position the new position of the object
        */
        void update(const std::shared_ptr<iQuadtreeObject> object, const iaVector2<F> &position);

        /*! \returns root of tree
         */
        const std::shared_ptr<iQuadtreeNode> &getRoot() const;

        /*! queries for objects within given circle

        \param circle the given circle
        \param objects the resulting found objects
        */
        void query(const iaCircle<F> &circle, std::vector<std::shared_ptr<iQuadtreeObject>> &objects);

        /*! queries for objects within given rectangle

        \param rectangle the given rectangle
        \param objects the resulting found objects
        */
        void query(const iaRectangle<F> &rectangle, std::vector<std::shared_ptr<iQuadtreeObject>> &objects);

        /*! clears the tree
         */
        void clear();

        /*! \returns dimensions of quadtree
         */
        const iaRectangle<F> &getRootBox() const;

        using Object = iQuadtreeObject;
        using Node = iQuadtreeNode;
        using ObjectPtr = std::shared_ptr<iQuadtreeObject>;
        using NodePtr = std::shared_ptr<iQuadtreeNode>;
        using Objects = std::vector<ObjectPtr>;

    private:
        /*! root node
         */
        std::shared_ptr<iQuadtreeNode> _root;

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
        void insertInternal(const std::shared_ptr<iQuadtreeNode> &node, const std::shared_ptr<iQuadtreeObject> object, uint32 &depth);

        /*! removes user data

        \param node the current node
        \param userData the user data to remove
        */
        bool removeInternal(const std::shared_ptr<iQuadtreeNode> &node, const std::shared_ptr<iQuadtreeObject> object);

        /*! queries for user data within given circle

        \param node the current node
        \param circle the given circle
        \param objects the resulting found user data
        */
        void queryInternal(const std::shared_ptr<iQuadtreeNode> &node, const iaCircle<F> &circle, std::vector<std::shared_ptr<iQuadtreeObject>> &objects);

        /*! queries for user data within given rectangle

        \param node the current node
        \param circle the given rectangle
        \param objects the resulting found user data
        */
        void queryInternal(const std::shared_ptr<iQuadtreeNode> &node, const iaRectangle<F> &rectangle, std::vector<std::shared_ptr<iQuadtreeObject>> &objects);

        /*! split given node

        \param node the node to split
        */
        void split(const std::shared_ptr<iQuadtreeNode> &node);

        /*! try to merge given node

        \param node the given node
        */
        bool tryMerge(const std::shared_ptr<iQuadtreeNode> &node);

        /*! \returns true if node has no children

        \param node the node to test
        */
        bool isLeaf(const std::shared_ptr<iQuadtreeNode> &node) const;
    };

#include <igor/data/iQuadtree.inl>

    typedef iQuadtree<float32> iQuadtreef;
    typedef iQuadtree<float64> iQuadtreed;

} // namespace igor

#endif // __IGOR_QUADTREE__
