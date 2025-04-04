//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin Loga
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

#ifndef IGOR_APPLICATION_H
#define IGOR_APPLICATION_H

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

        friend class iModule<iApplication>;

    public:
        /*! starts the application

        starts the main loop of the application and continues until stop() is called

        \returns exit code
        */
        int32 run();

        /*! stops the application

        quits the main application loop. works only in combination with iApplication::run

        \param exitCode optional exit code
        */
        void exit(int32 exitCode = 0);

        /*! stops/pauses the engine

        pauses all simulation but not IO or rendering

        \param pause if true simulations are paused
        */
        void pause(bool pause);

        /*! \returns true if engine is paused
        */
        bool isPaused() const;

        /*! returns true if application is still running
        */
        bool isRunning();

        /*! triggered on event

        \param event the triggered event
        */
        void onEvent(iEventPtr event);

        /*! adds layer

        Adding a layer to the layer stack passes ownership to the layer stack.

        Note: Most layers assume to be added after the window was already opened and the renderer being ready to receive calls.

        \param layer the layer to be added
        */
        void addLayer(iLayerPtr layer);

        /*! removes layer

        removing a layer from the layer stack passes ownership back to the caller

        \param layer the layer to be removed
        */
        void removeLayer(iLayerPtr layer);

        /*! clears layer stack and deletes all layers on it
        */
        void clearLayers();

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

        /*! destroys the window if it existed
		*/
        void destroyWindow();

        /*! \returns pointer to window

        if window did not exist before it gets created here
        */
        iWindowPtr getWindow();

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

        /*! flag if the application is currently running
		*/
        bool _running = false;

        /*! if true engine is paused
        */
        bool _paused = false;

        /*! the main window (only one supported)
		*/
        iWindowPtr _window = nullptr;

        /*! the layer stack
        */
        iLayerStack _layerStack;

        /*! the exit code
        */
        int32 _exitCode = 0;

        /*! handles events in the event queue
        */
        void dispatch();

        /*! updates layer stack
        */
        void onUpdateLayerStack();

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
        void updateWindow();

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

#endif // IGOR_APPLICATION_H