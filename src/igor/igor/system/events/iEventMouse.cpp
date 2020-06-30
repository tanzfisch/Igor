// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/system/events/iEventMouse.h>

namespace igor
{

    iMouseKeyDownEvent::iMouseKeyDownEvent(const iKeyCode key)
        : _key(key)
    {
    }

    iEventType iMouseKeyDownEvent::getEventType() const
    {
        return iEventType::iMouseKeyDownEvent;
    }

    iEventKindMask iMouseKeyDownEvent::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    iKeyCode iMouseKeyDownEvent::getKey() const
    {
        return _key;
    }

    iMouseKeyUpEvent::iMouseKeyUpEvent(const iKeyCode key)
        : _key(key)
    {
    }

    iEventType iMouseKeyUpEvent::getEventType() const
    {
        return iEventType::iMouseKeyUpEvent;
    }

    iEventKindMask iMouseKeyUpEvent::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    iKeyCode iMouseKeyUpEvent::getKey() const
    {
        return _key;
    }

    iMouseKeyDoubleClickEvent::iMouseKeyDoubleClickEvent(const iKeyCode key)
        : _key(key)
    {
    }

    iEventType iMouseKeyDoubleClickEvent::getEventType() const
    {
        return iEventType::iMouseKeyUpEvent;
    }

    iEventKindMask iMouseKeyDoubleClickEvent::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    iKeyCode iMouseKeyDoubleClickEvent::getKey() const
    {
        return _key;
    }

    iMouseWheelEvent::iMouseWheelEvent(const int32 wheelDelta)
        : _wheelDelta(wheelDelta)
    {
    }

    iEventType iMouseWheelEvent::getEventType() const
    {
        return iEventType::iMouseWheelEvent;
    }

    iEventKindMask iMouseWheelEvent::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    int32 iMouseWheelEvent::getWheelDelta() const
    {
        return _wheelDelta;
    }

    iMouseMoveEvent::iMouseMoveEvent(const iaVector2i &from, const iaVector2i &to, iWindow *window)
        : _from(from), _to(to), _window(window)
    {
    }

    iEventType iMouseMoveEvent::getEventType() const
    {
        return iEventType::iMouseMoveEvent;
    }

    iEventKindMask iMouseMoveEvent::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    iWindow *iMouseMoveEvent::getWindow() const
    {
        return _window;
    }
    iaVector2i *iMouseMoveEvent::getPosition() const
    {
        return _to;
    }
    iaVector2i *iMouseMoveEvent::getLastPosition() const
    {
        return _from;
    }

} // namespace igor