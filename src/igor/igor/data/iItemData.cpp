// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/data/iItemData.h>

namespace igor
{

    iItemPtr iItemData::getRoot()
    {
        return &_root;
    }

    iItemPtr iItemData::getItem(const iaString &itemPath)
    {
        std::vector<iaString> paths;
        itemPath.split('/', paths);

        iItemPtr current = &_root;
        iItemPtr next = nullptr;

        for(const auto &path : paths)
        {
            next = nullptr;

            for(const auto item : current->getItems())
            {
                if(item->getName() == path)
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

    iItemPtr iItemData::addItem(const iaString& name)
    {
        return _root.addItem(name);
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