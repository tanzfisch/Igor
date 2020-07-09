// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/events/iEventMouse.h>
#include <igor/system/iKeyboard.h>

namespace igor
{

    iEventMouseKeyDown::iEventMouseKeyDown(iWindow *window, const iKeyCode key)
        : iEvent(window), _key(key)
    {
    }

    iEventKindMask iEventMouseKeyDown::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    const iaString iEventMouseKeyDown::getInfo() const
    {
        std::wstringstream stream;
        stream << getName().getData() << "[" << _key << "]";

        return stream.str().c_str();
    }

    iKeyCode iEventMouseKeyDown::getKey() const
    {
        return _key;
    }

    iMouseKeyUpEvent_TMP::iMouseKeyUpEvent_TMP(iWindow *window, const iKeyCode key)
        : iEvent(window), _key(key)
    {
    }

    iEventKindMask iMouseKeyUpEvent_TMP::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    const iaString iMouseKeyUpEvent_TMP::getInfo() const
    {
        std::wstringstream stream;
        stream << getName().getData() << "[" << _key << "]";

        return stream.str().c_str();
    }

    iKeyCode iMouseKeyUpEvent_TMP::getKey() const
    {
        return _key;
    }

    iMouseKeyDoubleClickEvent_TMP::iMouseKeyDoubleClickEvent_TMP(iWindow *window, const iKeyCode key)
        : iEvent(window), _key(key)
    {
    }

    iEventKindMask iMouseKeyDoubleClickEvent_TMP::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    const iaString iMouseKeyDoubleClickEvent_TMP::getInfo() const
    {
        std::wstringstream stream;
        stream << getName().getData() << "[" << _key << "]";

        return stream.str().c_str();
    }

    iKeyCode iMouseKeyDoubleClickEvent_TMP::getKey() const
    {
        return _key;
    }

    iMouseWheelEvent_TMP::iMouseWheelEvent_TMP(iWindow *window, const int32 wheelDelta)
        : iEvent(window), _wheelDelta(wheelDelta)
    {
    }

    iEventKindMask iMouseWheelEvent_TMP::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    const iaString iMouseWheelEvent_TMP::getInfo() const
    {
        std::wstringstream stream;
        stream << getName().getData() << "[" << _wheelDelta << "]";

        return stream.str().c_str();
    }

    int32 iMouseWheelEvent_TMP::getWheelDelta() const
    {
        return _wheelDelta;
    }

    iMouseMoveEvent_TMP::iMouseMoveEvent_TMP(iWindow *window, const iaVector2i &from, const iaVector2i &to)
        : iEvent(window), _from(from), _to(to)
    {
    }

    const iaString iMouseMoveEvent_TMP::getInfo() const
    {
        std::wstringstream stream;
        stream << getName().getData() << "[" << _from << " -> " << _to << "]";

        return stream.str().c_str();
    }

    iEventKindMask iMouseMoveEvent_TMP::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
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