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

#include <memory>

namespace igor
{

    /*! quadtree implementation
     */
    class IGOR_API iQuadtree
    {

    public:
        /*! creates the quadtree including the root node

        \param box volume of the whole quadtree
        */
        iQuadtree(const iRectangled &box);

        /*! dtor
         */
        virtual ~iQuadtree();

        /*! insert user data at given position

        \param userData the user data
        \param pos the given position
        */
        void insert(iUserData userData, const iaVector2d &pos);

        /*! remove user data

        \param userData the user data
        */
        void remove(iUserData userData);

        /*! update user data position

        \param userData pointer to user data
        \param pos the new position
        */
        // void update(iUserData userData, const iaVector2d &pos);

    private:
        struct UserDataSet
        {
            UserDataSet(const iaVector2d &pos, iUserData userData)
                : _pos(pos), _userData(userData) {}

            iaVector2d _pos;
            iUserData _userData;
        };

        /*! node defintion
         */
        struct Node
        {
            /*! node box
             */
            iRectangled _box;

            /*! children of node
             */
            std::unique_ptr<Node> _children[4];

            /*! user data
             */
            std::vector<UserDataSet> _userData;
        };

        /*! root node
         */
        std::unique_ptr<Node> _root;

        /*! recursive insert new data implementation

        \param node the current node
        \param userData the user data
        \param pos the given position
        */
        void insertInternal(const std::unique_ptr<Node> &node, iUserData userData, const iaVector2d &pos);

        /*! split given node

        \param node the node to split
        */
        void split(const std::unique_ptr<Node> &node);
    };

    /*! Quadtree pointer definition
     */
    typedef iQuadtree *iQuadtreePtr;

} // namespace igor

#endif // __IGOR_QUADTREE__
