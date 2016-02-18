// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iaTransition.h>

#include <iaConsole.h>
#include <iaStateMachine.h>
using namespace IgorAux;

namespace IgorAux
{

    uint32 iaTransition::_nextID = INVALID_TRANSITION_ID + 1;

    iaTransition::iaTransition(uint32 from, uint32 to)
    {
        _from = from;
        _to = to;
        _id = _nextID++;
    }

    uint32 iaTransition::getID()
    {
        return _id;
    }

    uint32 iaTransition::createGate()
    {
        _gates[_nextGateID] = false;
        return _nextGateID++;
    }

    uint32 iaTransition::getFrom()
    {
        return _from;
    }

    uint32 iaTransition::getTo()
    {
        return _to;
    }

    void iaTransition::openGate(uint32 id)
    {
        auto idIter = _gates.find(id);
        if (_gates.end() != idIter)
        {
            _gates[id] = true;
        }
        else
        {
            con_err("gate id " << id << " does not exist");
        }
    }

    void iaTransition::closeGate(uint32 id)
    {
        auto idIter = _gates.find(id);
        if (_gates.end() != idIter)
        {
            _gates[id] = false;
        }
        else
        {
            con_err("gate id " << id << " does not exist");
        }
    }

    void iaTransition::resetGates()
    {
        auto idIter = _gates.begin();
        while (_gates.end() != idIter)
        {
            (*idIter).second = false;
            ++idIter;
        }
    }

    bool iaTransition::isOpen()
    {
        bool result = true;
        auto idIter = _gates.begin();
        while (_gates.end() != idIter)
        {
            if (!(*idIter).second)
            {
                result = false;
                break;
            }
            ++idIter;
        }

        return result;
    }

}