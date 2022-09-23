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
// (c) Copyright 2012-2020 by Martin Loga
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

#include <iaux/data/iaRectangle.h>
#include <iaux/data/iaCircle.h>

#include <memory>

namespace igor
{

    struct iQuadtreeNode;
    typedef std::shared_ptr<iQuadtreeNode> iQuadtreeNodePtr;

    /*! quadtree object

    \todo make it a template
     */
    struct iQuadtreeObject
    {
        /*! the circle representing this object
         */
        iaCircled _circle;

        /*! parent node
         */
        iQuadtreeNodePtr _parent;

        /*! pointer to user specific data
         */
        iUserData _userData;
    };

    /*! quadtree object pointer definition
     */
    typedef std::shared_ptr<iQuadtreeObject> iQuadtreeObjectPtr;

    /*! quadtree object list definition
     */
    typedef std::vector<iQuadtreeObjectPtr> iQuadtreeObjects;

    /*! node defintion

    \todo make it a template
     */
    struct iQuadtreeNode
    {
        /*! parent node
         */
        iQuadtreeNodePtr _parent;

        /*! children of node
         */
        iQuadtreeNodePtr _children[4];

        /*! node box
         */
        iaRectangled _box;

        /*! quadtree objects
         */
        std::vector<iQuadtreeObjectPtr> _objects;
    };

    /*! quadtree implementation

    \todo make it a template
     */
    class IGOR_API iQuadtree
    {

    public:
        /*! creates the quadtree including the root node

        \param box volume of the whole quadtree
        */
        iQuadtree(const iaRectangled &box, const uint32 splitThreshold = 4, const uint32 maxDepth = 16);

        /*! dtor
         */
        virtual ~iQuadtree();

        /*! insert user data at given position

        \param userData the user data
        */
        void insert(const iQuadtreeObjectPtr object);

        /*! remove user data

        \param userData the user data
        */
        void remove(const iQuadtreeObjectPtr object);

        /*! updates the user data

        \param userData the user data to update
        */
        void update(const iQuadtreeObjectPtr object, const iaVector2d &newPosition);

        /*! \returns root of tree
         */
        const iQuadtreeNodePtr &getRoot() const;

        /*! queries for user data within given circle

        \param circle the given circle
        \param userData the resulting found user data
        */
        void query(iaCircled circle, iQuadtreeObjects &objects);

        /*! clears the tree
         */
        void clear();

    private:
        /*! root node
         */
        iQuadtreeNodePtr _root;

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
        void insertInternal(const iQuadtreeNodePtr &node, const iQuadtreeObjectPtr userData, uint32 &depth);

        /*! removes user data

        \param node the current node
        \param userData the user data to remove
        */
        bool removeInternal(const iQuadtreeNodePtr &node, const iQuadtreeObjectPtr userData);

        /*! queries for user data within given circle

        \param node the current node
        \param circle the given circle
        \param userData the resulting found user data
        */
        void queryInternal(const iQuadtreeNodePtr &node, iaCircled circle, iQuadtreeObjects &userData);

        /*! split given node

        \param node the node to split
        */
        void split(const iQuadtreeNodePtr &node);

        /*! try to merge given node

        \param node the given node
        */
        bool tryMerge(const iQuadtreeNodePtr &node);

        /*! \returns true if node has no children

        \param node the node to test
        */
        bool isLeaf(const iQuadtreeNodePtr &node) const;
    };

    /*! Quadtree pointer definition
     */
    typedef iQuadtree *iQuadtreePtr;

} // namespace igor

#endif // __IGOR_QUADTREE__
