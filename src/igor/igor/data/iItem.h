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
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef IGOR_ITEM_H
#define IGOR_ITEM_H

#include <igor/data/iMimeData.h>

namespace igor
{

    class iItem;
    typedef iItem *iItemPtr;

    /*! \brief Item holding generic data and other items

    Useful for small hierarchies since all data is copied a lot
     */
    class IGOR_API iItem
    {
    public:
        /*! nothing to do
         */
        iItem() = default;

        /*! set value for given key

        key "name" is a reserved key. Every item has a name

        \param key to set
        \param value to set
        */
        template <typename T>
        void setValue(const iaString &key, const T &value);

        /*! \returns value for given key

        key "name" is a reserved key. Every item has a name

        \param key the given key
        */
        template <typename T>
        T getValue(const iaString &key) const;

        /*! \returns true if there is a value for given key
         */
        bool hasValue(const iaString &key) const;

        /*! \returns id of item

        it's a unique id amongst siblings
         */
        const iaString getID() const;

        /*! adds item to this item

        this item retains ownership

        \returns newly added item
        \param id if of the item (must be unique between siblings)
        */
        iItemPtr addItem(const iaString &id);

        /*! removes given item from this item

        \param item the item to remove
        */
        void removeItem(iItemPtr item);

        /*! \returns items of this item
         */
        const std::vector<iItemPtr> getItems() const;

        /*! \returns parent item. nullptr if root
         */
        iItemPtr getParent() const;

        /*! clear item and it's children
         */
        void clear();

        /*! compare two items recursively for being equal

        \returns true if equal
        \param other the other item
        */
        bool operator==(const iItem &other) const;

        /*! compare two items recursively for being not equal

        \returns true if not equal
        \param other the other item
        */
        bool operator!=(const iItem &other) const;

        /*! \returns true if this item has any data
        */
        bool hasData() const;

    private:
        /*! using mime data for implementation
         */
        iMimeData _data;

        /*! pointer to parent item
         */
        iItem *_parent = nullptr;

        /*! sub items of this item
         */
        std::vector<std::unique_ptr<iItem>> _items;
    };

#include <igor/data/iItem.inl>

}

#endif // IGOR_ITEM_H