// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeLODSwitch.h>

#include <igor/scene/nodes/iNodeLODTrigger.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/iScene.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iNodeLODSwitch::iNodeLODSwitch()
        : iNode()
    {
        setName(L"iNodeLODSwitch");
        _nodeType = iNodeType::iNodeLODSwitch;
        _nodeKind = iNodeKind::Node;
    }

    iNodeLODSwitch::iNodeLODSwitch(iNodeLODSwitch *node)
        : iNode()
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = iNodeType::iNodeLODSwitch;
        _nodeKind = iNodeKind::Node;
        _triggers = node->_triggers;

        setName(node->getName());
    }

    void iNodeLODSwitch::onPostCopyLink(std::map<uint32, uint32> &nodeIDMap)
    {
    }

    iNodeLODSwitch::~iNodeLODSwitch()
    {
        setScene(nullptr);
    }

    void iNodeLODSwitch::onUpdateTransform(iaMatrixd &matrix)
    {
        _worldPosition = matrix._pos;
    }

    const iaVector3d &iNodeLODSwitch::getWorldPosition() const
    {
        return _worldPosition;
    }

    void iNodeLODSwitch::addTrigger(uint32 triggerID)
    {
        if (find(_triggers.begin(), _triggers.end(), triggerID) == _triggers.end())
        {
            _triggers.push_back(triggerID);
        }
    }

    void iNodeLODSwitch::addTrigger(iNodeLODTrigger *trigger)
    {
        con_assert(trigger != nullptr, "zero pointer");

        if (trigger != nullptr)
        {
            addTrigger(trigger->getID());
        }
    }

    void iNodeLODSwitch::removeTrigger(uint32 triggerID)
    {
        auto iter = find(_triggers.begin(), _triggers.end(), triggerID);
        if (iter != _triggers.end())
        {
            _triggers.erase(iter);
        }
    }

    void iNodeLODSwitch::removeTrigger(iNodeLODTrigger *trigger)
    {
        con_assert(trigger != nullptr, "zero pointer");

        if (trigger != nullptr)
        {
            removeTrigger(trigger->getID());
        }
    }

    void iNodeLODSwitch::update()
    {
        iaVector3d delta;
        float32 quadricMin = -1.0f;

        for (auto triggerID : _triggers)
        {
            iNodeLODTrigger *trigger = static_cast<iNodeLODTrigger *>(iNodeManager::getInstance().getNode(triggerID));
            con_assert(trigger != nullptr, "zero pointer");
            if (trigger != nullptr)
            {
                delta = trigger->getWorldPosition();
                float32 quadricDistance = delta.distance2(getWorldPosition());

                if (quadricMin == -1.0f)
                {
                    quadricMin = quadricDistance;
                }
                else
                {
                    if (quadricMin > quadricDistance)
                    {
                        quadricMin = quadricDistance;
                    }
                }
            }
        }

        update(quadricMin);
    }

    void iNodeLODSwitch::update(float32 distance)
    {
        std::vector<iNodePtr> allChildren(_children);
        allChildren.insert(std::end(allChildren), std::begin(_inactiveChildren), std::end(_inactiveChildren));

        for (auto child : allChildren)
        {
            if (_thresholds[child]._min <= distance &&
                _thresholds[child]._max > distance)
            {
                child->setActive(true);
            }
            else
            {
                child->setActive(false);
            }
        }
    }

    void iNodeLODSwitch::onPreSetScene()
    {
        if (getScene())
        {
            getScene()->unregisterLODSwitch(this);
        }
    }

    void iNodeLODSwitch::onPostSetScene()
    {
        if (getScene())
        {
            getScene()->registerLODSwitch(this);
        }
    }

    void iNodeLODSwitch::setThresholds(iNodePtr node, float32 min, float32 max)
    {
        bool ok = isChild(node);
        con_assert(ok, "is not child of this node");

        if (ok)
        {
            _thresholds[node]._min = min * min;
            _thresholds[node]._max = max * max;
        }
    }

    void iNodeLODSwitch::setThresholds(const iaString &nodeName, float32 min, float32 max)
    {
        iNodePtr child = getChild(nodeName);
        con_assert(child != nullptr, "child with name " << nodeName << " not found");

        setThresholds(child, min, max);
    }

    void iNodeLODSwitch::setThresholds(uint32 nodeID, float32 min, float32 max)
    {
        iNodePtr child = getChild(nodeID);
        con_assert(child != nullptr, "child with id " << nodeID << " not found");

        setThresholds(child, min, max);
    }

}; // namespace igor
