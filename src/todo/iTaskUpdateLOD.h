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

#ifndef __IGOR_TASKUPDATELOD__
#define __IGOR_TASKUPDATELOD__

#include <iTask.h>

namespace Igor
{

    class iScene;

	class iTaskUpdateLOD : public iTask
	{

	public:

        /*! initialize members

        \param scene
        */
        iTaskUpdateLOD(iScene* scene);

        /*! does nothing
        */
		virtual ~iTaskUpdateLOD();

        /*! runs the task
        */
		void run();

        /*! aborts the task
        */
		void stop();

        /*! \returns true: if task is running
        */
        bool isRunning();

    private:

        /*! scene to update
        */
        iScene* _scene = nullptr;

        /*! if true this task keeps flushing resources
        */
        bool _running = false;

        /*! true if task is done
        */
        bool _done = false;

	};

};

#endif // __IGOR_TASKUPDATELOD__
