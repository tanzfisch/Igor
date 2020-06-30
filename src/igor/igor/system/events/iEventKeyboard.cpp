// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/system/events/iEventKeyboard.h>

namespace igor
{

    iKeyDownEvent::iKeyDownEvent(const iKeyCode key)
        : _key(key)
    {
    }

    iEventType iKeyDownEvent::getEventType() const
    {
        return iEventType::iKeyDownEvent;
    }

    iEventKindMask iKeyDownEvent::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Keyboard;
    }

    iKeyCode iKeyDownEvent::getKey() const
    {
        return _key;
    }

    iKeyUpEvent::iKeyUpEvent(const iKeyCode key)
        : _key(key)
    {
    }

    iEventType iKeyUpEvent::getEventType() const
    {
        return iEventType::iKeyUpEvent;
    }

    iEventKindMask iKeyUpEvent::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Keyboard;
    }

    iKeyCode iKeyUpEvent::getKey() const
    {
        return _key;
    }

    iKeyASCIIEvent::iKeyASCIIEvent(const char key)
        : _key(key)
    {
    }

    iEventType iKeyASCIIEvent::getEventType() const
    {
        return iEventType::iKeyASCIIEvent;
    }

    iEventKindMask iKeyASCIIEvent::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Keyboard;
    }

    char iKeyASCIIEvent::getKey() const
    {
        return _key;
    }
} // namespace igor