// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "PropertiesEditor.h"

PropertiesEditor::PropertiesEditor()
{
    initGUI();
}

void PropertiesEditor::initGUI()
{
    setTitle("Properties Editor");

    setDockable(true);
    setMinWidth(450);
    setHorizontalAlignment(iHorizontalAlignment::Stretch);
    setVerticalAlignment(iVerticalAlignment::Stretch);

    _scroll = new iWidgetScroll(this);
}

bool PropertiesEditor::onEvent(iEvent &event)
{
    iWidget::onEvent(event);

    event.dispatch<iEventProjectLoaded>(IGOR_BIND_EVENT_FUNCTION(PropertiesEditor::onProjectLoaded));
    event.dispatch<iEventProjectUnloaded>(IGOR_BIND_EVENT_FUNCTION(PropertiesEditor::onProjectUnloaded));

    return false;
}

bool PropertiesEditor::onProjectLoaded(iEventProjectLoaded &event)
{
    iProject::getInstance().getProjectScene()->getEntitySelectionChangedEvent().add(iEntitySelectionChangedDelegate(this, &PropertiesEditor::onSelectionChanged));

    return false;
}

bool PropertiesEditor::onProjectUnloaded(iEventProjectUnloaded &event)
{
    deinitProperties();

    return false;
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
