// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/system/events/iEventMouse.h>

namespace igor
{

    iMouseKeyDownEvent_TMP::iMouseKeyDownEvent_TMP(const iKeyCode key)
        : _key(key)
    {
    }

    iEventKindMask iMouseKeyDownEvent_TMP::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    iKeyCode iMouseKeyDownEvent_TMP::getKey() const
    {
        return _key;
    }

    iMouseKeyUpEvent_TMP::iMouseKeyUpEvent_TMP(const iKeyCode key)
        : _key(key)
    {
    }

    iEventKindMask iMouseKeyUpEvent_TMP::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    iKeyCode iMouseKeyUpEvent_TMP::getKey() const
    {
        return _key;
    }

    iMouseKeyDoubleClickEvent_TMP::iMouseKeyDoubleClickEvent_TMP(const iKeyCode key)
        : _key(key)
    {
    }

    iEventKindMask iMouseKeyDoubleClickEvent_TMP::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    iKeyCode iMouseKeyDoubleClickEvent_TMP::getKey() const
    {
        return _key;
    }

    iMouseWheelEvent_TMP::iMouseWheelEvent_TMP(const int32 wheelDelta)
        : _wheelDelta(wheelDelta)
    {
    }

    iEventKindMask iMouseWheelEvent_TMP::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    int32 iMouseWheelEvent_TMP::getWheelDelta() const
    {
        return _wheelDelta;
    }

    iMouseMoveEvent_TMP::iMouseMoveEvent_TMP(const iaVector2i &from, const iaVector2i &to, iWindow *window)
        : _from(from), _to(to), _window(window)
    {
    }

    iEventKindMask iMouseMoveEvent_TMP::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    iWindow *iMouseMoveEvent_TMP::getWindow() const
    {
        return _window;
    }

    const iaVector2i &iMouseMoveEvent_TMP::getPosition() const
    {
        return _to;
    }

    const iaVector2i &iMouseMoveEvent_TMP::getLastPosition() const
    {
        return _from;
    }

} // namespace igor