
// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/animation/iAnimationController.h>

#include <igor/entities/iEntity.h>

namespace igor
{

    iAnimationController::iAnimationController()
    {
    }

    void iAnimationController::addAnimation(iAnimationPtr animation)
    {        
        iaStateID state = _stateMachine.addState(animation->getName());
        _animations[state] = animation;

        if(_begin == IGOR_INVALID_ID)
        {
            _begin = state;
            _stateMachine.setInitialState(_begin);
        }
    }

    iaStateMachine& iAnimationController::getStateMachine()
    {
        return _stateMachine;
    }

    void iAnimationController::update(iEntity &entity)
    {
        _stateMachine.update();

        const iaStateID current = _stateMachine.getCurrentState();

        auto iter = _animations.find(current);
        if(_animations.end() == iter)
        {
            return;
        }

        iAnimationPtr &animation = iter->second;

        // animation->evaluate

        con_endl("iAnimationController::update");
    }

}; // namespace igor