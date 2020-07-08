// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/events/iEventScene.h>

namespace igor
{

    iEventNodeAddedToScene::iEventNodeAddedToScene(iScenePtr scene, iNodeID nodeID)
        : iEvent(nullptr), _scene(scene), _nodeID(nodeID)
    {
    }

    iScenePtr iEventNodeAddedToScene::getScene() const
    {
        return _scene;
    }

    iNodeID iEventNodeAddedToScene::getNodeID() const
    {
        return _nodeID;
    }

    iEventNodeRemovedFromScene::iEventNodeRemovedFromScene(iScenePtr scene, iNodeID nodeID)
        : iEvent(nullptr), _scene(scene), _nodeID(nodeID)
    {
    }

    iScenePtr iEventNodeRemovedFromScene::getScene() const
    {
        return _scene;
    }

    iNodeID iEventNodeRemovedFromScene::getNodeID() const
    {
        return _nodeID;
    }

} // namespace igor