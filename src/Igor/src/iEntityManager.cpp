// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntityManager.h>

#include <iEntity.h>
#include <iAACube.h>
#include <iaVector3.h>
#include <iOctree.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iEntityManager::iEntityManager()
    {
        // TODO
        _octree = new iOctree(iAACubed(iaVector3d(0, 0, 0), 1000000.0), 50.0, 100, 20);
    }

    iEntityManager::~iEntityManager()
    {

    }

    iEntity* iEntityManager::createEntity(uint64 entityTypeID)
    {
        iEntity* result;

        auto iter = _types.find(entityTypeID);
        if (iter != _types.end())
        {
            result = (*iter).second();
            _mutexEntities.lock();
            _entities[result->getID()] = result;
            _mutexEntities.unlock();
        }
        else
        {
            con_err("type id " << hex << entityTypeID << " not registered");
        }

        _mutexOctree.lock();
        _octree->insert(result->getID(), result->getSphere());
        _mutexOctree.unlock();

        return result;
    }


    iEntity* iEntityManager::createEntity(const iaString& entityTypeName)
    {
        iEntity* result;

        int64 hashValue = calcHashValue(entityTypeName);

        auto iter = _types.find(hashValue);
        if (iter != _types.end())
        {
            result = (*iter).second();
            _mutexEntities.lock();
            _entities[result->getID()] = result;
            _mutexEntities.unlock();
        }
        else
        {
            con_err("type " << entityTypeName << " not registered");
        }

        _mutexOctree.lock();
        _octree->insert(result->getID(), result->getSphere());
        _mutexOctree.unlock();
        
        return result;
    }

    iEntity* iEntityManager::getEntity(uint64 id)
    {
        iEntity* result;

        _mutexEntities.lock();
        auto iter = _entities.find(id);
        if (iter != _entities.end())
        {
            result = (*iter).second;
        }
        _mutexEntities.unlock();

        return result;
    }

    void iEntityManager::destroyEntity(uint64 id)
    {
        iEntity* entity = getEntity(id);
        if (entity != nullptr)
        {
            entity->deinit();

            _mutexOctree.lock();
            _octree->remove(entity->getID());
            _mutexOctree.unlock();

            _mutexEntities.lock();
            auto iter = _entities.find(id);
            if (iter != _entities.end())
            {
                _entities.erase(id);
            }
            _mutexEntities.unlock();

            delete entity;
        }
    }

    void iEntityManager::getEntities(const iSphered& sphere, vector<uint64>& result)
    {
        _mutexOctree.lock();

        _octree->resetFilter();
        _octree->filter(sphere);
        _octree->getResult(result);

        _mutexOctree.unlock();
    }

    void iEntityManager::handle()
    {
        vector<uint64> toDelete;

        for (auto entity : _entities)
        {
            if (entity.second->_state == iEntity::iEntityState::Created)
            {
                entity.second->init();
                entity.second->_state = iEntity::iEntityState::Running;
            }

            entity.second->handle();
            _octree->update(entity.second->_id, entity.second->_sphere);

            if (entity.second->_state == iEntity::iEntityState::Delete)
            {
                toDelete.push_back(entity.first);
            }
        }

        for (auto id : toDelete)
        {
            destroyEntity(id);
        }
    }
    
    uint64 iEntityManager::registerEntityType(const iaString& entityTypeName, iCreateEntityInstance functionPointer)
    {
        uint64 hashValue = calcHashValue(entityTypeName);

        auto generatorIter = _types.find(hashValue);
        if (generatorIter == _types.end())
        {
            _types[hashValue] = functionPointer;
        }
        else
        {
            con_err("entity type " << entityTypeName << " already registered");
        }

        return hashValue;
    }

    /*! /todo shity workaround because my iaString class does not work as key for std::map yet
    */
    uint64 iEntityManager::calcHashValue(const iaString& text)
    {
        std::hash<wstring> hashFunc;
        wstring keyValue = text.getData();
        return static_cast<uint64>(hashFunc(keyValue));
    }

    void iEntityManager::unregisterEntityType(const iaString& entityTypeName)
    {
        int64 hashValue = calcHashValue(entityTypeName);

        auto generatorIter = _types.find(hashValue);
        if (generatorIter != _types.end())
        {
            _types.erase(generatorIter);
        }
        else
        {
            con_err("entity tape " << entityTypeName << " was not registered");
        }
    }

    void iEntityManager::unregisterEntityType(uint64 entityTypeID)
    {
        auto generatorIter = _types.find(entityTypeID);
        if (generatorIter != _types.end())
        {
            _types.erase(generatorIter);
        }
        else
        {
            con_err("entity tape " << hex << entityTypeID << " was not registered");
        }
    }

}