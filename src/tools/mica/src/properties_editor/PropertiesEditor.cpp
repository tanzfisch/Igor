// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "PropertiesEditor.h"

PropertiesEditor::PropertiesEditor()
{
    onInitUI();

    iProject::getInstance().getProjectLoadedEvent().add(iProjectLoadedDelegate(this, &PropertiesEditor::onProjectLoaded));
    iProject::getInstance().getProjectUnloadedEvent().add(iProjectUnloadedDelegate(this, &PropertiesEditor::onProjectUnloaded));
    iProject::getInstance().getProjectReloadedEvent().add(iProjectReloadedDelegate(this, &PropertiesEditor::onProjectReloaded));
}

void PropertiesEditor::onInitUI()
{
    setTitle("Properties Editor");

    setDockable(true);
    setMinWidth(460);
    setHorizontalAlignment(iHorizontalAlignment::Stretch);
    setVerticalAlignment(iVerticalAlignment::Stretch);

    _scroll = new iWidgetScroll(this);
}

void PropertiesEditor::onProjectLoaded(const iaString &projectfile)
{
    auto projectScene = iProject::getInstance().getRootScene();
    projectScene->getEntitySelectionChangeEvent().add(iEntitySelectionChangeDelegate(this, &PropertiesEditor::onSelectionChanged));
}

void PropertiesEditor::onProjectUnloaded()
{
    deinitProperties();
}

void PropertiesEditor::onProjectReloaded()
{
    auto projectScene = iProject::getInstance().getRootScene();
    projectScene->getEntitySelectionChangeEvent().add(iEntitySelectionChangeDelegate(this, &PropertiesEditor::onSelectionChanged));
}

void PropertiesEditor::onSelectionChanged(const iEntitySceneID &sceneID, const std::vector<iEntityID> &entities)
{
    deinitProperties();

    if (entities.size() != 1 ||
        !sceneID.isValid() ||
        !entities[0].isValid())
    {
        return;
    }

    _userControlProperties = new UserControlProperties(UserControlProperties::PropertyType::Entity, {sceneID, entities[0]}, _scroll);
}

void PropertiesEditor::deinitProperties()
{
    if (_userControlProperties != nullptr)
    {
        _scroll->removeWidget(_userControlProperties);
        iWidgetManager::getInstance().deleteWidget(_userControlProperties);
        _userControlProperties = nullptr;
    }
}

void PropertiesEditor::setSelectionResource(const iResourceID &resourceID)
{
    deinitProperties();

    if (!resourceID.isValid())
    {
        return;
    }

    _userControlProperties = new UserControlProperties(UserControlProperties::PropertyType::Resource, {resourceID}, _scroll);
}
