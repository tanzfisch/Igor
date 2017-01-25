// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntityManager.h>

#include <iSystem.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    struct Byte1 { uint8 bytes[1]; };
    struct Byte2 { uint8 bytes[2]; };
    struct Byte3 { uint8 bytes[3]; };
    struct Byte4 { uint8 bytes[4]; };
    struct Byte5 { uint8 bytes[5]; };
    struct Byte6 { uint8 bytes[6]; };
    struct Byte7 { uint8 bytes[7]; };
    struct Byte8 { uint8 bytes[8]; };
    struct Byte9 { uint8 bytes[9]; };
    struct Byte10 { uint8 bytes[10]; };
    struct Byte11 { uint8 bytes[11]; };
    struct Byte12 { uint8 bytes[12]; };
    struct Byte13 { uint8 bytes[13]; };
    struct Byte14 { uint8 bytes[14]; };
    struct Byte15 { uint8 bytes[15]; };
    struct Byte16 { uint8 bytes[16]; };
    struct Byte17 { uint8 bytes[17]; };
    struct Byte18 { uint8 bytes[18]; };
    struct Byte19 { uint8 bytes[19]; };
    struct Byte20 { uint8 bytes[20]; };
    struct Byte21 { uint8 bytes[21]; };
    struct Byte22 { uint8 bytes[22]; };
    struct Byte23 { uint8 bytes[23]; };
    struct Byte24 { uint8 bytes[24]; };
    struct Byte25 { uint8 bytes[25]; };
    struct Byte26 { uint8 bytes[26]; };
    struct Byte27 { uint8 bytes[27]; };
    struct Byte28 { uint8 bytes[28]; };
    struct Byte29 { uint8 bytes[29]; };
    struct Byte30 { uint8 bytes[30]; };
    struct Byte31 { uint8 bytes[31]; };
    struct Byte32 { uint8 bytes[32]; };

    iEntityManager::~iEntityManager()
    {
        for (auto system : _systems)
        {
            system->_entityManager = nullptr;
        }

        for (auto entity : _entities)
        {
            delete entity.second;
        }
    }

    uint64 iEntityManager::createEntity()
    {
        iEntity* entity = new iEntity();
        _entities[entity->getID()] = entity;
        return entity->getID();
    }

    void iEntityManager::destroyEntity(uint64 entityID)
    {
        auto iter = _entities.find(entityID);
        if (iter != _entities.end())
        {
            _entities.erase(iter);
        }
        else
        {
            con_err("entity " << entityID << " not found");
        }
    }

    void iEntityManager::addComponent(uint64 entityID, uint64 componentTypeID)
    {
    }

    void iEntityManager::removeComponent(uint64 entityID, uint64 componentTypeID)
    {

    }

    void* iEntityManager::getComponentBuffer(uint64 componentTypeID, uint32 componentSize)
    {
        void* result = nullptr;


        return result;
    }

    void* iEntityManager::createComponentBuffer(uint32 componentSize)
    {
        void* result = nullptr;
        switch (componentSize)
        {
        case 1: result = reinterpret_cast<void*>(new map<uint64, Byte1>()); break;
        case 2: result = reinterpret_cast<void*>(new map<uint64, Byte2>()); break;
        case 3: result = reinterpret_cast<void*>(new map<uint64, Byte3>()); break;
        case 4: result = reinterpret_cast<void*>(new map<uint64, Byte4>()); break;
        case 5: result = reinterpret_cast<void*>(new map<uint64, Byte5>()); break;
        case 6: result = reinterpret_cast<void*>(new map<uint64, Byte6>()); break;
        case 7: result = reinterpret_cast<void*>(new map<uint64, Byte7>()); break;
        case 8: result = reinterpret_cast<void*>(new map<uint64, Byte8>()); break;
        case 9: result = reinterpret_cast<void*>(new map<uint64, Byte9>()); break;
        case 10: result = reinterpret_cast<void*>(new map<uint64, Byte10>()); break;
        case 11: result = reinterpret_cast<void*>(new map<uint64, Byte11>()); break;
        case 12: result = reinterpret_cast<void*>(new map<uint64, Byte12>()); break;
        case 13: result = reinterpret_cast<void*>(new map<uint64, Byte13>()); break;
        case 14: result = reinterpret_cast<void*>(new map<uint64, Byte14>()); break;
        case 15: result = reinterpret_cast<void*>(new map<uint64, Byte15>()); break;
        case 16: result = reinterpret_cast<void*>(new map<uint64, Byte16>()); break;
        case 17: result = reinterpret_cast<void*>(new map<uint64, Byte17>()); break;
        case 18: result = reinterpret_cast<void*>(new map<uint64, Byte18>()); break;
        case 19: result = reinterpret_cast<void*>(new map<uint64, Byte19>()); break;
        case 20: result = reinterpret_cast<void*>(new map<uint64, Byte20>()); break;
        case 21: result = reinterpret_cast<void*>(new map<uint64, Byte21>()); break;
        case 22: result = reinterpret_cast<void*>(new map<uint64, Byte22>()); break;
        case 23: result = reinterpret_cast<void*>(new map<uint64, Byte23>()); break;
        case 24: result = reinterpret_cast<void*>(new map<uint64, Byte24>()); break;
        case 25: result = reinterpret_cast<void*>(new map<uint64, Byte25>()); break;
        case 26: result = reinterpret_cast<void*>(new map<uint64, Byte26>()); break;
        case 27: result = reinterpret_cast<void*>(new map<uint64, Byte27>()); break;
        case 28: result = reinterpret_cast<void*>(new map<uint64, Byte28>()); break;
        case 29: result = reinterpret_cast<void*>(new map<uint64, Byte29>()); break;
        case 30: result = reinterpret_cast<void*>(new map<uint64, Byte30>()); break;
        case 31: result = reinterpret_cast<void*>(new map<uint64, Byte31>()); break;
        case 32: result = reinterpret_cast<void*>(new map<uint64, Byte32>()); break;

        default:
            con_err("unsupported size");
        }

        return result;
    }

    void iEntityManager::registerComponent(uint64 componentTypeID, uint32 componentSize)
    {
        auto iter = _components.find(componentTypeID);
        con_assert(iter == _components.end(), "component type id " << componentTypeID << " already used");
        if (iter == _components.end())
        {
            _components[componentTypeID] = pair<uint32, void*>(componentSize, createComponentBuffer(componentSize));
        }
        else
        {
            con_err("component type id " << componentTypeID << " already used");
        }
    }

    void iEntityManager::registerSystem(iSystem* system) // todo register directly with components needed
    {
        auto iter = find(_systems.begin(), _systems.end(), system);
        con_assert(iter == _systems.end(), "system already added");
        if (iter == _systems.end())
        {
            _systems.push_back(system);
            system->_entityManager = this;
        }
        else
        {
            con_err("system already added");
        }
    }

    void iEntityManager::handle()
    {
        for (auto system : _systems)
        {
            system->handle();
        }
    }

}