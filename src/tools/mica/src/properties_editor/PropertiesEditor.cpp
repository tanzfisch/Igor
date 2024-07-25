// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
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

void PropertiesEditor::deinitProperties()
{
    if (_userControlProperties != nullptr)
    {
        _scroll->removeWidget(_userControlProperties);
        iWidgetManager::getInstance().deleteWidget(_userControlProperties);
        _userControlProperties = nullptr;
    }
}

void PropertiesEditor::setSelection(iNodeID nodeID)
{
    deinitProperties();

    if (nodeID == iNode::INVALID_NODE_ID)
    {
        return;
    }

    _userControlProperties = new UserControlProperties(nodeID, _scroll);
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

void PropertiesEditor::setSelectionEntity(const iEntitySceneID &sceneID, const iEntityID &entityID)
{
    deinitProperties();

    if (!sceneID.isValid() ||
        !entityID.isValid())
    {
        return;
    }

    _userControlProperties = new UserControlProperties(UserControlProperties::PropertyType::Entity, {sceneID, entityID}, _scroll);
}
