// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/events/iEvent.h>

namespace igor
{

    iEvent::iEvent(iWindowPtr window)
        : _window(window)
    {
    }

    bool iEvent::isOfKind(const iEventKind kind)
    {
        return getEventKindMask() & (iEventKindMask)kind;
    }

    bool iEvent::isConsumed() const
    {
        return _consumed;
    }

    void iEvent::consume()
    {
        _consumed = true;
    }

    iWindowPtr iEvent::getWindow() const
    {
        return _window;
    }

    const iaString iEvent::getInfo() const
    {
        return getName();
    }

    std::wostream &operator<<(std::wostream &stream, const iEvent &event)
    {
        stream << event.getInfo();
        return stream;
    }

} // namespace igor