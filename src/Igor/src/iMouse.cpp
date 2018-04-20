// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iMouse.h>

#include <Windows.h>
#include <Windowsx.h>

#include <iWindow.h>
#include <iaConsole.h>
#include <iOSEvent.h>

namespace Igor
{

    iMouse::iMouse()
    {
        _rawInputDevice.usUsagePage = 1;
        _rawInputDevice.usUsage = 2;
        _rawInputDevice.dwFlags = 0;
        _rawInputDevice.hwndTarget = 0;
    }

    iMouse::~iMouse()
    {
        bool printWarn = false;
        if (_keyDownEvent.hasDelegates())
        {
            _keyDownEvent.clear();
            printWarn = true;
        }

        if (_keyUpEvent.hasDelegates())
        {
            _keyUpEvent.clear();
            printWarn = true;
        }

        if (_moveFullEvent.hasDelegates())
        {
            _moveFullEvent.clear();
            printWarn = true;
        }

        if (_moveEvent.hasDelegates())
        {
            _moveEvent.clear();
            printWarn = true;
        }

        if (_wheelEvent.hasDelegates())
        {
            _wheelEvent.clear();
            printWarn = true;
        }

        if (printWarn)
        {
            con_warn("not all delegates unregistered");
        }

        if (_rawInputBuffer)
        {
            delete[] _rawInputBuffer;
        }
    }

    bool iMouse::registerDevice()
    {
        if (!RegisterRawInputDevices(&_rawInputDevice, 1, sizeof(RAWINPUTDEVICE)))
        {
            con_err("register mouse device failed");
            return false;
        }

        return true;
    }

    bool iMouse::onOSEvent(const void *data)
    {
        con_assert(nullptr != _window, "not registered to a window");

        iOSEventWin32 *event = (iOSEventWin32*)data;

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

            RAWINPUT *raw = (RAWINPUT*)_rawInputBuffer;
            if (raw->header.dwType == RIM_TYPEMOUSE)
            {
                if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN)
                {
                    _button4 = true;
                    _keyDownEvent(iKeyCode::MouseButton4);
                }

                if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN)
                {
                    _button5 = true;
                    _keyDownEvent(iKeyCode::MouseButton5);
                }

                if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP)
                {
                    _button4 = false;
                    _keyUpEvent(iKeyCode::MouseButton4);
                }

                if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP)
                {
                    _button5 = false;
                    _keyUpEvent(iKeyCode::MouseButton5);
                }
            }
        }
        break;

        case WM_MOUSEMOVE:
            if (_ignoreNextMoveEvent)
            {
                _ignoreNextMoveEvent = false;
            }
            else
            {
                _posLast = _pos;
                _pos.set(GET_X_LPARAM(event->_lParam), GET_Y_LPARAM(event->_lParam));

                _moveFullEvent(_posLast, _pos, _window);
                _moveEvent(_pos);
            }
            break;

        case WM_RBUTTONDOWN:
            _rButton = true;
            _keyDownEvent(iKeyCode::MouseRight);
            break;

        case WM_RBUTTONUP:
            if (_rButton)
            {
                _clickEvent(iKeyCode::MouseRight);
            }

            _rButton = false;
            _keyUpEvent(iKeyCode::MouseRight);
            break;

        case WM_RBUTTONDBLCLK:
            _doubleClickEvent(iKeyCode::MouseRight);
            break;

        case WM_LBUTTONDOWN:
            _lButton = true;
            _keyDownEvent(iKeyCode::MouseLeft);
            break;

        case WM_LBUTTONUP:
            if (_lButton)
            {
                _clickEvent(iKeyCode::MouseLeft);
            }

            _lButton = false;
            _keyUpEvent(iKeyCode::MouseLeft);
            break;

        case WM_LBUTTONDBLCLK:
            _doubleClickEvent(iKeyCode::MouseLeft);
            break;

        case WM_MBUTTONDOWN:
            _mButton = true;
            _keyDownEvent(iKeyCode::MouseMiddle);
            break;

        case WM_MBUTTONUP:
            if (_mButton)
            {
                _clickEvent(iKeyCode::MouseMiddle);
            }

            _mButton = false;
            _keyUpEvent(iKeyCode::MouseMiddle);
            break;

        case WM_MBUTTONDBLCLK:
            _doubleClickEvent(iKeyCode::MouseMiddle);
            break;

        case WM_MOUSEWHEEL:
            _wheelDelta = int32(int16(HIWORD(event->_wParam))) / int32(WHEEL_DELTA);
            _wheelEvent(_wheelDelta);
            break;

        default: return false;
        }

        return true;
    }

    void iMouse::setCenter(bool supressMoveEvent)
    {
        setPosition(_window->getClientWidth() >> 1, _window->getClientHeight() >> 1, supressMoveEvent);
    }

    void iMouse::setPosition(int32 x, int32 y, bool supressMoveEvent)
    {
        _ignoreNextMoveEvent = supressMoveEvent;

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

    void iMouse::showCursor(bool show)
    {
        ShowCursor(show);
    }

    void iMouse::registerMouseClickDelegate(iMouseClickDelegate clickDelegate)
    {
        _clickEvent.append(clickDelegate);
    }

    void iMouse::unregisterMouseClickDelegate(iMouseClickDelegate clickDelegate)
    {
        _clickEvent.remove(clickDelegate);
    }

    void iMouse::registerMouseDoubleClickDelegate(iMouseKeyDoubleClickDelegate doubleClickDelegate)
    {
        _doubleClickEvent.append(doubleClickDelegate);
    }

    void iMouse::unregisterMouseDoubleClickDelegate(iMouseKeyDoubleClickDelegate doubleClickDelegate)
    {
        _doubleClickEvent.remove(doubleClickDelegate);
    }

    void iMouse::registerMouseKeyDownDelegate(iMouseKeyDownDelegate keydown_delegate)
    {
        _keyDownEvent.append(keydown_delegate);
    }

    void iMouse::registerMouseKeyUpDelegate(iMouseKeyUpDelegate keyup_delegate)
    {
        _keyUpEvent.append(keyup_delegate);
    }

    void iMouse::registerMouseMoveDelegate(iMouseMoveDelegate move_delegate)
    {
        _moveEvent.append(move_delegate);
    }

    void iMouse::unregisterMouseMoveDelegate(iMouseMoveDelegate move_delegate)
    {
        _moveEvent.remove(move_delegate);
    }

    void iMouse::registerMouseMoveFullDelegate(iMouseMoveFullDelegate move_delegate)
    {
        _moveFullEvent.append(move_delegate);
    }

    void iMouse::registerMouseWheelDelegate(iMouseWheelDelegate wheel_delegate)
    {
        _wheelEvent.append(wheel_delegate);
    }

    void iMouse::unregisterMouseKeyDownDelegate(iMouseKeyDownDelegate keydown_delegate)
    {
        _keyDownEvent.remove(keydown_delegate);
    }

    void iMouse::unregisterMouseKeyUpDelegate(iMouseKeyUpDelegate keyup_delegate)
    {
        _keyUpEvent.remove(keyup_delegate);
    }

    void iMouse::unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate move_delegate)
    {
        _moveFullEvent.remove(move_delegate);
    }

    void iMouse::unregisterMouseWheelDelegate(iMouseWheelDelegate wheel_delegate)
    {
        _wheelEvent.remove(wheel_delegate);
    }

    iaVector2i iMouse::getPos()
    {
        return _pos;
    }

    iaVector2i iMouse::getPosDelta()
    {
        return _pos - _posLast;
    }

    int32 iMouse::getWheelDelta()
    {
        return _wheelDelta;
    }

    bool iMouse::getRightButton()
    {
        return _rButton;
    }

    bool iMouse::getLeftButton()
    {
        return _lButton;
    }

    bool iMouse::getMiddleButton()
    {
        return _mButton;
    }

    bool iMouse::getButton4()
    {
        return _button4;
    }

    bool iMouse::getButton5()
    {
        return _button5;
    }

};
