// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <iaux/statemachine/iaTransition.h>

#include <iaux/system/iaConsole.h>
#include <iaux/statemachine/iaStateMachine.h>
using namespace iaux;

namespace iaux
{

    iaIDGenerator32 iaTransition::_idGenerator;

    iaTransition::iaTransition(iaStateID from, iaStateID to)
    {
        _from = from;
        _to = to;
        _id = _idGenerator.getNextID();
    }

    iaTransitionID iaTransition::getID() const
    {
        return _id;
    }

    iaStateID iaTransition::getFrom() const
    {
        return _from;
    }

    iaStateID iaTransition::getTo() const
    {
        return _to;
    }

    void iaTransition::activateGate(uint32 index)
    {
        con_assert(index <= 16, "index out of bounds");
        _activeGates.set(index);
    }

    void iaTransition::deactivateGate(uint32 index)
    {
        con_assert(index <= 16, "index out of bounds");
        _activeGates.reset(index);
    }

    void iaTransition::openGate(uint32 index)
    {
        con_assert(index <= 16, "index out of bounds");
        _gates.reset(index);
    }

    void iaTransition::closeGate(uint32 index)
    {
        con_assert(index <= 16, "index out of bounds");
        _gates.reset(index);
    }

    void iaTransition::resetGates()
    {
        _gates.reset();
    }

    bool iaTransition::isOpen() const
    {
        auto result = _gates & _activeGates;
        return result.any();
    }

}