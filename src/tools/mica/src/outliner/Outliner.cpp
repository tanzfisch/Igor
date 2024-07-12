// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include "Outliner.h"

Outliner::Outliner()
{
    initGUI();

    iEntitySystemModule::getInstance().getCreatedEntityEvent().add(iCreatedEntityDelegate(this, &Outliner::onEntityCreated));
    iEntitySystemModule::getInstance().getDestroyEntityEvent().add(iDestroyEntityDelegate(this, &Outliner::onEntityDestroyed));
}

void Outliner::setScene(const iEntitySceneID &entitySceneID)
{
    if(_entitySceneID == entitySceneID)
    {
        return;        
    }

    _treeView->clear();

    _entitySceneID = entitySceneID;

    populateTree();
}

void Outliner::initGUI()
{
    setDockable(true);
    setTitle("Outliner");
    setMinWidth(350);

    auto layout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);
    layout->setVerticalAlignment(iVerticalAlignment::Stretch);
    layout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    layout->setStretchIndex(1);

    auto buttonLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, layout);
    // add menus and action buttons 

    _treeView = new iUserControlTreeView(layout);
    _treeView->setMinWidth(150);
    _treeView->setVerticalAlignment(iVerticalAlignment::Stretch);
    _treeView->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _treeView->getClickEvent().add(iClickDelegate(this, &Outliner::onClickTreeView));
}

void Outliner::onClickTreeView(const iWidgetPtr source)
{
    iItemPtr item = std::any_cast<iItemPtr>(source->getUserData());
    // TODO
}

void Outliner::populateScene(iEntityScenePtr scene, iItemPtr sceneItem)
{

}

void Outliner::populateTree()
{
    _itemData = std::unique_ptr<iItemData>(new iItemData());

    for(const auto &scene : iEntitySystemModule::getInstance().getActiveScenes())
    {
        iItemPtr sceneItem = _itemData->addItem(scene->getName());
        sceneItem->setValue<iaString>(IGOR_ITEM_DATA_ICON, "igor_icon_scene");
    }

    _treeView->setItems(_itemData.get());
}

void Outliner::onEntityCreated(iEntityPtr scene)
{
    con_endl("onEntityCreated");
}

void Outliner::onEntityDestroyed(iEntityPtr scene)
{
    con_endl("onEntityDestroyed");
}