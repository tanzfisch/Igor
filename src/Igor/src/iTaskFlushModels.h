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

#ifndef __iTASKFLUSHMODELS__
#define __iTASKFLUSHMODELS__

#include <iTask.h>

namespace Igor
{

    /*! this task runs the model resource manager flush function repeadetly

    when ever there was a model requested this task get's it actually loaded from disk

    \see iModelResourceFactory
    */
	class Igor_API iTaskFlushModels : public iTask
	{

	public:

        /*! initializes member variables

        \param window window connected to render context
        */
        iTaskFlushModels(iWindow* window);

        /*! does nothing
        */
		virtual ~iTaskFlushModels() = default;

        /*! aborts the task
        */
		void abort();

    private:

        /*! runs the task
        */
        void run();

	};

};

#endif