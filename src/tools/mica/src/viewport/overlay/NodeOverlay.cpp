// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "NodeOverlay.h"

NodeOverlay::NodeOverlay(iViewPtr view, iScenePtr scene, WorkspacePtr workspace)
    : _view(view), _workspace(workspace), _scene(scene)
{
}

NodeOverlay::~NodeOverlay()
{
}

void NodeOverlay::setNodeID(uint64 nodeID)
{
    _nodeID = nodeID;
}

uint64 NodeOverlay::getNodeID() const
{
    return _nodeID;
}

void NodeOverlay::setOverlayMode(OverlayMode mode)
{
    _overlayMode = mode;
}

OverlayMode NodeOverlay::getOverlayMode() const
{
    return _overlayMode;
}

void NodeOverlay::setActive(bool active)
{
    _active = active;
}

bool NodeOverlay::isActive() const
{
    return _active;
}

bool NodeOverlay::onMouseKeyDownEvent(iEventMouseKeyDown &event)
{
    return false;
}

bool NodeOverlay::onMouseKeyUpEvent(iEventMouseKeyUp &event)
{
    return false;
}

bool NodeOverlay::onMouseMoveEvent(iEventMouseMove &event)
{
    return false;
}

bool NodeOverlay::onKeyDown(iEventKeyDown &event)
{
    return false;
}

bool NodeOverlay::onSceneSelectionChanged(iEventSceneSelectionChanged &event)
{
    return false;
}

WorkspacePtr NodeOverlay::getWorkspace() const
{
    return _workspace;
}

iScenePtr NodeOverlay::getScene() const
{
    return _scene;
}

iViewPtr NodeOverlay::getView() const
{
    return _view;
}