// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iComponent.h>

namespace Igor
{

    uint64 iComponent::_nextComponentID = iComponent::INVALID_COMPONENT_ID + 1;
    mutex iComponent::_mutexID;

    iComponent::iComponent()
    {
        _mutexID.lock();
        _id = _nextComponentID++;
        _mutexID.unlock();
    }

    uint64 iComponent::getID() const
    {
        return _id;
    }

}