// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/os/iMouse.h>

#include <igor/os/iWindow.h>
#include <igor/os/iDefinesWindows.h>
#include <igor/os/iDefinesLinux.h>

#include <iaux/system/iaTime.h>
#include <iaux/system/iaConsole.h>

namespace Igor
{

    struct iButtonState
    {
        bool _pressed = false;
        iaTime _time;
    };

    class iMouseImpl
    {

        friend class iMouse;

    public:
        iMouseImpl(iMouse *mouse)
            : _mouse(mouse)
        {
        }
        virtual ~iMouseImpl() = default;

        virtual bool initDevice(const void *data) = 0;
        virtual void deinitDevice() = 0;
        virtual bool onOSEvent(const void *data) = 0;
        virtual void setPosition(int32 x, int32 y) = 0;
        virtual void showCursor(bool show) = 0;
        virtual void setCenter() = 0;

    protected:
        iButtonState _buttonStates[5];
        iMouse *_mouse;
        iMouseKeyDownEvent _keyDownEvent;
        iMouseKeyUpEvent _keyUpEvent;
        iMouseClickEvent _clickEvent;
        iMouseKeyDoubleClickEvent _doubleClickEvent;
        iMouseMoveFullEvent _moveFullEvent;
        iMouseMoveEvent _moveEvent;
        iMouseWheelEvent _wheelEvent;
        iaVector2i _posLast;
        iaVector2i _pos;
        iWindow *_window = nullptr;
    };

#ifdef __IGOR_WINDOWS__

    class iMouseImplWindows : public iMouseImpl
    {
    public:
        iMouseImplWindows(iMouse *mouse)
            : iMouseImpl(mouse)
        {
            _rawInputDevice.usUsagePage = 1;
            _rawInputDevice.usUsage = 2;
            _rawInputDevice.dwFlags = 0;
            _rawInputDevice.hwndTarget = 0;
        }

        ~iMouseImplWindows()
        {
            if (_rawInputBuffer)
            {
                delete[] _rawInputBuffer;
            }
        }

        void setCenter() override
        {
            setPosition(_window->getClientWidth() >> 1, _window->getClientHeight() >> 1);
        }

        bool initDevice(const void *data) override
        {
            const iOSEventregisterData *specificData = static_cast<const iOSEventregisterData *>(data);
            _window = specificData->_window;
            if (!RegisterRawInputDevices(&_rawInputDevice, 1, sizeof(RAWINPUTDEVICE)))
            {
                con_err("register mouse device failed");
                return false;
            }

            return true;
        }
        void deinitDevice() override
        {
            _window = nullptr;
        }

        void showCursor(bool show) override
        {
            ShowCursor(show);
        }

        bool onOSEvent(const void *data)
        {
            iOSEvent *event = (iOSEvent *)data;

            switch (event->_uMsg)
            {
            case WM_INPUT:
            {
                if (!_rawInputBufferSize)
                {
                    GetRawInputData((HRAWINPUT)event->_lParam, RID_INPUT, 0, &_rawInputBufferSize, sizeof(RAWINPUTHEADER));
                    _rawInputBuffer = new uint8[_rawInputBufferSize];
                }

                GetRawInputData((HRAWINPUT)event->_lParam, RID_INPUT, (LPVOID)_rawInputBuffer, &_rawInputBufferSize, sizeof(RAWINPUTHEADER));

                RAWINPUT *raw = (RAWINPUT *)_rawInputBuffer;
                if (raw->header.dwType == RIM_TYPEMOUSE)
                {
                    if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN)
                    {
                        _buttonStates[3]._pressed = true;
                        _buttonStates[3]._time = iaTime::now();
                        _keyDownEvent(iKeyCode::MouseButton4);
                    }

                    if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN)
                    {
                        _buttonStates[4]._pressed = true;
                        _buttonStates[4]._time = iaTime::now();
                        _keyDownEvent(iKeyCode::MouseButton5);
                    }

                    if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP)
                    {
                        _buttonStates[3]._pressed = false;
                        _keyUpEvent(iKeyCode::MouseButton4);
                    }

                    if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP)
                    {
                        _buttonStates[4]._pressed = false;
                        _keyUpEvent(iKeyCode::MouseButton5);
                    }
                }
            }
            break;

            case WM_MOUSEMOVE:
                _posLast = _pos;
                _pos.set(GET_X_LPARAM(event->_lParam), GET_Y_LPARAM(event->_lParam));
                _moveFullEvent(_posLast, _pos, _window);
                _moveEvent(_pos);
                break;

            case WM_RBUTTONDOWN:
                _buttonStates[2]._pressed = true;
                _buttonStates[2]._time = iaTime::now();
                _keyDownEvent(iKeyCode::MouseRight);
                break;

            case WM_RBUTTONUP:
                if (_buttonStates[2]._pressed)
                {
                    _clickEvent(iKeyCode::MouseRight);
                }

                _buttonStates[2]._pressed = false;
                _keyUpEvent(iKeyCode::MouseRight);
                break;

            case WM_RBUTTONDBLCLK:
                _doubleClickEvent(iKeyCode::MouseRight);
                break;

            case WM_LBUTTONDOWN:
                _buttonStates[0]._pressed = true;
                _buttonStates[0]._time = iaTime::now();
                _keyDownEvent(iKeyCode::MouseLeft);
                break;

            case WM_LBUTTONUP:
                if (_buttonStates[0]._pressed)
                {
                    _clickEvent(iKeyCode::MouseLeft);
                }

                _buttonStates[0]._pressed = false;
                _keyUpEvent(iKeyCode::MouseLeft);
                break;

            case WM_LBUTTONDBLCLK:
                _doubleClickEvent(iKeyCode::MouseLeft);
                break;

            case WM_MBUTTONDOWN:
                _buttonStates[1]._pressed = true;
                _buttonStates[1]._time = iaTime::now();
                _keyDownEvent(iKeyCode::MouseMiddle);
                break;

            case WM_MBUTTONUP:
                if (_buttonStates[1]._pressed)
                {
                    _clickEvent(iKeyCode::MouseMiddle);
                }

                _buttonStates[1]._pressed = false;
                _keyUpEvent(iKeyCode::MouseMiddle);
                break;

            case WM_MBUTTONDBLCLK:
                _doubleClickEvent(iKeyCode::MouseMiddle);
                break;

            case WM_MOUSEWHEEL:
            {
                int32 wheelDelta = int32(int16(HIWORD(event->_wParam))) / int32(WHEEL_DELTA);
                _wheelEvent(wheelDelta);
            }
            break;

            default:
                return false;
            }

            return true;
        }

        void setPosition(int32 x, int32 y)
        {
            if (_window->isFullscreen())
            {
                SetCursorPos(x, y);
            }
            else
            {
                // SetCursorPos seems to subtract the border so we have to add it twice
                int32 borderX = (GetSystemMetrics(SM_CXSIZEFRAME) << 1);
                int32 borderY = (GetSystemMetrics(SM_CYSIZEFRAME) << 1);
                int32 headerY = GetSystemMetrics(SM_CYCAPTION);
                int32 client_rect_offset_pos_x = borderX + _window->getXPos();
                int32 client_rect_offset_pos_y = borderY + headerY + _window->getYPos();
                SetCursorPos(client_rect_offset_pos_x + x, client_rect_offset_pos_y + y);
            }

            _posLast = _pos;
            _pos.set(x, y);
        }

    private:
        /*! size of buffer needed to receive raw input messages
        */
        uint32 _rawInputBufferSize = 0;

        /*! pointer to the buffer to receive raw input messages
        */
        uint8 *_rawInputBuffer = nullptr;

        /*! configuration of raw input device
        */
        RAWINPUTDEVICE _rawInputDevice;
    };

#endif

#ifdef __IGOR_LINUX__

    class iMouseImplLinux : public iMouseImpl
    {
    public:
        iMouseImplLinux(iMouse *mouse)
            : iMouseImpl(mouse)
        {
        }

        ~iMouseImplLinux() = default;

        bool initDevice(const void *data) override
        {
            const iOSEventregisterData *specificData = static_cast<const iOSEventregisterData *>(data);
            _window = specificData->_window;
            _display = specificData->_display;
            _xWindow = specificData->_xWindow;
            return true;
        }

        void deinitDevice() override
        {
            _window = nullptr;
        }

        void setCenter() override
        {
            setPosition(_window->getClientWidth() >> 1, _window->getClientHeight() >> 1);
        }

        void handlePressEvent(iKeyCode buttonKey)
        {
            const iaTime doubleClickDelay = iaTime::fromMilliseconds(200);

            int buttonIndex = (int)buttonKey - (int)iKeyCode::MouseLeft;
            bool doubleClick = false;
            iaTime time = iaTime::now();

            if (!_buttonStates[buttonIndex]._pressed &&
                time - _buttonStates[buttonIndex]._time < doubleClickDelay)
            {
                doubleClick = true;
            }

            _buttonStates[buttonIndex]._pressed = true;
            _buttonStates[buttonIndex]._time = time;

            if (doubleClick)
            {
                _doubleClickEvent(buttonKey);
            }
            else
            {
                _keyDownEvent(buttonKey);
            }
        }

        bool onOSEvent(const void *data) override
        {
            const iOSEvent *osevent = static_cast<const iOSEvent *>(data);
            XEvent xevent = osevent->_event;

            switch (xevent.type)
            {
            case ButtonPress:
                switch (xevent.xbutton.button)
                {
                case 1:
                    handlePressEvent(iKeyCode::MouseLeft);
                    break;

                case 2:
                    handlePressEvent(iKeyCode::MouseMiddle);
                    break;

                case 3:
                    handlePressEvent(iKeyCode::MouseRight);
                    break;

                case 4:
                    _wheelEvent(1);
                    break;

                case 5:
                    _wheelEvent(-1);
                    break;

                case 8:
                    handlePressEvent(iKeyCode::MouseButton4);
                    break;

                case 9:
                    handlePressEvent(iKeyCode::MouseButton5);
                    break;
                };
                break;

            case ButtonRelease:
                switch (xevent.xbutton.button)
                {
                case 1:
                    _buttonStates[0]._pressed = false;
                    _keyUpEvent(iKeyCode::MouseLeft);
                    break;
                case 2:
                    _buttonStates[1]._pressed = false;
                    _keyUpEvent(iKeyCode::MouseMiddle);
                    break;
                case 3:
                    _buttonStates[2]._pressed = false;
                    _keyUpEvent(iKeyCode::MouseRight);
                    break;
                case 8:
                    _buttonStates[4]._pressed = false;
                    _keyUpEvent(iKeyCode::MouseButton4);
                    break;
                case 9:
                    _buttonStates[5]._pressed = false;
                    _keyUpEvent(iKeyCode::MouseButton5);
                    break;
                };
                break;

            case MotionNotify:
                _posLast = _pos;
                _pos.set(xevent.xmotion.x, xevent.xmotion.y);

                _moveFullEvent(_posLast, _pos, _window);
                _moveEvent(_pos);
                break;

            default:
                return false;
            }

            return true;
        }

        void setPosition(int32 x, int32 y) override
        {
            XWarpPointer(_display, None, _xWindow, 0, 0, 0, 0, x, y);
            XSync(_display, false);

            _posLast = _pos;
            _pos.set(x, y);
        }

        void showCursor(bool show) override
        {
            if (!_cursorinitialised)
            {
                Pixmap blank;
                XColor dummy;
                char data[1] = {0};

                blank = XCreateBitmapFromData(_display, _xWindow, data, 1, 1);
                _cursor = XCreatePixmapCursor(_display, blank, blank, &dummy, &dummy, 0, 0);
                XFreePixmap(_display, blank);

                _cursorinitialised = true;
            }

            if (show)
            {
                XUndefineCursor(_display, _xWindow);
            }
            else
            {
                XDefineCursor(_display, _xWindow, _cursor);
            }
        }

    private:
        bool _cursorinitialised = false;
        Cursor _cursor;

        Display *_display = nullptr;
        Window _xWindow = 0;
    };

#endif

    iMouse::iMouse()
    {
#ifdef __IGOR_WINDOWS__
        _impl = new iMouseImplWindows(this);
#endif
#ifdef __IGOR_LINUX__
        _impl = new iMouseImplLinux(this);
#endif
    }

    iMouse::~iMouse()
    {
        bool printWarn = false;
        if (_impl->_keyDownEvent.hasDelegates())
        {
            _impl->_keyDownEvent.clear();
            printWarn = true;
        }

        if (_impl->_keyUpEvent.hasDelegates())
        {
            _impl->_keyUpEvent.clear();
            printWarn = true;
        }

        if (_impl->_moveFullEvent.hasDelegates())
        {
            _impl->_moveFullEvent.clear();
            printWarn = true;
        }

        if (_impl->_moveEvent.hasDelegates())
        {
            _impl->_moveEvent.clear();
            printWarn = true;
        }

        if (_impl->_wheelEvent.hasDelegates())
        {
            _impl->_wheelEvent.clear();
            printWarn = true;
        }

        if (printWarn)
        {
            con_warn("not all delegates unregistered");
        }

        delete _impl;
    }

    bool iMouse::initDevice(const void *data)
    {
        return _impl->initDevice(data);
    }

    void iMouse::deinitDevice()
    {
        _impl->deinitDevice();
    }

    bool iMouse::onOSEvent(const void *data)
    {
        return _impl->onOSEvent(data);
    }

    void iMouse::setCenter()
    {
        _impl->setCenter();
    }

    void iMouse::setPosition(const iaVector2i &pos)
    {
        setPosition(pos._x, pos._y);
    }

    void iMouse::setPosition(int32 x, int32 y)
    {
        _impl->setPosition(x, y);
    }

    void iMouse::showCursor(bool show)
    {
        _impl->showCursor(show);
    }

    void iMouse::registerMouseClickDelegate(iMouseClickDelegate clickDelegate)
    {
        _impl->_clickEvent.append(clickDelegate);
    }

    void iMouse::unregisterMouseClickDelegate(iMouseClickDelegate clickDelegate)
    {
        _impl->_clickEvent.remove(clickDelegate);
    }

    void iMouse::registerMouseDoubleClickDelegate(iMouseKeyDoubleClickDelegate doubleClickDelegate)
    {
        _impl->_doubleClickEvent.append(doubleClickDelegate);
    }

    void iMouse::unregisterMouseDoubleClickDelegate(iMouseKeyDoubleClickDelegate doubleClickDelegate)
    {
        _impl->_doubleClickEvent.remove(doubleClickDelegate);
    }

    void iMouse::registerMouseKeyDownDelegate(iMouseKeyDownDelegate keydown_delegate)
    {
        _impl->_keyDownEvent.append(keydown_delegate);
    }

    void iMouse::registerMouseKeyUpDelegate(iMouseKeyUpDelegate keyup_delegate)
    {
        _impl->_keyUpEvent.append(keyup_delegate);
    }

    void iMouse::registerMouseMoveDelegate(iMouseMoveDelegate move_delegate)
    {
        _impl->_moveEvent.append(move_delegate);
    }

    void iMouse::unregisterMouseMoveDelegate(iMouseMoveDelegate move_delegate)
    {
        _impl->_moveEvent.remove(move_delegate);
    }

    void iMouse::registerMouseMoveFullDelegate(iMouseMoveFullDelegate move_delegate)
    {
        _impl->_moveFullEvent.append(move_delegate);
    }

    void iMouse::registerMouseWheelDelegate(iMouseWheelDelegate wheel_delegate)
    {
        _impl->_wheelEvent.append(wheel_delegate);
    }

    void iMouse::unregisterMouseKeyDownDelegate(iMouseKeyDownDelegate keydown_delegate)
    {
        _impl->_keyDownEvent.remove(keydown_delegate);
    }

    void iMouse::unregisterMouseKeyUpDelegate(iMouseKeyUpDelegate keyup_delegate)
    {
        _impl->_keyUpEvent.remove(keyup_delegate);
    }

    void iMouse::unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate move_delegate)
    {
        _impl->_moveFullEvent.remove(move_delegate);
    }

    void iMouse::unregisterMouseWheelDelegate(iMouseWheelDelegate wheel_delegate)
    {
        _impl->_wheelEvent.remove(wheel_delegate);
    }

    iaVector2i iMouse::getPos()
    {
        return _impl->_pos;
    }

    iaVector2i iMouse::getPosDelta()
    {
        return _impl->_pos - _impl->_posLast;
    }

    bool iMouse::getLeftButton()
    {
        return _impl->_buttonStates[0]._pressed;
    }

    bool iMouse::getMiddleButton()
    {
        return _impl->_buttonStates[1]._pressed;
    }

    bool iMouse::getRightButton()
    {
        return _impl->_buttonStates[2]._pressed;
    }

    bool iMouse::getButton4()
    {
        return _impl->_buttonStates[3]._pressed;
    }

    bool iMouse::getButton5()
    {
        return _impl->_buttonStates[4]._pressed;
    }

    iWindow *iMouse::getWindow() const
    {
        return _impl->_window;
    }

}; // namespace Igor
