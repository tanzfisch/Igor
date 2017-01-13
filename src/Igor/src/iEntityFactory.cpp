// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntityFactory.h>

namespace Igor
{

    iEntityFactory::iEntityFactory()
    {

    }

    iEntityFactory::~iEntityFactory()
    {

    }

    iEntity* iEntityFactory::createEntity(const iaString& entityType) const
    {

    }

    iEntity* iEntityFactory::getEntity(uint64 id) const
    {

    }

    void iEntityFactory::destroyEntity(uint64 id) const
    {

    }
    
    void registerEntityType(const iaString& entityType, iCreateEntityInstance functionPointer)
    {

    }

    void unregisterEntityType(const iaString& identifier)
    {

    }

}