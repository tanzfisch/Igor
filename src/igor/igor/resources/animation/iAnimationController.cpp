
// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/animation/iAnimationController.h>

#include <igor/resources/animation/iAnimation.h>
#include <igor/entities/iEntity.h>

namespace igor
{

    iAnimationController::iAnimationController()
    {
        _stateMachine.registerEnterStateDelegate(iaEnterStateDelegate(this, &iAnimationController::onEnterState));
    }

    void iAnimationController::onEnterState(iaStateID stateID)
    {
        // would be good to have the simulation frame time here but the actual time will do too
        _startTime = iaTime::getNow();
    }

    void iAnimationController::addClip(iClipPtr clip)
    {
        const iaString stateName = clip->getAnimations().empty() ? "no name" : clip->getAnimations().front()->getName();
        iaStateID state = _stateMachine.addState(stateName);
        _clips[state] = clip;

        if (_begin == IGOR_INVALID_ID)
        {
            _begin = state;
            _stateMachine.setInitialState(_begin);
            _stateMachine.start();
        }
    }

    void iAnimationController::update(const iaTime &time, iEntity &entity)
    {
        _stateMachine.update();

        const iaStateID current = _stateMachine.getCurrentState();

        auto iter = _clips.find(current);
        if (_clips.end() == iter)
        {
            return;
        }

        iClipPtr clip = iter->second;
        const float64 t = clip->getNormalizedTime(_startTime, time);
        con_endl("t " << t);

        for (const auto &animation : clip->getAnimations())
        {
            // TODO blending animations

            auto &transform = entity.getComponent<iTransformComponent>();

            if (animation->hasTranslateAnimation())
            {
                transform._position = animation->getTranslate(t);
            }

            if (animation->hasRotateAnimation())
            {
                transform._orientation = animation->getRotate(t);
            }

            if (animation->hasScaleAnimation())
            {
                transform._scale = animation->getScale(t);
            }
        }

        if (clip->isLooped())
        {
            if (time - _startTime >= clip->getDuration())
            {
                _startTime += clip->getDuration();
                con_endl("loop " << _startTime.getSeconds());
            }
        }
    }

}; // namespace igor