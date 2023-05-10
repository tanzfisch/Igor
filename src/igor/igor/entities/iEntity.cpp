// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntity.h>

#include <igor/entities/components/iComponents.h>

#include <entt.h>

namespace igor
{

    iEntity::iEntity(const iEntityID entity, iEntityScenePtr scene)
        : _entity(entity), _scene(scene)
    {
    }

    void iEntity::setName(const iaString &name)
    {
        auto *registry = static_cast<entt::registry*>(_scene->getRegistry());

        iBaseEntityComponent &component = registry->get<iBaseEntityComponent>(static_cast<entt::entity>(_entity));
        component._name = name;
    }

    const iaString iEntity::getName() const
    {
        auto *registry = static_cast<entt::registry*>(_scene->getRegistry());

        iBaseEntityComponent &component = registry->get<iBaseEntityComponent>(static_cast<entt::entity>(_entity));
        return component._name;
    }

    bool iEntity::isActive() const
    {
        auto *registry = static_cast<entt::registry*>(_scene->getRegistry());

        iActiveComponent *component = registry->try_get<iActiveComponent>(static_cast<entt::entity>(_entity));
        return component != nullptr;
    }

    void iEntity::setActive(bool active)
    {
        auto *registry = static_cast<entt::registry*>(_scene->getRegistry());
        
        iActiveComponent *component = registry->try_get<iActiveComponent>(static_cast<entt::entity>(_entity));
        if (component == nullptr && active)
        {
            registry->emplace_or_replace<iActiveComponent>(static_cast<entt::entity>(_entity));
        }
        else if (component != nullptr && !active)
        {
            registry->remove<iActiveComponent>(static_cast<entt::entity>(_entity));
        }
    }

    iEntityID iEntity::getID() const
    {
        return _entity;
    }

    bool iEntity::isValid() const
    {
        auto *registry = static_cast<entt::registry*>(_scene->getRegistry());

        return registry->valid(static_cast<entt::entity>(_entity));
    }

    void iEntity::addBehaviour(const iBehaviourDelegate &behaviour, const std::any &userData)
    {
        auto *registry = static_cast<entt::registry*>(_scene->getRegistry());

        iBehaviourComponent *component = registry->try_get<iBehaviourComponent>(static_cast<entt::entity>(_entity));
        if (component == nullptr)
        {
            component = &(registry->emplace_or_replace<iBehaviourComponent>(static_cast<entt::entity>(_entity)));
        }

        for (auto &behaviourData : component->_behaviour)
        {
            if (!behaviourData._delegate.isValid())
            {
                behaviourData._delegate = behaviour;
                behaviourData._userData = userData;
                return;
            }
        }

        con_err("can't add more then " << component->_behaviour.size() << " behaviors");
    }

    void iEntity::removeBehaviour(const iBehaviourDelegate &behaviour)
    {
        auto *registry = static_cast<entt::registry*>(_scene->getRegistry());

        iBehaviourComponent *component = registry->try_get<iBehaviourComponent>(static_cast<entt::entity>(_entity));
        if (component == nullptr)
        {
            con_err("no behaviour component available");
            return;
        }

        int nonZero = 0;
        for (auto &behaviourData : component->_behaviour)
        {
            if (behaviourData._delegate == behaviour)
            {
                nonZero++;
                behaviourData._delegate = nullptr;
            }
        }

        if (nonZero == 1)
        {
            registry->remove<iBehaviourComponent>(static_cast<entt::entity>(_entity));
        }

        if (nonZero == 0)
        {
            con_err("can't remove given behaviour");
        }
    }

    void iEntity::setParent(iEntityID parent)
    {
        auto *registry = static_cast<entt::registry*>(_scene->getRegistry());

        iHierarchyComponent *component = registry->try_get<iHierarchyComponent>(static_cast<entt::entity>(_entity));
        if (component == nullptr)
        {
            if (parent == IGOR_INVALID_ENTITY_ID)
            {
                return;
            }

            component = &(registry->emplace_or_replace<iHierarchyComponent>(static_cast<entt::entity>(_entity)));
        }

        if (component->_parent == parent)
        {
            return;
        }

        iHierarchyComponent *parentComponent = nullptr;

        if (component->_parent != IGOR_INVALID_ENTITY_ID)
        {
            parentComponent = registry->try_get<iHierarchyComponent>(static_cast<entt::entity>(component->_parent));
            if (parentComponent != nullptr)
            {
                parentComponent->_childCount = std::max(0, parentComponent->_childCount - 1);
            }

            component->_parent = IGOR_INVALID_ENTITY_ID;
        }

        if (parent != IGOR_INVALID_ENTITY_ID)
        {
            parentComponent = registry->try_get<iHierarchyComponent>(static_cast<entt::entity>(parent));
            if (parentComponent == nullptr)
            {
                parentComponent = &(registry->emplace_or_replace<iHierarchyComponent>(static_cast<entt::entity>(parent)));
            }

            parentComponent->_childCount++;
        }

        component->_parent = parent;
    }

    iEntityID iEntity::getParent() const
    {
        auto *registry = static_cast<entt::registry*>(_scene->getRegistry());

        iHierarchyComponent *component = registry->try_get<iHierarchyComponent>(static_cast<entt::entity>(_entity));
        if (component == nullptr)
        {
            return IGOR_INVALID_ENTITY_ID;
        }

        return component->_parent;
    }

    void iEntity::setMotionInteractionType(iMotionInteractionType interactionType)
    {
        auto *registry = static_cast<entt::registry*>(_scene->getRegistry());

        iMotionInteractionResolverComponent *component = registry->try_get<iMotionInteractionResolverComponent>(static_cast<entt::entity>(_entity));
        if (component == nullptr)
        {
            if (interactionType == iMotionInteractionType::None)
            {
                return;
            }

            component = &(registry->emplace_or_replace<iMotionInteractionResolverComponent>(static_cast<entt::entity>(_entity)));
        }

        if (interactionType == iMotionInteractionType::None)
        {
            registry->remove<iMotionInteractionResolverComponent>(static_cast<entt::entity>(_entity));
            return;
        }

        component->_type = interactionType;
    }

    iMotionInteractionType iEntity::getMotionInteractionType() const
    {
        auto *registry = static_cast<entt::registry*>(_scene->getRegistry());

        iMotionInteractionResolverComponent *component = registry->try_get<iMotionInteractionResolverComponent>(static_cast<entt::entity>(_entity));
        if (component == nullptr)
        {
            return iMotionInteractionType::None;
        }

        return component->_type;
    }

}