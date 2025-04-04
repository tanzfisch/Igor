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
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef __IGOR_TASK__
#define __IGOR_TASK__

#include <igor/iDefines.h>
#include <igor/physics/iPhysicsWorld.h>

#include <iaux/system/iaMutex.h>
using namespace iaux;

namespace igor
{
    class iWindow;
    typedef iWindow* iWindowPtr;

    /*! task context
    */
    enum class iTaskContext
    {
        Default,
        RenderContext
    };

    /*! task id definition
    */
    typedef uint64 iTaskID;

    /*! need a task to be done in parallel? than derive from this base class
    */
    class IGOR_API iTask
    {

        friend class iTaskManager;

    public:
        /*! invalid task id definition
        */
        static const iTaskID INVALID_TASK_ID = 0;

        /*! max priority for a task to run
        */
        static const uint32 TASK_PRIORITY_MAX = 0;

        /*! high priority for a task to run
        */
        static const uint32 TASK_PRIORITY_HIGH = 5;

        /*! default priority for a task to run
        */
        static const uint32 TASK_PRIORITY_DEFAULT = 10;

        /*! low priority for a task to run
        */
        static const uint32 TASK_PRIORITY_LOW = 20;

        /*! sets member variables

        \param window optional parameter only for task using render context
        \param priority the priority this task will run
        \param isRepeating if true the task will be triggered repeadetly
        \param needsRenderContext if true the task needs the render context e.g. to create a texture using the renderer
        */
        iTask(iWindowPtr window = nullptr, uint32 priority = iTask::TASK_PRIORITY_DEFAULT, bool isRepeating = false, iTaskContext taskContext = iTaskContext::Default);

        /*! does nothing
        */
        virtual ~iTask() = default;

        /*! aborts the task
        */
        virtual void abort();

        /*! \returns true: if task is still running; false: if it has stopped or not started yet
        */
        bool isRunning();

        /*! \returns context needed for this task
        */
        iTaskContext getContext();

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

        /*! \returns ID of task
        */
        iTaskID getID();

        /*! \returns physics world id if present
        \todo broken design?
        */
        uint64 getWorldID();

        /*! sets physics world id

        \param worldID the world id
        \todo broken design?
        */
        void setWorldID(uint64 worldID);

        /*! \returns pointer to window if present

        \todo broken design?
        */
        iWindowPtr getWindow();

    protected:
        /*! sets if the task is repeating

        \param repeat �f true the task will be repeated
        */
        void setRepeating(bool repeat);

        /*! runs the actual task
        */
        virtual void run() = 0;

    private:
        /*! tells the task manager that the task is finished
        */
        void finishTask();

        /*! priority of task

        zero is highest priority
        */
        uint32 _priority = TASK_PRIORITY_DEFAULT;

        /*! true: task is a repeating task; false: task will only be run once
        */
        bool _isRepeating = false;

        /*! optional pointer to window. only used by tasks that need the render context
        */
        iWindowPtr _window = nullptr;

        /*! optional physics world id used by tasks within physics context
        */
        uint64 _worldID = iPhysicsWorld::INVALID_WORLD_ID;

        /*! true: task is currently running
        */
        bool _isRunning = true;

        /*! secure the task id generation
        */
        static iaMutex _mutexID;

        /*! counter for next task ID
        */
        static iTaskID _nextTaskID;

        /*! the task id
        */
        iTaskID _taskID = INVALID_TASK_ID;

        /*! task context
        */
        iTaskContext _taskContext = iTaskContext::Default;
    };

}; // namespace igor

#endif // __IGOR_TASK__
