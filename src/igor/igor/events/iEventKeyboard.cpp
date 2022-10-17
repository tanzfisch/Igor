// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/events/iEventKeyboard.h>
#include <igor/system/iKeyboard.h>

#include <sstream>

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
        stream << getName() << "[" << _key << "]";

        return stream.str().c_str();
    }

    iKeyCode iEventKeyDown::getKey() const
    {
        return _key;
    }

    iEventKeyUp::iEventKeyUp(iWindow *window, const iKeyCode key)
        : iEvent(window), _key(key)
    {
    }

    iEventKindMask iEventKeyUp::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Keyboard;
    }

    const iaString iEventKeyUp::getInfo() const
    {
        std::wstringstream stream;
        stream << getName() << "[" << _key << "]";

        return stream.str().c_str();
    }

    iKeyCode iEventKeyUp::getKey() const
    {
        return _key;
    }

    iEventKeyASCII::iEventKeyASCII(iWindow *window, const char character)
        : iEvent(window), _character(character)
    {
    }

    iEventKindMask iEventKeyASCII::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Input | (iEventKindMask)iEventKind::Keyboard;
    }

    const iaString iEventKeyASCII::getInfo() const
    {
        std::wstringstream stream;
        stream << getName() << "[" << _character << "]";

        return stream.str().c_str();
    }

    char iEventKeyASCII::getChar() const
    {
        return _character;
    }
} // namespace igor