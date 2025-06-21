// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/events/iEvent.h>

namespace igor
{

    iEvent::iEvent(iWindowPtr window, bool cantConsume)
        : _window(window), _cantConsume(cantConsume)
    {
    }

    bool iEvent::isOfKind(const iEventKind kind) const
    {
        return getEventKindMask() & (iEventKindMask)kind;
    }

    bool iEvent::isConsumed() const
    {
        return _consumed;
    }

    void iEvent::consume()
    {
        if(_cantConsume)
        {
            return;
        }
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