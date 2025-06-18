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

    const iaString &iBehaviourComponent::getTypeName()
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

        for (const auto &behaviour : _behaviors)
        {
            result.push_back(behaviour._name);
        }

        return result;
    }

    const std::vector<iBehaviourData> &iBehaviourComponent::getBehaviors() const
    {
        return _behaviors;
    }

    void iBehaviourComponent::addBehaviour(const iBehaviourDelegate &delegate, const std::any &userData, const iaString &name, uint8 priority)
    {
        _behaviors.push_back({delegate, userData, name, priority});
    }

    void iBehaviourComponent::removeBehaviour(const iBehaviourDelegate &delegate)
    {
        auto iter = std::find_if(_behaviors.begin(), _behaviors.end(), [delegate](const iBehaviourData &behaviourData)
                                 { return behaviourData._delegate == delegate; });

        if (iter == _behaviors.end())
        {
            con_err("can't remove behavior");
            return;
        }

        _behaviors.erase(iter);
    }

    void iBehaviourComponent::execute()
    {
        for (auto &behaviourData : _behaviors)
        {
            if (!behaviourData._delegate.isValid())
            {
                continue;
            }

            behaviourData._delegate(getEntity(), behaviourData._userData);
        }
    }
}