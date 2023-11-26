// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/data/iItem.h>

namespace igor
{

    bool iItem::hasValue(const iaString &key) const
    {
        return _data.hasType(key);
    }

    iItemPtr iItem::addItem()
    {
        _items.emplace_back(std::make_unique<iItem>());
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

        for(auto &item : _items)
        {
            result.push_back(item.get());
        }

        return result;
    }

}