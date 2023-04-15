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
// (c) Copyright 2012-2023 by Martin Loga
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
#include <igor/resources/module/iModule.h>

using namespace iaux;

#include <vector>
#include <array>

namespace igor
{

    /*! Application
	
	The main entry point to an Igor application
	*/
    class IGOR_API iApplication : public iModule<iApplication>
    {

        friend class iWindow;
        friend class iModule<iApplication>;

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

        /*! triggered on event

        \param event the triggered event
        */
        void onEvent(iEventPtr event);

        /*! adds layer to stack for given window

        adding a layer to the layer stack passes ownership to the layer stack

        most layers assume to be added after the window was already opened and the renderer being ready to receive calls but layers can be added and removed any time

        \param layer the layer to be added
        */
        void addLayer(iLayer *layer);

        /*! removes layer from stack of given window

        removing a layer from the layer stack passes ownership back to the caller

        \param layer the layer to be removed
        */
        void removeLayer(iLayer *layer);

        /*! clears layer stack
        */
        void clearLayerStack();

        /*! \returns true if given event type is blocked

        \param eventType the given event type
        */
        bool isBlockedEvent(iEventType eventType);

        /*! unblocks the given event type

        \param eventType the given event type
        */
        void unblockEvent(iEventType eventType);

        /*! blocks the given event type

        \param eventType the given event type
        */
        void blockEvent(iEventType eventType);

        /*! creates window

        \returns window id
		*/
        iWindowPtr createWindow(const iaString& title = "");

        /*! destroy window
		*/
        void destroyWindow(iWindowPtr window);

        /*! \returns window for given window id

        \param windowID the given window id
        */
        iWindowPtr getWindow(iWindowID windowID) const;

    private:

        /*! queue of events
        */
        std::vector<iEventPtr> _eventQueue;

        /*! blocked events list
        */
        std::array<bool, (int)iEventType::iEventTypeCount> _blockedEvents;

        /*! securing the event queue
        */
        iaMutex _eventQueueMutex;

        /*! application profiler section id
        */
        uint32 _applicationSectionID;

        /*! evaluation profiler section id
        */
        uint32 _evaluationSectionID;

        /*! physics profiler section id
        */
        uint32 _physicsSectionID;

        /*! handle callbacks profiler section ID
        */
        uint32 _handleCallbacksSectionID = 0;

        /*! flag if the aplication is currently running
		*/
        bool _running = false;

        /*! list of windows registered to the application
		*/
        std::vector<iWindowPtr> _windows;

        /*! additional layer stack for windowless applications
            */
        iLayerStack _layerStack;

        /*! handles events in the event queue
        */
        void dispatch();

        /*! triggers pre draw event on all layers
        */
        void preDraw();

        /*! dispatch event on given layer stack

        \param event the event to despatch
        \param layerStack the given layer stack 
        \returns true if event was consumed
        */
        bool dispatchOnStack(iEvent &event, iLayerStack &layerStack);

        /*! handles window close event

        \param event the window close event
        */
        bool onWindowClose(iEventWindowClose &event);

        /*! triggers ApplicationHandleEvent and updates windows
		*/
        void onWindowUpdate();

        /*! rendering
        */
        void draw();

        /*! as alternative to run you can set up your own main loop and call iterate in it but run is recommended to use
        */
        void iterate();

        /*! init members
		*/
        iApplication();

        /*! clean up
		*/
        virtual ~iApplication();
    };

}; // namespace igor

#endif // __IGOR_APPLICATION_H__