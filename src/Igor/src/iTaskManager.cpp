// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
// see copyright notice in corresponding header file

#include <iTaskManager.h>

#include <iTask.h>
#include <iApplication.h>
#include <iThread.h>
#include <iRenderContextThread.h>
#include <iaConsole.h>
#include <iTask.h>
#include <iWindow.h>

#include <thread>

namespace Igor
{
    bool iTaskManager::_running = false;

    iTaskManager::iTaskManager()
    {
        iTaskManager::_running = true;

        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        int32 numThreads = sysinfo.dwNumberOfProcessors;

        for (int i = 0; i < numThreads; ++i)
        {
            createRegularThread();
        }

        con_info("created threads", numThreads << " regular threads");
    }

    iTaskManager::~iTaskManager()
    {
        iTaskManager::_running = false;

        flushIncommingTasks();

        // clear all tasks left in render context queue
        _mutexRenderContextTasks.lock();
        for (auto task : _renderContextTasksQueued)
        {
            delete task;
        }
        _renderContextTasksQueued.clear();
        _mutexRenderContextTasks.unlock();

        // clear all tasks left in regular queue
        _mutexRegularTasks.lock();
        for (auto task : _regularTasksQueued)
        {
            delete task;
        }
        _regularTasksQueued.clear();
        _mutexRegularTasks.unlock();

        // than finish all task that are already running
        con_debug_endl("waiting for " << _renderContextTasksRunning.size() << " render context tasks to finish");

        // first the render context tasks
        _mutexRenderContextTasks.lock();
        for (auto task : _renderContextTasksRunning)
        {
            task->abort();

            while (task->isRunning())
            {
                std::this_thread::yield();
            }

            delete task;
        }
        _renderContextTasksRunning.clear();
        _mutexRenderContextTasks.unlock();

        con_debug_endl("waiting for " << _regularTasksRunning.size() << " regular tasks to finish");

        // than the regular tasks
        _mutexRegularTasks.lock();
        for (auto task : _regularTasksRunning)
        {
            task->abort();

            while (task->isRunning())
            {
                std::this_thread::yield();
            }

            delete task;
        }
        _regularTasksRunning.clear();
        _mutexRegularTasks.unlock();

        // now stop and kill all threads
        con_debug_endl("waiting for " << _renderContextThreads.size() << " render context threads to join");

        // render context threads
        _mutexRenderContextThreads.lock();
        auto threadIter = _renderContextThreads.begin();
        while (_renderContextThreads.end() != threadIter)
        {
            (*threadIter).second._stopThread = true;
            (*threadIter).first->join();
            delete (*threadIter).first;
            threadIter++;
        }
        _renderContextThreads.clear();
        _mutexRenderContextThreads.unlock();

        con_debug_endl("waiting for " << _regularThreads.size() << " regular threads to join");

        // and regular threads
        _mutexRegularThreads.lock();
        for (auto thread : _regularThreads)
        {
            thread->join();
            delete thread;
        }
        _regularThreads.clear();
        _mutexRegularThreads.unlock();

        con_debug_endl("threading done");
    }

    void iTaskManager::abortTask(uint64 taskID)
    {
        iTask* task = getTask(taskID);
        if (task != nullptr)
        {
            task->abort();
        }
    }

    uint32 iTaskManager::getRegularThreadCount()
    {
        return static_cast<uint32>(_regularThreads.size());
    }

    uint32 iTaskManager::getRenderContextThreadCount()
    {
        return static_cast<uint32>(_renderContextThreads.size());
    }

    uint32 iTaskManager::getQueuedRegularTaskCount()
    {
        return static_cast<uint32>(_regularTasksQueued.size());
    }

    uint32 iTaskManager::getRunningRegularTaskCount()
    {
        return static_cast<uint32>(_regularTasksRunning.size());
    }

    uint32 iTaskManager::getQueuedRenderContextTaskCount()
    {
        return static_cast<uint32>(_renderContextTasksQueued.size());
    }

    uint32 iTaskManager::getRunningRenderContextTaskCount()
    {
        return static_cast<uint32>(_renderContextTasksRunning.size());
    }

    bool iTaskManager::isRunning()
    {
        return _running;
    }

    void iTaskManager::createRenderContextThreads(iWindow *window)
    {
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        int32 numThreads = 1; // sysinfo.dwNumberOfProcessors;

        for (int i = 0; i < numThreads; ++i)
        {
            createRenderContextThread(window);
        }

        con_info("created threads", numThreads << " render context threads");
    }

    bool iTaskManager::createRenderContextThread(iWindow *window)
    {
        iRenderContextThread* workerThread = new iRenderContextThread(window);
        if (workerThread->isValid())
        {
            ThreadContext context;
            context._window = window;

            _mutexRenderContextThreads.lock();
            _renderContextThreads[workerThread] = context;
            _mutexRenderContextThreads.unlock();

            workerThread->run(ThreadDelegate(this, &iTaskManager::workWithRenderContextTasks));
            return true;
        }
        else
        {
            delete workerThread;
            return false;
        }
    }

    void iTaskManager::createRegularThread()
    {
        iThread* workerThread = new iThread();
        workerThread->run(ThreadDelegate(this, &iTaskManager::workWithRegularTasks));

        _mutexRegularThreads.lock();
        _regularThreads.push_back(workerThread);
        _mutexRegularThreads.unlock();
    }

    void iTaskManager::killRenderContextThreads(iWindow *window)
    {
        _mutexRenderContextTasks.lock();
        for (auto task : _renderContextTasksQueued)
        {
            delete task;
        }
        _renderContextTasksQueued.clear();

        vector<uint64> removeFromAllTasks;

        // stop running render context tasks
        auto taskIter = _renderContextTasksRunning.begin();
        while (taskIter != _renderContextTasksRunning.end())
        {
            if ((*taskIter)->_window == window &&
                (*taskIter)->isRunning())
            {
                (*taskIter)->abort();

                while ((*taskIter)->isRunning())
                {
                    std::this_thread::yield();
                }

                removeFromAllTasks.push_back((*taskIter)->getID());
                taskIter = _renderContextTasksRunning.erase(taskIter);
            }
            else
            {
                taskIter++;
            }
        }
        _mutexRenderContextTasks.unlock();
        
        // remove stopped tasks from all task list
        for (auto id : removeFromAllTasks)
        {
            _mutexAllTasks.lock();
            auto iter = _allTasks.find(id);
            if (iter != _allTasks.end())
            {
                _allTasks.erase(iter);
            }
            else
            {
                con_err("inconsistent data");
            }
            _mutexAllTasks.unlock();

            delete (*iter).second;
            _tasksDone++;
            _taskFinished(id);
        }

        // remove render context threads
        _mutexRenderContextThreads.lock();
        auto threadIter = _renderContextThreads.begin();
        while (_renderContextThreads.end() != threadIter)
        {
            if ((*threadIter).second._window == window)
            {
                (*threadIter).second._stopThread = true;
                (*threadIter).first->join();
                delete (*threadIter).first;
                threadIter = _renderContextThreads.erase(threadIter);
            }
            else
            {
                ++threadIter;
            }
        }
        _mutexRenderContextThreads.unlock();
    }

    void iTaskManager::workWithRenderContextTasks(iThread* thread)
    {
        iTask* taskTodo = nullptr;
        ThreadContext* context = nullptr;

        // find the context we use in this thread
        _mutexRenderContextThreads.lock();
        auto threadIter = _renderContextThreads.begin();
        while (_renderContextThreads.end() != threadIter)
        {
            if ((*threadIter).first == thread)
            {
                context = &((*threadIter).second);
                break;
            }
            ++threadIter;
        }
        _mutexRenderContextThreads.unlock();

        while (iTaskManager::isRunning() && !context->_stopThread)
        {
            taskTodo = nullptr;

            flushIncommingTasks();

            _mutexRenderContextTasks.lock();
            if (_renderContextTasksQueued.size())
            {
                taskTodo = (*_renderContextTasksQueued.begin());
                _renderContextTasksQueued.pop_front();
                _renderContextTasksRunning.push_back(taskTodo);
            }
            _mutexRenderContextTasks.unlock();

            if (taskTodo)
            {
                taskTodo->run();
                taskTodo->finishTask();

                uint64 idToDelete = iTask::INVALID_TASK_ID;

                _mutexRenderContextTasks.lock();
                list<iTask*>::iterator findIter = find(_renderContextTasksRunning.begin(), _renderContextTasksRunning.end(), taskTodo);
                if (findIter != _renderContextTasksRunning.end())
                {
                    _renderContextTasksRunning.erase(findIter);

                    if (taskTodo->isRepeating())
                    {
                        _renderContextTasksQueued.push_back(taskTodo);
                    }
                    else
                    {
                        idToDelete = taskTodo->getID();
                    }
                }
                _mutexRenderContextTasks.unlock();

                if (idToDelete != iTask::INVALID_TASK_ID)
                {
                    _mutexAllTasks.lock();
                    auto iter = _allTasks.find(idToDelete);
                    if (iter != _allTasks.end())
                    {
                        _allTasks.erase(iter);
                    }
                    else
                    {
                        con_err("inconsistent data");
                    }
                    _mutexAllTasks.unlock();

                    delete taskTodo;
                    _tasksDone++;
                    _taskFinished(idToDelete);
                }

                taskTodo = nullptr;
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }

    uint64 iTaskManager::getTaksDoneCount() const
    {
        return _tasksDone;
    }
    
    void iTaskManager::workWithRegularTasks(iThread* thread)
    {
        iTask* taskTodo = nullptr;

        while (iTaskManager::isRunning())
        {
            taskTodo = nullptr;

            flushIncommingTasks();

            _mutexRegularTasks.lock();
            if (_regularTasksQueued.size())
            {
                taskTodo = (*_regularTasksQueued.begin());
                _regularTasksQueued.pop_front();
                _regularTasksRunning.push_back(taskTodo);
            }
            _mutexRegularTasks.unlock();

            if (taskTodo != nullptr)
            {
                taskTodo->setWorld(thread->getWorld());
                taskTodo->run();
                taskTodo->finishTask();

                uint64 idToDelete = iTask::INVALID_TASK_ID;

                _mutexRegularTasks.lock();
                list<iTask*>::iterator findIter = find(_regularTasksRunning.begin(), _regularTasksRunning.end(), taskTodo);
                if (findIter != _regularTasksRunning.end())
                {
                    _regularTasksRunning.erase(findIter);

                    if (taskTodo->isRepeating())
                    {
                        _regularTasksQueued.push_back(taskTodo);
                    }
                    else
                    {
                        idToDelete = taskTodo->getID();
                    }
                }
                else
                {
                    con_err("inconsistent data");
                }
                _mutexRegularTasks.unlock();

                if (idToDelete != iTask::INVALID_TASK_ID)
                {
                    _mutexAllTasks.lock();
                    auto iter = _allTasks.find(idToDelete);
                    if (iter != _allTasks.end())
                    {
                        _allTasks.erase(iter);
                    }
                    else
                    {
                        con_err("inconsistent data");
                    }
                    _mutexAllTasks.unlock();

                    delete taskTodo;
                    _tasksDone++;
                    _taskFinished(idToDelete);
                }

                taskTodo = nullptr;
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }

    void iTaskManager::flushIncommingTasks()
    {
        _mutexIncommingTasks.lock();
        list<iTask*> incomming = _tasksIncomming;
        _tasksIncomming.clear();
        _mutexIncommingTasks.unlock();

        for (auto incommingTask : incomming)
        {
            switch (incommingTask->getContext())
            {
            case iTaskContext::Default:
                _mutexRegularTasks.lock();
                _regularTasksQueued.push_back(incommingTask);
                _mutexRegularTasks.unlock();
                break;

            case iTaskContext::RenderContext:
                _mutexRenderContextTasks.lock();
                _renderContextTasksQueued.push_back(incommingTask);
                _mutexRenderContextTasks.unlock();
                break;
            }
        }

        _mutexRegularTasks.lock();
        _regularTasksQueued.sort([](const iTask* a, const iTask* b) { return a->_priority < b->_priority; });
        _mutexRegularTasks.unlock();

        _mutexRenderContextTasks.lock();
        _renderContextTasksQueued.sort([](const iTask* a, const iTask* b) { return a->_priority < b->_priority; });
        _mutexRenderContextTasks.unlock();
    }

    iTask* iTaskManager::getTask(uint64 taskID)
    {
        iTask* result = nullptr;

        _mutexAllTasks.lock();
        auto iterTasks = _allTasks.find(taskID);
        if (iterTasks != _allTasks.end())
        {
            result = (*iterTasks).second;
        }
        _mutexAllTasks.unlock();

        return result;
    }

    uint64 iTaskManager::addTask(iTask* task)
    {
        uint64 result = iTask::INVALID_TASK_ID;
        con_assert(task != nullptr, "zero pointer");

        if (task != nullptr)
        {
            result = task->getID();
            bool alreadyInserted = false;

            _mutexAllTasks.lock();
            auto iter = _allTasks.find(task->getID());
            if (iter == _allTasks.end())
            {
                _allTasks[task->getID()] = task;
            }
            else
            {
                alreadyInserted = true;
            }
            _mutexAllTasks.unlock();

            if (alreadyInserted)
            {
                con_warn("task already managed by task manager (id:" << task->getID() << ")");
            }
            else
            {
                _mutexIncommingTasks.lock();
                _tasksIncomming.push_back(task);
                _mutexIncommingTasks.unlock();
            }
        }
        else
        {
            con_err("can't add invalid task");
        }
        
        return result;
    }

    void iTaskManager::registerTaskFinishedDelegate(iTaskFinishedDelegate taskFinishedDelegate)
    {
        _taskFinished.append(taskFinishedDelegate);
    }

    void iTaskManager::unregisterTaskFinishedDelegate(iTaskFinishedDelegate taskFinishedDelegate)
    {
        _taskFinished.remove(taskFinishedDelegate);
    }

};
