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

#ifndef __iTASK__
#define __iTASK__

#include <iDefines.h>

#include <iaDelegate.h>
using namespace IgorAux;

namespace Igor
{

    class iWindow;

    /*! need a task to be done in parallel? than derive from this base class
    */
	class Igor_API iTask
	{

        friend class iTaskManager;

    public:

        static const uint32 DEFAULT_PRIORITY = 10;

        /*! sets member variables

        \param window optional parameter only for task using render context
        */
        iTask(iWindow* window = nullptr, uint32 priority = iTask::DEFAULT_PRIORITY, bool isRepeating = false, bool needsRenderContext = false);

        /*! does nothing
        */
        virtual ~iTask() = default;

        /*! aborts the task
        */
        virtual void abort();

        /*! \returns true: if task is still running; false: if it has stopped or not started yet
        */
        bool isRunning();

        /*! \returns true: if task needs a render context; false: no render context needed
        */
        bool needsRenderContext();

        /*! \returns priority of task
        */
        uint32 getPriority();

        /*! sets the task's priority

        \param priority new priority to set
        */
        void setPriority(uint32 priority);

        /*! \returns true: task is a repeating task; false: task will only be run once
        */
        bool isRepeating();

        uint64 getID();

	protected:

        /*! true: task is a repeating task; false: task will only be run once
        */
        bool _isRepeating = false;

        /*! true: task is currently running
        */
        bool _isRunning = true;

        /*! priority of task

        zero is highest priority
        */
        uint32 _priority = 0;

        /*! optional pointer to window. only used by tasks that need the render context
        */
        iWindow* _window = nullptr;

        /*! runs the actual task
        */
        virtual void run() = 0;

    private:

        /*! counter for next task ID
        */
        static uint64 _nextTaskID;

        /*! the task id
        */
        uint64 _taskID = 0;

        /*! if true: this task needs a render context
        */
        bool _needsRenderContext = false;

	};

};

#endif