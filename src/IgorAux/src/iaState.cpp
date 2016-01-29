// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iaState.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace IgorAux
{
    uint32 iaState::_nextID = 1;

    iaState::iaState()
    {
        _id = _nextID++;
    }

    iaState::~iaState()
    {

    }

    uint32 iaState::getID()
    {
        return _id;
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

    void iaState::handle()
    {
        _handleStateEvent();
    }

    void iaState::render()
    {
        _renderStateEvent();
    }

    void iaState::registerRenderStateDelegate(RenderStateDelegate renderStateDelegate)
    {
        _renderStateEvent.append(renderStateDelegate);
    }
        
    void iaState::unregisterRenderStateDelegate(RenderStateDelegate renderStateDelegate)
    {
        _renderStateEvent.remove(renderStateDelegate);
    }

    void iaState::registerHandleStateDelegate(HandleStateDelegate handleStateDelegate)
    {
        _handleStateEvent.append(handleStateDelegate);
    }

    void iaState::unregisterHandleStateDelegate(HandleStateDelegate handleStateDelegate)
    {
        _handleStateEvent.remove(handleStateDelegate);
    }

    void iaState::registerEnterStateDelegate(EnterStateDelegate enterStateDelegate)
    {
        _enterStateEvent.append(enterStateDelegate);
    }

    void iaState::registerReEnterStateDelegate(ReEnterStateDelegate reEnterStateDelegate)
    {
        _reEnterStateEvent.append(reEnterStateDelegate);
    }

    void iaState::registerLeaveStateDelegate(LeaveStateDelegate leaveStateDelegate)
    {
        _leaveStateEvent.append(leaveStateDelegate);
    }

    void iaState::unregisterEnterStateDelegate(EnterStateDelegate enterStateDelegate)
    {
        _enterStateEvent.remove(enterStateDelegate);
    }

    void iaState::unregisterReEnterStateDelegate(ReEnterStateDelegate reEnterStateDelegate)
    {
        _reEnterStateEvent.remove(reEnterStateDelegate);
    }

    void iaState::unregisterLeaveStateDelegate(LeaveStateDelegate leaveStateDelegate)
    {
        _leaveStateEvent.remove(leaveStateDelegate);
    }

}