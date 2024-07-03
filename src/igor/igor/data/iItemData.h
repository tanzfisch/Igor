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

#ifndef IGOR_ITEM_DATA_H
#define IGOR_ITEM_DATA_H

#include <igor/data/iItem.h>

namespace igor
{
    /*! container for items

    Caller needs to make sure all sibling items have unique names
     */
    class iItemData
    {
    public:
        /*! adds item to this container

        \returns newly added item
        \param name unique name of item
        */
        iItemPtr addItem(const iaString &name);

        /*! removes given item from this container

        \param item the item to remove
        */
        void removeItem(iItemPtr item);

        /*! \returns items of this container
         */
        const std::vector<iItemPtr> getItems() const;

        /*! clear all data
         */
        void clear();

        /*! \returns an item for given path. nullptr if it does not exist

        \param itemPath the given item path
        */
        iItemPtr getItem(const iaString &itemPath);

        /*! compare two item data sets for equality

        \returns true if equal
        \param other the other data set
        */
        bool operator==(const iItemData &other) const;

        /*! compare two item data sets for inequality

        \returns true if not equal
        \param other the other data set
        */
        bool operator!=(const iItemData &other) const;

    private:
        /*! root item of this container
         */
        iItem _root;
    };

}

#endif // IGOR_ITEM_DATA_H
