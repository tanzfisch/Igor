//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2020 by Martin Loga
//
// This library is free software; you can redistribute it and or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
//
// contact: igorgameengine@protonmail.com

#ifndef __iWINDOW__
#define __iWINDOW__

#include <igor/iDefines.h>

#include <iaux/system/iaEvent.h>
#include <iaux/data/iaString.h>
#include <iaux/system/iaMutex.h>
using namespace iaux;

#include <vector>

namespace igor
{
    class iOSEventListener;
    class iView;
    class iWindowImpl;

    /*! window close event
    */
    iaEVENT(WindowCloseEvent, WindowCloseDelegate, void, (), ());

    /*! window resize event
    */
    iaEVENT(WindowResizeEvent, WindowResizeDelegate, void, (int32 clientWidth, int32 clientHeight), (clientWidth, clientHeight));

    /*! render context pointer definition
    */
    typedef void *iRenderContextPtr;

    /*! application window

    \todo currently Igor only fully supports one window
    */
    class Igor_API iWindow
    {

        friend class iApplication;
        friend class iRenderContextThread;
        friend class iWindowImpl;

    public:
        /*! ctor

        initializes all member variables and registeres os event listeners
        */
        iWindow();

        /*! dtor

        unregisteres os event listeners
        */
        virtual ~iWindow();

        /*! sets if the window understands double clicks

        can only be called as long as window is closed

        \param doubleClick if true window understands double clicks
        */
        void setDoubleClick(bool doubleClick);

        /*! \returns true if the window understands double clicks
        */
        bool getDoubleClick() const;

        /*! sets the usage of vsync or not
        \param vsync if true vsync will be used
        */
        void setVSync(bool vsync);

        /*! \returns true if vsync is activated
        */
        bool getVSync() const;

        /*! sets title of window

        \param title the windows title
        */
        void setTitle(const iaString &title);

        /*! \returns title of the window
        */
        iaString getTitle() const;

        /*! \returns width of window
        */
        int32 getWidth() const;

        /*! \returns height of window
        */
        int32 getHeight() const;

        /*! \returns width of render area
        */
        int32 getClientWidth() const;

        /*! \returns height of render area
        */
        int32 getClientHeight() const;

        /*! \returns horizontal position of window
        */
        int32 getXPos() const;

        /*! \returns vertical position of window
        */
        int32 getYPos() const;

        /*! returns if window is open or closed

        \returns true: window is open; false: window is closed
        */
        bool isOpen() const;

        /*! opens window

        \returns true: success; false: fail
        */
        bool open();

        /*! closes window
        */
        void close();

        /*! set color depth

        \param colordepth in bit per pixel
        */
        void setColorDepth(uint8 colordepth);

        /*! \returns color buffer depth
        */
        uint8 getColorDepth() const;

        /*! set depth buffer depth

        \param zdepth the depth buffer depth in bits
        */
        void setZDepth(uint8 zdepth);

        /*! \returns depth buffer depth in bits
        */
        uint8 getZDepth() const;

        /*! set resolution of window

        \param width horizontal width in pixel
        \param height vertical height in pixel
        */
        void setSize(int32 width, int32 height);

        /*! set resolution of window by setting the client rectangle size

        \param width horizontal width in pixel of client rectangle
        \param height vertical height in pixel of client rectangle
        */
        void setClientSize(int32 width, int32 height);

        /*! set window resolution so same value as desktop resolution
        */
        void setSizeByDesktop();

        /*! returns desktop size

        \param[out] width the width of the desktop
        \param[out] height the height of the desktop
        */
        void getDesktopSize(int32 &width, int32 &height);

        /*! set position of window relative to desktop

        \param x horizontal position in pixel
        \param y vertical position in pixel
        */
        void setPosition(int32 x, int32 y);

        /*! sets the windows full screen or window mode

        \param fullscreen true: set window to full screen mode; false: set window to window mode
        */
        void setFullscreen(bool fullscreen = true);

        /*! centers the window relative to the desktop horizontally and vertically
        */
        void setCentered();

        /*! \returns if window is in full screen or window mode
        */
        bool isFullscreen();

        /*! adds view to the window

        \param view the view to be added
        \todo would be nice to manipulate the order of views beeing added
        */
        void addView(iView *view, int32 zIndex = 0);

        /*! removes view from the window

        \param view the view to be removed
        */
        void removeView(iView *view);

        /*! regiters delegate to window close event

        \param windowCloseDelegate the delegate to be registered
        */
        void registerWindowCloseDelegate(WindowCloseDelegate windowCloseDelegate);

        /*! unregiters delegate from window close event

        \param windowCloseDelegate the delegate to be unregistered
        */
        void unregisterWindowCloseDelegate(WindowCloseDelegate windowCloseDelegate);

        /*! register callback to resize event

        \param windowResizeDelegate the delegate to register
        */
        void registerWindowResizeDelegate(WindowResizeDelegate windowResizeDelegate);

        /*! unregister callback from resize event

        \param windowResizeDelegate the delegate to unregister
        */
        void unregisterWindowResizeDelegate(WindowResizeDelegate windowResizeDelegate);

    private:
        /*! pimpl
        */
        iWindowImpl *_impl = nullptr;

        /*! list of views
        */
        std::vector<iView *> _views;

        /*! swap buffer stats section id
        */
        int32 _swapBufferSectionID = 0;

        /*! triggered by window closed OS message
        */
        WindowCloseEvent _windowCloseEvent;

        /*! triggered by window resize OS message
        */
        WindowResizeEvent _windowResizeEvent;

        /*! called when window size has changed
        \param width new width
        \param height new height
        */
        void onSizeChanged(int32 width, int32 height);

        /*! registers a listener to os events like windows messages

        \param listener pointer to listener
        */
        void registerOSListener(iOSEventListener *listener);

        /*! deregisters os event listener

        \param listener pointer to listener
        */
        void unregisterOSListener(iOSEventListener *listener);

        /*! \returns render context created for current device context
        \param renderContext render context to share lists with (optional)
        */
        iRenderContextPtr createRenderContext(iRenderContextPtr renderContext = nullptr);

        /*! makes render contex current

        \param renderContext render context to make current
        \returns true if succeeded
        */
        bool makeCurrent(iRenderContextPtr renderContext);

        /*! deletes render context

        \param renderContext render context to delte
        */
        bool deleteRenderContext(iRenderContextPtr renderContext);

        /*! \returns default render context
        */
        iRenderContextPtr getDefaultRenderContext() const;

        /*! handle to be called in the main loop

        handles os messages
        */
        void handle();

        /*! swaps video buffers
        */
        void swapBuffers();

        /*! draws contend of view in to the window
        */
        void draw();
    };

}; // namespace igor

#endif