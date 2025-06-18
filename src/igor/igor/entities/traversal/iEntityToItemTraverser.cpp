// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/traversal/iEntityToItemTraverser.h>

namespace igor
{

    iEntityToItemTraverser::iEntityToItemTraverser(iItemPtr item)
        : _startItem(item)
    {
        setIgnoreInactive(false);
    }

    void iEntityToItemTraverser::preTraverse()
    {
        _currentItem = _startItem;
    }

    bool iEntityToItemTraverser::preOrderVisit(iEntityPtr entity)
    {
        _itemStack.push_back(_currentItem);

        auto item = _currentItem->addItem(entity->getID().toString());
        item->setValue<iaString>(IGOR_ITEM_DATA_NAME, entity->getName());
        item->setValue<iEntityID>(IGOR_ITEM_DATA_ENTITY_ID, entity->getID());
        item->setValue<iEntitySceneID>(IGOR_ITEM_DATA_SCENE_ID, entity->getScene()->getID());

        _currentItem = item;
        return true;
    }

    void iEntityToItemTraverser::postOrderVisit(iEntityPtr entity)
    {
        _currentItem = _itemStack.back();
        _itemStack.pop_back();
    }

    void iEntityToItemTraverser::postTraverse()
    {
        con_assert(_itemStack.size() == 0, "item stack should be empty");
    }

} // namespace igor