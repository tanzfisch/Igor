// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntity.h>

#include <igor/entities/iComponents.h>

#include <igor/entities/iEntityManager.h> // deprecated

namespace igor
{

    iEntity::iEntity(const entt::entity entity, iEntityScene &scene)
        : _entity(entity), _scene(&scene)
    {
    }

    const iaString iEntity::getName() const
    { 
        NameComponent *component = _scene->getRegistry().try_get<NameComponent>(_entity);
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
        return _scene->getRegistry().valid(_entity);
    }

    ////////////// old deprecated stuff
    const uint64 iEntity_Old::INVALID_ENTITY_ID = 0;
    uint64 iEntity_Old::_nextID = iEntity_Old::INVALID_ENTITY_ID + 1;
    iaMutex iEntity_Old::_mutexID;

    iEntity_Old::iEntity_Old()
    {
        _mutexID.lock();
        _id = _nextID++;
        _mutexID.unlock();

        iEntityManager::getInstance().registerEntity(this);

        _type = iEntityType::Base;
    }

    iEntity_Old::~iEntity_Old()
    {
        iEntityManager::getInstance().unregisterEntity(this);
    }

    uint64 iEntity_Old::getID() const
    {
        return _id;
    }

    iEntityType iEntity_Old::getType() const
    {
        return _type;
    }

}