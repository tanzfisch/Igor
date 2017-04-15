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
// (c) Copyright 2012-2017 by Martin Loga
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

#ifndef __iTASKMANAGER__
#define __iTASKMANAGER__

#include <iDefines.h>

#include <iaSingleton.h>
#include <iaEvent.h>
using namespace IgorAux;

#include <list>
#include <map>
#include <vector>
using namespace std;

namespace Igor
{

	class iTask;
	class iRenderContextThread;
	class iThread;
	class iWindow;

    /*! event triggered when task was finished
    */
    iaEVENT(iTaskFinishedEvent, iTaskFinishedDelegate, void, (uint64 taskID), (taskID));

    /*! manages tasks to be done in parallel

    \todo there should be IDs for tasks so we can sefely kill tasks later by ID
    */
	class Igor_API iTaskManager : public iaSingleton<iTaskManager>
	{

		friend class iaSingleton<iTaskManager>;
		friend class iWindow;

        /*! some extra data we give the render context threads with

        \todo can this be moved to iRenderContextThread?
        */
        struct ThreadContext
        {
        public: 

            /*! window connected to render context
            */
            iWindow* _window = nullptr;

            /*! flag to control the render context thread
            */
            bool _stopThread = false;
        };

    public:

        /*! adds a task to be processed

        !!! ATTENTION task get's consumed and later deleted by task manager

        \param task the task to be added
        \returns the task's id
        */
        uint64 addTask(iTask* task);

        /*! \returns task by id

        \param taskID the task ID to search for
        */
        iTask* getTask(uint64 taskID);

        /*! aborts a task by id

        \param taskID the task id to abort
        */
        void abortTask(uint64 taskID);

        /*! \returns regular thread count
        */
        uint32 getRegularThreadCount();

        /*! \returns render context thread count
        */
        uint32 getRenderContextThreadCount();

        /*! \returns tasks in queue count
        */
        uint32 getQueuedRegularTaskCount();

        /*! \returns running tasks count
        */
        uint32 getRunningRegularTaskCount();

        /*! \returns render context tasks in queue count
        */
        uint32 getQueuedRenderContextTaskCount();

        /*! \returns running render context tasks count
        */
        uint32 getRunningRenderContextTaskCount();

        /*! \returns true if the task manager is running
        */
        static bool isRunning();

        /*! \returns how many tasks where done since program start 
        */
        uint64 getTaksDoneCount() const;

        /*! registers delegate to task finished event

        \param taskFinishedDelegate the delegate to register
        */
        void registerTaskFinishedDelegate(iTaskFinishedDelegate taskFinishedDelegate);

        /*! unregisters delegate from task finished event

        \param taskFinishedDelegate the delegate to unregister
        */
        void unregisterTaskFinishedDelegate(iTaskFinishedDelegate taskFinishedDelegate);

	private:

        /*! counts how many tasks where done
        */
        uint64 _tasksDone = 0;

        /*! task finished event
        */
        iTaskFinishedEvent _taskFinished;

        /*! if true the task manager is running
        */
		static bool _running;

        /*! incomming tasks
        */
        list<iTask*> _tasksIncomming;

        /*! mutex for incomming tasks
        */
        mutex _mutexIncommingTasks;

        /*! list of all tasks
        */
        map<uint64, iTask*> _allTasks;

        /*! mutex for all tasks list
        */
        mutex _mutexAllTasks;

        /*! list of regular threads
        */
        vector<iThread*> _regularThreads;

        /*! mutex for regular threads
        */
        mutex _mutexRegularThreads;

        /*! list of render context threads

        \todo check if we realy need the extra data. because the thread it self has that data too
        */
        map<iRenderContextThread*, ThreadContext> _renderContextThreads;

        /*! mutex for render context threads
        */
        mutex _mutexRenderContextThreads;

        /*! list of queued tasks
        */
        list<iTask*> _regularTasksQueued;

        /*! list of running tasks
        */
        list<iTask*> _regularTasksRunning;

        /*! mutex for regular tasks
        */
        mutex _mutexRegularTasks;

        /*! list of queued tasks that need render context
        */
		list<iTask*> _renderContextTasksQueued;

        /*! list of running tasks that need render context
        */
        list<iTask*> _renderContextTasksRunning;

        /*! mutex for render context tasks
        */
        mutex _mutexRenderContextTasks;

        /*! the method a regular thread is launched with

        \param thread the thread this method is launched with
        */
        void workWithRegularTasks(iThread* thread);

        /*! the method a thread with render context is launched with

        \param thread the thread this method is launched with
        */
        void workWithRenderContextTasks(iThread* thread);

        /*! creates a number of render context threads for a specified window

        \param window the window thes render contexts are connected with
        */
        void createRenderContextThreads(iWindow *window);

        /*! creates a render context thread

        \param window the window this render context is connected to
        \returns true if success
        */
        bool createRenderContextThread(iWindow *window);

        /*! kills all render context tasks and threads that are associated with a specified window

        \param window the window these tasks and threads are associated with 
        */
        void killRenderContextThreads(iWindow *window);

        /*! creates a regular thread
        */
		void createRegularThread();

		/*! flush incomming tasks queue to task list
		*/
        void flushIncommingTasks();

        /*! creates some regular threads and starts them
        */
		iTaskManager();

        /*! stops all threads and cleans up
        */
		virtual ~iTaskManager();

	};

};

#endif
