// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iaStateMachine.h>

#include <iaTransition.h>

#include <iaConsole.h>

namespace IgorAux
{

    void iaStateMachine::enterCurrentState()
    {
        auto transIter = _transitions.begin();
        while (_transitions.end() != transIter)
        {
            (*transIter).second->resetGates();
            ++transIter;
        }

        _currentState->enter();
    }

    void iaStateMachine::reEnterCurrentState()
    {
        auto transIter = _transitions.begin();
        while (_transitions.end() != transIter)
        {
            (*transIter).second->resetGates();
            ++transIter;
        }

        _currentState->reEnter();
    }

    void iaStateMachine::start()
    {
        con_assert(nullptr != _initState, "no init state defined");
        _initialisation = false;
        _currentState = _initState;
        enterCurrentState();
    }

    void iaStateMachine::render()
    {
        con_assert(nullptr != _currentState, "invalid state. try run start()");
        _currentState->render();
    }

    void iaStateMachine::handle()
    {
        con_assert(nullptr != _currentState, "invalid state. try run start()");
        _currentState->handle();
    }

    uint32 iaStateMachine::getCurrentState()
    {
        return _currentState->getID();
    }

    uint32 iaStateMachine::createState()
    {
        con_assert(_initialisation, "not allowed");

        iaState* result = new iaState();
        _states[result->getID()] = result;
        return result->getID();
    }

    iaState* iaStateMachine::getState(uint32 id)
    {
        auto stateIter = _states.find(id);
        if (_states.end() != stateIter)
        {
            return (*stateIter).second;
        }
        else
        {
            con_err("state id " << id << " not found");
        }

        return 0;
    }

    void iaStateMachine::transit(iaTransition* transition)
    {
        if (transition->getFrom() == transition->getTo())
        {
            reEnterCurrentState();
        }
        else
        {
            getState(transition->getFrom())->leave();
            _currentState = getState(transition->getTo());
            enterCurrentState();
        }
    }

    iaTransition* iaStateMachine::getTransition(uint32 id)
    {
        auto transIter = _transitions.find(id);
        if (_transitions.end() != transIter)
        {
            return (*transIter).second;
        }
        else
        {
            con_err("transition id " << id << " not found");
        }

        return 0;
    }

    void iaStateMachine::setInitialState(uint32 id)
    {
        con_assert(_initialisation, "not allowed");
        
        iaState* state = getState(id);
        if (nullptr != state)
        {
            _initState = state;
        }
    }

    uint32 iaStateMachine::createTransition(uint32 from, uint32 to)
    {
        con_assert(_initialisation, "not allowed");

        auto fromState = getState(from);
        auto toState = getState(to);

        if (nullptr != fromState &&
            nullptr != toState)
        {
            iaTransition* result = new iaTransition(from, to);
            _transitions[result->getID()] = result;
            return result->getID();
        }
        else
        {
            con_err("impossible transition");
            return 0;
        }
    }

    uint32 iaStateMachine::createGate(uint32 transitionID)
    {
        con_assert(_initialisation, "not allowed");

        return getTransition(transitionID)->createGate();
    }

    void iaStateMachine::doTransition(uint32 transitionID)
    {
        auto transition = getTransition(transitionID);

        if (transition->getFrom() == _currentState->getID())
        {
            if (transition->isOpen())
            {
                transit(transition);
            }
            else
            {
                con_err("gates are closed");
            }
        }
        else
        {
            con_err("illegal transition");
        }
    }

    void iaStateMachine::openGate(uint32 transitionID, uint32 id)
    {
        auto transition = getTransition(transitionID);
        transition->openGate(id);

        if (transition->isOpen())
        {
            transit(transition);
        }
    }

    void iaStateMachine::closeGate(uint32 transitionID, uint32 id)
    {
        getTransition(transitionID)->closeGate(id);
    }

    void iaStateMachine::resetGates(uint32 transitionID)
    {
        getTransition(transitionID)->resetGates();
    }

    void iaStateMachine::registerEnterStateDelegate(uint32 stateID, EnterStateDelegate enterStateDelegate)
    {
        con_assert(_initialisation, "not allowed");

        getState(stateID)->registerEnterStateDelegate(enterStateDelegate);
    }

    void iaStateMachine::registerReEnterStateDelegate(uint32 stateID, ReEnterStateDelegate reEnterStateDelegate)
    {
        con_assert(_initialisation, "not allowed");

        getState(stateID)->registerReEnterStateDelegate(reEnterStateDelegate);
    }

    void iaStateMachine::registerLeaveStateDelegate(uint32 stateID, LeaveStateDelegate leaveStateDelegate)
    {
        con_assert(_initialisation, "not allowed");

        getState(stateID)->registerLeaveStateDelegate(leaveStateDelegate);
    }

    void iaStateMachine::registerHandleStateDelegate(uint32 stateID, HandleStateDelegate handleStateDelegate)
    {
        con_assert(_initialisation, "not allowed");

        getState(stateID)->registerHandleStateDelegate(handleStateDelegate);
    }

    void iaStateMachine::unregisterEnterStateDelegate(uint32 stateID, EnterStateDelegate enterStateDelegate)
    {
        con_assert(_initialisation, "not allowed");

        getState(stateID)->unregisterEnterStateDelegate(enterStateDelegate);
    }

    void iaStateMachine::unregisterReEnterStateDelegate(uint32 stateID, ReEnterStateDelegate reEnterStateDelegate)
    {
        con_assert(_initialisation, "not allowed");

        getState(stateID)->unregisterReEnterStateDelegate(reEnterStateDelegate);
    }

    void iaStateMachine::unregisterLeaveStateDelegate(uint32 stateID, LeaveStateDelegate leaveStateDelegate)
    {
        con_assert(_initialisation, "not allowed");

        getState(stateID)->unregisterLeaveStateDelegate(leaveStateDelegate);
    }

    void iaStateMachine::unregisterHandleStateDelegate(uint32 stateID, HandleStateDelegate handleStateDelegate)
    {
        con_assert(_initialisation, "not allowed");

        getState(stateID)->unregisterHandleStateDelegate(handleStateDelegate);
    }

    void iaStateMachine::registerRenderStateDelegate(uint32 stateID, RenderStateDelegate renderStateDelegate)
    {
        con_assert(_initialisation, "not allowed");

        getState(stateID)->registerRenderStateDelegate(renderStateDelegate);
    }

    void iaStateMachine::unregisterRenderStateDelegate(uint32 stateID, RenderStateDelegate renderStateDelegate)
    {
        con_assert(_initialisation, "not allowed");

        getState(stateID)->unregisterRenderStateDelegate(renderStateDelegate);
    }

}