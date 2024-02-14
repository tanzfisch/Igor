// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntity.h>

#include <igor/entities/iEntityScene.h>

#include <algorithm>

namespace igor
{

    iEntity::iEntity(const iaString &name)
        : _name(name)
    {
    }

    const iEntityID &iEntity::getID() const
    {
        return _id;
    }

    const iaString &iEntity::getName() const
    {
        return _name;
    }

    void iEntity::addBehaviour(const iBehaviourDelegate &delegate, const std::any &userData)
    {
        iBehaviourComponent *behaviourComponent = getComponent<iBehaviourComponent>();
        if (behaviourComponent == nullptr)
        {
            behaviourComponent = static_cast<iBehaviourComponent *>(addComponent(new iBehaviourComponent()));
        }

        behaviourComponent->_behaviors.push_back({delegate, userData});
    }

    void iEntity::removeBehaviour(const iBehaviourDelegate &delegate)
    {
        iBehaviourComponent *behaviourComponent = getComponent<iBehaviourComponent>();

        if (behaviourComponent == nullptr)
        {
            con_err("no behaviour component available");
            return;
        }

        auto &behaviors = behaviourComponent->_behaviors;

        auto iter = std::find_if(behaviors.begin(), behaviors.end(), [delegate](const iBehaviourData &behaviourData)
                                 { return behaviourData._delegate == delegate; });

        if (iter == behaviors.end())
        {
            con_err("can't remove given behavior");
            return;
        }

        behaviors.erase(iter);

        if (!behaviors.empty())
        {
            return;
        }

        destroyComponent<iBehaviourComponent>();
    }

    iEntityScenePtr iEntity::getScene() const
    {
        return _scene;
    }

    void iEntity::setParent(const iEntityID &parentID)
    {
        con_assert_sticky(!hasParent(), "already has parent");

        iEntityPtr parent = getScene()->getEntity(parentID);
        con_assert_sticky(parent != nullptr, "parent not in same scene");

        _parent = parent;
    }

    const iEntityID iEntity::getParent() const
    {
        if (!hasParent())
        {
            return iEntityID::getInvalid();
        }

        return _parent->getID();
    }

    bool iEntity::hasParent() const
    {
        return _parent != nullptr;
    }

    void iEntity::onComponentsChanged()
    {
        _scene->onComponentsChanged(this);
    }

}