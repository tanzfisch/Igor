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
// (c) Copyright 2014-2016 by Martin Loga
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
// contact: martinloga@gmx.de  

#ifndef __iWINDOW__
#define __iWINDOW__

#include <windows.h>

#include <iDefines.h>

#include <iaFlushVector.h>
#include <iaEvent.h>
#include <iaString.h>
using namespace IgorAux;

#include <list>
#include <mutex>
using namespace std;

namespace Igor
{
	class iOSEventListener;
	class iView;

	/*! window close event
	*/
	iaEVENT(WindowCloseEvent, WindowCloseDelegate, void, (), ());

    /*! window resize event
    */
    iaEVENT(WindowResizeEvent, WindowResizeDelegate, void, (int32 clientWidth, int32 clientHeight), (clientWidth, clientHeight));
    
    /*! application window

	\todo currently Igor only fully supports one window. 
	\todo add some events like closing window, move window etc.
    \todo more documentation
	*/
	class Igor_API iWindow
	{
		friend class iApplication;
		friend class iRenderContextThread;

		friend LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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

        /*! sets title of window

        \param title the windows title
        */
        void setTitle(const iaString& title);

        /*! \returns title of the window
        */
        iaString getTitle() const;

        /*! \returns width of window
        */
        uint32 getWidth() const;

        /*! \returns height of window
        */
        uint32 getHeight() const;

        /*! \returns width of render area
        */
        uint32 getClientWidth() const;

        /*! \returns height of render area
        */
        uint32 getClientHeight() const;

        /*! \returns horizontal position of window
        */
        uint32 getXPos() const;

        /*! \returns vertical position of window
        */
        uint32 getYPos() const;

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
        void setColorDepth(unsigned char colordepth);

        /*! set resolution of window

        set indirect the size of the window

        \param width horizontal width in pixel
        \param height vertical height in pixel
        */
        void setSize(uint32 width, uint32 height);

        /*! set window resolution so same value as desktop resolution
        */
        void setSizeByDesktop();

        /*! set position of window relative to desktop

        \param x horizontal position in pixel
        \param y vertical position in pixel
        */
        void setPosition(uint32 x, uint32 y);

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
        void addView(iView* view);

        /*! removes view from the window

        \param view the view to be removed
        */
        void removeView(iView* view);

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

	protected:

        /*! swap buffer seciton id
        */
        uint32 _swapBufferSectionID = 0;

        /*! triggered by window closed OS message
        */
        WindowCloseEvent _windowCloseEvent;

        /*! triggered by window resize OS message
        */
        WindowResizeEvent _windowResizeEvent;

		/*! list of windows

		\todo why is this a flush list again?
		*/
        iaFlushVector<iView*> _views;
						
		/*! list of registeres os event listeners

		currently all devices are singletons so all listeners will be added automatically
		*/
		list<iOSEventListener*> _oseventlisteners;

		/*! width of the render area in pixel
		*/
		uint32 _width = 640;

		/*! height of the render area in pixel
		*/
		uint32 _height = 480;

        /*! the actuall width of the rendering area.

        If fullscreen it's equal to _width. If window mode it's usually less then _width depending on window design settings.
        */
        uint32 _clientWidth = 0;

        /*! the actuall height of the rendering area.

        see details at _clientWidth
        */
        uint32 _clientHeight = 0;

		/*! x position of the window in pixel
		*/
		uint32 _xPos = 100;

		/*! y position of the window in pixel
		*/
		uint32 _yPos = 100;

		/*! color depth of the render area in bit per pixel
		*/
		unsigned char _colordepth = 32;

		/*! true for fullscreen mode
		*/
		bool _fullscreen = false;

		/*! the window title
		*/
        iaString _title = IGOR_DFAULT_WINDOW_TITLE;

		/*! true if the window is opened
		*/
		bool _isOpen = false;

        /*! flag if the window gets double click events from windows
        */
        bool _doubleClick = false;

		/*! handle to be called in the main loop

		handles os messages
		*/
		virtual void handle();

		/*! swaps video buffers
		*/
		virtual void swapBuffers();

		/*! draws contend of view in to the window
		*/
		void draw();

		/*! initializes rendering context render buffers etc.
		*/
		bool initOpenGLContext();

		/*! \returns window handle
		*/
		HWND getHandle() const;

		/*! \returns device context handle
		*/
		HDC getDeviceContext() const;

		/*! \returns render context handle
		*/
		HGLRC getRenderContext() const;

        /*! \returns render context created for current device context
        */
        HGLRC createRenderContext();

        /*! deletes render context

        \param renderContext render context to delte
        */
        bool deleteRenderContext(HGLRC renderContext);

        /*! shares render lists between window render context and a second one

        \param renderContext the render context to share with
        \return true if succeeded
        */
        bool shareLists(HGLRC renderContext);

        /*! makes render contex current

        \param renderContext render context to make current
        \returns true if succeeded
        */
        bool makeCurrent(HGLRC renderContext);

		/*! registers a listener to os events like windows messages

		\param listener pointer to listener
		*/
		void registerOSListener(iOSEventListener* listener);

		/*! deregisters os event listener

		\param listener pointer to listener
		*/
		void unregisterOSListener(iOSEventListener* listener);

        /*! this function handles if the size got updated from outside e.g. by user

        \param width new window size
        \param height new window height
        */
        void updateSize(int32 width, int32 height);

        /*! this function handles if the position got updated from outside e.g. by user

        \param x new window x position
        \param y new window y position
        */
        void updatePos(int32 x, int32 y);

        /*! updates the client size of this window

        If the window is not open yet the client size will be calculated
        */
        void calcMinSize();
		
    private:

        DWORD _dwExStyle = 0;
        DWORD _dwStyle = 0;
        DEVMODE _dmScreenSettings;
        HINSTANCE _hInstance = nullptr;
        HWND _hWnd = nullptr;

        /*! device context
        */
        HDC _hDC = nullptr;

        /*! primary render context
        */
        HGLRC _renderContext = nullptr;

        /*! secures wgl interfaces used within window
        */
        mutex _wglMutex;

        MSG _msg;
        WNDCLASS _wc;

	};

};

#endif
