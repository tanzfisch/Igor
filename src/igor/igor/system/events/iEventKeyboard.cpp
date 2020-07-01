// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/system/events/iEventKeyboard.h>

namespace igor
{

    iKeyDownEvent_TMP::iKeyDownEvent_TMP(const iKeyCode key)
        : _key(key)
    {
    }

    iEventKindMask iKeyDownEvent_TMP::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Keyboard;
    }

    iKeyCode iKeyDownEvent_TMP::getKey() const
    {
        return _key;
    }

    iKeyUpEvent_TMP::iKeyUpEvent_TMP(const iKeyCode key)
        : _key(key)
    {
    }

    iEventKindMask iKeyUpEvent_TMP::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Keyboard;
    }

    iKeyCode iKeyUpEvent_TMP::getKey() const
    {
        return _key;
    }

    iKeyASCIIEvent_TMP::iKeyASCIIEvent_TMP(const char character)
        : _character(character)
    {
    }

    iEventKindMask iKeyASCIIEvent_TMP::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Keyboard;
    }

    char iKeyASCIIEvent_TMP::getChar() const
    {
        return _character;
    }
} // namespace igor