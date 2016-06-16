// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWindow.h>

#include <iApplication.h>
#include <iaConsole.h>
#include <iOSEventListener.h>
#include <iView.h>
#include <iMouse.h>
#include <iKeyboard.h>
#include <iOSEvent.h>
#include <iRenderer.h>
#include <iTaskManager.h>
#include <iStatistics.h>

#include <algorithm>
#include <sstream>
using namespace std;

namespace Igor
{

    LRESULT CALLBACK WndProc(HWND _hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        static iWindow* window = nullptr;
        LPCREATESTRUCT lpcs;

        iOSEventWin32 os_event;

        os_event._hWnd = _hWnd;
        os_event._uMsg = uMsg;
        os_event._wParam = wParam;
        os_event._lParam = lParam;

        bool result = false;

        if (WM_CREATE != uMsg)
        {
            window = (iWindow*)GetWindowLongPtr(_hWnd, GWLP_USERDATA);
        }

        if (window)
        {
            //! \todo critical section ?
            //! \todo move code inside window class
            auto listener = window->_oseventlisteners.begin();
            while (listener != window->_oseventlisteners.end())
            {
                if ((*listener)->onOSEvent(&os_event))
                {
                    result = true;
                }

                listener++;
            };
        }

        if (result)
        {
            return true;
        }

        switch (uMsg)
        {
        case WM_DESTROY:
            window->_hWnd = nullptr;
            PostQuitMessage(0);
            window->_windowCloseEvent();
            return true;

        case WM_SIZE:
            if (nullptr != window->_hWnd)
            {
                RECT rect;
                GetWindowRect(window->_hWnd, &rect);
                window->updateSize(rect.right - rect.left, rect.bottom - rect.top);
            }
            return true;

        case WM_MOVE:
            if (nullptr != window->_hWnd)
            {
                RECT rect;
                GetWindowRect(window->_hWnd, &rect);
                window->updatePos(rect.left, rect.top);
            }
            return true;

        case WM_CREATE:
            lpcs = (LPCREATESTRUCT)lParam;
            window = (iWindow*)(lpcs->lpCreateParams);
            SetWindowLongPtr(_hWnd, GWLP_USERDATA, (LONG_PTR)window);
            window->updateSize(window->getWidth(), window->getHeight());
            return true;
        }

        return(DefWindowProc(_hWnd, uMsg, wParam, lParam));
    }

    iWindow::iWindow()
    {
        memset(&_dmScreenSettings, 0, sizeof(_dmScreenSettings));
        memset(&_msg, 0, sizeof(_msg));

        _hInstance = GetModuleHandle(NULL);
        if (_doubleClick)
        {
            _wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
        }
        else
        {
            _wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        }
        
        _wc.lpfnWndProc = (WNDPROC)WndProc;
        _wc.cbClsExtra = 0;
        _wc.cbWndExtra = 0;
        _wc.hInstance = _hInstance;
        _wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
        _wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        _wc.hbrBackground = NULL;
        _wc.lpszMenuName = NULL;
        _wc.lpszClassName = TEXT("Igor");

        RegisterClass(&_wc);

        _renderContext = 0;

        updateClientSize();

        _swapBufferSectionID = iStatistics::getInstance().registerSection("w_swap", iaColor4f(0.33, 0, 0, 1), 0);
    }

    void iWindow::setDoubleClick(bool doubleClick)
    {
        con_assert(!isOpen(), "window is open already");

        if (!isOpen())
        {
            if (_doubleClick != doubleClick)
            {
                UnregisterClass(TEXT("Igor"), _wc.hInstance);

                _doubleClick = doubleClick;

                if (_doubleClick)
                {
                    _wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
                }
                else
                {
                    _wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
                }

                RegisterClass(&_wc);
            }
        }
    }

    bool iWindow::getDoubleClick() const
    {
        return _doubleClick;
    }

    iWindow::~iWindow()
    {
        if (isOpen())
        {
            close();
        }

        _views.flush();
        if (_views.getList().size())
        {
            con_err("still views registered to this window");
        }

        UnregisterClass(TEXT("Igor"), _hInstance);
        _hInstance = 0;
    }

    void iWindow::handle()
    {
        while (PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&_msg);
            DispatchMessage(&_msg);
        }
    }

    void iWindow::swapBuffers()
    {
        SwapBuffers(_hDC);
    }

    HWND iWindow::getHandle() const
    {
        if (!_hWnd)
        {
            con_err("window not _isOpen");
        }

        return _hWnd;
    }

    HDC iWindow::getDeviceContext() const
    {
        if (!_hDC)
        {
            con_err("window not _isOpen");
        }

        return _hDC;
    }

    HGLRC iWindow::getRenderContext() const
    {
        if (!_renderContext)
        {
            con_err("window not _isOpen");
        }

        return _renderContext;
    }

    bool iWindow::initOpenGLContext()
    {
        con_endl("opening GL Window \"" << _title << "\" (" << _width << "x" << _height << ")" << (_fullscreen ? " FULLSCREEN" : ""));

        uint32 posx = _xPos;
        uint32 posy = _yPos;

        if (_fullscreen)
        {
            posx = 0;
            posy = 0;
            memset(&_dmScreenSettings, 0, sizeof(_dmScreenSettings));
            _dmScreenSettings.dmSize = sizeof(_dmScreenSettings);
            _dmScreenSettings.dmPelsWidth = _width;
            _dmScreenSettings.dmPelsHeight = _height;
            _dmScreenSettings.dmBitsPerPel = _colordepth;
            _dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

            if (ChangeDisplaySettings(&_dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
            {
                con_err("The resolution (" << _width << "x" << _height << ") is not supported by this system. Using window mode instead");
                _fullscreen = false;
                posx = _xPos;
                posy = _yPos;
            }

            _dwExStyle = WS_EX_APPWINDOW;
            _dwStyle = WS_POPUP;
        }
        else
        {
            _dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
            _dwStyle = WS_OVERLAPPEDWINDOW;
        }

        // Create The Window
        _hWnd = CreateWindowEx(_dwExStyle, TEXT("Igor"), _title.getData(), _dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, posx, posy, _width, _height, 0, 0, _hInstance, this);

        if (!_hWnd)
        {
            con_err("can't create window");
            close();
            return false;
        }

        static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
        {
            sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
            1,											// Version Number
            PFD_DRAW_TO_WINDOW |						// Format Must Support Window
            PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
            PFD_DOUBLEBUFFER,							// Must Support Double Buffering
            PFD_TYPE_RGBA,								// Request An RGBA Format
            _colordepth,									// Select Our Color Depth
            0, 0, 0, 0, 0, 0,							// Color Bits Ignored
            0,											// No Alpha Buffer
            0,											// Shift Bit Ignored
            0,											// No Accumulation Buffer
            0, 0, 0, 0,									// Accumulation Bits Ignored
            16,											// 16Bit Z-Buffer (Depth Buffer)
            0,											// No Stencil Buffer
            0,											// No Auxiliary Buffer
            PFD_MAIN_PLANE,								// Main Drawing Layer
            0,											// Reserved
            0, 0, 0										// Layer Masks Ignored
        };

        if (!(_hDC = GetDC(_hWnd)))
        {
            con_err("can't create Device Context");
            close();
            return false;
        }

        uint32 pixelformat;

        if (!(pixelformat = ChoosePixelFormat(_hDC, &pfd)))
        {
            con_err("invalid pixel format");
            close();
            return false;
        }

        if (!SetPixelFormat(_hDC, pixelformat, &pfd))
        {
            con_err("can't set pixel format");
            close();
            return false;
        }

        if (!(_renderContext = wglCreateContext(_hDC)))
        {
            con_err("can't create rendering context");
            close();
            return false;
        }

        iTaskManager::getInstance().createThreads(this);

        if (!wglMakeCurrent(_hDC, _renderContext))
        {
            con_err_win("can't make rendering context curent");
            close();
            return false;
        }

        registerOSListener(&iMouse::getInstance());
        registerOSListener(&iKeyboard::getInstance());

        ShowWindow(_hWnd, SW_SHOW);
        SetForegroundWindow(_hWnd);
        SetFocus(_hWnd);

        _isOpen = true;
        iRenderer::getInstance().init();
        
        SwapBuffers(_hDC);

        return true;
    }

    bool iWindow::open()
    {
        if (_isOpen)
        {
            con_warn("window is already opened");
            return false;
        }

        _views.flush();
        if (_views.getList().size() == 0)
        {
            // we do this warning here for quick response on a black screen because no view was added to the window
            con_warn("opened window without views");
        }

        iApplication::getInstance().addWindow(this);
        return initOpenGLContext();
    }

    void iWindow::close()
    {
        if (isOpen())
        {
            if (iRenderer::isInstantiated())
            {
                iRenderer::getInstance().deinit();
            }

            iTaskManager::getInstance().killThreads(this);

            if (_fullscreen)
            {
                ChangeDisplaySettings(NULL, 0);
            }

            if (_renderContext)
            {
                if (!wglMakeCurrent(NULL, NULL))
                {
                    con_err("detach of render context failed");
                }

                if (!wglDeleteContext(_renderContext))
                {
                    con_err("release of render context failed");
                }

                _renderContext = 0;
            }

            if (_hWnd && _hDC && !ReleaseDC(_hWnd, _hDC))
            {
                con_err("release of device context failed");
            }

            if (_hWnd && !DestroyWindow(_hWnd))
            {
                con_err("could not release _hWnd");
            }

            unregisterOSListener(&iMouse::getInstance());
            unregisterOSListener(&iKeyboard::getInstance());

            _isOpen = false;

            iApplication::getInstance().removeWindow(this);
        }
        else
        {
            con_warn("window was not opened");
        }
    }

    void iWindow::setSizeByDesktop()
    {
        setSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    }

    void iWindow::setSize(uint32 width, uint32 height)
    {
        if (_width != width ||
            _height != height)
        {
            _width = width;
            _height = height;
            updateClientSize();

            if (_isOpen)
            {
                if (_fullscreen)
                {
                    MoveWindow(_hWnd, 0, 0, _width, _height, true);
                }
                else
                {
                    MoveWindow(_hWnd, _xPos, _yPos, _width, _height, true);
                }
            }
        }
    }

    void iWindow::setPosition(uint32 xPos, uint32 yPos)
    {
        _xPos = xPos;
        _yPos = yPos;

        if (!_fullscreen &&
            _isOpen)
        {
            MoveWindow(_hWnd, _xPos, _yPos, _width, _height, true);
        }
    }

    bool iWindow::isFullscreen()
    {
        return _fullscreen;
    }

    void iWindow::setFullscreen(bool fullscreen)
    {
        if (!_isOpen)
        {
            _fullscreen = fullscreen;
            updateClientSize();
        }
        else
        {
            con_err("can't switch between window and fullscreen when window opened");
        }
    }

    void iWindow::setCentered()
    {
        if (!_fullscreen)
        {
            int32 desktopWidth = GetSystemMetrics(SM_CXSCREEN);
            int32 desktopHeight = GetSystemMetrics(SM_CYSCREEN);

            setPosition((desktopWidth - _width) >> 1, (desktopHeight - _height) >> 1);
        }
    }

    void iWindow::setColorDepth(unsigned char colordepth)
    {
        _colordepth = colordepth;
        //! \todo not implemented
    }

    void iWindow::addView(iView* view)
    {
        con_assert_sticky(view != nullptr, "zero pointer");

        _views.add(view);
        iRectanglei windowRect;
        windowRect.setWidth(_clientWidth);
        windowRect.setHeight(_clientHeight);
        view->updateWindowRect(windowRect);
    }

    void iWindow::removeView(iView* view)
    {
        con_assert_sticky(view != nullptr, "zero pointer");

        _views.remove(view);
    }

    bool iWindow::isOpen() const
    {
        return _isOpen;
    }

    void iWindow::setTitle(const iaString& title)
    {
        if (!_isOpen)
        {
            if (!title.isEmpty())
            {
                _title = title;
            }
            else
            {
                con_err("title can not be empty");
            }
        }
        else
        {
            con_err("title can only be changed when window is closed");
        }
    }

    iaString iWindow::getTitle() const
    {
        return _title;
    }

    uint32 iWindow::getWidth() const
    {
        return _width;
    }

    uint32 iWindow::getHeight() const
    {
        return _height;
    }

    uint32 iWindow::getClientWidth() const
    {
        return _clientWidth;
    }

    uint32 iWindow::getClientHeight() const
    {
        return _clientHeight;
    }

    void iWindow::updateSize(int32 width, int32 height)
    {
        _width = width;
        _height = height;

        updateClientSize();

        iRectanglei windowRect;
        windowRect.setWidth(_clientWidth);
        windowRect.setHeight(_clientHeight);

        for (iView* view : _views.getList())
        {
            view->updateWindowRect(windowRect);
        }

        _windowResizeEvent(_clientWidth, _clientHeight);
    }

    void iWindow::updatePos(int32 x, int32 y)
    {
        _xPos = x;
        _yPos = y;
    }

    void iWindow::updateClientSize()
    {
        if (_fullscreen)
        {
            _clientWidth = _width;
            _clientHeight = _height;
        }
        else
        {
            if (_isOpen)
            {
                RECT clientRect;
                GetClientRect(_hWnd, &clientRect);
                _clientWidth = clientRect.right - clientRect.left;
                _clientHeight = clientRect.bottom - clientRect.top;
            }
            else
            {
                _clientWidth = _width - (GetSystemMetrics(SM_CXSIZEFRAME) << 2);
                _clientHeight = _height - (GetSystemMetrics(SM_CYSIZEFRAME) << 2) - GetSystemMetrics(SM_CYCAPTION);
            }
        }

    }

    uint32 iWindow::getXPos() const
    {
        return _xPos;
    }

    uint32 iWindow::getYPos() const
    {
        return _yPos;
    }

    void iWindow::registerOSListener(iOSEventListener *listener)
    {
        auto found = find(_oseventlisteners.begin(), _oseventlisteners.end(), listener);

        if (found != _oseventlisteners.end())
        {
            con_err("listener already registered");
        }
        else
        {
            if (listener->registerDevice())
            {
                listener->_window = this;
                _oseventlisteners.push_back(listener);
            }
        }
    }

    void iWindow::unregisterOSListener(iOSEventListener *listener)
    {
        auto found = find(_oseventlisteners.begin(), _oseventlisteners.end(), listener);

        if (found == _oseventlisteners.end())
        {
            con_err("no os listener found to unregister");
        }
        else
        {
            listener->_window = nullptr;
            _oseventlisteners.erase(found);
        }
    }

    void iWindow::draw()
    {
        _views.flush();
        auto view = _views.getList().begin();
        while (view != _views.getList().end())
        {
            iRectanglei windowRect;
            windowRect.setWidth(_clientWidth);
            windowRect.setHeight(_clientHeight);
            (*view)->draw();
            view++;
        }

        iStatistics::getInstance().beginSection(_swapBufferSectionID);
        swapBuffers();
        iStatistics::getInstance().endSection(_swapBufferSectionID);
    }

    void iWindow::registerWindowCloseDelegate(WindowCloseDelegate windowCloseDelegate)
    {
        _windowCloseEvent.append(windowCloseDelegate);
    }

    void iWindow::unregisterWindowCloseDelegate(WindowCloseDelegate windowCloseDelegate)
    {
        _windowCloseEvent.remove(windowCloseDelegate);
    }

    void iWindow::registerWindowResizeDelegate(WindowResizeDelegate windowResizeDelegate)
    {
        _windowResizeEvent.append(windowResizeDelegate);
    }

    void iWindow::unregisterWindowResizeDelegate(WindowResizeDelegate windowResizeDelegate)
    {
        _windowResizeEvent.remove(windowResizeDelegate);
    }

};
