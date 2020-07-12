// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/events/iEventMouse.h>
#include <igor/system/iKeyboard.h>

#include <sstream>

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
        stream << getName() << "[" << _key << "]";

        return stream.str().c_str();
    }

    iKeyCode iEventMouseKeyDown::getKey() const
    {
        return _key;
    }

    iEventMouseKeyUp::iEventMouseKeyUp(iWindow *window, const iKeyCode key)
        : iEvent(window), _key(key)
    {
    }

    iEventKindMask iEventMouseKeyUp::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    const iaString iEventMouseKeyUp::getInfo() const
    {
        std::wstringstream stream;
        stream << getName() << "[" << _key << "]";

        return stream.str().c_str();
    }

    iKeyCode iEventMouseKeyUp::getKey() const
    {
        return _key;
    }

    iEventMouseKeyDoubleClick::iEventMouseKeyDoubleClick(iWindow *window, const iKeyCode key)
        : iEvent(window), _key(key)
    {
    }

    iEventKindMask iEventMouseKeyDoubleClick::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    const iaString iEventMouseKeyDoubleClick::getInfo() const
    {
        std::wstringstream stream;
        stream << getName() << "[" << _key << "]";

        return stream.str().c_str();
    }

    iKeyCode iEventMouseKeyDoubleClick::getKey() const
    {
        return _key;
    }

    iEventMouseWheel::iEventMouseWheel(iWindow *window, const int32 wheelDelta)
        : iEvent(window), _wheelDelta(wheelDelta)
    {
    }

    iEventKindMask iEventMouseWheel::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    const iaString iEventMouseWheel::getInfo() const
    {
        std::wstringstream stream;
        stream << getName() << "[" << _wheelDelta << "]";

        return stream.str().c_str();
    }

    int32 iEventMouseWheel::getWheelDelta() const
    {
        return _wheelDelta;
    }

    iEventMouseMove::iEventMouseMove(iWindow *window, const iaVector2i &from, const iaVector2i &to)
        : iEvent(window), _from(from), _to(to)
    {
    }

    const iaString iEventMouseMove::getInfo() const
    {
        std::wstringstream stream;
        stream << getName() << "[" << _from << " -> " << _to << "]";

        return stream.str().c_str();
    }

    iEventKindMask iEventMouseMove::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    const iaVector2i &iEventMouseMove::getPosition() const
    {
        return _to;
    }

    const iaVector2i &iEventMouseMove::getLastPosition() const
    {
        return _from;
    }

} // namespace igor