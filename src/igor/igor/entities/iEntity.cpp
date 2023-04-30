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

        if (component != nullptr && !active)
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

    iTransformComponent &iEntity::addTransformComponent(const iaVector3d &position, const iaVector3d &orientation, const iaVector3d &scale)
    {
        return _scene->addTransformComponent(_entity, position, orientation, scale);
    }

    iSpriteRendererComponent &iEntity::addSpriteRendererComponent(iTexturePtr texture, const iaColor4f &color, int32 zIndex)
    {
        return _scene->addSpriteRendererComponent(_entity, texture, color, zIndex);
    }

    iVelocityComponent &iEntity::addVelocityComponent(const iaVector3d &velocity, const iaVector3d &angularVelocity)
    {
        return _scene->addVelocityComponent(_entity, velocity, angularVelocity);
    }

    void iEntity::addToQuadtree(float64 size)
    {
        _scene->addToQuadtree(_entity, size);
    }

    void iEntity::removeFromQuadtree()
    {
        _scene->removeFromQuadtree(_entity);
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
        iBehaviourComponent *component = _scene->getRegistry().try_get<iBehaviourComponent>(_entity);
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
            _scene->removeComponent<iBehaviourComponent>(_entity);
        }

        if (nonZero == 0)
        {
            con_err("can't remove given behaviour");
        }
    }

    void iEntity::setParent(iEntityID parent)
    {
        iTransformComponent *component = _scene->getRegistry().try_get<iTransformComponent>(_entity);
        if (component == nullptr)
        {
            if (parent == IGOR_INVALID_ENTITY_ID)
            {
                return;
            }

            component = &(_scene->getRegistry().emplace_or_replace<iTransformComponent>(_entity));
        }

        if (component->_parent == parent)
        {
            return;
        }

        iTransformComponent *parentComponent = nullptr;

        if (component->_parent != IGOR_INVALID_ENTITY_ID)
        {
            parentComponent = _scene->getRegistry().try_get<iTransformComponent>(component->_parent);
            if (parentComponent != nullptr)
            {
                parentComponent->_childCount = std::max(0, parentComponent->_childCount--);
            }

            component->_parent = IGOR_INVALID_ENTITY_ID;
        }

        if (parent != IGOR_INVALID_ENTITY_ID)
        {
            parentComponent = _scene->getRegistry().try_get<iTransformComponent>(parent);
            if (parentComponent == nullptr)
            {
                parentComponent = &(_scene->getRegistry().emplace_or_replace<iTransformComponent>(parent));
            }

            parentComponent->_childCount++;
        }

        component->_parent = parent;
    }

    iEntityID iEntity::getParent() const
    {
        iTransformComponent *component = _scene->getRegistry().try_get<iTransformComponent>(_entity);
        if (component == nullptr)
        {
            return IGOR_INVALID_ENTITY_ID;
        }

        return component->_parent;
    }
}