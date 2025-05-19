// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iAnimationComponent.h>

namespace igor
{

    iAnimationComponent::iAnimationComponent(iAnimationControllerPtr animationController, const iaString &name)
        : _animationController(animationController)
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
        component->_animationController = _animationController;
        return component;
    }
}