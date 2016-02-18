// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntityManager.h>
#include <iSystem.h>
#include <iApplication.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
    iEntityManager::iEntityManager()
    {
        iApplication::getInstance().registerApplicationHandleDelegate(iApplicationHandleDelegate(this, &iEntityManager::onHandle));
    }

    iEntityManager::~iEntityManager()
    {
        iApplication::getInstance().unregisterApplicationHandleDelegate(iApplicationHandleDelegate(this, &iEntityManager::onHandle));

        if (_entityIDs.size())
        {
            con_err("possible mem leak! not all entities released");
        }
    }

    uint64 iEntityManager::createEntity()
    {
        uint64 result = _nextID++;
        _entityIDs.insert(result);
        return result;
    }

    void iEntityManager::onHandle()
    {
        for (auto system : _systems)
        {
            system->handle();
        }
    }

    void iEntityManager::destroyEntity(uint64 entityID)
    {
        auto iter = _entityIDs.find(entityID);
        con_assert(iter != _entityIDs.end(), "invalid param");

        if (iter != _entityIDs.end())
        {
            _entityIDs.erase(iter);

            for (auto system : _systems)
            {
                if (system->hasEntity(entityID))
                {
                    system->unregisterEntity(entityID);
                }
            }
        }
    }

    bool iEntityManager::isEntity(uint64 entityID)
    {
        auto iter = _entityIDs.find(entityID);
        if (iter != _entityIDs.end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void iEntityManager::registerSystem(iSystem* system)
    {
        auto iter = find(_systems.begin(), _systems.end(), system);
        if (iter == _systems.end())
        {
            _systems.push_back(system);
        }
        else
        {
            con_err("system already registered");
        }
    }

    void iEntityManager::unregisterSystem(iSystem* system)
    {
        auto iter = find(_systems.begin(), _systems.end(), system);
        if (iter != _systems.end())
        {
            _systems.erase(iter);
        }
        else
        {
            con_err("system not found");
        }
    }

}