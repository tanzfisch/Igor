// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntityManager.h>

#include <iEntity.h>
#include <iAACube.h>
#include <iaVector3.h>
#include <iOctree.h>

#include <iEntityPositioned.h>

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

    iEntity* iEntityManager::createEntity(const iaString& entityType)
    {
        iEntity* result;

        int64 hashValue = calcHashValue(entityType);

        auto iter = _types.find(hashValue);
        if (iter != _types.end())
        {
            result = (*iter).second();
            _mutexEntities.lock();
            _entities[result->getID()] = result;
            _positionedEntitties.push_back(static_cast<iEntityPositioned*>(result));
            _mutexEntities.unlock();
        }
        else
        {
            con_err("type " << entityType << " not registered");
        }

        if (result->hasPosition())
        {
            _mutexOctree.lock();
            _octree->insert(result->getID(), static_cast<iEntityPositioned*>(result)->getSphere());
            _mutexOctree.unlock();
        }

        result->init();

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

            if (entity->hasPosition())
            {
                _mutexOctree.lock();
                _octree->remove(entity->getID());
                _mutexOctree.unlock();

                _mutexEntities.lock();
                auto iter = find(_positionedEntitties.begin(), _positionedEntitties.end(), entity);
                if (iter != _positionedEntitties.end())
                {
                    _positionedEntitties.erase(iter);
                }
                _mutexEntities.unlock();
            }

            _mutexEntities.lock();
            auto iter = _entities.find(id);
            if (iter != _entities.end())
            {
                _entities.erase(id);
            }
            _mutexEntities.unlock();
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
        for (auto entity : _entities)
        {
            entity.second->handle();
        }

        for (auto entity : _positionedEntitties)
        {
            _octree->update(entity->_id, entity->_sphere);
        }

/*        vector<uint64> toDelete;
        for (auto entity : _entities)
        {
            if (entity.second->getHealth() <= 0.0f)
            {
                toDelete.push_back(entity.first);
            }
        }

        for (auto id : toDelete)
        {
            auto iter = _entities.find(id);
            if (iter != _entities.end())
            {
                delete (*iter).second;
            }
        }*/
    }
    
    void iEntityManager::registerEntityType(const iaString& entityType, iCreateEntityInstance functionPointer)
    {
        uint64 hashValue = calcHashValue(entityType);

        auto generatorIter = _types.find(hashValue);
        if (generatorIter == _types.end())
        {
            _types[hashValue] = functionPointer;
        }
        else
        {
            con_err("entity type " << entityType << " already registered");
        }
    }

    /*! /todo shity workaround because my iaString class does not work as key for std::map yet
    */
    uint64 iEntityManager::calcHashValue(const iaString& text)
    {
        std::hash<wstring> hashFunc;
        wstring keyValue = text.getData();
        return static_cast<uint64>(hashFunc(keyValue));
    }

    void iEntityManager::unregisterEntityType(const iaString& entityType)
    {
        int64 hashValue = calcHashValue(entityType);

        auto generatorIter = _types.find(hashValue);
        if (generatorIter != _types.end())
        {
            _types.erase(generatorIter);
        }
        else
        {
            con_err("entity tape " << entityType << " was not registered");
        }
    }

}