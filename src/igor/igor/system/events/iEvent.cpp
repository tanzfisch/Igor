// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/system/events/iEvent.h>

namespace igor
{

    bool iEvent::isOfKind(const iEventKind kind)
    {
        return getEventKindMask() & (iEventKindMask)kind;
    }

    iaString iEvent::getInfo() const
    {
        return getName();
    }

    bool iEvent::isConsumed() const
    {
        return _consumed;
    }

} // namespace igor