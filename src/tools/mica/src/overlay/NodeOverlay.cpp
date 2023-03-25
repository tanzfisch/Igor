// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "NodeOverlay.h"

NodeOverlay::NodeOverlay(iViewPtr view, iScenePtr scene, WorkspacePtr workspace)
    : _view(view), _workspace(workspace), _scene(scene)
{
}

NodeOverlay::~NodeOverlay()
{
}

void NodeOverlay::setVisible(bool visible)
{
    _visible = visible;
}

bool NodeOverlay::isVisible() const
{
    return _visible;
}

OverlayMode NodeOverlay::getOverlayMode() const
{
    return _userNodeOverlayMode;
}

void NodeOverlay::setNodeID(uint64 nodeID)
{
    iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
    if (node != nullptr)
    {
        _nodeID = nodeID;
    }
    else
    {
        _nodeID = iNode::INVALID_NODE_ID;
    }
}

uint64 NodeOverlay::getNodeID() const
{
    return _nodeID;
}

bool NodeOverlay::isSelected() const
{
    return false;
}

void NodeOverlay::onMouseMoved(const iaVector2i &from, const iaVector2i &to)
{
}

void NodeOverlay::setOverlayMode(OverlayMode manipulatorMode)
{
    _userNodeOverlayMode = manipulatorMode;
}

bool NodeOverlay::select(iNodeID nodeID)
{
    return false;
}

void NodeOverlay::unselect()
{
}