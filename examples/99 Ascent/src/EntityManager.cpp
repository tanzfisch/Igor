// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include "EntityManager.h"

#include "Entity.h"

#include <iAACube.h>
#include <iaVector3.h>
#include <iApplication.h>
using namespace Igor;

#include <iaConsole.h>
using namespace IgorAux;

EntityManager::EntityManager()
{
    // TODO should be configurable
    _octree = new iOctree(iAACubed(iaVector3d(0, 0, 0), 1000000.0), 50.0, 100, 20);
}

EntityManager::~EntityManager()
{
    if (_isRunning)
    {
        iApplication::getInstance().unregisterApplicationHandleDelegate(iApplicationHandleDelegate(this, &EntityManager::handle));
    }

    if (_types.size())
    {
        con_err("possible mem leak. " << _types.size() << " entity types still registered");
    }

    if (_entities.size())
    {
        con_err("possible mem leak. " << _entities.size() << " entity still existing");
    }

    delete _octree;
}

void EntityManager::start()
{
    iApplication::getInstance().registerApplicationHandleDelegate(iApplicationHandleDelegate(this, &EntityManager::handle));
    _isRunning = true;
}

void EntityManager::stop()
{
    if (_isRunning)
    {
        iApplication::getInstance().unregisterApplicationHandleDelegate(iApplicationHandleDelegate(this, &EntityManager::handle));
    }

    _isRunning = false;
}

bool EntityManager::isRunning()
{
    return _isRunning;
}

Entity* EntityManager::createEntity(uint64 entityTypeID)
{
    Entity* result = nullptr;

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


Entity* EntityManager::createEntity(const iaString& entityTypeName)
{
    Entity* result = nullptr;

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

Entity* EntityManager::getEntity(uint64 id)
{
    Entity* result = nullptr;

    _mutexEntities.lock();
    auto iter = _entities.find(id);
    if (iter != _entities.end())
    {
        result = (*iter).second;
    }
    _mutexEntities.unlock();

    return result;
}

void EntityManager::destroyEntity(uint64 id)
{
    Entity* entity = getEntity(id);
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

void EntityManager::getEntities(const iSphered& sphere, vector<uint64>& result)
{
    _mutexOctree.lock();

    _octree->resetFilter();
    _octree->filter(sphere);
    _octree->getResult(result);

    _mutexOctree.unlock();
}

void EntityManager::getEntities(vector<uint64>& result)
{
    for (auto entity : _entities)
    {
        result.push_back(entity.first);
    }
}

void EntityManager::handle()
{
    vector<uint64> toDelete;

    for (auto entity : _entities)
    {
        if (entity.second->_state == Entity::EntityState::Created)
        {
            entity.second->init();
            entity.second->_state = Entity::EntityState::Running;
        }

        entity.second->handle();
        _octree->update(entity.second->_id, entity.second->_sphere);

        if (entity.second->_state == Entity::EntityState::Delete)
        {
            toDelete.push_back(entity.first);
        }
    }

    for (auto id : toDelete)
    {
        destroyEntity(id);
    }
}

uint64 EntityManager::registerEntityType(const iaString& entityTypeName, iCreateEntityInstance functionPointer)
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
uint64 EntityManager::calcHashValue(const iaString& text)
{
    std::hash<wstring> hashFunc;
    wstring keyValue = text.getData();
    return static_cast<uint64>(hashFunc(keyValue));
}

void EntityManager::unregisterEntityType(const iaString& entityTypeName)
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

void EntityManager::unregisterEntityType(uint64 entityTypeID)
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
