// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntity.h>

#include <igor/entities/components/iComponents.h>

namespace igor
{

    iEntity::iEntity(const iEntityID entity, iEntityScenePtr scene)
        : _entity(entity), _scene(scene)
    {
    }

    void iEntity::setName(const iaString &name)
    {
        iBaseEntityComponent &component = _scene->getRegistry().get<iBaseEntityComponent>(_entity);
        component._name = name;
    }

    const iaString iEntity::getName() const
    {
        iBaseEntityComponent &component = _scene->getRegistry().get<iBaseEntityComponent>(_entity);
        return component._name;
    }

    bool iEntity::isActive() const
    {
        iActiveComponent *component = _scene->getRegistry().try_get<iActiveComponent>(_entity);
        return component != nullptr;
    }

    void iEntity::setActive(bool active)
    {
        iActiveComponent *component = _scene->getRegistry().try_get<iActiveComponent>(_entity);
        if (component == nullptr && active)
        {
            _scene->getRegistry().emplace_or_replace<iActiveComponent>(_entity);
        }
        else if (component != nullptr && !active)
        {
            _scene->getRegistry().remove<iActiveComponent>(_entity);
        }
    }

    iEntityID iEntity::getID() const
    {
        return _entity;
    }

    bool iEntity::isValid() const
    {
        return _scene->getRegistry().valid(_entity);
    }

    void iEntity::addBehaviour(const iBehaviourDelegate &delegate, void *userData)
    {
        iBehaviourComponent *component = _scene->getRegistry().try_get<iBehaviourComponent>(_entity);
        if (component == nullptr)
        {
            component = &(_scene->getRegistry().emplace_or_replace<iBehaviourComponent>(_entity));
        }

        for (auto &behaviourData : component->_behaviour)
        {
            if (!behaviourData._delegate.isValid())
            {
                behaviourData._delegate = delegate;
                behaviourData._userData = userData;
                return;
            }
        }

        con_err("can't add more then " << component->_behaviour.size() << " behaviors");
    }

    void iEntity::removeBehaviour(const iBehaviourDelegate &behaviour)
    {
        auto &registry = _scene->getRegistry();

        iBehaviourComponent *component = registry.try_get<iBehaviourComponent>(_entity);
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
                behaviourData._userData = nullptr;
            }
        }

        if (nonZero == 1)
        {
            registry.remove<iBehaviourComponent>(_entity);
        }

        if (nonZero == 0)
        {
            con_err("can't remove given behaviour");
        }
    }

    void iEntity::setParent(iEntityID parent)
    {
        iHierarchyComponent *component = _scene->getRegistry().try_get<iHierarchyComponent>(_entity);
        if (component == nullptr)
        {
            if (parent == IGOR_INVALID_ENTITY_ID)
            {
                return;
            }

            component = &(_scene->getRegistry().emplace_or_replace<iHierarchyComponent>(_entity));
        }

        if (component->_parent == parent)
        {
            return;
        }

        iHierarchyComponent *parentComponent = nullptr;

        if (component->_parent != IGOR_INVALID_ENTITY_ID)
        {
            parentComponent = _scene->getRegistry().try_get<iHierarchyComponent>(component->_parent);
            if (parentComponent != nullptr)
            {
                parentComponent->_childCount = std::max(0, parentComponent->_childCount - 1);
            }

            component->_parent = IGOR_INVALID_ENTITY_ID;
        }

        if (parent != IGOR_INVALID_ENTITY_ID)
        {
            parentComponent = _scene->getRegistry().try_get<iHierarchyComponent>(parent);
            if (parentComponent == nullptr)
            {
                parentComponent = &(_scene->getRegistry().emplace_or_replace<iHierarchyComponent>(parent));
            }

            parentComponent->_childCount++;
        }

        component->_parent = parent;
    }

    iEntityID iEntity::getParent() const
    {
        iHierarchyComponent *component = _scene->getRegistry().try_get<iHierarchyComponent>(_entity);
        if (component == nullptr)
        {
            return IGOR_INVALID_ENTITY_ID;
        }

        return component->_parent;
    }

    void iEntity::setMotionInteractionType(iMotionInteractionType interactionType)
    {
        auto &registry = _scene->getRegistry();

        iMotionInteractionResolverComponent *component = registry.try_get<iMotionInteractionResolverComponent>(_entity);
        if (component == nullptr)
        {
            if (interactionType == iMotionInteractionType::None)
            {
                return;
            }

            component = &(registry.emplace_or_replace<iMotionInteractionResolverComponent>(_entity));
        }

        if (interactionType == iMotionInteractionType::None)
        {
            registry.remove<iMotionInteractionResolverComponent>(_entity);
            return;
        }

        component->_type = interactionType;
    }

    iMotionInteractionType iEntity::getMotionInteractionType() const
    {
        iMotionInteractionResolverComponent *component = _scene->getRegistry().try_get<iMotionInteractionResolverComponent>(_entity);
        if (component == nullptr)
        {
            return iMotionInteractionType::None;
        }

        return component->_type;
    }

}