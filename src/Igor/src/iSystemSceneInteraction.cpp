// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iSystemSceneInteraction.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
    iSystemSceneInteraction::iSystemSceneInteraction(iScene* scene)
    {
        con_assert_sticky(scene != nullptr, "zero pointer");
        _scene = scene;
    }

    iSystemSceneInteraction::~iSystemSceneInteraction()
    {

    }

    void iSystemSceneInteraction::setCallbacks(uint64 entityID, InitNodes initFunction, DeInitNodes deinitFunction)
    {
        auto iter = _transformNodes.find(entityID);
        if (iter != _transformNodes.end())
        {
            (*iter).second._deinitFunction = deinitFunction;
            (*iter).second._initFunction = initFunction;
        }
    }

    void iSystemSceneInteraction::registerEntity(uint64 entityID)
    {
        con_assert(_transformNodes.find(entityID) == _transformNodes.end(), "already registered");

        if (_transformNodes.find(entityID) == _transformNodes.end())
        {
            EntityData entityData;
            _transformNodes[entityID] = entityData;
        }
    }

    void iSystemSceneInteraction::unregisterEntity(uint64 entityID)
    {
        con_assert(_transformNodes.find(entityID) != _transformNodes.end(), "entity id not found");

        auto iter = _transformNodes.find(entityID);
        if (iter != _transformNodes.end())
        {
            _transformNodes.erase(iter);
        }
    }

    bool iSystemSceneInteraction::hasEntity(uint64 entityID)
    {
        auto iter = _transformNodes.find(entityID);
        if (iter != _transformNodes.end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void iSystemSceneInteraction::init(uint64 entityID)
    {
        auto iter = _transformNodes.find(entityID);
        if (iter != _transformNodes.end())
        {
            if (!(*iter).second._initialized)
            {
                (*iter).second._transformNodeID = (*iter).second._initFunction(entityID);
                (*iter).second._initialized = true;
            }
        }
    }

    void iSystemSceneInteraction::deinit(uint64 entityID)
    {
        auto iter = _transformNodes.find(entityID);
        if (iter != _transformNodes.end())
        {
            if ((*iter).second._initialized)
            {
                (*iter).second._deinitFunction(entityID, (*iter).second._transformNodeID);
                (*iter).second._initialized = false;
            }
        }
    }
    
    bool iSystemSceneInteraction::isInitialized(uint64 entityID) const
    {
        auto iter = _transformNodes.find(entityID);
        if (iter != _transformNodes.end())
        {
            return (*iter).second._initialized;
        }

        return false;
    }

}