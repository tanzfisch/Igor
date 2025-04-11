// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "Outliner.h"

Outliner::Outliner()
{
    initGUI();

    iEntitySystemModule::getInstance().getCreatedEntityEvent().add(iCreatedEntityDelegate(this, &Outliner::onEntityCreated));
    iEntitySystemModule::getInstance().getDestroyEntityEvent().add(iDestroyEntityDelegate(this, &Outliner::onEntityDestroyed));
    iEntitySystemModule::getInstance().getHierarchyChangedEvent().add(iHierarchyChangedDelegate(this, &Outliner::onHierarchyChanged));
    iEntitySystemModule::getInstance().getEntityNameChangedEvent().add(iEntityNameChangedDelegate(this, &Outliner::onEntityNameChanged));
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

    if (item->hasValue(IGOR_ITEM_DATA_SCENE_ID) &&
        item->hasValue(IGOR_ITEM_DATA_ENTITY_ID))
    {
        iEntitySceneID sceneID = item->getValue<iEntitySceneID>(IGOR_ITEM_DATA_SCENE_ID);
        iEntityID entityID = item->getValue<iEntityID>(IGOR_ITEM_DATA_ENTITY_ID);
        _entitySelectionChangedEvent(sceneID, entityID);
    }
    else
    {
        con_err("unexpected item");
    }
}

void Outliner::onContextMenuTreeView(const iWidgetPtr source)
{
    if(!source->getUserData().has_value())
    {
        return;
    }

    iaString itemPath = std::any_cast<iaString>(source->getUserData());
    iItemPtr item = _itemData->getItem(itemPath);

    iEntitySceneID sceneID = iProject::getInstance().getScene()->getID();
    iEntityScenePtr scene = iEntitySystemModule::getInstance().getScene(sceneID);
    std::vector<iEntityID> entities;

    bool isRoot = false;
    iEntityPtr entity = nullptr;
    if (item->hasValue(IGOR_ITEM_DATA_ENTITY_ID))
    {
        iEntityID entityID = item->getValue<iEntityID>(IGOR_ITEM_DATA_ENTITY_ID);
        entities.push_back(entityID);
        entity = scene->getEntity(entityID);
        isRoot = scene->getRootEntity() == entity;
    }

    bool subScene = false;
    if (item->hasValue(IGOR_ITEM_DATA_UUID))
    {
        auto prefabID = item->getValue<iResourceID>(IGOR_ITEM_DATA_UUID);
        const auto &subScenes = iProject::getInstance().getSubScenes();
        subScene = std::find(subScenes.begin(), subScenes.end(), prefabID) != subScenes.end();
    }

    iActionContextPtr actionContext = std::make_shared<iEntityActionContext>(sceneID, entities);

    _contextMenu.clear();
    _contextMenu.setPos(iMouse::getInstance().getPos());

    if (entity)
    {
        if (!isRoot)
        {
            if (entity->isActive())
            {
                _contextMenu.addAction("igor:set_entities_inactive", actionContext);
            }
            else
            {
                _contextMenu.addAction("igor:set_entities_active", actionContext);
            }
        }

        if (!isRoot)
        {
            _contextMenu.addAction("igor:delete_entities", actionContext);
            _contextMenu.addAction("igor:cut_entities", actionContext);
            _contextMenu.addAction("igor:copy_entities", actionContext);
        }

        if (iClipboard::getInstance().has(iClipboardDataFormat::EntityID))
        {
            _contextMenu.addAction("igor:paste_entities", actionContext);
        }
    }

    if (subScene)
    {
    }

    if(_contextMenu.hasActions())
    {
        _contextMenu.open();
    }
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
        const auto &scenes = iProject::getInstance().getSubScenes();
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

void Outliner::populateTree(iItemPtr item, iEntityPtr entity)
{
    iEntityToItemTraverser traverser(item);

    for (auto child : entity->getChildren())
    {
        traverser.traverse(child);
    }

    for (auto child : entity->getInactiveChildren())
    {
        traverser.traverse(child);
    }
}

void Outliner::populateSubScenes(const std::vector<iEntityPtr> &children, bool active)
{
    for (const auto &child : children)
    {
        auto scene = child->getScene();
        iItemPtr item = _itemData->addItem(child->getID().toString());

        item->setValue<iaString>(IGOR_ITEM_DATA_NAME, child->getName());
        item->setValue<iResourceID>(IGOR_ITEM_DATA_ENABLED, active);
        item->setValue<iEntitySceneID>(IGOR_ITEM_DATA_SCENE_ID, scene->getID());
        item->setValue<iEntityID>(IGOR_ITEM_DATA_ENTITY_ID, child->getID());

        auto prefabComponent = child->getComponent<iPrefabComponent>();
        if (prefabComponent != nullptr &&
            prefabComponent->getPrefab() != nullptr &&
            prefabComponent->getPrefab()->isValid())
        {
            auto prefab = prefabComponent->getPrefab();
            item->setValue<iResourceID>(IGOR_ITEM_DATA_UUID, prefab->getID());
            item->setValue<iaString>(IGOR_ITEM_DATA_ICON, "igor_icon_scene");
        }

        populateTree(item, child);
    }
}

void Outliner::populateTree()
{
    _treeView->clear();
    _itemData = nullptr;

    auto &project = iProject::getInstance();
    if (!project.isLoaded())
    {
        return;
    }

    _itemData = std::unique_ptr<iItemData>(new iItemData());

    auto projectScene = project.getScene();
    auto rootItem = _itemData->getItem("");
    rootItem->setValue<iEntitySceneID>(IGOR_ITEM_DATA_SCENE_ID, projectScene->getID());
    rootItem->setValue<iaString>(IGOR_ITEM_DATA_NAME, iaString("root"));
    rootItem->setValue<iResourceID>(IGOR_ITEM_DATA_ENABLED, false);
    rootItem->setValue<iEntityID>(IGOR_ITEM_DATA_ENTITY_ID, projectScene->getRootEntity()->getID());

    populateSubScenes(projectScene->getRootEntity()->getChildren(), true);
    populateSubScenes(projectScene->getRootEntity()->getInactiveChildren(), false);

    _treeView->setItems(_itemData.get());
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

void Outliner::onEntityNameChanged(iEntityPtr entity)
{
    refresh();
}

void Outliner::onEntityCreated(iEntityPtr entity)
{
    refresh();
}

void Outliner::onEntityDestroyed(iEntityPtr entity)
{
    refresh();
}

void Outliner::onHierarchyChanged(iEntityScenePtr scene)
{
    refresh();
}

EntitySelectionChangedEvent &Outliner::getEntitySelectionChangedEvent()
{
    return _entitySelectionChangedEvent;
}