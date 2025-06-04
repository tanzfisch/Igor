// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iAnimationSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/system/iTimer.h>

#include <iaux/math/iaRandom.h>

namespace igor
{

    iAnimationSystem::iAnimationSystem()
    {
        _viewTransform = createView<iTransformComponent, iAnimationComponent>();
        _viewSprite = createView<iSpriteRenderComponent, iAnimationComponent>();
    }

    iEntitySystemPtr iAnimationSystem::createInstance()
    {
        return new iAnimationSystem();
    }

    const iaString &iAnimationSystem::getTypeName()
    {
        static const iaString typeName("igor_animation_system");
        return typeName;
    }

    iEntitySystemStage iAnimationSystem::getStage() const
    {
        return iEntitySystemStage::Update;
    }

    /* iAnimationController::iAnimationController()
     {
         _stateMachine.registerEnterStateDelegate(iaEnterStateDelegate(this, &iAnimationController::onEnterState));
     }

     void iAnimationController::onEnterState(iaStateID stateID)
     {

     }*/

    void iAnimationSystem::onUpdate(const iEntitySceneUpdateContext &context)
    {
        const iaTime &time = context._time;

        for (auto entity : _viewTransform->getEntities())
        {
            auto animationComp = entity->getComponent<iAnimationComponent>();
            auto transformComp = entity->getComponent<iTransformComponent>();
            onUpdateTransform(time, animationComp, transformComp);
        }

        for (auto entity : _viewSprite->getEntities())
        {
            auto animationComp = entity->getComponent<iAnimationComponent>();
            auto spriteRenderComp = entity->getComponent<iSpriteRenderComponent>();
            onUpdateSprite(time, animationComp, spriteRenderComp);
        }
    }

    float64 iAnimationSystem::mapTime(const iaTime &time, iAnimationComponentPtr animationComp, iClipPtr clip, iaStateMachine::iaEvent lastEvent)
    {
        if (lastEvent == iaStateMachine::iaEvent::Enter)
        {
            animationComp->_startTime = time;

            if (clip->hasRandomStart())
            {
                animationComp->_offsetTime = clip->getDuration() * iaRandom::getNextFloat();
            }
            else
            {
                animationComp->_offsetTime = iaTime::fromSeconds(0.0);
            }
        }

        return clip->getNormalizedTime(animationComp->_startTime, time) * clip->getDuration().getSeconds();        
    }

    void iAnimationSystem::onUpdateSprite(const iaTime &time, iAnimationComponentPtr animationComp, iSpriteRenderComponentPtr spriteRenderComp)
    {
        const auto lastEvent = animationComp->_stateMachine.update();

        auto &clips = animationComp->_clips;
        auto iterClip = clips.find(animationComp->_stateMachine.getCurrentState());
        if (clips.end() == iterClip)
        {
            return;
        }

        iClipPtr clip = iterClip->second;
        float64 t = mapTime(time, animationComp, clip, lastEvent);

        for (const auto &animation : clip->getAnimations())
        {
            if (animation->hasFrameIndexAnimation())
            {
                spriteRenderComp->setFrameIndex(animation->getFrameIndex(t));
            }
        }

        if (clip->isLooped())
        {
            if (t >= clip->getDuration().getSeconds())
            {
                animationComp->_startTime += clip->getDuration();
            }
        }
    }

    void iAnimationSystem::onUpdateTransform(const iaTime &time, iAnimationComponentPtr animationComp, iTransformComponentPtr transformComp)
    {
        const auto lastEvent = animationComp->_stateMachine.update();

        auto &clips = animationComp->_clips;
        auto iterClip = clips.find(animationComp->_stateMachine.getCurrentState());
        if (clips.end() == iterClip)
        {
            return;
        }

        iClipPtr clip = iterClip->second;
        float64 t = mapTime(time, animationComp, clip, lastEvent);

        for (const auto &animation : clip->getAnimations())
        {
            if (animation->hasTranslateAnimation())
            {
                transformComp->setPosition(animation->getTranslate(t));
            }

            if (animation->hasRotateAnimation())
            {
                transformComp->setOrientation(iaQuaterniond::fromEuler(animation->getRotate(t)));
            }

            if (animation->hasScaleAnimation())
            {
                transformComp->setScale(animation->getScale(t));
            }
        }

        if (clip->isLooped())
        {
            if (t >= clip->getDuration().getSeconds())
            {
                animationComp->_startTime += clip->getDuration();
            }
        }
    }

} // igor