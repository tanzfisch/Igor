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
// contact: martinloga@gmx.de

#ifndef __iAPPLICATION__
#define __iAPPLICATION__

#include <igor/iDefines.h>

#include <iaux/system/iaEvent.h>
#include <iaux/system/iaSingleton.h>
using namespace iaux;

#include <vector>

namespace igor
{

    class iWindow;

    /*! This event is triggered once per frame right before the rendering
	*/
    iaEVENT(iPreDrawEvent, iPreDrawDelegate, void, (), ());

    /*! This event is triggered once per frame right after the rendering
    */
    iaEVENT(iPostDrawEvent, iPostDrawDelegate, void, (), ());

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

        /*! as alternative to run you can set up your own main loop and call iterate in it but run is recommended to use
        */
        void iterate();

        /*! stopes the application

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

    private:
        /*! frame performance section id
        */
        uint32 _frameSectionID = 0;

        /*! handle performance section id
        */
        uint32 _handleSectionID = 0;

        /*! evaluation performance section id
        */
        uint32 _evaluationSectionID = 0;

        /*! physics performance section id
        */
        uint32 _physicsSectionID = 0;

        /*! draw performance section id
        */
        uint32 _drawSectionID = 0;

        /*! user performance section id
        */
        uint32 _userSectionID = 0;

        /*! handle callbacks performance section ID
        */
        uint32 _handleCallbacksSectionID = 0;

        /*! flag if the aplication is currently running
		*/
        bool _running = false;

        /*! list of windows registered to the application
		*/
        std::vector<iWindow *> _windows;

        /*! handle event called before rendering
		*/
        iPreDrawEvent _preDrawHandleEvent;

        /*! handle event called after rendering
        */
        iPostDrawEvent _postDrawHandleEvent;

        /*! init statistics sections
        */
        void deinitStatistics();

        /*! deinit statistics sections
        */
        void initStatistics();

        /*! triggers ApplicationHandleEvent and updates windows
		*/
        void windowHandle();

        /*! rendering
        */
        void draw();

        /*! add window

		\param window pointer to window
		*/
        void addWindow(iWindow *window);

        /*! remove window

		\param window pointer to window
		*/
        void removeWindow(iWindow *window);

        /*! ctor

		initializes member variables
		*/
        iApplication();

        /*! dtor

		does nothing
		*/
        virtual ~iApplication();
    };

}; // namespace igor

#endif