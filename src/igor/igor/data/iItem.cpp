// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/data/iItem.h>

namespace igor
{

    void iItem::setName(const iaString &name)
    {
        setValue<iaString>(IGOR_ITEM_DATA_NAME, name);
    }

    bool iItem::hasValue(const iaString &key) const
    {
        return _data.hasType(key);
    }

    const iaString iItem::getName() const
    {
        return getValue<iaString>(IGOR_ITEM_DATA_NAME);
    }

    bool iItem::hasData() const
    {
        return _data.hasData();
    }

    iItemPtr iItem::addItem(const iaString &name)
    {
        _items.emplace_back(std::make_unique<iItem>());
        _items.back().get()->setName(name);
        _items.back().get()->_parent = this;
        return _items.back().get();
    }

    void iItem::removeItem(iItemPtr item)
    {
        auto iter = std::find_if(_items.begin(), _items.end(), [item](const std::unique_ptr<iItem> &ptr)
                                 { return ptr.get() == item; });

        if (iter == _items.end())
        {
            con_err("item not found");
        }

        _items.erase(iter);
    }

    const std::vector<iItemPtr> iItem::getItems() const
    {
        std::vector<iItemPtr> result;
        result.reserve(_items.size());

        for (auto &item : _items)
        {
            result.push_back(item.get());
        }

        return result;
    }

    void iItem::clear()
    {
        _data.clear();
        _items.clear();
    }

    iItemPtr iItem::getParent() const
    {
        return _parent;
    }

    bool iItem::operator==(const iItem &other) const
    {
        if (_items.size() != other._items.size())
        {
            return false;
        }

        for (int i = 0; i < _items.size(); ++i)
        {
            if (*_items[i] != *other._items[i])
            {
                return false;
            }
        }

        return _data == other._data;
    }

    bool iItem::operator!=(const iItem &other) const
    {
        return !(*this == other);
    }
}