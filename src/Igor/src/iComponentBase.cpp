// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iComponentBase.h>

namespace Igor
{

    uint64 iComponentBase::_nextComponentID = iComponentBase::INVALID_COMPONENT_ID + 1;
    mutex iComponentBase::_mutexID;

    iComponentBase::iComponentBase()
    {
        _mutexID.lock();
        _id = _nextComponentID++;
        _mutexID.unlock();
    }

    uint64 iComponentBase::getID() const
    {
        return _id;
    }

    iaString iComponentBase::getName() const
    {
        return _name;
    }

}