// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/statemachine/iaState.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace iaux
{
    iaIDGenerator32 iaState::_idGenerator;

    iaState::iaState(const iaString &name)
    {
        _name = name;
        _id = _idGenerator.getNextID();
    }

    iaStateID iaState::getID() const
    {
        return _id;
    }

    const iaString &iaState::getName() const
    {
        return _name;
    }

    void iaState::leave()
    {
        _leaveStateEvent();
    }

    void iaState::enter()
    {
        _enterStateEvent();
    }

    void iaState::reEnter()
    {
        _reEnterStateEvent();
    }

    void iaState::update()
    {
        _updateStateEvent();
    }

    void iaState::registerUpdateStateDelegate(iaUpdateStateDelegate updateStateDelegate)
    {
        _updateStateEvent.add(updateStateDelegate);
    }

    void iaState::unregisterUpdateStateDelegate(iaUpdateStateDelegate updateStateDelegate)
    {
        _updateStateEvent.remove(updateStateDelegate);
    }

    void iaState::registerEnterStateDelegate(iaEnterStateDelegate enterStateDelegate)
    {
        _enterStateEvent.add(enterStateDelegate);
    }

    void iaState::registerReEnterStateDelegate(iaReEnterStateDelegate reEnterStateDelegate)
    {
        _reEnterStateEvent.add(reEnterStateDelegate);
    }

    void iaState::registerLeaveStateDelegate(iaLeaveStateDelegate leaveStateDelegate)
    {
        _leaveStateEvent.add(leaveStateDelegate);
    }

    void iaState::unregisterEnterStateDelegate(iaEnterStateDelegate enterStateDelegate)
    {
        _enterStateEvent.remove(enterStateDelegate);
    }

    void iaState::unregisterReEnterStateDelegate(iaReEnterStateDelegate reEnterStateDelegate)
    {
        _reEnterStateEvent.remove(reEnterStateDelegate);
    }

    void iaState::unregisterLeaveStateDelegate(iaLeaveStateDelegate leaveStateDelegate)
    {
        _leaveStateEvent.remove(leaveStateDelegate);
    }

} // namespace iaux