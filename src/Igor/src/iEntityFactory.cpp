// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntityFactory.h>
#include <iEntityControl.h>
#include <iEntity.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    uint32 iEntityFactory::_nextID = iEntity::INVALID_ENTITY_ID + 1;

    iEntityFactory::iEntityFactory()
    {
        iEntityControl::getInstance();
    }

    iEntityFactory::~iEntityFactory()
    {
        if (_entities.size())
        {
            con_err("possible mem leak! not all entities released");
        }

        auto entityIter = _entities.begin();
        while(_entities.end() != entityIter)
        {
            delete (*entityIter).second;
            entityIter++;
        }

        _entities.clear();
    }

    iEntity* iEntityFactory::createEntity()
    {
        iEntity* result = new iEntity();
        result->_id = _nextID++;
        _entities[result->getID()] = result;
        return result;
    }

    void iEntityFactory::destroyEntity(uint32 entityID)
    {
        destroyEntity(getEntity(entityID));
    }

    iEntity* iEntityFactory::getEntity(uint32 entityID)
    {
        con_assert(_entities.end() != _entities.find(entityID), "id not found");

        if (_entities.end() != _entities.find(entityID))
        {
            return _entities[entityID];
        }
        else
        {
            return nullptr;
        }
    }

    void iEntityFactory::destroyEntity(iEntity* entity)
    {
        con_assert(entity != nullptr, "zero pointer");

        if (entity != nullptr)
        {
            auto entityIter = _entities.find(entity->getID());
            if (_entities.end() != entityIter)
            {
                _entities.erase(entityIter);
            }
            else
            {
                con_err("entity id " << entity->getID() << " does not exist");
            }
        }
    }

    map<uint32, iEntity*>& iEntityFactory::getEntities()
    {
        return _entities;
    }

}