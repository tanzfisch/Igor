// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/events/iEventECS.h>

#include <sstream>

namespace igor
{
    iEventEntityCreated::iEventEntityCreated(const iEntityPtr entity)
        : iEvent(nullptr), _entity(entity)
    {
    }

    const iaString iEventEntityCreated::getInfo() const
    {
        std::wstringstream stream;

        stream << getName() << "[" << std::hex << _entity << std::dec << "]";

        return stream.str().c_str();
    }

    iEntityPtr iEventEntityCreated::getEntity() const
    {
        return _entity;
    }

    iEventEntityDestroyed::iEventEntityDestroyed(const iEntityPtr entity)
        : iEvent(nullptr), _entity(entity)
    {
    }

    const iaString iEventEntityDestroyed::getInfo() const
    {
        std::wstringstream stream;

        stream << getName() << "[" << std::hex << _entity << std::dec << "]";

        return stream.str().c_str();
    }

    iEntityPtr iEventEntityDestroyed::getEntity() const
    {
        return _entity;
    }

} // namespace igor