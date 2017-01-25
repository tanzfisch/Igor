// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntityManager.h>

#include <iSystem.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iEntityManager::~iEntityManager()
    {
        for (auto system : _systems)
        {
            system->_entityManager = nullptr;
        }

        for (auto entity : _entities)
        {
            delete entity.second;
        }
    }

    uint64 iEntityManager::createEntity()
    {
        iEntity* entity = new iEntity();
        _entities[entity->getID()] = entity;
        return entity->getID();
    }

    void iEntityManager::destroyEntity(uint64 entityID)
    {
        auto iter = _entities.find(entityID);
        if (iter != _entities.end())
        {
            _entities.erase(iter);
        }
        else
        {
            con_err("entity " << entityID << " not found");
        }
    }

    void iEntityManager::addComponent(uint64 entityID, uint64 componentTypeID)
    {

    }

    void iEntityManager::removeComponent(uint64 entityID, uint64 componentTypeID)
    {

    }

    void iEntityManager::registerComponent(uint64 componentTypeID, iCreateComponentInstance componentCreator)
    {
        auto iter = _componentTypes.find(componentTypeID);
        con_assert(iter == _componentTypes.end(), "component type id " << componentTypeID << " already used");
        if (iter == _componentTypes.end())
        {
            _componentTypes[componentTypeID] = componentCreator;
        }
        else
        {
            con_err("component type id " << componentTypeID << " already used");
        }
    }

    void iEntityManager::registerSystem(iSystem* system)
    {
        auto iter = find(_systems.begin(), _systems.end(), system);
        con_assert(iter == _systems.end(), "system already added");
        if (iter == _systems.end())
        {
            _systems.push_back(system);
            system->_entityManager = this;
        }
        else
        {
            con_err("system already added");
        }
    }

    void iEntityManager::handle()
    {
        for (auto system : _systems)
        {
            system->handle();
        }
    }

}