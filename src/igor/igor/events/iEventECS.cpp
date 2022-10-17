// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/events/iEventECS.h>

#include <sstream>

namespace igor
{
#if 0
    iEventEntityCreated::iEventEntityCreated(const iEntityPtr entity, const iEntityComponentSystemPtr ecs)
        : iEvent(nullptr), _entity(entity), _ecs(ecs)
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

    iEntityComponentSystemPtr iEventEntityCreated::getECS() const
    {
        return _ecs;
    }

    iEventEntityDestroyed::iEventEntityDestroyed(const iEntityPtr entity, const iEntityComponentSystemPtr ecs)
        : iEvent(nullptr), _entity(entity), _ecs(ecs)
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

    iEntityComponentSystemPtr iEventEntityDestroyed::getECS() const
    {
        return _ecs;
    }
#endif
} // namespace igor