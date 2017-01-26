// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntityManager.h>

#include <iSystem.h>
#include <iComponent.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iEntityManager::~iEntityManager()
    {
        for (auto system : _systems)
        {
            system.second->_entityManager = nullptr;
        }

        for (auto entity : _entities)
        {
            delete entity.second;
        }
    }

    uint64 iEntityManager::createEntity()
    {
        uint64 result = iEntity::INVALID_ENTITY_ID;
        iEntity* entity = new iEntity();
        result = entity->_id;
        _entities[result] = entity;
        _entityComponents[result];
        return result;
    }

    void iEntityManager::destroyEntity(uint64 entityID)
    {
        auto iterEnt = _entities.find(entityID);
        if (iterEnt != _entities.end())
        {
            _entities.erase(iterEnt);
        }
        else
        {
            con_err("entity " << entityID << " not found");
        }

        auto iterComp = _entityComponents.find(entityID);
        if (iterComp != _entityComponents.end())
        {
            _entityComponents.erase(iterComp);
        }
        else
        {
            con_err("entity " << entityID << " not found");
        }
    }

    void iEntityManager::linkComponent(uint64 entityID, uint64 componentID)
    {
        con_assert(_components.find(componentID) != _components.end(), "component " << componentID << " not found");
        _components[componentID]->linkEntity(entityID);
        _entityComponents[entityID].push_back(componentID);

        findMatch(entityID);
    }

    void iEntityManager::findMatch(uint64 entityID)
    {
        for (auto system : _systems)
        {
            bool match = true;
            if (_entityComponents[entityID].size() >= system.second->_dependencies.size())
            {
                for (auto dependency : system.second->_dependencies)
                {
                    bool found = false;
                    for (auto entdep : _entityComponents[entityID])
                    {
                        if (dependency == entdep)
                        {
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                    {
                        match = false;
                        break;
                    }
                }
            }

            if (match)
            {
                auto& entities = system.second->_entities;
                auto iter = find(entities.begin(), entities.end(), entityID);
                if (iter == entities.end())
                {
                    system.second->_entities.push_back(entityID);
                }
            }
        }
    }

    void iEntityManager::clearMatch(uint64 entityID)
    {
        for (auto system : _systems)
        {
            auto& entities = system.second->_entities;
            auto iter = find(entities.begin(), entities.end(), entityID);
            if (iter != entities.end())
            {
                bool match = true;
                if (_entityComponents[entityID].size() < system.second->_dependencies.size())
                {
                    match = false;
                }
                else
                {
                    for (auto dependency : system.second->_dependencies)
                    {
                        bool found = false;
                        for (auto entdep : _entityComponents[entityID])
                        {
                            if (dependency == entdep)
                            {
                                found = true;
                                break;
                            }
                        }

                        if (!found)
                        {
                            match = false;
                            break;
                        }
                    }
                }

                if (!match)
                {
                    entities.erase(iter);
                }
            }
        }
    }

    void iEntityManager::unlinkComponent(uint64 entityID, uint64 componentID)
    {
        _components[componentID]->unlinkEntity(entityID);

        auto& components = _entityComponents[entityID];
        auto iterComp = find(components.begin(), components.end(), componentID);
        components.erase(iterComp);

        clearMatch(entityID);
    }

    iComponent* iEntityManager::getComponent(uint64 componentID)
    {
        return _components[componentID];
    }

    void iEntityManager::registerComponent(iComponent* component)
    {
        con_assert(component != nullptr, "zero pointer");
        if (component != nullptr)
        {
            auto iter = _components.find(component->_id);
            con_assert(iter == _components.end(), "component type id " << component->_id << " already used");
            if (iter == _components.end())
            {
                _components[component->_id] = component;
            }
            else
            {
                con_err("component type id " << component->_id << " already used");
            }
        }
        else
        {
            con_err("invalid parameter");
        }
    }

    void iEntityManager::registerSystem(iSystem* system, const vector<uint64>& dependencies)
    {
        auto iter = _systems.find(system->_id);
        con_assert(iter == _systems.end(), "system already added");
        if (iter == _systems.end())
        {
            _systems[system->_id] = system;
            for (auto component : dependencies)
            {
                system->_dependencies.push_back(component);
            }
            system->_entityManager = this;
        }
        else
        {
            con_err("system already added");
        }
    }

    void iEntityManager::registerSystem(iSystem* system, const vector<iComponent*>& dependencies)
    {
        vector<uint64> temp;

        for (auto iter : dependencies)
        {
            temp.push_back(iter->getID());
        }

        registerSystem(system, temp);
    }

    void iEntityManager::handle()
    {
        for (auto system : _systems)
        {
            system.second->handle();
        }
    }

}