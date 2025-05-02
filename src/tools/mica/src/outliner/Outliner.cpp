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
    _treeView->getSelectionChangedEvent().add(iSelectionChangedDelegate(this, &Outliner::onTreeViewSelectionChanged));
    _treeView->getContextMenuTreeViewEvent().add(iContextMenuTreeViewDelegate(this, &Outliner::onContextMenuTreeView));
    _treeView->setMultiSelection(true);

    refresh();
}

void Outliner::onTreeViewSelectionChanged(const iWidgetPtr source)
{
    if (_treeView != source)
    {
        return;
    }

    // collect selection changes for each scene
    std::unordered_map<iEntityScenePtr, std::vector<iEntityID>> selections;

    for (const auto widget : _treeView->getSelection())
    {
        iItemPath itemPath = std::any_cast<iItemPath>(widget->getUserData());
        iItemPtr item = _itemData->getItem(itemPath);
        if (item == nullptr)
        {
            continue;
        }

        if (item->hasValue(IGOR_ITEM_DATA_SCENE_ID) &&
            item->hasValue(IGOR_ITEM_DATA_ENTITY_ID))
        {
            const iEntitySceneID sceneID = item->getValue<iEntitySceneID>(IGOR_ITEM_DATA_SCENE_ID);
            auto scene = iEntitySystemModule::getInstance().getScene(sceneID);
            if (scene != nullptr)
            {
                if (selections.find(scene) == selections.end())
                {
                    selections[scene] = std::vector<iEntityID>();
                }

                selections[scene].push_back(item->getValue<iEntityID>(IGOR_ITEM_DATA_ENTITY_ID));
            }
        }
        else
        {
            con_err("unexpected item");
        }
    }

    for(const auto &selection : selections)
    {
        selection.first->setSelection(selection.second);
    }
}

void Outliner::onContextMenuTreeView(const iWidgetPtr source)
{
    if (!source->getUserData().has_value())
    {
        return;
    }

    iEntitySceneID sceneID = iProject::getInstance().getProjectScene()->getID();
    iEntityScenePtr projectScene = iEntitySystemModule::getInstance().getScene(sceneID);
    std::vector<iEntityID> entities;
    bool isRoot = false;
    bool isScene = false;

    std::vector<iItemPath> selectedItemPaths = _treeView->getSelectedItemPaths();
    const iItemPath itemPath = std::any_cast<iItemPath>(source->getUserData());
    selectedItemPaths.erase(std::remove(selectedItemPaths.begin(), selectedItemPaths.end(), itemPath), selectedItemPaths.end());
    selectedItemPaths.insert(selectedItemPaths.begin(), itemPath);

    for (const auto &selectedItemPath : selectedItemPaths)
    {
        iItemPtr item = _itemData->getItem(selectedItemPath);

        if (item->hasValue(IGOR_ITEM_DATA_ENTITY_ID))
        {
            iEntityID entityID = item->getValue<iEntityID>(IGOR_ITEM_DATA_ENTITY_ID);
            isRoot = projectScene->getRootEntity()->getID() == entityID;

            if (isRoot)
            {
                entities.clear();
                entities.push_back(entityID);
                break;
            }

            entities.push_back(entityID);
        }

        if (item->hasValue(IGOR_ITEM_DATA_UUID))
        {
            auto prefabID = item->getValue<iResourceID>(IGOR_ITEM_DATA_UUID);
            const auto &subScenes = iProject::getInstance().getScenes();
            isScene = std::find(subScenes.begin(), subScenes.end(), prefabID) != subScenes.end();
        }
    }

    bool active = true;
    bool inactive = true;

    for (const auto &entityID : entities)
    {
        iEntityPtr entity = projectScene->getEntity(entityID);
        if (entity->isActive())
        {
            active = false;
        }
        else
        {
            inactive = false;
        }
    }

    iActionContextPtr actionContext = std::make_shared<iEntityActionContext>(sceneID, entities);

    _contextMenu.clear();
    _contextMenu.setPos(iMouse::getInstance().getPos());

    if (!entities.empty())
    {
        if (isRoot)
        {
            // TODO
        }
        else
        {
            _contextMenu.addAction("igor:create_new_entity", actionContext);
            _contextMenu.addAction("igor:delete_entity", actionContext);
            _contextMenu.addSeparator();

            _contextMenu.addAction("igor:cut_entity", actionContext);
            _contextMenu.addAction("igor:copy_entity", actionContext);

            if (iClipboard::getInstance().has(iClipboardDataFormat::EntityID))
            {
                _contextMenu.addAction("igor:paste_entity", actionContext);
            }

            _contextMenu.addSeparator();
            if (inactive)
            {
                _contextMenu.addAction("igor:set_entity_inactive", actionContext);
            }
            if (active)
            {
                _contextMenu.addAction("igor:set_entity_active", actionContext);
            }

            if (isScene && entities.size() == 1)
            {
                _contextMenu.addCallback(iClickDelegate(this, &Outliner::onSavePrefab), "save prefab", "Save the prefab scene.", "", true, actionContext);
            }
        }
    }

    if (_contextMenu.hasEntries())
    {
        _contextMenu.open();
    }
}

void Outliner::onSavePrefab(const iWidgetPtr source)
{
    iWidgetButtonPtr button = static_cast<iWidgetButtonPtr>(source);
    iEntityActionContext *context = static_cast<iEntityActionContext *>(&*button->getActionContext());
    auto scene = iEntitySystemModule::getInstance().getScene(context->getSceneID());

    auto prefabEntity = scene->getEntity(context->getEntities()[0]);

    auto prefabComp = prefabEntity->getComponent<iPrefabComponent>();
    auto prefab = prefabComp->getPrefab();
    auto prefabScene = iEntitySystemModule::getInstance().getScene(prefab->getSceneID());
    prefabScene->clear();

    iEntityCopyTraverser traverser(prefabScene->getRootEntity(), true);

    for (const auto entity : prefabEntity->getChildren())
    {
        traverser.traverse(entity);
    }

    for (const auto entity : prefabEntity->getInactiveChildren())
    {
        traverser.traverse(entity);
    }

    iResourceManager::getInstance().saveResource(prefab->getID());
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
        if (child->getName().find("mica_") != iaString::INVALID_POSITION)
        {
            continue;
        }

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

    auto projectScene = project.getProjectScene();
    auto rootItem = _itemData->getItem(iItemPath(""));
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
        iaString name = mimeData.hasText() ? mimeData.getText() : "scene";

        iProject::getInstance().addScene(id, name);
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

bool Outliner::onEvent(iEvent &event)
{
    if (iWidget::onEvent(event))
    {
        return true;
    }

    event.dispatch<iEventProjectLoaded>(IGOR_BIND_EVENT_FUNCTION(Outliner::onProjectLoaded));
    event.dispatch<iEventProjectUnloaded>(IGOR_BIND_EVENT_FUNCTION(Outliner::onProjectUnloaded));

    return false;
}

void Outliner::onSelectionChanged(const iEntitySceneID &sceneID, const std::vector<iEntityID> &entities)
{
    refresh();
    // TODO do something like _treeView->setSelection instead
}

bool Outliner::onProjectLoaded(iEventProjectLoaded &event)
{
    iProject::getInstance().getProjectScene()->getEntitySelectionChangedEvent().add(iEntitySelectionChangedDelegate(this, &Outliner::onSelectionChanged));

    refresh();
    return false;
}

bool Outliner::onProjectUnloaded(iEventProjectUnloaded &event)
{
    refresh();
    return false;
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
