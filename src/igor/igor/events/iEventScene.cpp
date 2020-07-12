// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/events/iEventScene.h>
#include <igor/scene/nodes/iNodeManager.h>

namespace igor
{

    iEventNodeAddedToScene::iEventNodeAddedToScene(iScenePtr scene, iNodeID nodeID)
        : iEvent(nullptr), _scene(scene), _nodeID(nodeID)
    {
    }

    const iaString iEventNodeAddedToScene::getInfo() const
    {
        std::wstringstream stream;

        iNodePtr node = iNodeManager::getInstance().getNode(_nodeID);
        stream << getName().getData() << "[" << _scene->getName() << ", " << (node != nullptr ? node->getName() : iaString::toString(_nodeID)) << "]";

        return stream.str().c_str();
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

    const iaString iEventNodeRemovedFromScene::getInfo() const
    {
        std::wstringstream stream;

        iNodePtr node = iNodeManager::getInstance().getNode(_nodeID);
        stream << getName().getData() << "[" << _scene->getName() << ", " << (node != nullptr ? node->getName() : iaString::toString(_nodeID)) << "]";

        return stream.str().c_str();
    }

    iScenePtr iEventNodeRemovedFromScene::getScene() const
    {
        return _scene;
    }

    iNodeID iEventNodeRemovedFromScene::getNodeID() const
    {
        return _nodeID;
    }

    iEventSceneSelectionChanged::iEventSceneSelectionChanged(iScenePtr scene)
        : iEvent(nullptr), _scene(scene)
    {
    }

    const iaString iEventSceneSelectionChanged::getInfo() const
    {
        std::wstringstream stream;

        stream << getName().getData() << "[" << _scene->getName() << " | ";

        if (_scene->getSelection().empty())
        {
            stream << "no selection]";
        }
        else
        {
            bool first = true;
            for (auto nodeID : _scene->getSelection())
            {
                if (!first)
                {
                    stream << ", ";
                }
                stream << nodeID;
                first = false;
            }
            stream << "]";
        }

        return stream.str().c_str();
    }

    iScenePtr iEventSceneSelectionChanged::getScene() const
    {
        return _scene;
    }

} // namespace igor