
// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/animation/iAnimationController.h>

#include <igor/entities/iEntity.h>
#include <igor/entities/components/iSpriteRenderComponent.h>
#include <igor/entities/components/iTransformComponent.h>
#include <igor/resources/animation/iAnimation.h>

#include <iaux/math/iaRandom.h>

namespace igor
{

    iAnimationController::iAnimationController()
    {
        _stateMachine.registerEnterStateDelegate(iaEnterStateDelegate(this, &iAnimationController::onEnterState));
    }

    void iAnimationController::onEnterState(iaStateID stateID)
    {
        auto iter = _clips.find(_stateMachine.getCurrentState());
        if (_clips.end() == iter)
        {
            return;
        }

        iClipPtr clip = iter->second;

        // would be good to have the simulation frame time here but the actual time will do too
        _startTime = iaTime::getNow();

        if (clip->hasRandomStart())
        {
            _offsetTime = clip->getDuration() * iaRandom::getNextFloat();
        }
        else
        {
            _offsetTime = iaTime::fromSeconds(0.0);
        }
    }

    void iAnimationController::addClip(iClipPtr clip)
    {
        const iaString stateName = clip->getAnimations().empty() ? "no name" : clip->getAnimations().front()->getInfo();
        iaStateID state = _stateMachine.addState(stateName);
        _clips[state] = clip;

        if (_begin == IGOR_INVALID_ID)
        {
            _begin = state;
            _stateMachine.setInitialState(_begin);
            _stateMachine.start();
        }
    }

    void iAnimationController::update(const iaTime &time, iEntityPtr entity)
    {
        _stateMachine.update();

        auto iter = _clips.find(_stateMachine.getCurrentState());
        if (_clips.end() == iter)
        {
            return;
        }

        iClipPtr clip = iter->second;
        float64 t = clip->getNormalizedTime(_startTime, time) * clip->getDuration().getSeconds();

        auto transform = entity->getComponent<iTransformComponent>();
        auto spriteRender = entity->getComponent<iSpriteRenderComponent>();

        for (const auto &animation : clip->getAnimations())
        {
            // TODO blending animations
            // TODO this is not good ... but simple enough for now

            if (animation->hasTranslateAnimation())
            {
                transform->setPosition(animation->getTranslate(t));
            }

            if (animation->hasRotateAnimation())
            {
                transform->setOrientation(iaQuaterniond::fromEuler(animation->getRotate(t)));
            }

            if (animation->hasScaleAnimation())
            {
                transform->setScale(animation->getScale(t));
            }

            if (spriteRender &&
                animation->hasFrameIndexAnimation())
            {
                spriteRender->_frameIndex = animation->getFrameIndex(t);
            }
        }

        if (clip->isLooped())
        {
            if (t >= clip->getDuration().getSeconds())
            {
                _startTime += clip->getDuration();
            }
        }
    }

}; // namespace igor