// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/events/iEventKeyboard.h>
#include <igor/system/iKeyboard.h>

namespace igor
{

    iEventKeyDown::iEventKeyDown(iWindow *window, const iKeyCode key)
        : iEvent(window), _key(key)
    {
    }

    iEventKindMask iEventKeyDown::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Keyboard;
    }

    const iaString iEventKeyDown::getInfo() const
    {
        std::wstringstream stream;
        stream << getName().getData() << "[" << _key << "]";

        return stream.str().c_str();
    }

    iKeyCode iEventKeyDown::getKey() const
    {
        return _key;
    }

    iKeyUpEvent_TMP::iKeyUpEvent_TMP(iWindow *window, const iKeyCode key)
        : iEvent(window), _key(key)
    {
    }

    iEventKindMask iKeyUpEvent_TMP::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Keyboard;
    }

    const iaString iKeyUpEvent_TMP::getInfo() const
    {
        std::wstringstream stream;
        stream << getName().getData() << "[" << _key << "]";

        return stream.str().c_str();
    }

    iKeyCode iKeyUpEvent_TMP::getKey() const
    {
        return _key;
    }

    iKeyASCIIEvent_TMP::iKeyASCIIEvent_TMP(iWindow *window, const char character)
        : iEvent(window), _character(character)
    {
    }

    iEventKindMask iKeyASCIIEvent_TMP::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Keyboard;
    }

    const iaString iKeyASCIIEvent_TMP::getInfo() const
    {
        std::wstringstream stream;
        stream << getName().getData() << "[" << _character << "]";

        return stream.str().c_str();
    }

    char iKeyASCIIEvent_TMP::getChar() const
    {
        return _character;
    }
} // namespace igor