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
// (c) Copyright 2014-2015 by Martin Loga
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

#include <iaEvent.h>
#include <iaSingleton.h>
#include <iaFlushList.h>
using namespace IgorAux;

namespace Igor
{

	/*! This event is triggered first in every application cycle. 
	
	Since the last thing in application cycle is swap buffers this is also related to render frame intervals.
	*/
	iaEVENT(iApplicationHandleEvent, iApplicationHandleDelegate, void, (), ());

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

        /*! stopes the application

        quits the main application loop
        */
        void stop();

        /*! returns true if application is still running
        */
        bool isRunning();

        /*! registers application handle delegate
        */
        void registerApplicationHandleDelegate(iApplicationHandleDelegate handleDelegate);

        /*! unregisters application handle delegate
        */
        void unregisterApplicationHandleDelegate(iApplicationHandleDelegate handleDelegate);

	private:

        /*! frame section id
        */
        uint32 _frameSectionID = 0;

        /*! handle section id
        */
        uint32 _handleSectionID = 0;

        /*! handle callbacks section ID
        */
        uint32 _handleCallbacksSectionID = 0;

		/*! flag if the aplication is currently running
		*/
		bool _running = false;
		
		/*! list of windows registered to the application
		*/
		iaFlushList<iWindow*> _windows;

		/*! handle event called after last render frame
		*/
		iApplicationHandleEvent _handleEvent;

		/*! triggers ApplicationHandleEvent and updates windows
		*/
		void handle();

        /*! rendering
        */
        void draw();

		/*! add window

		\param window pointer to window
		*/
		void addWindow(iWindow* window);

		/*! remove window

		\param window pointer to window
		*/
		void removeWindow(iWindow* window);

		/*! ctor

		initializes member variables
		*/
		iApplication();

		/*! dtor

		does nothing
		*/
		virtual ~iApplication();

	};

};

#endif