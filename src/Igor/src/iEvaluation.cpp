// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iEvaluation.h>
#include <iNodeTransform.h>
#include <iNodeManager.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
    iaIDGenerator64 iEvaluation::_idGenerator;

    iEvaluation::iEvaluation(uint64 nodeID)
    {
        if(iNodeManager::getInstance().isNode(nodeID))
        {
            _nodeID = nodeID;
        }

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

    void iEvaluation::setStart(float64 start)
    {
        _start = start;
    }

    float64 iEvaluation::getStart() const
    {
        return _start;
    }

    void iEvaluation::setStop(float64 stop)
    {
        _stop = stop;
    }
    
    float64 iEvaluation::getStop() const
    {
        return _stop;
    }

};

