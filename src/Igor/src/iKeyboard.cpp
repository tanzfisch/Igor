// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iKeyboard.h>
#include <iWindow.h>
#include <iaConsole.h>

#include <iOSEvent.h>

namespace Igor
{

    iKeyboard::iKeyboard()
    {
        for (int32 i = 0; i < static_cast<int32>(iKeyCode::KeyCodeCount) - 1; ++i)
        {
            _keys[i] = false;
        }
    }

    iKeyboard::~iKeyboard()
    {
        bool printWarn = false;

        if (_keyDownEventExt.hasDelegates())
        {
            printWarn = true;
            _keyDownEventExt.clear();
        }

        if (_keyUpEventExt.hasDelegates())
        {
            printWarn = true;
            _keyUpEventExt.clear();
        }

        if (_keyASCIIEvent.hasDelegates())
        {
            printWarn = true;
            _keyASCIIEvent.clear();
        }

        for (int32 i = 0; i < static_cast<int32>(iKeyCode::KeyCodeCount) - 1; ++i)
        {
            if (_keyDownEvent[i].hasDelegates())
            {
                printWarn = true;
                _keyDownEvent[i].clear();
            }

            if (_keyUpEvent[i].hasDelegates())
            {
                printWarn = true;
                _keyUpEvent[i].clear();
            }
        }

        if (printWarn)
        {
            con_warn("not all delegates unregistered");
        }
    }

    bool iKeyboard::keyPressed()
    {
        for (int32 i = 0; i < static_cast<int32>(iKeyCode::KeyCodeCount) - 1; ++i)
        {
            if (_keys[i])
            {
                return true;
            }
        }

        return false;
    }

    bool iKeyboard::getKey(iKeyCode keyCode)
    {
        return _keys[static_cast<unsigned int>(keyCode)];
    }

    void iKeyboard::registerKeyDownDelegate(iKeyDownDelegate keyDownDelegate)
    {
        _keyDownEventExt.append(keyDownDelegate);
    }

    void iKeyboard::registerKeyUpDelegate(iKeyUpDelegate keyUpDelegate)
    {
        _keyUpEventExt.append(keyUpDelegate);
    }

    void iKeyboard::registerKeyDownDelegate(iKeyDownSpecificDelegate keyDownDelegate, iKeyCode key)
    {
        _keyDownEvent[static_cast<unsigned int>(key)].append(keyDownDelegate);
    }

    void iKeyboard::registerKeyUpDelegate(iKeyUpSpecificDelegate keyUpDelegate, iKeyCode key)
    {
        _keyUpEvent[static_cast<unsigned int>(key)].append(keyUpDelegate);
    }

    void iKeyboard::registerKeyASCIIDelegate(iKeyASCIIDelegate keyASCIIDelegate)
    {
        _keyASCIIEvent.append(keyASCIIDelegate);
    }

    void iKeyboard::unregisterKeyDownDelegate(iKeyDownDelegate keyDownDelegate)
    {
        _keyDownEventExt.remove(keyDownDelegate);
    }

    void iKeyboard::unregisterKeyUpDelegate(iKeyUpDelegate keyUpDelegate)
    {
        _keyUpEventExt.remove(keyUpDelegate);
    }

    void iKeyboard::unregisterKeyDownDelegate(iKeyDownSpecificDelegate keyDownDelegate, iKeyCode key)
    {
        _keyDownEvent[static_cast<unsigned int>(key)].remove(keyDownDelegate);
    }

    void iKeyboard::unregisterKeyUpDelegate(iKeyUpSpecificDelegate keyUpDelegate, iKeyCode key)
    {
        _keyUpEvent[static_cast<unsigned int>(key)].remove(keyUpDelegate);
    }

    void iKeyboard::unregisterKeyASCIIDelegate(iKeyASCIIDelegate keyASCIIDelegate)
    {
        _keyASCIIEvent.remove(keyASCIIDelegate);
    }

    bool iKeyboard::registerDevice()
    {
        return true;
    }

    bool iKeyboard::onOSEvent(const void *data)
    {
        iOSEventWin32 *osevent = (iOSEventWin32*)data;

        iKeyCode current_key;

        switch (osevent->_uMsg)
        {
        case WM_SYSKEYDOWN:
            current_key = translate(static_cast<uint32>(osevent->_wParam), static_cast<uint32>(osevent->_lParam), true);
            if (current_key != iKeyCode::Undefined)
            {
                _keys[static_cast<unsigned int>(current_key)] = true;
                _keyDownEvent[static_cast<unsigned int>(current_key)]();
                _keyDownEventExt(current_key);
            }
            return true;

        case WM_KEYDOWN:
            current_key = translate(static_cast<uint32>(osevent->_wParam), static_cast<uint32>(osevent->_lParam), false);
            if (current_key != iKeyCode::Undefined)
            {
                _keys[static_cast<unsigned int>(current_key)] = true;
                _keyDownEvent[static_cast<unsigned int>(current_key)]();
                _keyDownEventExt(current_key);
            }
            return true;

        case WM_SYSKEYUP:
            current_key = translate(static_cast<uint32>(osevent->_wParam), static_cast<uint32>(osevent->_lParam), true);
            if (current_key != iKeyCode::Undefined)
            {
                _keys[static_cast<unsigned int>(current_key)] = false;
                _keyUpEvent[static_cast<unsigned int>(current_key)]();
                _keyUpEventExt(current_key);
            }
            return true;

        case WM_KEYUP:
            current_key = translate(static_cast<uint32>(osevent->_wParam), static_cast<uint32>(osevent->_lParam), false);
            if (current_key != iKeyCode::Undefined)
            {
                _keys[static_cast<unsigned int>(current_key)] = false;
                _keyUpEvent[static_cast<unsigned int>(current_key)]();
                _keyUpEventExt(current_key);
            }
            return true;

        case WM_DEADCHAR:
            return true;

        case WM_CHAR:
            _keyASCIIEvent((uint8)osevent->_wParam);
            return true;
        }
        return false;
    }

    iKeyCode iKeyboard::translate(uint32 wparam, uint32 lparam, bool syskey)
    {
        // keycode list
        // http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx

        const uint32 rightContorl = 0x1000000;
        const uint32 leftShift1 = 0x80000;
        const uint32 leftShift2 = 0x20000;
        const uint32 rightShift = 0x40000;

        if (syskey)
        {
            switch (wparam)
            {
            case 17: return iKeyCode::AltGr;
            case 18: return iKeyCode::LAlt;
            case 121: return iKeyCode::F10;
            default: return iKeyCode::Undefined;
            }
        }
        else
        {
            switch (wparam)
            {
            case 8: return iKeyCode::Backspace;
            case 9: return iKeyCode::Tab;
            case 12: return iKeyCode::Clear;
            case 13: return iKeyCode::Return;
            case 16:
                if (GetAsyncKeyState(VK_LSHIFT))
                {
                    return iKeyCode::LShift;
                }
                else if (GetAsyncKeyState(VK_RSHIFT))
                {
                    return iKeyCode::RShift;
                }
                else if ((lparam & leftShift1) &&
                    (lparam & leftShift2) &&
                    !(lparam & rightShift))
                {
                    return iKeyCode::LShift;
                }
                else
                {
                    return iKeyCode::RShift;
                }

            case 17:
                if (GetAsyncKeyState(VK_RMENU))
                {
                    return iKeyCode::AltGr;
                }
                else if (GetAsyncKeyState(VK_LCONTROL))
                {
                    return iKeyCode::LControl;
                }
                else if (GetAsyncKeyState(VK_RCONTROL))
                {
                    return iKeyCode::RControl;
                }
                else if (lparam & rightContorl)
                {
                    return iKeyCode::RControl;
                }
                else
                {
                    return iKeyCode::LControl;
                }

            case 19: return iKeyCode::Break;
            case 20: return iKeyCode::CapsLock;
            case 27: return iKeyCode::ESC;
            case 32: return iKeyCode::Space;
            case 33: return iKeyCode::PgUp;
            case 34: return iKeyCode::PgDown;
            case 35: return iKeyCode::End;
            case 36: return iKeyCode::Home;
            case 37: return iKeyCode::Left;
            case 38: return iKeyCode::Up;
            case 39: return iKeyCode::Right;
            case 40: return iKeyCode::Down;
            case 44: return iKeyCode::Print;
            case 45: return iKeyCode::Insert;
            case 46: return iKeyCode::Delete;
            case 48: return iKeyCode::Zero;
            case 49: return iKeyCode::One;
            case 50: return iKeyCode::Two;
            case 51: return iKeyCode::Three;
            case 52: return iKeyCode::Four;
            case 53: return iKeyCode::Five;
            case 54: return iKeyCode::Six;
            case 55: return iKeyCode::Seven;
            case 56: return iKeyCode::Eight;
            case 57: return iKeyCode::Nine;
            case 65: return iKeyCode::A;
            case 66: return iKeyCode::B;
            case 67: return iKeyCode::C;
            case 68: return iKeyCode::D;
            case 69: return iKeyCode::E;
            case 70: return iKeyCode::F;
            case 71: return iKeyCode::G;
            case 72: return iKeyCode::H;
            case 73: return iKeyCode::I;
            case 74: return iKeyCode::J;
            case 75: return iKeyCode::K;
            case 76: return iKeyCode::L;
            case 77: return iKeyCode::M;
            case 78: return iKeyCode::N;
            case 79: return iKeyCode::O;
            case 80: return iKeyCode::P;
            case 81: return iKeyCode::Q;
            case 82: return iKeyCode::R;
            case 83: return iKeyCode::S;
            case 84: return iKeyCode::T;
            case 85: return iKeyCode::U;
            case 86: return iKeyCode::V;
            case 87: return iKeyCode::W;
            case 88: return iKeyCode::X;
            case 89: return iKeyCode::Y;
            case 90: return iKeyCode::Z;
            case 91: return iKeyCode::WindowsLeft;
            case 92: return iKeyCode::WindowsRight;
            case 93: return iKeyCode::WindowsContext;
            case 96: return iKeyCode::NP_Zero;
            case 97: return iKeyCode::NP_One;
            case 98: return iKeyCode::NP_Two;
            case 99: return iKeyCode::NP_Three;
            case 100: return iKeyCode::NP_Four;
            case 101: return iKeyCode::NP_Five;
            case 102: return iKeyCode::NP_Six;
            case 103: return iKeyCode::NP_Seven;
            case 104: return iKeyCode::NP_Eight;
            case 105: return iKeyCode::NP_Nine;
            case 106: return iKeyCode::NP_Multiply;
            case 107: return iKeyCode::NP_Add;
            case 109: return iKeyCode::NP_Subtract;
            case 110: return iKeyCode::NP_Decimal;
            case 111: return iKeyCode::NP_Divide;
            case VK_F1: return iKeyCode::F1;
            case VK_F2: return iKeyCode::F2;
            case VK_F3: return iKeyCode::F3;
            case VK_F4: return iKeyCode::F4;
            case VK_F5: return iKeyCode::F5;
            case VK_F6: return iKeyCode::F6;
            case VK_F7: return iKeyCode::F7;
            case VK_F8: return iKeyCode::F8;
            case VK_F9: return iKeyCode::F9;
            case VK_F10: return iKeyCode::F10;
            case VK_F11: return iKeyCode::F11;
            case VK_F12: return iKeyCode::F12;
            case VK_F13: return iKeyCode::F13;
            case VK_F14: return iKeyCode::F14;
            case VK_F15: return iKeyCode::F15;
            case VK_F16: return iKeyCode::F16;
            case VK_F17: return iKeyCode::F17;
            case VK_F18: return iKeyCode::F18;
            case VK_F19: return iKeyCode::F19;
            case VK_F20: return iKeyCode::F20;
            case VK_F21: return iKeyCode::F21;
            case VK_F22: return iKeyCode::F22;
            case VK_F23: return iKeyCode::F23;
            case VK_F24: return iKeyCode::F24;
            case 144: return iKeyCode::NumLock;
            case 145: return iKeyCode::ScrollLock;
            case VK_OEM_1: return iKeyCode::OEM1;
            case VK_OEM_PLUS: return iKeyCode::OEMPlus;
            case VK_OEM_COMMA: return iKeyCode::OEMComma;
            case VK_OEM_MINUS: return iKeyCode::OEMMinus;
            case VK_OEM_PERIOD: return iKeyCode::OEMPeriod;
            case VK_OEM_2: return iKeyCode::OEM2;
            case VK_OEM_3: return iKeyCode::OEM3;
            case VK_OEM_4: return iKeyCode::OEM4;
            case VK_OEM_5: return iKeyCode::OEM5;
            case VK_OEM_6: return iKeyCode::OEM6;
            case VK_OEM_7: return iKeyCode::OEM7;
            case VK_OEM_102: return iKeyCode::OEM102;

            default:
                return iKeyCode::Undefined;
            }
        }
    }

};
