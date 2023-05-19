// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/statemachine/iaStateMachine.h>

#include <iaux/statemachine/iaTransition.h>
#include <iaux/system/iaConsole.h>

namespace iaux
{

    void iaStateMachine::enterCurrentState()
    {
        for(auto &transition : _transitions)
        {
            transition.second->resetGates();
        }

        _currentState->enter();
    }

    void iaStateMachine::reEnterCurrentState()
    {
        for(auto &transition : _transitions)
        {
            transition.second->resetGates();
        }

        _currentState->reEnter();
    }

    void iaStateMachine::start()
    {
        con_assert(nullptr != _initState, "no init state defined");
        _currentState = _initState;
        enterCurrentState();
    }

    void iaStateMachine::update()
    {
        con_assert(nullptr != _currentState, "invalid state. try run start()");
        _currentState->update();
    }

    iaStateID iaStateMachine::getCurrentState() const
    {
        return _currentState->getID();
    }

    iaStateID iaStateMachine::addState(const iaString &name)
    {
        iaStatePtr result = new iaState(name);
        _states[result->getID()] = result;
        return result->getID();
    }

    iaStatePtr iaStateMachine::getState(iaStateID id)
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

        return nullptr;
    }

    void iaStateMachine::transit(iaTransitionPtr transition)
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

    std::vector<iaTransitionID> iaStateMachine::getOutgoingTransitions(iaStateID stateID)
    {
        std::vector<iaTransitionID> result;

        for(const auto &transition : _transitions)
        {
            if(transition.second->getFrom() == stateID)
            {
                result.push_back(transition.first);
            }
        }

        return result;
    }

    iaTransition *iaStateMachine::getTransition(iaTransitionID id)
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

    void iaStateMachine::setInitialState(iaStateID id)
    {
        iaStatePtr state = getState(id);
        if (nullptr != state)
        {
            _initState = state;
        }
    }

    uint32 iaStateMachine::addTransition(iaStateID from, iaStateID to)
    {
        auto fromState = getState(from);
        auto toState = getState(to);

        if (nullptr != fromState &&
            nullptr != toState)
        {
            iaTransition *result = new iaTransition(from, to);
            _transitions[result->getID()] = result;
            return result->getID();
        }
        else
        {
            con_err("impossible transition");
            return 0;
        }
    }

    void iaStateMachine::doTransition(iaTransitionID transitionID)
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

    void iaStateMachine::activateGate(iaTransitionID transitionID, uint32 index)
    {
        auto transition = getTransition(transitionID);
        transition->activateGate(index);
    }

    void iaStateMachine::deactivateGate(iaTransitionID transitionID, uint32 index)
    {
        auto transition = getTransition(transitionID);
        transition->deactivateGate(index);
    }

    void iaStateMachine::openGate(iaTransitionID transitionID, uint32 index)
    {
        auto transition = getTransition(transitionID);
        transition->openGate(index);

        if (transition->isOpen())
        {
            transit(transition);
        }
    }

    void iaStateMachine::closeGate(iaTransitionID transitionID, uint32 index)
    {
        auto transition = getTransition(transitionID);
        transition->closeGate(index);
    }

    void iaStateMachine::resetGates(iaTransitionID transitionID)
    {
        auto transition = getTransition(transitionID);
        transition->resetGates();
    }

    void iaStateMachine::registerEnterStateDelegate(uint32 stateID, iaEnterStateDelegate enterStateDelegate)
    {
        getState(stateID)->registerEnterStateDelegate(enterStateDelegate);
    }

    void iaStateMachine::registerReEnterStateDelegate(uint32 stateID, iaReEnterStateDelegate reEnterStateDelegate)
    {
        getState(stateID)->registerReEnterStateDelegate(reEnterStateDelegate);
    }

    void iaStateMachine::registerLeaveStateDelegate(uint32 stateID, iaLeaveStateDelegate leaveStateDelegate)
    {
        getState(stateID)->registerLeaveStateDelegate(leaveStateDelegate);
    }

    void iaStateMachine::registerUpdateStateDelegate(uint32 stateID, iaUpdateStateDelegate updateStateDelegate)
    {
        getState(stateID)->registerUpdateStateDelegate(updateStateDelegate);
    }

    void iaStateMachine::unregisterEnterStateDelegate(uint32 stateID, iaEnterStateDelegate enterStateDelegate)
    {
        getState(stateID)->unregisterEnterStateDelegate(enterStateDelegate);
    }

    void iaStateMachine::unregisterReEnterStateDelegate(uint32 stateID, iaReEnterStateDelegate reEnterStateDelegate)
    {
        getState(stateID)->unregisterReEnterStateDelegate(reEnterStateDelegate);
    }

    void iaStateMachine::unregisterLeaveStateDelegate(uint32 stateID, iaLeaveStateDelegate leaveStateDelegate)
    {
        getState(stateID)->unregisterLeaveStateDelegate(leaveStateDelegate);
    }

    void iaStateMachine::unregisterUpdateStateDelegate(uint32 stateID, iaUpdateStateDelegate updateStateDelegate)
    {
        getState(stateID)->unregisterUpdateStateDelegate(updateStateDelegate);
    }

} // namespace iaux