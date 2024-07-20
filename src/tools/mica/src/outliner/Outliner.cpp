// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include "Outliner.h"

Outliner::Outliner()
{
    initGUI();

    iEntitySystemModule::getInstance().getCreatedEntityEvent().add(iCreatedEntityDelegate(this, &Outliner::onEntityCreated));
    iEntitySystemModule::getInstance().getDestroyEntityEvent().add(iDestroyEntityDelegate(this, &Outliner::onEntityDestroyed));
    iProject::getInstance().getProjectSceneAddedEvent().add(iProjectSceneAddedDelegate(this, &Outliner::onSceneAdded));
    iProject::getInstance().getProjectSceneRemovedEvent().add(iProjectSceneRemovedDelegate(this, &Outliner::onSceneRemoved));
    iProject::getInstance().getProjectLoadedEvent().add(iProjectLoadedDelegate(this, &Outliner::onProjectLoaded));
    iProject::getInstance().getProjectUnloadedEvent().add(iProjectUnloadedDelegate(this, &Outliner::onProjectUnloaded));

    iResourceManager::getInstance().getResourceProcessedEvent().add(iResourceProcessedDelegate(this, &Outliner::onResourceLoaded), false, true);
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
    _treeView->getContextMenuTreeViewEvent().add(iContextMenuTreeViewDelegate(this, &Outliner::onContextMenuTreeView));

    refresh();
}

void Outliner::onClickTreeView(const iWidgetPtr source)
{
    iaString itemPath = std::any_cast<iaString>(source->getUserData());
    iItemPtr item = _itemData->getItem(itemPath);

    if (item->hasValue(IGOR_ITEM_DATA_SCENE_ID))
    {
        iEntitySceneID sceneID = item->getValue<iEntitySceneID>(IGOR_ITEM_DATA_SCENE_ID);
        if (item->hasValue(IGOR_ITEM_DATA_ENTITY_ID))
        {
            iEntityID entityID = item->getValue<iEntityID>(IGOR_ITEM_DATA_ENTITY_ID);
            _entitySelectionChangedEvent(sceneID, entityID);
        }
        else
        {
            _entitySelectionChangedEvent(sceneID, iEntityID::getInvalid());
        }
    }
    else if (item->hasValue(IGOR_ITEM_DATA_UUID))
    {
        iResourceID resourceID = item->getValue<iResourceID>(IGOR_ITEM_DATA_UUID);
        iPrefabPtr scene = iResourceManager::getInstance().getResource<iPrefab>(resourceID);
        if (scene != nullptr)
        {
            con_endl("unloaded scene");
        }
    }
}

void Outliner::onContextMenuTreeView(const iWidgetPtr source)
{
    iaString itemPath = std::any_cast<iaString>(source->getUserData());
    iItemPtr item = _itemData->getItem(itemPath);

    _contextMenu.clear();
    _contextMenu.setPos(iMouse::getInstance().getPos());

    if (item->hasValue(IGOR_ITEM_DATA_SCENE_ID))
    {
        iEntitySceneID sceneID = item->getValue<iEntitySceneID>(IGOR_ITEM_DATA_SCENE_ID);
        if (item->hasValue(IGOR_ITEM_DATA_ENTITY_ID))
        {
            iEntityID entityID = item->getValue<iEntityID>(IGOR_ITEM_DATA_ENTITY_ID);
            _entitySelectionChangedEvent(sceneID, entityID);
        }
        else
        {
            _entitySelectionChangedEvent(sceneID, iEntityID::getInvalid());
        }
    }
    else if (item->hasValue(IGOR_ITEM_DATA_UUID))
    {
        iResourceID resourceID = item->getValue<iResourceID>(IGOR_ITEM_DATA_UUID);
        iPrefabPtr scene = iResourceManager::getInstance().getResource<iPrefab>(resourceID);

        if (scene != nullptr)
        {
            _contextMenu.addCallback(iClickDelegate(this, &Outliner::onUnloadScene), "Unload Scene", "Unloads the scene", "");
        }
        else
        {
            _contextMenu.addCallback(iClickDelegate(this, &Outliner::onLoadScene), "Load Scene", "Loads the scene", "");
        }
    }

    _contextMenu.open();
}

void Outliner::onLoadScene(const iWidgetPtr source)
{
    iResourceManager::getInstance().requestResource<iPrefab>(_contextResourceID);
}

void Outliner::onResourceLoaded(iResourceID resourceID)
{
    const iaString resourceType = iResourceManager::getInstance().getType(resourceID);
    if (resourceType == IGOR_RESOURCE_PREFAB)
    {
        const auto &scenes = iProject::getInstance().getScenes();
        if (std::find(scenes.begin(), scenes.end(), resourceID) != scenes.end())
        {
            refresh();
        }
    }
}

void Outliner::onRefresh()
{
    populateTree();
}

void Outliner::onUnloadScene(const iWidgetPtr source)
{
    // TODO unload resource
}

void Outliner::populateTree(iItemPtr item, iEntityScenePtr scene)
{
    iEntityToItemTraverser traverser(item);
    traverser.traverse(scene);
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

    for (const auto &resourceID : scenes)
    {
        iaFile file(iResourceManager::getInstance().getFilename(resourceID));
        iItemPtr item = _itemData->addItem(file.getStem());
        item->setValue<iaString>(IGOR_ITEM_DATA_ICON, "igor_icon_scene");
        item->setValue<iResourceID>(IGOR_ITEM_DATA_UUID, resourceID);

        // check if it is loaded
        iPrefabPtr prefab = iResourceManager::getInstance().getResource<iPrefab>(resourceID);
        if (prefab == nullptr)
        {
            continue;
        }

        iEntityScenePtr scene = iEntitySystemModule::getInstance().getScene(prefab->getSceneID());
        if (scene == nullptr)
        {
            continue;
        }

        item->setValue<iEntitySceneID>(IGOR_ITEM_DATA_SCENE_ID, prefab->getSceneID());

        populateTree(item, scene);
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
        refresh();
        return;
    }
}

void Outliner::onSceneAdded(const iResourceID &sceneID)
{
    refresh();
}

void Outliner::onSceneRemoved(const iResourceID &sceneID)
{
    refresh();
}

void Outliner::onProjectLoaded()
{
    refresh();
}

void Outliner::onProjectUnloaded()
{
    refresh();
}

EntitySelectionChangedEvent &Outliner::getEntitySelectionChangedEvent()
{
    return _entitySelectionChangedEvent;
}