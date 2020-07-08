// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/events/iEvent.h>

namespace igor
{

    iEvent::iEvent(iWindow *window)
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

    iWindow *iEvent::getWindow() const
    {
        return _window;
    }

    std::wostream &operator<<(std::wostream &stream, const iEventType &eventType)
    {
        const static iaString text[] = {
            "iKeyDownEvent",
            "iKeyUpEvent",
            "iKeyASCIIEvent",

            "iMouseKeyDownEvent",
            "iMouseKeyUpEvent",
            "iMouseKeyDoubleClickEvent",
            "iMouseMoveEvent",
            "iMouseWheelEvent",

            "iWindowCloseEvent",
            "iWindowResizeEvent"};

        stream << text[static_cast<int>(eventType)].getData();
        return stream;
    }

} // namespace igor