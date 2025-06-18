// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iAnimationComponent.h>

namespace igor
{

    iAnimationComponent::iAnimationComponent()
    {
    }

    iEntityComponent *iAnimationComponent::createInstance()
    {
        return new iAnimationComponent();
    }

    const iaString &iAnimationComponent::getTypeName()
    {
        static const iaString name("igor_animation_component");
        return name;
    }

    iEntityComponentPtr iAnimationComponent::getCopy()
    {
        iAnimationComponent *component = new iAnimationComponent();
        component->_begin = _begin;
        component->_clips = _clips;
        component->_offsetTime = _offsetTime;
        component->_startTime = _startTime;
        component->_stateMachine = _stateMachine;
        return component;
    }

    void iAnimationComponent::addClip(iClipPtr clip)
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

}