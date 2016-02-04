// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntityFactory.h>
#include <iEntity.h>
#include <iApplication.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    uint32 iEntityFactory::_nextID = iEntity::INVALID_ENTITY_ID + 1;

    iEntityFactory::iEntityFactory()
    {
        iApplication::getInstance().registerApplicationHandleDelegate(ApplicationHandleDelegate(this, &iEntityFactory::onHandle));
    }

    iEntityFactory::~iEntityFactory()
    {
        iApplication::getInstance().unregisterApplicationHandleDelegate(ApplicationHandleDelegate(this, &iEntityFactory::onHandle));

        if (_entities.size())
        {
            con_err("possible mem leak! not all entities released");
        }

        if (!_entityCreators.empty())
        {
            con_err("possible mem leak! Still " << _entityCreators.size() << " entity creators registered");
        }

        auto entityIter = _entities.begin();
        while(_entities.end() != entityIter)
        {
            delete (*entityIter).second;
            entityIter++;
        }

        _entities.clear();
    }

    void iEntityFactory::registerEntityCreator(const iaString& identifier, CreateEntity functionPointer)
    {
        int64 hashValue = identifier.getHashValue();

        auto generatorIter = _entityCreators.find(hashValue);
        if (generatorIter == _entityCreators.end())
        {
            _entityCreators[hashValue] = functionPointer;
        }
        else
        {
            con_err("entity creator " << identifier << " already registered");
        }
    }

    void iEntityFactory::unregisterEntityCreator(const iaString& identifier)
    {
        int64 hashValue = identifier.getHashValue();

        auto generatorIter = _entityCreators.find(hashValue);
        if (generatorIter != _entityCreators.end())
        {
            _entityCreators.erase(generatorIter);
        }
        else
        {
            con_err("entity creator " << identifier << " was not registered");
        }
    }

    iEntity* iEntityFactory::createEntity(const iaString& identifier)
    {
        iEntity* result = nullptr;
        int64 hashValue = identifier.getHashValue();

        auto iter = _entityCreators.find(hashValue);
        if (iter != _entityCreators.end())
        {
            result = (*iter).second();
        }

        con_assert(result != nullptr, "can't create entity of type " << identifier);

        if (result != nullptr)
        {
            _creationMutex.lock();
            result->_id = _nextID++;
            _creationQueue.push_back(result);
            _creationMutex.unlock();
        }

        return result;
    }

    void iEntityFactory::destroyEntity(uint32 entityID)
    {
        destroyEntity(getEntity(entityID));
    }

    iEntity* iEntityFactory::getEntity(uint32 entityID)
    {
        iEntity* result = nullptr;

        con_assert(_entities.end() != _entities.find(entityID), "id not found");
        if (_entities.end() != _entities.find(entityID))
        {
            result = _entities[entityID];
        }

        return result;
    }

    void iEntityFactory::destroyEntity(iEntity* entity)
    {
        con_assert(entity != nullptr, "zero pointer");

        if (entity != nullptr)
        {
            _destroyMutex.lock();
            _destroyQueue.push_back(entity);
            _destroyMutex.unlock();
        }
    }

    void iEntityFactory::getEntityIDs(vector<uint32>& entityIDs)
    {
        entityIDs.clear();

        for (auto entity : _entities)
        {
            entityIDs.push_back(entity.first);
        }
    }

    void iEntityFactory::onHandle()
    {
        vector<iEntity*> destroy;
        _destroyMutex.lock();
        destroy = std::move(_destroyQueue);
        _destroyMutex.unlock();

        for (auto entity : destroy)
        {
            auto entityIter = _entities.find(entity->getID());
            if (entityIter != _entities.end())
            {
                delete entityIter->second;
                _entities.erase(entityIter);
            }
            else
            {
                con_err("inconsystant data");
            }
        }

        vector<iEntity*> create;
        _creationMutex.lock();
        create = std::move(_creationQueue);
        _creationMutex.unlock();

        for (auto entity : create)
        {
            _entities[entity->getID()] = entity;
        }

        for (auto entity : _entities)
        {
            entity.second->onHandle();
        }
    }

}