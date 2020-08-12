// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntityManager.h>

#include <igor/entities/iEntity.h>
#include <igor/entities/iEntityLocatable.h>

namespace igor
{

    iEntityManager::iEntityManager()
    {
        _octree = new iOctree(iAACubed(iaVector3d(0, 0, 0), 100000.0), 50.0, 100, 20);
    }

    iEntityManager::~iEntityManager()
    {
        if (_entities.size() > 0)
        {
            con_err("possible mem leak. " << _entities.size() << " entitties still registered");
        }

        delete _octree;
    }

    iEntity *iEntityManager::getEntity(uint64 entityID)
    {
        iEntity *result = nullptr;

        auto iter = _entities.find(entityID);
        if (iter != _entities.end())
        {
            result = (*iter).second;
        }

        return result;
    }

    void iEntityManager::registerEntity(iEntity *entity)
    {
        if (entity->_type == iEntityType::Locatable)
        {
            iEntityLocatable *locatable = static_cast<iEntityLocatable *>(entity);
            locatable->_sphere._center = locatable->getCurrentPos();
            _octree->insert(entity->getID(), locatable->_sphere);
        }

        _entities[entity->getID()] = entity;
    }

    void iEntityManager::unregisterEntity(iEntity *entity)
    {
        if (entity->_type == iEntityType::Locatable)
        {
            _octree->remove(entity->getID());
        }

        auto iter = _entities.find(entity->getID());
        _entities.erase(iter);
    }

    void iEntityManager::getEntities(const iSphered &sphere, std::vector<uint64> &entities)
    {
        _octree->clearFilter();
        _octree->addFilter(sphere);
        _octree->filter();
        entities = _octree->getResult();
    }

    void iEntityManager::getEntities(std::vector<uint64> &entities)
    {
        for (auto entity : _entities)
        {
            entities.push_back(entity.second->getID());
        }
    }

    void iEntityManager::destroyEntity(uint64 entityID)
    {
        _toDelete.push_back(entityID);
    }

    void iEntityManager::handle()
    {
        iEntityLocatable *locatable = nullptr;

        for (auto entity : _entities)
        {
            entity.second->handle();

            if (entity.second->_type == iEntityType::Locatable)
            {
                locatable = static_cast<iEntityLocatable *>(entity.second);
                locatable->_sphere._center = locatable->getCurrentPos();
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

} // namespace igor