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

void Outliner::initGUI()
{
    setTitle("Outliner");
    setMinWidth(350);
    setDockable(true);
    setAcceptDrop(true);

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

    populateTree();
}

void Outliner::onClickTreeView(const iWidgetPtr source)
{
    iaString selectedItemPath = std::any_cast<iaString>(source->getUserData());
    con_endl(selectedItemPath);
}

void Outliner::populateTree()
{
    _treeView->clear();

    auto &project = iProject::getInstance();

    if (!project.isLoaded())
    {
        return;
    }

    _itemData = std::unique_ptr<iItemData>(new iItemData());
    auto scenes = project.getScenes();

    for(const auto &resourceID : scenes)
    {
        iaFile file(iResourceManager::getInstance().getFilename(resourceID));
        iItemPtr sceneItem = _itemData->addItem(file.getStem());
        sceneItem->setValue<iaString>(IGOR_ITEM_DATA_ICON, "igor_icon_scene");
        sceneItem->setValue<iResourceID>(IGOR_ITEM_DATA_UUID, resourceID);

        iPrefabPtr scene = iResourceManager::getInstance().getResource<iPrefab>(resourceID);
        if(scene == nullptr)
        {
            continue;
        }

        // TODO populate the scenes
    }

    _treeView->setItems(_itemData.get());
}

void Outliner::onEntityCreated(iEntityPtr entity)
{
    con_endl("onEntityCreated");
}

void Outliner::onEntityDestroyed(iEntityPtr entity)
{
    con_endl("onEntityDestroyed");
}

void Outliner::onDragMove(iDrag &drag, const iaVector2f &mousePos)
{
    const iMimeData &mimeData = drag.getMimeData();
    if (!mimeData.hasResourceID())
    {
        drag.reject();
        return;
    }

    iResourceID id = mimeData.getResourceID();

    const iaString resourceType = iResourceManager::getInstance().getType(id);
    if (resourceType == IGOR_RESOURCE_PREFAB)
    {
        drag.accept();
        return;
    }

    drag.reject();
}

void Outliner::onDrop(const iDrag &drag, const iaVector2f &mousePos)
{
    const iMimeData &mimeData = drag.getMimeData();
    if (!mimeData.hasResourceID())
    {
        return;
    }

    iResourceID id = mimeData.getResourceID();

    const iaString resourceType = iResourceManager::getInstance().getType(id);
    if (resourceType == IGOR_RESOURCE_PREFAB)
    {
        iProject::getInstance().addScene(id);
        populateTree();
        return;
    }
}