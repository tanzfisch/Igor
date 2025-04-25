// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "EntityOverlay.h"

EntityOverlay::EntityOverlay(iViewPtr view)
    : _view(view)
{
}

void EntityOverlay::setOverlayMode(OverlayMode mode)
{
    _overlayMode = mode;
}

OverlayMode EntityOverlay::getOverlayMode() const
{
    return _overlayMode;
}

void EntityOverlay::setActive(bool active)
{
    _active = active;
}

bool EntityOverlay::isActive() const
{
    return _active;
}

bool EntityOverlay::onMouseKeyDownEvent(iEventMouseKeyDown &event)
{
    return false;
}

bool EntityOverlay::onMouseKeyUpEvent(iEventMouseKeyUp &event)
{
    return false;
}

bool EntityOverlay::onMouseMoveEvent(iEventMouseMove &event)
{
    return false;
}

bool EntityOverlay::onKeyDown(iEventKeyDown &event)
{
    return false;
}

iViewPtr EntityOverlay::getView() const
{
    return _view;
}

void EntityOverlay::setEntity(iEntityID entityID)
{
    _entityID = entityID;
}

const iEntityID &EntityOverlay::getEntity() const
{
    return _entityID;
}
