// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntityManager.h>

#include <iEntity.h>
#include <iEntityLocatable.h>

namespace Igor
{

    iEntityManager::iEntityManager()
    {
        _octree = new iOctree(iAACubed(iaVector3d(0, 0, 0), 100000.0), 50.0, 100, 20);
    }

    iEntityManager::~iEntityManager()
    {
        delete _octree;
    }

    iEntity* iEntityManager::getEntity(uint64 entityID)
    {
        iEntity* result = nullptr;

        auto iter = _entities.find(entityID);
        if (iter != _entities.end())
        {
            result = (*iter).second;
        }

        return result;
    }

    void iEntityManager::registerEntity(iEntity* entity)
    {
        if (entity->_type == iEntityType::Locatable)
        {
            iEntityLocatable* locatable = static_cast<iEntityLocatable*>(entity);
            _octree->insert(entity->getID(), locatable->_sphere);
        }

        _entities[entity->getID()] = entity;
        
    }

    void iEntityManager::unregisterEntity(iEntity* entity)
    {
        if (entity->_type == iEntityType::Locatable)
        {
            _octree->remove(entity->getID());
        }

        auto iter = _entities.find(entity->getID());
        _entities.erase(iter);
    }

    void iEntityManager::getEntities(const iSphered& sphere, vector<uint64>& result)
    {
        _octree->resetFilter();
        _octree->filter(sphere);
        _octree->getResult(result);
    }

    void iEntityManager::getEntities(vector<uint64>& result)
    {
        for (auto entity : _entities)
        {
            result.push_back(entity.second->getID());
        }
    }

    void iEntityManager::killEntity(uint64 entityID)
    {
        _toDelete.push_back(entityID);
    }

    void iEntityManager::handle()
    {
        iEntityLocatable* locatable = nullptr;

        for (auto entity : _entities)
        {
            entity.second->handle();

            if (entity.second->_type == iEntityType::Locatable)
            {
                locatable = static_cast<iEntityLocatable*>(entity.second);
                _octree->update(locatable->_id, locatable->_sphere);
            }
        }

        for (auto id : _toDelete)
        {
            auto iter = _entities.find(id);
            if (iter != _entities.end())
            {
                delete (*iter).second;
            }
        }
    }

}