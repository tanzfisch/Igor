// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntityFactory.h>

#include <iEntity.h>
#include <iAACube.h>
#include <iaVector3.h>
#include <iOctree.h>

#include <iEntityPositioned.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iEntityFactory::iEntityFactory()
    {
        _octree = new iOctree(iAACubed(iaVector3d(0, 0, 0), 1000000.0), 50.0, 100, 20);

        //registerEntityType("Positioned", &iEntityPositioned::createInstance);
    }

    iEntityFactory::~iEntityFactory()
    {

    }

    void iEntityFactory::setScene(iScene* scene)
    {
        _scene = scene;
    }

    iScene* iEntityFactory::getScene() const
    {
        return _scene;
    }

    iEntity* iEntityFactory::createEntity(const iaString& entityType)
    {
        iEntity* result;

        int64 hashValue = calcHashValue(entityType);

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
            con_err("type " << entityType << " not registered");
        }

        return result;
    }

    iEntity* iEntityFactory::getEntity(uint64 id)
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

    void iEntityFactory::destroyEntity(uint64 id)
    {
        _mutexEntities.lock();
        auto iter = _entities.find(id);
        if (iter != _entities.end())
        {
            _entities.erase(id);
        }
        _mutexEntities.unlock();
    }
    
    void iEntityFactory::registerEntityType(const iaString& entityType, iCreateEntityInstance functionPointer)
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
    uint64 iEntityFactory::calcHashValue(const iaString& text)
    {
        std::hash<wstring> hashFunc;
        wstring keyValue = text.getData();
        return static_cast<uint64>(hashFunc(keyValue));
    }

    void iEntityFactory::unregisterEntityType(const iaString& entityType)
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