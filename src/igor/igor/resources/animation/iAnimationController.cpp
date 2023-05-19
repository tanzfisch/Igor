
// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/animation/iAnimationController.h>

#include <igor/entities/iEntity.h>

namespace igor
{

    iAnimationController::iAnimationController()
    {
        _begin = _stateMachine.addState("begin");
        _stateMachine.setInitialState(_begin);
    }

    void iAnimationController::addAnimation(iAnimationPtr animation)
    {

    }

    iaStateMachine& iAnimationController::getStateMachine()
    {
        return _stateMachine;
    }

    void iAnimationController::update(iEntity &entity)
    {
        if(_stateMachine.getCurrentState() == _begin)
        {
            return;
        }

        con_endl("iAnimationController::update");
    }

}; // namespace igor