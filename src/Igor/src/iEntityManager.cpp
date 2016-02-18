// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntityManager.h>
#include <iSystem.h>
#include <iApplication.h>
#include <iEntityData.h>

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

        if (_entitys.size())
        {
            con_err("possible mem leak! not all entities released");
        }
    }

    uint64 iEntityManager::createEntity()
    {
        uint64 result = _nextID++;
        _entitys[result] = 0;
        return result;
    }

    void iEntityManager::destroyEntity(uint64 entityID)
    {
        auto iter = _entitys.find(entityID);
        con_assert(iter != _entitys.end(), "invalid param");

        if (iter != _entitys.end())
        {
            _entitys.erase(iter);
        }
    }

    bool iEntityManager::isEntity(uint64 entityID)
    {
        auto iter = _entitys.find(entityID);
        if (iter != _entitys.end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void iEntityManager::setEntityDataMask(uint64 entityID, uint64 dataMask)
    {
        auto iter = _entitys.find(entityID);
        if (iter != _entitys.end())
        {
            _entitys[entityID] = dataMask;
        }
        else
        {
            con_err("entity " << entityID << " is not an entity");
        }
    }

    uint64 iEntityManager::getEntityDataMask(uint64 entityID)
    {
        uint64 result = 0;

        auto iter = _entitys.find(entityID);
        if (iter != _entitys.end())
        {
            result = _entitys[entityID];
        }
        else
        {
            con_err("entity " << entityID << " is not an entity");
        }

        return result;
    }
    
    void iEntityManager::onHandle()
    {
        uint64 sysMask = 0;
        uint64 entityMask = 0;

        for (auto system : _systems)
        {
            sysMask = system.first;

            for (auto entity : _entitys)
            {
                entityMask = entity.second;

                if ((sysMask & entityMask) == sysMask)
                {
                    system.second->handle(entity.first);
                }
            }
        }
    }

    void iEntityManager::registerEntityData(iEntityData* entityData)
    {
        con_assert_sticky(entityData != nullptr, "zero pointer");

        if (entityData != nullptr)
        {
            auto iter = _entityData.find(entityData->getDataMask());
            if (iter == _entityData.end())
            {
                _entityData[entityData->getDataMask()] = entityData;
            }
            else
            {
                con_err("data with mask " << entityData->getDataMask() << " already registered");
            }
        }
    }

    void iEntityManager::unregisterEntityData(iEntityData* entityData)
    {
        con_assert_sticky(entityData != nullptr, "zero pointer");

        if (entityData != nullptr)
        {
            auto iter = _entityData.find(entityData->getDataMask());
            if (iter != _entityData.end())
            {
                _entityData.erase(iter);
            }
            else
            {
                con_err("data with mask " << entityData->getDataMask() << " was not registered");
            }
        }
    }

    iEntityData* iEntityManager::getEntityData(uint64 dataMask)
    {
        iEntityData* result = nullptr;

        auto iter = _entityData.find(dataMask);
        if (iter != _entityData.end())
        {
            result = (*iter).second;
        }

        return result;
    }

    void iEntityManager::registerSystem(iSystem* system)
    {
        auto iter = _systems.find(system->getDataMask());
        if (iter == _systems.end())
        {
            _systems[system->getDataMask()] = system;
            system->onRegistration();
        }
        else
        {
            con_err("system already registered");
        }
    }

    void iEntityManager::unregisterSystem(iSystem* system)
    {
        auto iter = _systems.find(system->getDataMask());
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