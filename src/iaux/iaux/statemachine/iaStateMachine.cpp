// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <iaux/statemachine/iaStateMachine.h>

#include <iaux/statemachine/iaTransition.h>
#include <iaux/system/iaConsole.h>

namespace iaux
{

    void iaStateMachine::enterCurrentState()
    {
        for (auto &transition : _transitions)
        {
            transition.second->resetGates();
        }

        _enterStateEvent(_currentState->getID());
    }

    void iaStateMachine::reEnterCurrentState()
    {
        for (auto &transition : _transitions)
        {
            transition.second->resetGates();
        }

        _reEnterStateEvent(_currentState->getID());
    }

    void iaStateMachine::start()
    {
        con_assert(nullptr != _initState, "no init state defined");
        _currentState = _initState;
        enterCurrentState();
    }

    void iaStateMachine::update()
    {
        for (const auto &pair : _transitions)
        {
            auto transition = pair.second;

            if (transition->getFrom() != _currentState->getID())
            {
                continue;
            }

            if (transition->isOpen())
            {
                transit(transition);
                return;
            }
        }

        _updateStateEvent(_currentState->getID());
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
            _leaveStateEvent(transition->getFrom());
            _currentState = getState(transition->getTo());
            enterCurrentState();
        }
    }

    std::vector<iaTransitionID> iaStateMachine::getOutgoingTransitions(iaStateID stateID)
    {
        std::vector<iaTransitionID> result;

        for (const auto &transition : _transitions)
        {
            if (transition.second->getFrom() == stateID)
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

    iaStateID iaStateMachine::getInitialState() const
    {
        return _initState != nullptr ? _initState->getID() : IGOR_INVALID_ID;
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

    void iaStateMachine::registerEnterStateDelegate(iaEnterStateDelegate enterStateDelegate)
    {
        _enterStateEvent.add(enterStateDelegate);
    }

    void iaStateMachine::registerReEnterStateDelegate(iaReEnterStateDelegate reEnterStateDelegate)
    {
        _reEnterStateEvent.add(reEnterStateDelegate);
    }

    void iaStateMachine::registerLeaveStateDelegate(iaLeaveStateDelegate leaveStateDelegate)
    {
        _leaveStateEvent.add(leaveStateDelegate);
    }

    void iaStateMachine::registerUpdateStateDelegate(iaUpdateStateDelegate updateStateDelegate)
    {
        _updateStateEvent.add(updateStateDelegate);
    }

    void iaStateMachine::unregisterEnterStateDelegate(iaEnterStateDelegate enterStateDelegate)
    {
        _enterStateEvent.remove(enterStateDelegate);
    }

    void iaStateMachine::unregisterReEnterStateDelegate(iaReEnterStateDelegate reEnterStateDelegate)
    {
        _reEnterStateEvent.remove(reEnterStateDelegate);
    }

    void iaStateMachine::unregisterLeaveStateDelegate(iaLeaveStateDelegate leaveStateDelegate)
    {
        _leaveStateEvent.remove(leaveStateDelegate);
    }

    void iaStateMachine::unregisterUpdateStateDelegate(iaUpdateStateDelegate updateStateDelegate)
    {
        _updateStateEvent.remove(updateStateDelegate);
    }

} // namespace iaux