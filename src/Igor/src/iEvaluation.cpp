// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iEvaluation.h>
#include <iNode.h>
#include <iNodeManager.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
    iaIDGenerator64 iEvaluation::_idGenerator;

    iEvaluation::iEvaluation()
    {
        _evaluatorID = iEvaluation::_idGenerator.createID();
    }

    uint64 iEvaluation::getID() const
    {
        return _evaluatorID;
    }

    void iEvaluation::setLooped(bool loop)
    {
        _looped = loop;
    }

    bool iEvaluation::isLooped() const
    {
        return _looped;
    }

    void iEvaluation::addNode(const iNodePtr node)
    {
        con_assert(node != nullptr, "zero pointer");

        if (node == nullptr)
        {
            return;
        }

        uint64 nodeID = node->getID();

        if (!iNodeManager::getInstance().isNode(nodeID))
        {
            return;
        }

        if (std::find(_nodes.begin(), _nodes.end(), nodeID) != _nodes.end())
        {
            return;
        }

        _nodes.push_back(nodeID);
    }

    void iEvaluation::removeNode(const iNodePtr node)
    {
        con_assert(node != nullptr, "zero pointer");

        if (node == nullptr)
        {
            return;
        }

        uint64 nodeID = node->getID();

        auto iter = std::find(_nodes.begin(), _nodes.end(), nodeID);
        if(iter == _nodes.end())
        {
            return;
        }

        _nodes.erase(iter);

    }

};

