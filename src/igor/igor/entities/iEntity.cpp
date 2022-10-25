// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntity.h>

#include <igor/entities/iComponents.h>

namespace igor
{

    iEntity::iEntity(const entt::entity entity, iEntityScene &scene)
        : _entity(entity), _scene(&scene)
    {
    }

    const iaString iEntity::getName() const
    { 
        iNameComponent *component = _scene->_registry.try_get<iNameComponent>(_entity);
        if(component != nullptr)
        {
            return component->_name;
        }
        else
        {
            return "";
        }
    } 

    iEntityID iEntity::getID() const
    {
        return _entity;
    }
    
    bool iEntity::isValid() const
    {
        return _scene->_registry.valid(_entity);
    }

}