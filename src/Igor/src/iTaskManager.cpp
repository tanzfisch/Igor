// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iTaskManager.h>

#include <iTask.h>
#include <iApplication.h>
#include <iThread.h>
#include <iRenderContextThread.h>
#include <iaConsole.h>
#include <iTask.h>
#include <iWindow.h>

namespace Igor
{
    bool iTaskManager::_running = false;

    iTaskManager::iTaskManager()
    {
        iTaskManager::_running = true;

        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        int32 numThreads = sysinfo.dwNumberOfProcessors - 1;

        if (numThreads < 4)
        {
            numThreads = 4;
        }

        for (int i = 0; i < numThreads; ++i)
        {
            createThread();
        }
    }

    iTaskManager::~iTaskManager()
    {
        iTaskManager::_running = false;

        _mutexTasks.lock();
        // first clear all tasks left in queue
        for (auto task : _renderContextTasksQueued)
        {
            delete task;
        }
        _renderContextTasksQueued.clear();

        for (auto task : _tasksQueued)
        {
            delete task;
        }
        _tasksQueued.clear();

        con_debug_endl("waiting for " << _renderContextTasksRunning.size() << " render context tasks to finish");

        // than finish all task that are already running
        for (auto task : _renderContextTasksRunning)
        {
            task->abort();

            while (task->isRunning())
            {
                _sleep(1);
            }

            delete task;
        }
        _renderContextTasksRunning.clear();

        con_debug_endl("waiting for " << _tasksRunning.size() << " regular tasks to finish");

        for (auto task : _tasksRunning)
        {
            task->abort();

            while (task->isRunning())
            {
                _sleep(1);
            }

            delete task;
        }
        _tasksRunning.clear();

        _mutexTasks.unlock();

        con_debug_endl("waiting for " << _renderContextThreads.size() << " render context threads to join");

        // now stop and kill all threads
        _mutexRenderThreads.lock();
        auto threadIter = _renderContextThreads.begin();
        while (_renderContextThreads.end() != threadIter)
        {
            (*threadIter).second._stopThread = true;
            (*threadIter).first->join();
            delete (*threadIter).first;
            threadIter++;
        }
        _renderContextThreads.clear();
        _mutexRenderThreads.unlock();

        con_debug_endl("waiting for " << _threads.size() << " regular threads to join");

        _mutexThreads.lock();
        for (auto thread : _threads)
        {
            thread->join();
            delete thread;
        }
        _threads.clear();
        _mutexThreads.unlock();

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

    uint32 iTaskManager::getThreadCount()
    {
        return _threads.size();
    }

    uint32 iTaskManager::getRenderContextThreadCount()
    {
        return _renderContextThreads.size();
    }

    uint32 iTaskManager::getQueuedTaskCount()
    {
        return _tasksQueued.size();
    }

    uint32 iTaskManager::getRunningTaskCount()
    {
        return _tasksRunning.size();
    }

    uint32 iTaskManager::getQueuedRenderContextTaskCount()
    {
        return _renderContextTasksQueued.size();
    }

    uint32 iTaskManager::getRunningRenderContextTaskCount()
    {
        return _renderContextTasksRunning.size();
    }

    bool iTaskManager::isRunning()
    {
        return _running;
    }

    iTask* iTaskManager::getTask(uint64 taskID)
    {
        iTask* result = nullptr;

		_mutexIncommingTasks.lock();
		auto iterIncomming = _tasksIncomming.begin();
		while (iterIncomming != _tasksIncomming.end())
		{
			if ((*iterIncomming)->getID() == taskID)
			{
				result = (*iterIncomming);
				break;
			}
			iterIncomming++;
		}
		_mutexIncommingTasks.unlock();
        
		auto iterTasks = _tasks.find(taskID);
        if (iterTasks != _tasks.end())
        {
            result = (*iterTasks).second;
        }

        return result;
    }

    void iTaskManager::createRenderContextThreads(iWindow *window)
    {
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        int32 numThreads = sysinfo.dwNumberOfProcessors + 1;

        if (numThreads < 4)
        {
            numThreads = 4;
        }

        for (int i = 0; i < numThreads; ++i)
        {
            createRenderContextThread(window);
            _sleep(10);
        }
    }

    bool iTaskManager::createRenderContextThread(iWindow *window)
    {
        iRenderContextThread* workerThread = new iRenderContextThread(window);
        if (workerThread->isValid())
        {
            ThreadContext context;
            context._window = window;

            _mutexRenderThreads.lock();
            _renderContextThreads[workerThread] = context;
            _mutexRenderThreads.unlock();

            workerThread->run(ThreadDelegate(this, &iTaskManager::workWithRenderContext));
            return true;
        }
        else
        {
            delete workerThread;
            return false;
        }
    }

    void iTaskManager::createThread()
    {
        iThread* workerThread = new iThread();
        workerThread->run(ThreadDelegate(this, &iTaskManager::work));
        _threads.push_back(workerThread);
    }

    void iTaskManager::killRenderContextThreads(iWindow *window)
    {
        _mutexTasks.lock();

        for (auto task : _renderContextTasksQueued)
        {
            delete task;
        }
        _renderContextTasksQueued.clear();

        auto taskIter = _renderContextTasksRunning.begin();
        while (taskIter != _renderContextTasksRunning.end())
        {
            if ((*taskIter)->_window == window &&
                (*taskIter)->isRunning())
            {
                (*taskIter)->abort();

                while ((*taskIter)->isRunning())
                {
                    _sleep(1);
                }

                uint64 id = (*taskIter)->getID();

                auto iter = _tasks.find(id);
                if (iter != _tasks.end())
                {
                    _tasks.erase(iter);
                }
                else
                {
                    con_err("inconsistent data");
                }

                delete (*taskIter);
                _taskFinished(id);

                taskIter = _renderContextTasksRunning.erase(taskIter);
            }
            else
            {
                taskIter++;
            }
        }

        _mutexTasks.unlock();

        _mutexRenderThreads.lock();
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
        _mutexRenderThreads.unlock();
    }

    void iTaskManager::workWithRenderContext(iThread* thread)
    {
        iTask* taskTodo = nullptr;
        ThreadContext* context = nullptr;

        _mutexRenderThreads.lock();
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
        _mutexRenderThreads.unlock();

        while (iTaskManager::isRunning() && !context->_stopThread)
        {
            taskTodo = nullptr;

            flushIncomming();

            _mutexTasks.lock();
            if (_renderContextTasksQueued.size())
            {
                taskTodo = (*_renderContextTasksQueued.begin());
                _renderContextTasksQueued.pop_front();
                _renderContextTasksRunning.push_back(taskTodo);
            }
            _mutexTasks.unlock();

            if (taskTodo)
            {
                taskTodo->run();

                _mutexTasks.lock();
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
                        uint64 id = taskTodo->getID();

                        auto iter = _tasks.find(taskTodo->getID());
                        if (iter != _tasks.end())
                        {
                            _tasks.erase(iter);
                        }
                        else
                        {
                            con_err("inconsistent data");
                        }

                        delete taskTodo;

                        _taskFinished(id);
                    }

                    taskTodo = nullptr;
                }
                _mutexTasks.unlock();

            }
            else
            {
                _sleep(0);
            }
        }
    }
    
    void iTaskManager::work(iThread* thread)
    {
        iTask* taskTodo = nullptr;

        while (iTaskManager::isRunning())
        {
            taskTodo = nullptr;

            flushIncomming();

            _mutexTasks.lock();
            if (_tasksQueued.size())
            {
                taskTodo = (*_tasksQueued.begin());
                _tasksQueued.pop_front();
                _tasksRunning.push_back(taskTodo);
            }
            _mutexTasks.unlock();

            if (taskTodo)
            {
                taskTodo->setWorld(thread->getWorld());
                taskTodo->run();

                _mutexTasks.lock();
                list<iTask*>::iterator findIter = find(_tasksRunning.begin(), _tasksRunning.end(), taskTodo);
                if (findIter != _tasksRunning.end())
                {
                    _tasksRunning.erase(findIter);

                    if (taskTodo->isRepeating())
                    {
                        _tasksQueued.push_back(taskTodo);
                    }
                    else
                    {
                        uint64 id = taskTodo->getID();

                        auto iter = _tasks.find(taskTodo->getID());
                        if (iter != _tasks.end())
                        {
                            _tasks.erase(iter);
                        }
                        else
                        {
                            con_err("inconsistent data");
                        }

                        delete taskTodo;

                        _taskFinished(id);
                    }

                    taskTodo = nullptr;
                }
                else
                {
                    con_err("inconsistent data");
                }

                _mutexTasks.unlock();
            }
            else
            {
                _sleep(0);
            }
        }
    }

    void iTaskManager::flushIncomming()
    {
        _mutexIncommingTasks.lock();
        list<iTask*> incomming = _tasksIncomming;
        _tasksIncomming.clear();
        _mutexIncommingTasks.unlock();

        _mutexTasks.lock();
        for (auto incommingTask : incomming)
        {
            uint64 taskID = incommingTask->getID();
            auto iter = _tasks.find(taskID);
            if (iter == _tasks.end())
            {
                _tasks[taskID] = incommingTask;

                switch (incommingTask->getContext())
                {
                case iTaskContext::Default:
                    _tasksQueued.push_back(incommingTask);
                    break;

                case iTaskContext::RenderContext:
                    _renderContextTasksQueued.push_back(incommingTask);
                    break;
                }
            }
            else
            {
                con_warn("task already managed by task manager (id:" << taskID << ")");
            }
        }

        _tasksQueued.sort([](const iTask* a, const iTask* b) { return a->_priority < b->_priority; });
        _renderContextTasksQueued.sort([](const iTask* a, const iTask* b) { return a->_priority < b->_priority; });
        _mutexTasks.unlock();
    }

    uint64 iTaskManager::addTask(iTask* task)
    {
        uint64 taskID = 0;
        con_assert(task != nullptr, "zero pointer");

        if (task != nullptr)
        {
            taskID = task->getID();
            _mutexIncommingTasks.lock();
            _tasksIncomming.push_back(task);
            _mutexIncommingTasks.unlock();
        }
        
        return taskID;
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
