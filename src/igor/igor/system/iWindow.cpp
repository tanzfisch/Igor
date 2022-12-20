// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/system/iWindow.h>

#include <igor/system/iApplication.h>
#include <iaux/system/iaConsole.h>
#include <igor/system/iOSEventListener.h>
#include <igor/system/iMouse.h>
#include <igor/system/iKeyboard.h>
#include <igor/renderer/iView.h>
#include <igor/renderer/iRenderer.h>
#include <igor/threading/iTaskManager.h>
#include <igor/events/iEventWindow.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/resources/texture/iTextureResourceFactory.h>

#include <algorithm>
#include <sstream>

// have to include this after GLee
#include <igor/system/iDefinesWindows.h>
#include <igor/system/iDefinesLinux.h>

namespace igor
{

    /*! default igor window title
     */
    static const iaString s_defaultWindowTitle = "Igor";

    iaIDGenerator64 iWindow::_idGenerator;

    /*! base class for internal iWindow implementation
     */
    class iWindowImpl
    {

        friend class iWindow;

    public:
        iWindowImpl(iWindowPtr window) { _window = window; }
        virtual ~iWindowImpl() = default;

        virtual void calcClientSize() = 0;
        virtual void setDoubleClick(bool doubleClick) = 0;
        virtual void swapBuffers() = 0;
        virtual void handle() = 0;
        virtual bool open() = 0;
        virtual void close() = 0;
        virtual void setSizeByDesktop() = 0;
        virtual void setClientSize(int32 width, int32 height) = 0;
        virtual void setSize(int32 width, int32 height) = 0;
        virtual void setPosition(int32 xPos, int32 yPos) = 0;
        virtual void setFullscreen(bool fullscreen) = 0;
        virtual void setCentered() = 0;
        virtual iRenderContextPtr getDefaultRenderContext() const = 0;
        virtual iRenderContextPtr createRenderContext(iRenderContextPtr renderContext) = 0;
        virtual bool makeCurrent(iRenderContextPtr renderContext) = 0;
        virtual bool deleteRenderContext(iRenderContextPtr renderContext) = 0;
        virtual void setVSync(bool vsync) = 0;
        virtual bool getVSync() const = 0;
        virtual void registerOSListener(iOSEventListener *listener) = 0;
        virtual void unregisterOSListener(iOSEventListener *listener) = 0;
        virtual void getDesktopSize(int32 &width, int32 &height) = 0;

        __IGOR_INLINE__ void closeEvent()
        {
            iApplication::getInstance().onEvent(iEventPtr(new iEventWindowClose(_window)));
        }

        __IGOR_INLINE__ void sizeChanged(int32 width, int32 height)
        {
            _window->onSizeChanged(width, height);

            iApplication::getInstance().onEvent(iEventPtr(new iEventWindowResize(_window, width, height)));
        }

        __IGOR_INLINE__ static iWindowImpl *getImpl(iWindowPtr window)
        {
            return window->_impl;
        }

    protected:
        /*! list of registeres os event listeners

        currently all devices are singletons so all listeners will be added automatically
        */
        std::vector<iOSEventListener *> _oseventlisteners;

        /*! width of the render area in pixel
         */
        int32 _width = 640;

        /*! height of the render area in pixel
         */
        int32 _height = 480;

        /*! the actuall width of the rendering area.

        If fullscreen it's equal to _width. If window mode it's usually less then _width depending on window design settings.
        */
        int32 _clientWidth = 0;

        /*! the actuall height of the rendering area.

        see details at _clientWidth
        */
        int32 _clientHeight = 0;

        /*! x position of the window in pixel
         */
        int32 _x = 0;

        /*! y position of the window in pixel
         */
        int32 _y = 0;

        /*! color depth of the render area in bit per pixel
         */
        uint8 _colordepth = 32;

        /*! depth buffer depth in bits
         */
        uint8 _zdepth = 16;

        /*! true for fullscreen mode
         */
        bool _fullscreen = false;

        /*! the window title
         */
        iaString _title = s_defaultWindowTitle;

        /*! true if the window is opened
         */
        bool _isOpen = false;

        /*! flag if the window gets double click events from windows
         */
        bool _doubleClick = false;

        /*! window handle
         */
        iWindowPtr _window = nullptr;
    };

#ifdef __IGOR_WINDOWS__

    /*! windows implementation of a window
     */
    class iWindowImplWindows : public iWindowImpl
    {

        friend LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    public:
        iWindowImplWindows(iWindowPtr window)
            : iWindowImpl(window)
        {
            memset(&_dmScreenSettings, 0, sizeof(_dmScreenSettings));

            _moduleHandle = GetModuleHandle(NULL);
            if (_doubleClick)
            {
                _windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
            }
            else
            {
                _windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            }

            _windowClass.lpfnWndProc = (WNDPROC)WndProc;
            _windowClass.cbClsExtra = 0;
            _windowClass.cbWndExtra = 0;
            _windowClass.hInstance = _moduleHandle;
            _windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
            _windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
            _windowClass.hbrBackground = NULL;
            _windowClass.lpszMenuName = NULL;
            _windowClass.lpszClassName = TEXT("Igor");

            RegisterClass(&_windowClass);

            _renderContext = nullptr;
        }

        ~iWindowImplWindows()
        {
            UnregisterClass(TEXT("Igor"), _moduleHandle);
            _moduleHandle = 0;
        }

        void registerOSListener(iOSEventListener *listener)
        {
            auto found = std::find(_oseventlisteners.begin(), _oseventlisteners.end(), listener);

            if (found != _oseventlisteners.end())
            {
                con_err("listener already registered");
            }
            else
            {
                iOSEventregisterData data = {_window};
                if (listener->initDevice(static_cast<const void *>(&data)))
                {
                    _oseventlisteners.push_back(listener);
                }
            }
        }

        void unregisterOSListener(iOSEventListener *listener)
        {
            auto found = std::find(_oseventlisteners.begin(), _oseventlisteners.end(), listener);

            if (found == _oseventlisteners.end())
            {
                con_err("no os listener found to unregister");
            }
            else
            {
                listener->deinitDevice();
                _oseventlisteners.erase(found);
            }
        }

        void handle() override
        {
            MSG msg;

            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        void setVSync(bool vsync) override
        {
            /*PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress(“wglSwapIntervalEXT”);
            wglSwapIntervalEXT(vsync ? 1 : 0);*/
        }

        bool getVSync() const override
        {
            /*PFNWGLSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC) wglGetProcAddress(“wglGetSwapIntervalEXT”);
            return wglGetSwapInter() > 0 ? true : false;*/
            return false;
        }

        void swapBuffers() override
        {
            IGOR_PROFILER_SCOPED(swap);
            SwapBuffers(_hDC);
        }

        void calcClientSize() override
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

        void setDoubleClick(bool doubleClick) override
        {
            if (_doubleClick != doubleClick)
            {
                UnregisterClass(TEXT("Igor"), _windowClass.hInstance);

                _doubleClick = doubleClick;

                if (_doubleClick)
                {
                    _windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
                }
                else
                {
                    _windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
                }

                RegisterClass(&_windowClass);
            }
        }

        bool open() override
        {
            int32 posx = _x;
            int32 posy = _y;

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
                    con_err("The resolution (" << _clientWidth << "x" << _clientHeight << ") is not supported by this system. Using window mode instead");
                    _fullscreen = false;
                    posx = _x;
                    posy = _y;
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
            _hWnd = CreateWindowEx(_dwExStyle, TEXT("Igor"), _title.getData(), _dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, posx, posy, _width, _height, 0, 0, _moduleHandle, _window);

            if (!_hWnd)
            {
                con_err("can't create window");
                close();
                return false;
            }

            static PIXELFORMATDESCRIPTOR pfd = // pfd Tells Windows How We Want Things To Be
                {
                    sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
                    1,                             // Version Number
                    PFD_DRAW_TO_WINDOW |           // Format Must Support Window
                        PFD_SUPPORT_OPENGL |       // Format Must Support OpenGL
                        PFD_DOUBLEBUFFER,          // Must Support Double Buffering
                    PFD_TYPE_RGBA,                 // Request An RGBA Format
                    _colordepth,                   // Select Our Color Depth
                    0, 0, 0, 0, 0, 0,              // Color Bits Ignored
                    0,                             // No Alpha Buffer
                    0,                             // Shift Bit Ignored
                    0,                             // No Accumulation Buffer
                    0, 0, 0, 0,                    // Accumulation Bits Ignored
                    _zdepth,                       // Z-Buffer bits (Depth Buffer)
                    1,                             // use stencil buffer
                    0,                             // No Auxiliary Buffer
                    PFD_MAIN_PLANE,                // Main Drawing Layer
                    0,                             // Reserved
                    0, 0, 0                        // Layer Masks Ignored
                };

            _hDC = GetDC(_hWnd);
            if (_hDC == nullptr)
            {
                con_err("can't create Device Context");
                close();
                return false;
            }

            int32 pixelformat = ChoosePixelFormat(_hDC, &pfd);
            if (pixelformat == 0)
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

            _renderContext = createRenderContext(nullptr);
            if (_renderContext == nullptr)
            {
                close();
                return false;
            }

            if (!makeCurrent(_renderContext))
            {
                close();
                return false;
            }

            if (!gladLoadGL())
            {
                con_err("Can't initialize Glad");
                close();
                return false;
            }                      

            ShowWindow(_hWnd, SW_SHOW);
            SetForegroundWindow(_hWnd);
            SetFocus(_hWnd);

            _isOpen = true;

            registerOSListener(&iMouse::getInstance());
            registerOSListener(&iKeyboard::getInstance());

            return true;
        }

        void close() override
        {
            if (_fullscreen)
            {
                ChangeDisplaySettings(NULL, 0);
            }

            if (_renderContext != nullptr)
            {
                if (!makeCurrent(_renderContext))
                {
                    con_err("detach of render context failed");
                }

                if (!deleteRenderContext(_renderContext))
                {
                    con_err("release of render context failed");
                }

                _renderContext = nullptr;
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
        }

        void setSizeByDesktop() override
        {
            int32 width, height;
            getDesktopSize(width, height);
            setSize(width, height);
        }

        void setClientSize(int32 width, int32 height) override
        {
            setSize(width + (GetSystemMetrics(SM_CXSIZEFRAME) << 2), height + (GetSystemMetrics(SM_CYSIZEFRAME) << 2) + GetSystemMetrics(SM_CYCAPTION));
        }

        void setSize(int32 width, int32 height) override
        {
            _width = width;
            _height = height;
            calcClientSize();

            if (_isOpen)
            {
                if (_fullscreen)
                {
                    MoveWindow(_hWnd, 0, 0, _width, _height, true);
                }
                else
                {
                    MoveWindow(_hWnd, _x, _y, _width, _height, true);
                }
            }
        }

        void setPosition(int32 xPos, int32 yPos) override
        {
            _x = xPos;
            _y = yPos;

            if (!_fullscreen &&
                _isOpen)
            {
                MoveWindow(_hWnd, _x, _y, _width, _height, true);
            }
        }

        void setFullscreen(bool fullscreen) override
        {
            if (!_isOpen)
            {
                _fullscreen = fullscreen;
                calcClientSize();
            }
            else
            {
                // TODO
                con_err("can't switch between window and fullscreen when window opened");
            }
        }

        void setCentered() override
        {
            if (!_fullscreen)
            {
                int32 width, height;
                getDesktopSize(width, height);

                // make sure the window title stays on screen
                int32 y = std::max((height - _height) / 2, 0);
                int32 x = std::max((width - _width) / 2, 0);
                setPosition(x, y);
            }
        }

        void getDesktopSize(int32 &width, int32 &height) override
        {
            width = GetSystemMetrics(SM_CXSCREEN);
            height = GetSystemMetrics(SM_CYSCREEN);
        }

    private:
        /*! window extended style
         */
        DWORD _dwExStyle = 0;

        /*! window style
         */
        DWORD _dwStyle = 0;

        /*! screen/display settings
         */
        DEVMODE _dmScreenSettings;

        /*! module handle
         */
        HINSTANCE _moduleHandle = nullptr;

        /*! window handle
         */
        HWND _hWnd = nullptr;

        /*! device context
         */
        HDC _hDC = nullptr;

        /*! primary render context
         */
        iRenderContextPtr _renderContext = nullptr;

        /*! secures wgl interfaces used within window
         */
        iaMutex _wglMutex;

        /*! window class
         */
        WNDCLASS _windowClass;

        iRenderContextPtr createRenderContext(iRenderContextPtr renderContext) override
        {
            iRenderContextPtr result;

            _wglMutex.lock();
            result = wglCreateContext(_hDC);
            _wglMutex.unlock();

            if (result == nullptr)
            {
                con_err("can't create rendering context");
                return result;
            }

            if (renderContext != nullptr)
            {
                _wglMutex.lock();
                if (!wglShareLists((HGLRC)renderContext, (HGLRC)result))
                {
                    con_err("can't share lists");
                }
                _wglMutex.unlock();
            }

            return result;
        }

        iRenderContextPtr getDefaultRenderContext() const override
        {
            return _renderContext;
        }

        bool makeCurrent(iRenderContextPtr renderContext)
        {
            bool result = false;

            _wglMutex.lock();
            result = wglMakeCurrent(_hDC, (HGLRC)renderContext);
            _wglMutex.unlock();

            if (!result)
            {
                con_err("can't make render context current");
            }

            return result;
        }

        bool deleteRenderContext(iRenderContextPtr renderContext)
        {
            bool result = false;

            _wglMutex.lock();
            result = wglDeleteContext((HGLRC)renderContext);
            _wglMutex.unlock();

            if (!result)
            {
                con_err("can't delete render context");
            }

            return result;
        }

        bool openOGLWindow()
        {
        }

        bool onOSEvent(const iOSEvent *os_event)
        {
            bool result = false;
            for (auto listener : _oseventlisteners)
            {
                if (listener->onOSEvent(static_cast<const void *>(os_event)))
                {
                    result = true;
                }
            }

            return result;
        }
    };

    // TODO this is a mess
    LRESULT CALLBACK WndProc(HWND _hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        iWindowPtr window = nullptr;
        iWindowImplWindows *impl = nullptr;

        if (WM_CREATE == uMsg)
        {
            LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
            window = (iWindowPtr )(lpcs->lpCreateParams);
            SetWindowLongPtr(_hWnd, GWLP_USERDATA, (LONG_PTR)window);
            impl = static_cast<iWindowImplWindows *>(iWindowImpl::getImpl(window));
            impl->sizeChanged(impl->_width, impl->_height);
            return true;
        }

        window = (iWindowPtr )GetWindowLongPtr(_hWnd, GWLP_USERDATA);

        if (window)
        {
            iOSEvent os_event = {_hWnd, uMsg, wParam, lParam};
            impl = static_cast<iWindowImplWindows *>(iWindowImpl::getImpl(window));
            if (impl->onOSEvent(&os_event))
            {
                return true;
            }

            switch (uMsg)
            {
            case WM_DESTROY:
                impl->_hWnd = nullptr;
                PostQuitMessage(0);
                impl->closeEvent();
                return true;

            case WM_SIZE:
                if (nullptr != impl->_hWnd)
                {
                    RECT rect;
                    GetWindowRect(impl->_hWnd, &rect);
                    impl->sizeChanged(rect.right - rect.left, rect.bottom - rect.top);
                }
                return true;

            case WM_MOVE:
                if (nullptr != impl->_hWnd)
                {
                    RECT rect;
                    GetWindowRect(impl->_hWnd, &rect);
                    impl->_x = rect.left;
                    impl->_y = rect.top;
                }
                return true;
            }
        }

        return (DefWindowProc(_hWnd, uMsg, wParam, lParam));
    }

#endif // __IGOR_WINDOWS__

#ifdef __IGOR_LINUX__
    class iWindowImplLinux : public iWindowImpl
    {
        friend class iWindow;

    public:
        iWindowImplLinux(iWindowPtr window)
            : iWindowImpl(window)
        {
        }

        virtual ~iWindowImplLinux() = default;

        void registerOSListener(iOSEventListener *listener)
        {
            auto found = std::find(_oseventlisteners.begin(), _oseventlisteners.end(), listener);

            if (found != _oseventlisteners.end())
            {
                con_err("listener already registered");
            }
            else
            {
                const iOSEventregisterData data = {_window, _display, _xwindow};
                if (listener->initDevice(static_cast<const void *>(&data)))
                {
                    _oseventlisteners.push_back(listener);
                }
            }
        }

        void unregisterOSListener(iOSEventListener *listener)
        {
            auto found = std::find(_oseventlisteners.begin(), _oseventlisteners.end(), listener);

            if (found == _oseventlisteners.end())
            {
                con_err("no os listener found to unregister");
            }
            else
            {
                listener->deinitDevice();
                _oseventlisteners.erase(found);
            }
        }

        void calcClientSize() override
        {
            if (_fullscreen)
            {
                _clientWidth = _width;
                _clientHeight = _height;
            }
            else
            {
                // TODO not supported yet
                _clientWidth = _width;
                _clientHeight = _height;
            }
        }

        void setDoubleClick(bool doubleClick) override
        {
            // TODO
        }

        void swapBuffers() override
        {
            IGOR_PROFILER_SCOPED(swap);
            _glxMutex.lock();
            glXSwapBuffers(_display, glXGetCurrentDrawable());
            _glxMutex.unlock();
        }

        int pending()
        {
            XFlush(_display);
            if (XEventsQueued(_display, QueuedAlready))
            {
                return 1;
            }

            return XPending(_display);
        }

        void handle() override
        {
            iOSEvent os_event;
            os_event._display = _display;
            os_event._visual = _visual;

            while (pending())
            {
                XNextEvent(_display, &os_event._event);

                bool eventHandled = false;

                for (auto listener : _oseventlisteners)
                {
                    if (listener->onOSEvent(&os_event))
                    {
                        eventHandled = true;
                        break;
                    }
                };

                if (!eventHandled)
                {
                    switch (os_event._event.type)
                    {
                    case ConfigureNotify:
                        sizeChanged(os_event._event.xconfigure.width, os_event._event.xconfigure.height);
                        _x = os_event._event.xconfigure.x;
                        _y = os_event._event.xconfigure.y;
                        break;

                    case ClientMessage:
                        _window->close();
                        closeEvent();
                        return;
                    default:;
                    }
                }
            }
        }

        bool open() override
        {
            _display = XOpenDisplay(nullptr);
            if (_display == nullptr)
            {
                con_err("Can't connect to X server !");
                close();
                return false;
            }

            int errorb;
            int event;

            _glxMutex.lock();
            int result = glXQueryExtension(_display, &errorb, &event);
            _glxMutex.unlock();
            if (!result)
            {
                con_err("GLX extension not available! unable to init OpenGL-display...");
                close();
                return false;
            }

            GLint attr[] = {
                GLX_USE_GL,
                GLX_RED_SIZE, 1,
                GLX_GREEN_SIZE, 1,
                GLX_BLUE_SIZE, 1,
                GLX_DEPTH_SIZE, 16,
                GLX_DOUBLEBUFFER,
                GLX_RGBA,
                None};

            _glxMutex.lock();
            _visual = glXChooseVisual(_display, DefaultScreen(_display), attr);
            _glxMutex.unlock();
            if (_visual == nullptr)
            {
                con_err("Can't choose visual!");
                close();
                return false;
            }

            _renderContext = createRenderContext(nullptr);
            if (_renderContext == nullptr)
            {
                close();
                return false;
            }

            _colorMap = XCreateColormap(_display, RootWindow(_display, _visual->screen), _visual->visual, AllocNone);
            if (!_colorMap)
            {
                con_err("Can't create XColormap!");
                close();
                return false;
            }

            _windowAttributes.colormap = _colorMap;
            _windowAttributes.border_pixel = 1;
            _windowAttributes.event_mask = ExposureMask | ButtonPressMask | ButtonReleaseMask |
                                           PointerMotionMask | StructureNotifyMask |
                                           FocusChangeMask | SubstructureNotifyMask |
                                           KeyPressMask | KeyReleaseMask;

            if (_fullscreen)
            {
#ifdef XF86VMODE_FOUND
                XF86VidModeModeInfo **modes;
                int modenum;
                int choosenmode = -1;

                XF86VidModeGetAllModeLines(_display, DefaultScreen(_display), &modenum, &modes);
                _desktopmode = *modes[0];

                for (long i = 0; i < modenum; i++)
                {
                    if ((modes[i]->hdisplay == _width) && (modes[i]->vdisplay == _height))
                    {
                        choosenmode = i;
                    }
                }

                if (choosenmode == -1)
                {
                    // TODO switch to window mode
                    con_err("video mode " << _width << "x" << _height << " is not supported");
                    return false;
                }

                XF86VidModeSwitchToMode(_display, DefaultScreen(_display), modes[choosenmode]);
                XF86VidModeSetViewPort(_display, DefaultScreen(_display), 0, 0);
                XFree(modes);

                _windowAttributes.override_redirect = true;

                _xwindow = XCreateWindow(_display, RootWindow(_display, _visual->screen),
                                         0, 0, _width, _height, 0, _visual->depth, InputOutput, _visual->visual,
                                         CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
                                         &_windowAttributes);

                XWarpPointer(_display, None, _xwindow, 0, 0, 0, 0, 0, 0);
                XMapRaised(_display, _xwindow);
                XGrabKeyboard(_display, _xwindow, True, GrabModeAsync, GrabModeAsync, CurrentTime);
                XGrabPointer(_display, _xwindow, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, _xwindow, None, CurrentTime);
#endif
            }
            else
            {
                _xwindow = XCreateWindow(_display, RootWindow(_display, _visual->screen),
                                         _x, _y, _width, _height,
                                         1, _visual->depth, InputOutput, _visual->visual,
                                         CWBorderPixel | CWColormap | CWEventMask, &_windowAttributes);

                if (!_xwindow)
                {
                    con_err("Can't create window!");
                    close();
                    return false;
                }

                Atom wmDelete = XInternAtom(_display, "WM_DELETE_WINDOW", True);
                XSetWMProtocols(_display, _xwindow, &wmDelete, 1);
                char temp[256];
                _title.getData(temp, 256);
                XSetStandardProperties(_display, _xwindow, temp, temp, None, NULL, 0, NULL);
                XMapRaised(_display, _xwindow);
            }

            _isOpen = true;    

            XSelectInput(_display, _xwindow, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);

            setPosition(_x, _y);
            setClientSize(_width, _height);

            if (!makeCurrent(_renderContext))
            {
                close();
                return false;
            }        

            if (!gladLoadGL())
            {
                con_err("Can't initialize Glad");
                close();
                return false;
            }          

            XMapWindow(_display, _xwindow);
            XFlush(_display);

            registerOSListener(&iMouse::getInstance());
            registerOSListener(&iKeyboard::getInstance());

            return true;
        }

        void close() override
        {
            if (_renderContext != nullptr)
            {
                if (!makeCurrent(nullptr))
                {
                    con_err("Could not release drawing context.");
                }

                deleteRenderContext(_renderContext);
                _renderContext = nullptr;
            }

            /* switch back to original desktop resolution if we were in fs */
            if (_fullscreen)
            {
#ifdef XF86VMODE_FOUND
                XF86VidModeSwitchToMode(_display, DefaultScreen(_display), &_desktopmode);
                XF86VidModeSetViewPort(_display, DefaultScreen(_display), 0, 0);
#endif
            }
            XCloseDisplay(_display);

            unregisterOSListener(&iMouse::getInstance());
            unregisterOSListener(&iKeyboard::getInstance());

            _display = nullptr;
            _isOpen = false;
        }

        void setSizeByDesktop() override
        {
            int32 width, height;
            getDesktopSize(width, height);
            setSize(width, height);
        }

        void setClientSize(int32 width, int32 height) override
        {
            // TODO recalculate correct client size
            setSize(width, height);
        }

        void setSize(int32 width, int32 height) override
        {
            _width = width;
            _height = height;
            calcClientSize();

            if (_isOpen)
            {
                if (_fullscreen)
                {
                    // TODO
                }
                else
                {
                    XResizeWindow(_display, _xwindow, _width, _height);
                }
                sizeChanged(_width, _height); //  XResizeWindow does not seem to send a resize event
            }
        }

        void
        setPosition(int32 xPos, int32 yPos) override
        {
            _x = xPos;
            _y = yPos;

            if (!_fullscreen &&
                _isOpen)
            {
                XMoveWindow(_display, _xwindow, _x, _y);
            }
        }

        void setFullscreen(bool fullscreen) override
        {
            if (!_isOpen)
            {
                _fullscreen = fullscreen;
                calcClientSize();
            }
            else
            {
                // TODO
                con_err("can't switch between window and fullscreen when window opened");
            }
        }

        void setCentered() override
        {
            if (!_fullscreen)
            {
                int32 width, height;
                getDesktopSize(width, height);
                int32 x = std::max((width - _width) / 2, 0);
                int32 y = std::max((height - _height) / 2, 0);
                setPosition(x, y);
            }
        }

        iRenderContextPtr createRenderContext(iRenderContextPtr renderContext) override
        {
            _glxMutex.lock();
            iRenderContextPtr result = static_cast<iRenderContextPtr>(glXCreateContext(_display, _visual, static_cast<GLXContext>(renderContext), true));
            _glxMutex.unlock();

            con_assert(result != nullptr, "Can't create GLX-context!");
            return result;
        }

        iRenderContextPtr getDefaultRenderContext() const override
        {
            return _renderContext;
        }

        bool makeCurrent(iRenderContextPtr renderContext) override
        {
            _glxMutex.lock();
            bool result = glXMakeCurrent(_display, renderContext != nullptr ? _xwindow : None, static_cast<GLXContext>(renderContext));
            _glxMutex.unlock();

            if (!result)
            {
                con_err("can't make render context current");
                return false;
            }

            return true;
        }

        bool deleteRenderContext(iRenderContextPtr renderContext) override
        {
            _glxMutex.lock();
            glXDestroyContext(_display, static_cast<GLXContext>(renderContext));
            _glxMutex.unlock();

            return true;
        }

        void setVSync(bool vsync) override
        {
            _vsync = vsync;
            // glXSwapIntervalEXT(_display, _xwindow, _vsync ? 1 : 0);
        }

        bool getVSync() const override
        {
            return _vsync;
        }

        void getDesktopSize(int32 &width, int32 &height) override
        {
            Display *display = nullptr;
            if (_display != nullptr)
            {
                display = _display;
            }
            else
            {
                display = XOpenDisplay(nullptr);
            }

            Screen *screen = DefaultScreenOfDisplay(display);
            width = screen->width;
            height = screen->height;
        }

    private:
        /*! display instance
         */
        Display *_display = nullptr;

        /*! window instance
         */
        Window _xwindow;

        /*! visual information
         */
        XVisualInfo *_visual = nullptr;

        /*! color map
         */
        Colormap _colorMap;

        /*! window attributes
         */
        XSetWindowAttributes _windowAttributes;

        /*! secures glx interfaces used within window
         */
        iaMutex _glxMutex;

        /*! primary render context
         */
        iRenderContextPtr _renderContext = nullptr;

        /*! desktop mode
         */
#ifdef XF86VMODE_FOUND
        XF86VidModeModeInfo _desktopmode;
#endif

        /*! stores the last vsync setting
         */
        bool _vsync = true;
    };

#endif // __IGOR_LINUX__

    iWindow::iWindow(const iaString &title)
    {
        _windowID = iWindow::_idGenerator.createID();

#ifdef __IGOR_WINDOWS__
        _impl = new iWindowImplWindows(this);
#endif
#ifdef __IGOR_LINUX__
        _impl = new iWindowImplLinux(this);
#endif

        if(!title.isEmpty())
        {
            setTitle(title);
        }

        _impl->calcClientSize();
    }

    iWindow::~iWindow()
    {
        if (_impl->_isOpen)
        {
            close();
        }

        delete _impl;
    }

    iWindowID iWindow::getID() const
    {
        return _windowID;
    }

    void iWindow::onSizeChanged(int32 width, int32 height)
    {
        _impl->_width = width;
        _impl->_height = height;

        _impl->calcClientSize();

        // update views with new client size
        iaRectanglei windowRect;
        windowRect.setWidth(_impl->_clientWidth);
        windowRect.setHeight(_impl->_clientHeight);

        for (auto view : _views)
        {
            view->updateWindowRect(windowRect);
        }
    }

    void iWindow::handle()
    {
        _impl->handle();
    }

    void iWindow::swapBuffers()
    {
        _impl->swapBuffers();
    }

    void iWindow::setVSync(bool vsync)
    {
        _impl->setVSync(vsync);
    }

    bool iWindow::getVSync() const
    {
        return _impl->getVSync();
    }

    bool iWindow::open()
    {
        if (_impl->_isOpen)
        {
            con_warn("window is already opened");
            return false;
        }

        con_info("open window \"" << _impl->_title << "\" (" << _impl->_clientWidth << "x" << _impl->_clientHeight << ")" << (_impl->_fullscreen ? " FULLSCREEN" : ""));

        bool result = _impl->open();

        if (result)
        {
            iTaskManager::getInstance().createRenderContextThreads(this);
            
            iRenderer::getInstance().init();
            iMaterialResourceFactory::getInstance().init();
            iTextureResourceFactory::getInstance().init();
            _impl->swapBuffers();

            iApplication::getInstance().onEvent(iEventPtr(new iEventWindowOpen(this)));
        }

        return result;
    }

    void iWindow::close()
    {
        if (!_impl->_isOpen)
        {
            con_warn("window was not opened");
        }

        iTextureResourceFactory::getInstance().deinit();
        iRenderer::getInstance().deinit();
        iMaterialResourceFactory::getInstance().deinit();

        iTaskManager::getInstance().killRenderContextThreads(this);

        _impl->close();

        con_info("closed window \"" << _impl->_title << "\"");
    }

    void iWindow::getDesktopSize(int32 &width, int32 &height)
    {
        _impl->getDesktopSize(width, height);
    }

    void iWindow::setSizeByDesktop()
    {
        _impl->setSizeByDesktop();
    }

    void iWindow::setClientSize(int32 width, int32 height)
    {
        _impl->setClientSize(width, height);
    }

    void iWindow::setSize(int32 width, int32 height)
    {
        _impl->setSize(width, height);
    }

    void iWindow::setPosition(int32 xPos, int32 yPos)
    {
        _impl->setSize(xPos, yPos);
    }

    bool iWindow::isFullscreen()
    {
        return _impl->_fullscreen;
    }

    void iWindow::setFullscreen(bool fullscreen)
    {
        _impl->setFullscreen(fullscreen);
    }

    void iWindow::setCentered()
    {
        _impl->setCentered();
    }

    void iWindow::setZDepth(uint8 zdepth)
    {
        if (_impl->_isOpen)
        {
            con_warn("depth buffer depth can only be changed before the windows was opened");
            return;
        }

        _impl->_zdepth = zdepth;
    }

    uint8 iWindow::getZDepth() const
    {
        return _impl->_zdepth;
    }

    uint8 iWindow::getColorDepth() const
    {
        return _impl->_colordepth;
    }

    void iWindow::setColorDepth(uint8 colordepth)
    {
        if (_impl->_isOpen)
        {
            con_warn("color depth can only be changed before the windows was opened");
            return;
        }

        _impl->_colordepth = colordepth;
    }

    void iWindow::addView(iView *view, int32 zIndex)
    {
        con_assert_sticky(view != nullptr, "zero pointer");

        _views.push_back(view);
        iaRectanglei windowRect;
        windowRect.setWidth(_impl->_clientWidth);
        windowRect.setHeight(_impl->_clientHeight);
        view->updateWindowRect(windowRect);
        view->setZIndex(zIndex);

        std::sort(_views.begin(), _views.end(), [](const iView *a, const iView *b) -> bool
                  { return a->getZIndex() < b->getZIndex(); });
    }

    void iWindow::removeView(iView *view)
    {
        auto iter = std::find(_views.begin(), _views.end(), view);
        if (iter != _views.end())
        {
            _views.erase(iter);
        }
        else
        {
            con_warn("view was not registered to window");
        }
    }

    bool iWindow::isOpen() const
    {
        return _impl->_isOpen;
    }

    void iWindow::setTitle(const iaString &title)
    {
        if (!title.isEmpty())
        {
            _impl->_title = title;
            // TODO update title when windows is open already
        }
        else
        {
            con_err("title can not be empty");
        }
    }

    iaString iWindow::getTitle() const
    {
        return _impl->_title;
    }

    int32 iWindow::getWidth() const
    {
        return _impl->_width;
    }

    int32 iWindow::getHeight() const
    {
        return _impl->_height;
    }

    int32 iWindow::getClientWidth() const
    {
        return _impl->_clientWidth;
    }

    int32 iWindow::getClientHeight() const
    {
        return _impl->_clientHeight;
    }

    int32 iWindow::getXPos() const
    {
        return _impl->_x;
    }

    int32 iWindow::getYPos() const
    {
        return _impl->_y;
    }

    iRenderContextPtr iWindow::createRenderContext(iRenderContextPtr renderContext)
    {
        return _impl->createRenderContext(renderContext);
    }

    iRenderContextPtr iWindow::getDefaultRenderContext() const
    {
        return _impl->getDefaultRenderContext();
    }

    bool iWindow::makeCurrent(iRenderContextPtr renderContext)
    {
        return _impl->makeCurrent(renderContext);
    }

    bool iWindow::deleteRenderContext(iRenderContextPtr renderContext)
    {
        return _impl->deleteRenderContext(renderContext);
    }

    void iWindow::draw()
    {        
        iRenderer::getInstance().clearStats();

        for (auto view : _views)
        {
            view->draw();
        }        
        swapBuffers();
    }

    void iWindow::setDoubleClick(bool doubleClick)
    {
        con_assert(!_impl->_isOpen, "window is open already");

        if (!_impl->_isOpen)
        {
            _impl->setDoubleClick(doubleClick);
        }
    }

    bool iWindow::getDoubleClick() const
    {
        return _impl->_doubleClick;
    }

}; // namespace igor
