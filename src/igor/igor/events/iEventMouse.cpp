// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/events/iEventMouse.h>
#include <igor/system/iKeyboard.h>

#include <sstream>

namespace igor
{

    iEventMouseKeyDown::iEventMouseKeyDown(iWindowPtr window, const iKeyCode key, const iaVector2i &pos)
        : iEvent(window), _key(key), _pos(pos)
    {
    }

    iEventKindMask iEventMouseKeyDown::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    const iaString iEventMouseKeyDown::getInfo() const
    {
        std::wstringstream stream;
        stream << getName() << "[" << _key << "] @" << _pos;

        return stream.str().c_str();
    }

    iKeyCode iEventMouseKeyDown::getKey() const
    {
        return _key;
    }

    const iaVector2i &iEventMouseKeyDown::getPosition() const
    {
        return _pos;
    }

    iEventMouseKeyUp::iEventMouseKeyUp(iWindowPtr window, const iKeyCode key, const iaVector2i &pos)
        : iEvent(window), _key(key), _pos(pos)
    {
    }

    iEventKindMask iEventMouseKeyUp::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    const iaString iEventMouseKeyUp::getInfo() const
    {
        std::wstringstream stream;
        stream << getName() << "[" << _key << "] @" << _pos;

        return stream.str().c_str();
    }

    iKeyCode iEventMouseKeyUp::getKey() const
    {
        return _key;
    }

    const iaVector2i &iEventMouseKeyUp::getPosition() const
    {
        return _pos;
    }

    iEventMouseKeyDoubleClick::iEventMouseKeyDoubleClick(iWindowPtr window, const iKeyCode key, const iaVector2i &pos)
        : iEvent(window), _key(key), _pos(pos)
    {
    }

    iEventKindMask iEventMouseKeyDoubleClick::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Mouse;
    }

    const iaString iEventMouseKeyDoubleClick::getInfo() const
    {
        std::wstringstream stream;
        stream << getName() << "[" << _key << "] @" << _pos;

        return stream.str().c_str();
    }

    iKeyCode iEventMouseKeyDoubleClick::getKey() const
    {
        return _key;
    }

    const iaVector2i &iEventMouseKeyDoubleClick::getPosition() const
    {
        return _pos;
    }

    iEventMouseWheel::iEventMouseWheel(iWindowPtr window, const int32 wheelDelta)
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

    iEventMouseMove::iEventMouseMove(iWindowPtr window, const iaVector2i &from, const iaVector2i &to)
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