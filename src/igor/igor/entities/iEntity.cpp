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
        iBaseEntityComponent &component = _scene->getRegistry().get<iBaseEntityComponent>(_entity);
        return component._active;
    }

    void iEntity::setActive(bool active)
    {
        iBaseEntityComponent &component = _scene->getRegistry().get<iBaseEntityComponent>(_entity);
        component._active = active;
    }

    iEntityID iEntity::getID() const
    {
        return _entity;
    }

    bool iEntity::isValid() const
    {
        return _scene->getRegistry().valid(_entity);
    }

    iTransformComponent& iEntity::addTransformComponent(const iaVector3d &position, const iaVector3d& orientation, const iaVector3d &scale, iEntityID parent, const iaMatrixd &worldMatrix)
    {
        return _scene->getRegistry().emplace_or_replace<iTransformComponent>(_entity, position, orientation, scale, parent, worldMatrix);
    }    

    iSpriteRendererComponent & iEntity::addSpriteRendererComponent(iTexturePtr texture, const iaColor4f &color, int32 zIndex)
    {
        return _scene->getRegistry().emplace_or_replace<iSpriteRendererComponent>(_entity, texture, color, zIndex);
    }

}