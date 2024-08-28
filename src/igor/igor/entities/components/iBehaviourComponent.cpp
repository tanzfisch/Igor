// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iBehaviourComponent.h>

namespace igor
{
    iBehaviourComponent::iBehaviourComponent()
    {
    }

    iEntityComponent *iBehaviourComponent::createInstance()
    {
        return new iBehaviourComponent();
    }

    iEntityComponentPtr iBehaviourComponent::getCopy()
    {
        iBehaviourComponent *component = new iBehaviourComponent();
        component->_behaviors = _behaviors;
        return component;
    }
}