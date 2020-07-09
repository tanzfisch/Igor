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

#ifndef __IGOR_APPLICATION_H__
#define __IGOR_APPLICATION_H__

#include <igor/events/iEventWindow.h>
#include <igor/layers/iLayerStack.h>
#include <igor/system/iWindow.h>

#include <iaux/system/iaEvent.h>
#include <iaux/system/iaSingleton.h>
using namespace iaux;

#include <vector>

namespace igor
{

    /*! This event is triggered once per frame right before the rendering
	*/
    iaEVENT(iPreDrawEvent, iPreDrawDelegate, (), ());

    /*! This event is triggered once per frame right after the rendering
    */
    iaEVENT(iPostDrawEvent, iPostDrawDelegate, (), ());

    /*! Application (Singleton)
	
	The main entry point to an Igor application (Singleton)
	*/
    class Igor_API iApplication : public iaSingleton<iApplication>
    {

        friend class iWindow;
        friend class iaSingleton<iApplication>;

    public:
        /*! starts the application

        starts the main loop of the application and continues until stop() is called
        */
        void run();

        /*! stops the application

        quits the main application loop. works only in combination with iApplication::run
        */
        void stop();

        /*! returns true if application is still running
        */
        bool isRunning();

        /*! registers application pre draw handle delegate

        \param handleDelegate delegate to register
        */
        void registerApplicationPreDrawHandleDelegate(iPreDrawDelegate handleDelegate);

        /*! unregisters application pre draw handle delegate

        \param handleDelegate delegate to unregister
        */
        void unregisterApplicationPreDrawHandleDelegate(iPreDrawDelegate handleDelegate);

        /*! registers application post draw handle delegate

        \param handleDelegate delegate to register
        */
        void registerApplicationPostDrawHandleDelegate(iPostDrawDelegate handleDelegate);

        /*! unregisters application post draw handle delegate

        \param handleDelegate delegate to unregister
        */
        void unregisterApplicationPostDrawHandleDelegate(iPostDrawDelegate handleDelegate);

        /*! triggered on event

        \param event the triggered event
        */
        void onEvent(iEventPtr event);

        /*! adds layer to stack for given window

        adding a layer to the layer stack passes ownership to the layer stack

        \param layer the layer to be added
        */
        void addLayer(iLayer *layer);

        /*! removes layer from stack of given window

        removing a layer from the layer stack passes ownership back to the caller

        \param layer the layer to be removed
        */
        void removeLayer(iLayer *layer);

        /*! clears layer stack for given window

        \param window the given window 
        */
        void clearLayerStack(iWindow *window = nullptr);

        /*! creates window

        \returns window id
		*/
        iWindow *createWindow();

        /*! destroy window
		*/
        void destroyWindow(iWindow *window);

        /*! \returns window for given window id

        \param windowID the given window id
        */
        iWindow *getWindow(iWindowID windowID) const;

    private:
        /*! internal window data structure
        */
        struct WindowData
        {
            /*! the window instance
            */
            iWindow *_window;

            /*! layer stack
            */
            iLayerStack _layerStack;
        };

        /*! queue of events
        */
        std::vector<iEventPtr> _eventQueue;

        /*! securing the event queue
        */
        iaMutex _eventQueueMutex;

        /*! frame profiler section id
        */
        uint32 _frameSectionID = 0;

        /*! handle profiler section id
        */
        uint32 _handleSectionID = 0;

        /*! dispatch profiler section id
        */
        uint32 _dispatchSectionID = 0;

        /*! evaluation profiler section id
        */
        uint32 _evaluationSectionID = 0;

        /*! physics profiler section id
        */
        uint32 _physicsSectionID = 0;

        /*! draw profiler section id
        */
        uint32 _drawSectionID = 0;

        /*! user profiler section id
        */
        uint32 _userSectionID = 0;

        /*! handle callbacks profiler section ID
        */
        uint32 _handleCallbacksSectionID = 0;

        /*! flag if the aplication is currently running
		*/
        bool _running = false;

        /*! list of windows registered to the application
		*/
        std::map<iWindowID, WindowData> _windows;

        /*! additional layer stack for windowless applications
            */
        iLayerStack _layerStack;

        /*! handle event called before rendering
		*/
        iPreDrawEvent _preDrawHandleEvent;

        /*! handle event called after rendering
        */
        iPostDrawEvent _postDrawHandleEvent;

        /*! handles events in the event queue
        */
        void dispatchEventStack();

        /*! handles window close event

        \param event the window close event
        */
        bool onWindowClose(iWindowCloseEvent_TMP &event);

        /*! init profiling sections
        */
        void deinitProfiling();

        /*! deinit profiling sections
        */
        void initProfiling();

        /*! triggers ApplicationHandleEvent and updates windows
		*/
        void windowHandle();

        /*! rendering
        */
        void draw();

        /*! as alternative to run you can set up your own main loop and call iterate in it but run is recommended to use
        */
        void iterate();

        /*! does nothing
		*/
        iApplication() = default;

        /*! dtor

		does nothing
		*/
        virtual ~iApplication();
    };

}; // namespace igor

#endif // __IGOR_APPLICATION_H__