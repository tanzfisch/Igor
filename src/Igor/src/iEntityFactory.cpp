// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntityFactory.h>

#include <iEntity.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iEntityFactory::iEntityFactory()
    {

    }

    iEntityFactory::~iEntityFactory()
    {

    }

    iEntity* iEntityFactory::createEntity(const iaString& entityType)
    {
        iEntity* result;

        int64 hashValue = calcHashValue(entityType);

        _mutexTypes.lock();
        auto iter = _types.find(hashValue);
        if (iter != _types.end())
        {
            result = (*iter).second();
        }
        _mutexTypes.unlock();

        _entities[result->getID()] = result;

        return result;
    }

    iEntity* iEntityFactory::getEntity(uint64 id) const
    {
        iEntity* result;

        auto iter = _entities.find(id);
        if (iter != _entities.end())
        {
            result = (*iter).second;
        }

        return result;
    }

    void iEntityFactory::destroyEntity(uint64 id)
    {
        auto iter = _entities.find(id);
        if (iter != _entities.end())
        {
            _entities.erase(id);
        }
    }
    
    void iEntityFactory::registerEntityType(const iaString& entityType, iCreateEntityInstance functionPointer)
    {
        uint64 hashValue = calcHashValue(entityType);

        _mutexTypes.lock();
        auto generatorIter = _types.find(hashValue);
        if (generatorIter == _types.end())
        {
            _types[hashValue] = functionPointer;
        }
        else
        {
            con_err("entity type " << entityType << " already registered");
        }
        _mutexTypes.unlock();
    }

    uint64 iEntityFactory::calcHashValue(const iaString& text)
    {
        std::hash<wstring> hashFunc;
        wstring keyValue = text.getData();
        return static_cast<uint64>(hashFunc(keyValue));
    }

    void iEntityFactory::unregisterEntityType(const iaString& entityType)
    {
        int64 hashValue = calcHashValue(entityType);

        _mutexTypes.lock();
        auto generatorIter = _types.find(hashValue);
        if (generatorIter != _types.end())
        {
            _types.erase(generatorIter);
        }
        else
        {
            con_err("entity tape " << entityType << " was not registered");
        }
        _mutexTypes.unlock();
    }

}