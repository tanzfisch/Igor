// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/system/iKeyboard.h>

#include <igor/system/iApplication.h>
#include <igor/system/iWindow.h>
#include <igor/events/iEventKeyboard.h>
#include <igor/system/iDefinesWindows.h>
#include <igor/system/iDefinesLinux.h>

#include <iaux/system/iaConsole.h>

namespace igor
{

    const static iaString s_keyNames[] = {
        L"Undefined",
        L"Backspace",
        L"Tab",
        L"Return",
        L"LShift",
        L"LControl",
        L"Break",
        L"CapsLock",
        L"ESC",
        L"Space",
        L"PgUp",
        L"PgDown",
        L"End",
        L"Home",
        L"Left",
        L"Up",
        L"Right",
        L"Down",
        L"Insert",
        L"Delete",
        L"0",
        L"1",
        L"2",
        L"3",
        L"4",
        L"5",
        L"6",
        L"7",
        L"8",
        L"9",
        L"A",
        L"B",
        L"C",
        L"D",
        L"E",
        L"F",
        L"G",
        L"H",
        L"I",
        L"J",
        L"K",
        L"L",
        L"M",
        L"N",
        L"O",
        L"P",
        L"Q",
        L"R",
        L"S",
        L"T",
        L"U",
        L"V",
        L"W",
        L"X",
        L"Y",
        L"Z",
        L"WindowsLeft",
        L"WindowsRight",
        L"WindowsContext",
        L"NP_0",
        L"NP_1",
        L"NP_2",
        L"NP_3",
        L"NP_4",
        L"NP_5",
        L"NP_6",
        L"NP_7",
        L"NP_8",
        L"NP_9",
        L"NP_Multiply",
        L"NP_Add",
        L"NP_Subtract",
        L"NP_Decimal",
        L"NP_Divide",
        L"F1",
        L"F2",
        L"F3",
        L"F4",
        L"F5",
        L"F6",
        L"F7",
        L"F8",
        L"F9",
        L"F10",
        L"F11",
        L"F12",
        L"F13",
        L"F14",
        L"F15",
        L"F16",
        L"F17",
        L"F18",
        L"F19",
        L"F20",
        L"F21",
        L"F22",
        L"F23",
        L"F24",
        L"NumLock",
        L"ScrollLock",
        L"OEM1",
        L"OEMPlus",
        L"OEMComma",
        L"OEMMinus",
        L"OEMPeriod",
        L"OEM2",
        L"OEM3",
        L"OEM4",
        L"OEM5",
        L"OEM6",
        L"OEM7",
        L"OEM102",
        L"RControl",
        L"AltGr",
        L"Alt",
        L"Print",
        L"RShift",
        L"Enter",
        L"MouseLeft",
        L"MouseMiddle",
        L"MouseRight",
        L"MouseButton4",
        L"MouseButton5",
        L"MouseWheelUp",
        L"MouseWheelDown"};

    std::wostream &operator<<(std::wostream &stream, const iKeyCode &keyCode)
    {
        stream << s_keyNames[static_cast<int>(keyCode)].getData();
        return stream;
    }

    class iKeyboardImpl
    {
        friend class iKeyboard;

    public:
        iKeyboardImpl()
        {
            for (int32 i = 0; i < static_cast<int32>(iKeyCode::KeyCodeCount) - 1; ++i)
            {
                _keys[i] = false;
            }
        }

        virtual ~iKeyboardImpl() = default;

        virtual bool initDevice(const void *data) = 0;
        virtual void deinitDevice() = 0;
        virtual bool onOSEvent(const void *data) = 0;

    protected:
        bool _keys[static_cast<uint32>(iKeyCode::KeyCodeCount)];
        iKeyDownSpecificEvent _keyDownEvent[static_cast<uint32>(iKeyCode::KeyCodeCount)];
        iKeyUpSpecificEvent _keyUpEvent[static_cast<uint32>(iKeyCode::KeyCodeCount)];
        iKeyASCIIEvent _keyASCIIEvent;
        iKeyDownEvent _keyDownEventExt;
        iKeyUpEvent _keyUpEventExt;
        iWindow *_window = nullptr;
    };

#ifdef __IGOR_WINDOWS__
    class iKeyboardImplWindows : public iKeyboardImpl
    {
    public:
        iKeyboardImplWindows() = default;
        virtual ~iKeyboardImplWindows() = default;

        bool initDevice(const void *data) override
        {
            const iOSEventregisterData *specificData = static_cast<const iOSEventregisterData *>(data);
            _window = specificData->_window;

            return true;
        }
        void deinitDevice() override
        {
            // nothing to do
        }

        bool onOSEvent(const void *data) override
        {
            const iOSEvent *osevent = static_cast<const iOSEvent *>(data);

            iKeyCode currentKey;

            switch (osevent->_uMsg)
            {
            case WM_SYSKEYDOWN:
                currentKey = translate(static_cast<uint32>(osevent->_wParam), static_cast<uint32>(osevent->_lParam), true);
                if (currentKey != iKeyCode::Undefined)
                {
                    _keys[static_cast<unsigned int>(currentKey)] = true;
                    _keyDownEvent[static_cast<unsigned int>(currentKey)]();
                    _keyDownEventExt(currentKey);
                }
                return true;

            case WM_KEYDOWN:
                currentKey = translate(static_cast<uint32>(osevent->_wParam), static_cast<uint32>(osevent->_lParam), false);
                if (currentKey != iKeyCode::Undefined)
                {
                    _keys[static_cast<unsigned int>(currentKey)] = true;
                    _keyDownEvent[static_cast<unsigned int>(currentKey)]();
                    _keyDownEventExt(currentKey);
                }
                return true;

            case WM_SYSKEYUP:
                currentKey = translate(static_cast<uint32>(osevent->_wParam), static_cast<uint32>(osevent->_lParam), true);
                if (currentKey != iKeyCode::Undefined)
                {
                    _keys[static_cast<unsigned int>(currentKey)] = false;
                    _keyUpEvent[static_cast<unsigned int>(currentKey)]();
                    _keyUpEventExt(currentKey);
                }
                return true;

            case WM_KEYUP:
                currentKey = translate(static_cast<uint32>(osevent->_wParam), static_cast<uint32>(osevent->_lParam), false);
                if (currentKey != iKeyCode::Undefined)
                {
                    _keys[static_cast<unsigned int>(currentKey)] = false;
                    _keyUpEvent[static_cast<unsigned int>(currentKey)]();
                    _keyUpEventExt(currentKey);
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

    private:
        iKeyCode translate(uint32 wparam, uint32 lparam, bool syskey)
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
                case 17:
                    return iKeyCode::AltGr;
                case 18:
                    return iKeyCode::Alt;
                case 121:
                    return iKeyCode::F10;
                default:
                    return iKeyCode::Undefined;
                }
            }
            else
            {
                switch (wparam)
                {
                case 8:
                    return iKeyCode::Backspace;
                case 9:
                    return iKeyCode::Tab;
                case 13:
                    return iKeyCode::Return;
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

                case 19:
                    return iKeyCode::Break;
                case 20:
                    return iKeyCode::CapsLock;
                case 27:
                    return iKeyCode::ESC;
                case 32:
                    return iKeyCode::Space;
                case 33:
                    return iKeyCode::PgUp;
                case 34:
                    return iKeyCode::PgDown;
                case 35:
                    return iKeyCode::End;
                case 36:
                    return iKeyCode::Home;
                case 37:
                    return iKeyCode::Left;
                case 38:
                    return iKeyCode::Up;
                case 39:
                    return iKeyCode::Right;
                case 40:
                    return iKeyCode::Down;
                case 44:
                    return iKeyCode::Print;
                case 45:
                    return iKeyCode::Insert;
                case 46:
                    return iKeyCode::Delete;
                case 48:
                    return iKeyCode::Zero;
                case 49:
                    return iKeyCode::One;
                case 50:
                    return iKeyCode::Two;
                case 51:
                    return iKeyCode::Three;
                case 52:
                    return iKeyCode::Four;
                case 53:
                    return iKeyCode::Five;
                case 54:
                    return iKeyCode::Six;
                case 55:
                    return iKeyCode::Seven;
                case 56:
                    return iKeyCode::Eight;
                case 57:
                    return iKeyCode::Nine;
                case 65:
                    return iKeyCode::A;
                case 66:
                    return iKeyCode::B;
                case 67:
                    return iKeyCode::C;
                case 68:
                    return iKeyCode::D;
                case 69:
                    return iKeyCode::E;
                case 70:
                    return iKeyCode::F;
                case 71:
                    return iKeyCode::G;
                case 72:
                    return iKeyCode::H;
                case 73:
                    return iKeyCode::I;
                case 74:
                    return iKeyCode::J;
                case 75:
                    return iKeyCode::K;
                case 76:
                    return iKeyCode::L;
                case 77:
                    return iKeyCode::M;
                case 78:
                    return iKeyCode::N;
                case 79:
                    return iKeyCode::O;
                case 80:
                    return iKeyCode::P;
                case 81:
                    return iKeyCode::Q;
                case 82:
                    return iKeyCode::R;
                case 83:
                    return iKeyCode::S;
                case 84:
                    return iKeyCode::T;
                case 85:
                    return iKeyCode::U;
                case 86:
                    return iKeyCode::V;
                case 87:
                    return iKeyCode::W;
                case 88:
                    return iKeyCode::X;
                case 89:
                    return iKeyCode::Y;
                case 90:
                    return iKeyCode::Z;
                case 91:
                    return iKeyCode::WindowsLeft;
                case 92:
                    return iKeyCode::WindowsRight;
                case 93:
                    return iKeyCode::WindowsContext;
                case 96:
                    return iKeyCode::NP_Zero;
                case 97:
                    return iKeyCode::NP_One;
                case 98:
                    return iKeyCode::NP_Two;
                case 99:
                    return iKeyCode::NP_Three;
                case 100:
                    return iKeyCode::NP_Four;
                case 101:
                    return iKeyCode::NP_Five;
                case 102:
                    return iKeyCode::NP_Six;
                case 103:
                    return iKeyCode::NP_Seven;
                case 104:
                    return iKeyCode::NP_Eight;
                case 105:
                    return iKeyCode::NP_Nine;
                case 106:
                    return iKeyCode::NP_Multiply;
                case 107:
                    return iKeyCode::NP_Add;
                case 109:
                    return iKeyCode::NP_Subtract;
                case 110:
                    return iKeyCode::NP_Decimal;
                case 111:
                    return iKeyCode::NP_Divide;
                case VK_F1:
                    return iKeyCode::F1;
                case VK_F2:
                    return iKeyCode::F2;
                case VK_F3:
                    return iKeyCode::F3;
                case VK_F4:
                    return iKeyCode::F4;
                case VK_F5:
                    return iKeyCode::F5;
                case VK_F6:
                    return iKeyCode::F6;
                case VK_F7:
                    return iKeyCode::F7;
                case VK_F8:
                    return iKeyCode::F8;
                case VK_F9:
                    return iKeyCode::F9;
                case VK_F10:
                    return iKeyCode::F10;
                case VK_F11:
                    return iKeyCode::F11;
                case VK_F12:
                    return iKeyCode::F12;
                case VK_F13:
                    return iKeyCode::F13;
                case VK_F14:
                    return iKeyCode::F14;
                case VK_F15:
                    return iKeyCode::F15;
                case VK_F16:
                    return iKeyCode::F16;
                case VK_F17:
                    return iKeyCode::F17;
                case VK_F18:
                    return iKeyCode::F18;
                case VK_F19:
                    return iKeyCode::F19;
                case VK_F20:
                    return iKeyCode::F20;
                case VK_F21:
                    return iKeyCode::F21;
                case VK_F22:
                    return iKeyCode::F22;
                case VK_F23:
                    return iKeyCode::F23;
                case VK_F24:
                    return iKeyCode::F24;
                case 144:
                    return iKeyCode::NumLock;
                case 145:
                    return iKeyCode::ScrollLock;
                case VK_OEM_1:
                    return iKeyCode::OEM1;
                case VK_OEM_PLUS:
                    return iKeyCode::OEMPlus;
                case VK_OEM_COMMA:
                    return iKeyCode::OEMComma;
                case VK_OEM_MINUS:
                    return iKeyCode::OEMMinus;
                case VK_OEM_PERIOD:
                    return iKeyCode::OEMPeriod;
                case VK_OEM_2:
                    return iKeyCode::OEM2;
                case VK_OEM_3:
                    return iKeyCode::OEM3;
                case VK_OEM_4:
                    return iKeyCode::OEM4;
                case VK_OEM_5:
                    return iKeyCode::OEM5;
                case VK_OEM_6:
                    return iKeyCode::OEM6;
                case VK_OEM_7:
                    return iKeyCode::OEM7;
                case VK_OEM_102:
                    return iKeyCode::OEM102;

                default:
                    return iKeyCode::Undefined;
                }
            }
        }
    };
#endif

#ifdef __IGOR_LINUX__
    class iKeyboardImplLinux : public iKeyboardImpl
    {
    public:
        iKeyboardImplLinux() = default;
        ~iKeyboardImplLinux() = default;

        bool initDevice(const void *data) override
        {
            const iOSEventregisterData *specificData = static_cast<const iOSEventregisterData *>(data);
            _window = specificData->_window;

            return true;
        }

        void deinitDevice() override
        {
            // nothing to do
        }

        iKeyCode translate(unsigned int key)
        {
            switch (key)
            {
            case 22:
                return iKeyCode::Backspace;
            case 23:
                return iKeyCode::Tab;
            case 36:
                return iKeyCode::Return;
            case 50:
                return iKeyCode::LShift;
            case 37:
                return iKeyCode::LControl;
            case 64:
                return iKeyCode::Alt;
            case 127:
                return iKeyCode::Break;
            case 66:
                return iKeyCode::CapsLock;
            case 9:
                return iKeyCode::ESC;
            case 65:
                return iKeyCode::Space;
            case 112:
                return iKeyCode::PgUp;
            case 117:
                return iKeyCode::PgDown;
            case 115:
                return iKeyCode::End;
            case 110:
                return iKeyCode::Home;
            case 113:
                return iKeyCode::Left;
            case 111:
                return iKeyCode::Up;
            case 114:
                return iKeyCode::Right;
            case 116:
                return iKeyCode::Down;
            case 118:
                return iKeyCode::Insert;
            case 119:
                return iKeyCode::Delete;
            case 10:
                return iKeyCode::One;
            case 11:
                return iKeyCode::Two;
            case 12:
                return iKeyCode::Three;
            case 13:
                return iKeyCode::Four;
            case 14:
                return iKeyCode::Five;
            case 15:
                return iKeyCode::Six;
            case 16:
                return iKeyCode::Seven;
            case 17:
                return iKeyCode::Eight;
            case 18:
                return iKeyCode::Nine;
            case 19:
                return iKeyCode::Zero;
            case 38:
                return iKeyCode::A;
            case 56:
                return iKeyCode::B;
            case 54:
                return iKeyCode::C;
            case 40:
                return iKeyCode::D;
            case 26:
                return iKeyCode::E;
            case 41:
                return iKeyCode::F;
            case 42:
                return iKeyCode::G;
            case 43:
                return iKeyCode::H;
            case 31:
                return iKeyCode::I;
            case 44:
                return iKeyCode::J;
            case 45:
                return iKeyCode::K;
            case 46:
                return iKeyCode::L;
            case 58:
                return iKeyCode::M;
            case 57:
                return iKeyCode::N;
            case 32:
                return iKeyCode::O;
            case 33:
                return iKeyCode::P;
            case 24:
                return iKeyCode::Q;
            case 27:
                return iKeyCode::R;
            case 39:
                return iKeyCode::S;
            case 28:
                return iKeyCode::T;
            case 30:
                return iKeyCode::U;
            case 55:
                return iKeyCode::V;
            case 25:
                return iKeyCode::W;
            case 53:
                return iKeyCode::X;
            case 52:
                return iKeyCode::Y;
            case 29:
                return iKeyCode::Z;
            case 134:
                return iKeyCode::WindowsRight;
            case 135:
                return iKeyCode::WindowsContext;
            case 90:
                return iKeyCode::NP_Zero;
            case 87:
                return iKeyCode::NP_One;
            case 88:
                return iKeyCode::NP_Two;
            case 89:
                return iKeyCode::NP_Three;
            case 83:
                return iKeyCode::NP_Four;
            case 84:
                return iKeyCode::NP_Five;
            case 85:
                return iKeyCode::NP_Six;
            case 79:
                return iKeyCode::NP_Seven;
            case 80:
                return iKeyCode::NP_Eight;
            case 81:
                return iKeyCode::NP_Nine;
            case 63:
                return iKeyCode::NP_Multiply;
            case 86:
                return iKeyCode::NP_Add;
            case 82:
                return iKeyCode::NP_Subtract;
            case 129:
                return iKeyCode::NP_Decimal;
            case 106:
                return iKeyCode::NP_Divide;
            case 67:
                return iKeyCode::F1;
            case 68:
                return iKeyCode::F2;
            case 69:
                return iKeyCode::F3;
            case 70:
                return iKeyCode::F4;
            case 71:
                return iKeyCode::F5;
            case 72:
                return iKeyCode::F6;
            case 73:
                return iKeyCode::F7;
            case 74:
                return iKeyCode::F8;
            case 75:
                return iKeyCode::F9;
            case 76:
                return iKeyCode::F10;
            case 95:
                return iKeyCode::F11;
            case 96:
                return iKeyCode::F12;
            case 77:
                return iKeyCode::NumLock;
            case 78:
                return iKeyCode::ScrollLock;
            case 34:
                return iKeyCode::OEM1;
            case 35:
                return iKeyCode::OEMPlus;
            case 59:
                return iKeyCode::OEMComma;
            case 61:
                return iKeyCode::OEMMinus;
            case 60:
                return iKeyCode::OEMPeriod;
            case 51:
                return iKeyCode::OEM2;
            case 47:
                return iKeyCode::OEM3;
            case 20:
                return iKeyCode::OEM4;
            case 49:
                return iKeyCode::OEM5;
            case 21:
                return iKeyCode::OEM6;
            case 48:
                return iKeyCode::OEM7;
            case 94:
                return iKeyCode::OEM102;
            case 105:
                return iKeyCode::RControl;
            case 108:
                return iKeyCode::AltGr;
            case 62:
                return iKeyCode::RShift;
            case 104:
                return iKeyCode::Enter;
            default:
                return iKeyCode::Undefined;
            }
        }

        int keycode2charcode(XKeyEvent *xevt)
        {
            char buf[32];
            KeySym symbolic;
            int nConv = XLookupString(xevt, buf, sizeof(buf), &symbolic, 0);
            int charCode = (int)(unsigned char)buf[0];

            if (nConv == 0)
                return -1; // unknown

            return charCode;
        }

        bool onOSEvent(const void *data) override
        {
            const iOSEvent *osevent = static_cast<const iOSEvent *>(data);
            XEvent xevent = osevent->_event;

            int characterCode;
            iKeyCode currentKey;

            switch (osevent->_event.type)
            {
            case KeyPress:
                characterCode = keycode2charcode(&xevent.xkey);
                if (characterCode != -1)
                {
                    _keyASCIIEvent(static_cast<char>(characterCode));
                    iApplication::getInstance().onEvent(iEventPtr(new iKeyASCIIEvent_TMP(_window, characterCode)));
                }

                currentKey = translate(xevent.xkey.keycode);
                if (currentKey != iKeyCode::Undefined)
                {
                    _keys[static_cast<unsigned int>(currentKey)] = true;
                    _keyDownEvent[static_cast<unsigned int>(currentKey)]();
                    _keyDownEventExt(currentKey);

                    iApplication::getInstance().onEvent(iEventPtr(new iEventKeyDown(_window, currentKey)));
                }
                return true;

            case KeyRelease:
                if (XEventsQueued(osevent->_display, QueuedAlready))
                {
                    XEvent nev;
                    XPeekEvent(osevent->_display, &nev);

                    if (nev.type == KeyPress &&
                        nev.xkey.time == xevent.xkey.time &&
                        nev.xkey.keycode == xevent.xkey.keycode)
                    {
                        return true;
                    }
                }

                currentKey = translate(xevent.xkey.keycode);
                if (currentKey != iKeyCode::Undefined)
                {
                    _keys[static_cast<unsigned int>(currentKey)] = false;
                    _keyUpEvent[static_cast<unsigned int>(currentKey)]();
                    _keyUpEventExt(currentKey);

                    iApplication::getInstance().onEvent(iEventPtr(new iKeyUpEvent_TMP(_window, currentKey)));
                }
                return true;

            case ButtonPress:

                switch (osevent->_event.xbutton.button)
                {
                case 1:
                    _keys[static_cast<uint32>(iKeyCode::MouseLeft)] = true;
                    break;
                case 2:
                    _keys[static_cast<uint32>(iKeyCode::MouseMiddle)] = true;
                    break;
                case 3:
                    _keys[static_cast<uint32>(iKeyCode::MouseRight)] = true;
                    break;
                case 4:
                    _keys[static_cast<uint32>(iKeyCode::MouseButton4)] = true;
                    break;
                case 5:
                    _keys[static_cast<uint32>(iKeyCode::MouseButton5)] = true;
                    break;
                };
                return true;

            case ButtonRelease:

                switch (osevent->_event.xbutton.button)
                {
                case 1:
                    _keys[static_cast<uint32>(iKeyCode::MouseLeft)] = false;
                    break;
                case 2:
                    _keys[static_cast<uint32>(iKeyCode::MouseMiddle)] = false;
                    break;
                case 3:
                    _keys[static_cast<uint32>(iKeyCode::MouseRight)] = false;
                    break;
                case 4:
                    _keys[static_cast<uint32>(iKeyCode::MouseButton4)] = false;
                    break;
                case 5:
                    _keys[static_cast<uint32>(iKeyCode::MouseButton5)] = false;
                    break;
                };
                return true;

            default:
                return false;
            }

            return false;
        }
    };
#endif

    iKeyboard::iKeyboard()
    {
#ifdef __IGOR_WINDOWS__
        _impl = new iKeyboardImplWindows();
#endif

#ifdef __IGOR_LINUX__
        _impl = new iKeyboardImplLinux();
#endif
    }

    iKeyboard::~iKeyboard()
    {
        bool printWarn = false;

        if (_impl->_keyDownEventExt.hasDelegates())
        {
            printWarn = true;
            _impl->_keyDownEventExt.clear();
        }

        if (_impl->_keyUpEventExt.hasDelegates())
        {
            printWarn = true;
            _impl->_keyUpEventExt.clear();
        }

        if (_impl->_keyASCIIEvent.hasDelegates())
        {
            printWarn = true;
            _impl->_keyASCIIEvent.clear();
        }

        for (int32 i = 0; i < static_cast<int32>(iKeyCode::KeyCodeCount) - 1; ++i)
        {
            if (_impl->_keyDownEvent[i].hasDelegates())
            {
                printWarn = true;
                _impl->_keyDownEvent[i].clear();
            }

            if (_impl->_keyUpEvent[i].hasDelegates())
            {
                printWarn = true;
                _impl->_keyUpEvent[i].clear();
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
            if (_impl->_keys[i])
            {
                return true;
            }
        }

        return false;
    }

    bool iKeyboard::getKey(iKeyCode keyCode)
    {
        return _impl->_keys[static_cast<unsigned int>(keyCode)];
    }

    void iKeyboard::registerKeyDownDelegate(iKeyDownDelegate keyDownDelegate)
    {
        _impl->_keyDownEventExt.append(keyDownDelegate);
    }

    void iKeyboard::registerKeyUpDelegate(iKeyUpDelegate keyUpDelegate)
    {
        _impl->_keyUpEventExt.append(keyUpDelegate);
    }

    void iKeyboard::registerKeyDownDelegate(iKeyDownSpecificDelegate keyDownDelegate, iKeyCode key)
    {
        _impl->_keyDownEvent[static_cast<unsigned int>(key)].append(keyDownDelegate);
    }

    void iKeyboard::registerKeyUpDelegate(iKeyUpSpecificDelegate keyUpDelegate, iKeyCode key)
    {
        _impl->_keyUpEvent[static_cast<unsigned int>(key)].append(keyUpDelegate);
    }

    void iKeyboard::registerKeyASCIIDelegate(iKeyASCIIDelegate keyASCIIDelegate)
    {
        _impl->_keyASCIIEvent.append(keyASCIIDelegate);
    }

    void iKeyboard::unregisterKeyDownDelegate(iKeyDownDelegate keyDownDelegate)
    {
        _impl->_keyDownEventExt.remove(keyDownDelegate);
    }

    void iKeyboard::unregisterKeyUpDelegate(iKeyUpDelegate keyUpDelegate)
    {
        _impl->_keyUpEventExt.remove(keyUpDelegate);
    }

    void iKeyboard::unregisterKeyDownDelegate(iKeyDownSpecificDelegate keyDownDelegate, iKeyCode key)
    {
        _impl->_keyDownEvent[static_cast<unsigned int>(key)].remove(keyDownDelegate);
    }

    void iKeyboard::unregisterKeyUpDelegate(iKeyUpSpecificDelegate keyUpDelegate, iKeyCode key)
    {
        _impl->_keyUpEvent[static_cast<unsigned int>(key)].remove(keyUpDelegate);
    }

    void iKeyboard::unregisterKeyASCIIDelegate(iKeyASCIIDelegate keyASCIIDelegate)
    {
        _impl->_keyASCIIEvent.remove(keyASCIIDelegate);
    }

    bool iKeyboard::initDevice(const void *data)
    {
        return _impl->initDevice(data);
    }

    void iKeyboard::deinitDevice()
    {
        _impl->deinitDevice();
    }

    bool iKeyboard::onOSEvent(const void *data)
    {
        return _impl->onOSEvent(data);
    }

    iKeyCode iKeyboard::getKeyCode(const iaString &keyName)
    {
        int index = 0;

        for (int i = 0; i < 128; ++i)
        {
            if (s_keyNames[i] == keyName)
            {
                return static_cast<iKeyCode>(index);
            }

            index++;
        }

        return iKeyCode::Undefined;
    }

    iWindow *iKeyboard::getWindow() const
    {
        return _impl->_window;
    }

}; // namespace igor
