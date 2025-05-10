// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
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

    const iaString& iBehaviourComponent::getTypeName()
    {
        static const iaString name("igor_behaviour_component");
        return name;
    }

    iEntityComponentPtr iBehaviourComponent::getCopy()
    {
        iBehaviourComponent *component = new iBehaviourComponent();
        component->_behaviors = _behaviors;
        return component;
    }

    std::vector<iaString> iBehaviourComponent::getInfo() const
    {
        std::vector<iaString> result = iEntityComponent::getInfo();

        for(const auto &behaviour : _behaviors)
        {
            result.push_back(behaviour._name);
        }

        return result;
    }
}