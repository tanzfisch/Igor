// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/system/iMouse.h>

#include <igor/system/iKeyboard.h>
#include <igor/system/iApplication.h>

#include <igor/system/iWindow.h>
#include <igor/system/iDefinesWindows.h>
#include <igor/system/iDefinesLinux.h>
#include <igor/events/iEventMouse.h>

#include <iaux/system/iaTime.h>
#include <iaux/system/iaConsole.h>

namespace igor
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
        virtual void hideCursor(bool hide) = 0;
        virtual void setCursorType(iMouseCursorType cursorType) = 0;
        virtual void resetCursorType() = 0;
        virtual void setCenter() = 0;

    protected:
        iButtonState _buttonStates[5];
        iMouse *_mouse;
        iaVector2i _posLast;
        iaVector2i _pos;
        iWindowPtr _window = nullptr;
    };

#ifdef IGOR_WINDOWS

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

        void hideCursor(bool hide) override
        {
            ShowCursor(!hide);
        }

        void setCursorType(iMouseCursorType cursorType) override
        {
            // not implemented
            con_err("setCursorType not implemented");
        }

        void resetCursorType() override
        {
            // not implemented
            con_err("resetCursorType not implemented");
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
                        _buttonStates[3]._time = iaTime::getNow();
                        iApplication::getInstance().onEvent(iEventPtr(new iEventMouseKeyDown(_window, iKeyCode::MouseButton4, _pos)));
                    }

                    if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN)
                    {
                        _buttonStates[4]._pressed = true;
                        _buttonStates[4]._time = iaTime::getNow();
                        iApplication::getInstance().onEvent(iEventPtr(new iEventMouseKeyDown(_window, iKeyCode::MouseButton5, _pos)));
                    }

                    if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP)
                    {
                        _buttonStates[3]._pressed = false;
                        iApplication::getInstance().onEvent(iEventPtr(new iEventMouseKeyUp(_window, iKeyCode::MouseButton4, _pos)));
                    }

                    if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP)
                    {
                        _buttonStates[4]._pressed = false;
                        iApplication::getInstance().onEvent(iEventPtr(new iEventMouseKeyUp(_window, iKeyCode::MouseButton5, _pos)));
                    }
                }
            }
            break;

            case WM_MOUSEMOVE:
                _posLast = _pos;
                _pos.set(GET_X_LPARAM(event->_lParam), GET_Y_LPARAM(event->_lParam));
                iApplication::getInstance().onEvent(iEventPtr(new iEventMouseMove(_window, _posLast, _pos)));
                break;

            case WM_RBUTTONDOWN:
                _buttonStates[2]._pressed = true;
                _buttonStates[2]._time = iaTime::getNow();
                iApplication::getInstance().onEvent(iEventPtr(new iEventMouseKeyDown(_window, iKeyCode::MouseRight, _pos)));
                break;

            case WM_RBUTTONUP:
                _buttonStates[2]._pressed = false;
                iApplication::getInstance().onEvent(iEventPtr(new iEventMouseKeyUp(_window, iKeyCode::MouseRight, _pos)));
                break;

            case WM_RBUTTONDBLCLK:
                iApplication::getInstance().onEvent(iEventPtr(new iEventMouseKeyDoubleClick(_window, iKeyCode::MouseRight, _pos)));
                break;

            case WM_LBUTTONDOWN:
                _buttonStates[0]._pressed = true;
                _buttonStates[0]._time = iaTime::getNow();
                iApplication::getInstance().onEvent(iEventPtr(new iEventMouseKeyDown(_window, iKeyCode::MouseLeft, _pos)));
                break;

            case WM_LBUTTONUP:
                _buttonStates[0]._pressed = false;
                iApplication::getInstance().onEvent(iEventPtr(new iEventMouseKeyUp(_window, iKeyCode::MouseLeft, _pos)));
                break;

            case WM_LBUTTONDBLCLK:
                iApplication::getInstance().onEvent(iEventPtr(new iEventMouseKeyDoubleClick(_window, iKeyCode::MouseLeft, _pos)));
                break;

            case WM_MBUTTONDOWN:
                _buttonStates[1]._pressed = true;
                _buttonStates[1]._time = iaTime::getNow();
                iApplication::getInstance().onEvent(iEventPtr(new iEventMouseKeyDown(_window, iKeyCode::MouseMiddle, _pos)));
                break;

            case WM_MBUTTONUP:
                _buttonStates[1]._pressed = false;
                iApplication::getInstance().onEvent(iEventPtr(new iEventMouseKeyUp(_window, iKeyCode::MouseMiddle, _pos)));
                break;

            case WM_MBUTTONDBLCLK:
                iApplication::getInstance().onEvent(iEventPtr(new iEventMouseKeyDoubleClick(_window, iKeyCode::MouseMiddle, _pos)));
                break;

            case WM_MOUSEWHEEL:
            {
                int32 wheelDelta = int32(int16(HIWORD(event->_wParam))) / int32(WHEEL_DELTA);
                iApplication::getInstance().onEvent(iEventPtr(new iEventMouseWheel(_window, wheelDelta)));
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

        ~iMouseImplLinux()
        {
            if (_blankCursor != None)
            {
                XFreeCursor(_display, _blankCursor);
                _blankCursor = None;
            }
        }

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
            const int buttonIndex = (int)buttonKey - (int)iKeyCode::MouseLeft;
            const iaTime time = iaTime::getNow();

            bool doubleClick = false;

            if (!_buttonStates[buttonIndex]._pressed &&
                time - _buttonStates[buttonIndex]._time < doubleClickDelay)
            {
                doubleClick = true;
            }

            _buttonStates[buttonIndex]._pressed = true;
            _buttonStates[buttonIndex]._time = time;

            if (doubleClick)
            {
                iApplication::getInstance().onEvent(iEventPtr(new iEventMouseKeyDoubleClick(_window, buttonKey, _pos)));
            }
            else
            {
                iApplication::getInstance().onEvent(iEventPtr(new iEventMouseKeyDown(_window, buttonKey, _pos)));
            }
        }

        void handleReleaseEvent(iKeyCode buttonKey)
        {
            const int buttonIndex = (int)buttonKey - (int)iKeyCode::MouseLeft;
            _buttonStates[buttonIndex]._pressed = false;
            iApplication::getInstance().onEvent(iEventPtr(new iEventMouseKeyUp(_window, buttonKey, _pos)));
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
                    iApplication::getInstance().onEvent(iEventPtr(new iEventMouseWheel(_window, 1)));
                    break;

                case 5:
                    iApplication::getInstance().onEvent(iEventPtr(new iEventMouseWheel(_window, -1)));
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
                    handleReleaseEvent(iKeyCode::MouseLeft);
                    break;
                case 2:
                    handleReleaseEvent(iKeyCode::MouseMiddle);
                    break;
                case 3:
                    handleReleaseEvent(iKeyCode::MouseRight);
                    break;
                case 8:
                    handleReleaseEvent(iKeyCode::MouseButton4);
                    break;
                case 9:
                    handleReleaseEvent(iKeyCode::MouseButton5);
                    break;
                };
                break;

            case MotionNotify:
            {
                iaVector2i pos(xevent.xmotion.x, xevent.xmotion.y);
                if (_pos != pos)
                {
                    _posLast = _pos;
                    _pos = pos;

                    iApplication::getInstance().onEvent(iEventPtr(new iEventMouseMove(_window, _posLast, _pos)));
                }
            }
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

        void resetCursorType() override
        {
            XUndefineCursor(_display, _xWindow);
        }

        void setCursorType(iMouseCursorType cursorType) override
        {
            int xcursorType = 0;

            switch (cursorType)
            {
            case iMouseCursorType::Arrow:
                xcursorType = XC_left_ptr;
                break;
            case iMouseCursorType::ArrowLeftEdge:
                xcursorType = XC_left_side;
                break;
            case iMouseCursorType::ArrowRightEdge:
                xcursorType = XC_right_side;
                break;
            case iMouseCursorType::ArrowTopEdge:
                xcursorType = XC_top_side;
                break;
            case iMouseCursorType::ArrowBottomEdge:
                xcursorType = XC_bottom_side;
                break;
            case iMouseCursorType::ArrowTopLeftCorner:
                xcursorType = XC_top_left_corner;
                break;
            case iMouseCursorType::ArrowTopRightCorner:
                xcursorType = XC_top_right_corner;
                break;
            case iMouseCursorType::ArrowBottomLeftCorner:
                xcursorType = XC_bottom_left_corner;
                break;
            case iMouseCursorType::ArrowBottomRightCorner:
                xcursorType = XC_bottom_right_corner;
                break;
            case iMouseCursorType::VeticalSplit:
                xcursorType = XC_sb_v_double_arrow;
                break;
            case iMouseCursorType::HorizontalSplit:
                xcursorType = XC_sb_h_double_arrow;
                break;
            }

            Cursor cursor = XCreateFontCursor(_display, xcursorType);
            XDefineCursor(_display, _xWindow, cursor);
        }

        void hideCursor(bool hide) override
        {
            if (hide)
            {
                if (_blankCursor == None)
                {
                    XColor dummy;
                    char data[1] = {0};

                    Pixmap blank = XCreateBitmapFromData(_display, _xWindow, data, 1, 1);
                    _blankCursor = XCreatePixmapCursor(_display, blank, blank, &dummy, &dummy, 0, 0);
                    XFreePixmap(_display, blank);
                }

                XDefineCursor(_display, _xWindow, _blankCursor);
            }
            else
            {
                XUndefineCursor(_display, _xWindow);
            }
        }

    private:
        /*! blank cursor definition
         */
        Cursor _blankCursor = None;

        /*! display handle
         */
        Display *_display = nullptr;

        /*! window handle
         */
        Window _xWindow = None;
    };

#endif

    iMouse::iMouse()
    {
#ifdef IGOR_WINDOWS
        _impl = new iMouseImplWindows(this);
#endif
#ifdef __IGOR_LINUX__
        _impl = new iMouseImplLinux(this);
#endif
    }

    iMouse::~iMouse()
    {
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

    void iMouse::resetCursorType()
    {
        _impl->resetCursorType();
    }

    void iMouse::setCursorType(iMouseCursorType cursorType)
    {
        _impl->setCursorType(cursorType);
    }

    void iMouse::hideCursor(bool hide)
    {
        _impl->hideCursor(hide);
    }

    const iaVector2i &iMouse::getPos() const
    {
        return _impl->_pos;
    }

    iaVector2i iMouse::getPosDelta() const
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

    iWindowPtr iMouse::getWindow() const
    {
        return _impl->_window;
    }

}; // namespace igor
