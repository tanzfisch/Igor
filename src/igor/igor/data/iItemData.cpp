// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/data/iItemData.h>

namespace igor
{

    iItemPtr iItemData::getItem(const iItemPath &itemPath)
    {
        iItemPtr current = &_root;
        iItemPtr next = nullptr;

        for(const auto &itemID : itemPath.getPath())
        {
            next = nullptr;

            for(const auto item : current->getItems())
            {
                if(item->getID() == itemID)
                {
                    next = item;
                    break;
                }
            }

            if(next == nullptr)
            {
                return nullptr;
            }

            current = next;
        }

        return current;
    }

    iItemPtr iItemData::addItem(const iaString& id)
    {
        return _root.addItem(id);
    }

    void iItemData::removeItem(iItemPtr item)
    {
        _root.removeItem(item);
    }

    const std::vector<iItemPtr> iItemData::getItems() const
    {
        return _root.getItems();
    }

    void iItemData::clear()
    {
        _root.clear();
    }

    bool iItemData::operator==(const iItemData &other) const
    {
        return this->_root == other._root;
    }

    bool iItemData::operator!=(const iItemData &other) const
    {
        return this->_root != other._root;
    }

}