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

    iTransformComponent &iEntity::addTransformComponent(const iaVector3d &position, const iaVector3d &orientation, const iaVector3d &scale, iEntityID parent, const iaMatrixd &worldMatrix)
    {
        return _scene->getRegistry().emplace_or_replace<iTransformComponent>(_entity, position, orientation, scale, parent, worldMatrix);
    }

    iSpriteRendererComponent &iEntity::addSpriteRendererComponent(iTexturePtr texture, const iaColor4f &color, int32 zIndex)
    {
        return _scene->getRegistry().emplace_or_replace<iSpriteRendererComponent>(_entity, texture, color, zIndex);
    }

    iVelocityComponent &iEntity::addVelocityComponent(const iaVector3d &velocity, const iaVector3d &angularVelocity)
    {
        return _scene->getRegistry().emplace_or_replace<iVelocityComponent>(_entity, velocity, angularVelocity);
    }

    void iEntity::addBehaviour(const iBehaviourDelegate &behaviour)
    {
        if (_scene->getRegistry().try_get<iBehaviourComponent>(_entity) == nullptr)
        {
            _scene->getRegistry().emplace_or_replace<iBehaviourComponent>(_entity);
        }

        iBehaviourComponent &component = _scene->getRegistry().get<iBehaviourComponent>(_entity);
        for (auto &function : component._behaviour)
        {
            if (!function.isValid())
            {
                function = behaviour;
                return;
            }
        }

        con_err("can't add more then " << component._behaviour.size() << " behaviors");
    }

    void iEntity::removeBehaviour(const iBehaviourDelegate &behaviour)
    {
        if (_scene->getRegistry().try_get<iBehaviourComponent>(_entity) == nullptr)
        {
            con_err("no behaviour component available");
            return;
        }

        iBehaviourComponent &component = _scene->getRegistry().get<iBehaviourComponent>(_entity);
        for (auto b : component._behaviour)
        {
            if (b == behaviour)
            {
                b = nullptr;
                return;
            }
        }

        // TODO remove component

        con_err("can't remove given behaviour");
    }

}