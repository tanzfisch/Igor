// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iTaskManager.h>

#include <iTask.h>
#include <iApplication.h>
#include <iThread.h>
#include <iRenderContextThread.h>
#include <iPhysicsContextThread.h>
#include <iaConsole.h>
#include <iWindow.h>

namespace Igor
{
    bool iTaskManager::_running = false;

    iTaskManager::iTaskManager()
    {
        iTaskManager::_running = true;

        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        int32 numThreads = sysinfo.dwNumberOfProcessors + 1;

        if (numThreads < 4)
        {
            numThreads = 4;
        }

        for (int i = 0; i < numThreads; ++i)
        {
            createThread();
        }

        createPhysicsContextThread();
    }

    iTaskManager::~iTaskManager()
    {
        iTaskManager::_running = false;

        _mutex.lock();
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

        _mutex.unlock();

        con_debug_endl("waiting for " << _renderContextThreads.size() << " render context threads to join");

        // now stop and kill all threads
        _renderContextThreadsMutex.lock();
        auto threadIter = _renderContextThreads.begin();
        while (_renderContextThreads.end() != threadIter)
        {
            (*threadIter).second._stopThread = true;
            (*threadIter).first->join();
            delete (*threadIter).first;
            threadIter++;
        }
        _renderContextThreads.clear();
        _renderContextThreadsMutex.unlock();

        con_debug_endl("waiting for " << _threads.size() << " regular threads to join");

        for (auto thread : _threads)
        {
            thread->join();
            delete thread;
        }
        _threads.clear();

        con_debug_endl("threading done");
    }

    uint32 iTaskManager::getThreadCount()
    {
        return _threads.size();
    }

    uint32 iTaskManager::getRenderContextThreadCount()
    {
        return _renderContextThreads.size();
    }

    uint32 iTaskManager::getPhysicsContextThreadCount()
    {
        return _physicsContextThreads.size();
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

    uint32 iTaskManager::getQueuedPhysicsContextTaskCount()
    {
        return _physicsContextTasksQueued.size();
    }

    uint32 iTaskManager::getRunningPhysicsContextTaskCount()
    {
        return _physicsContextTasksRunning.size();
    }

    bool iTaskManager::isRunning()
    {
        return _running;
    }

    iTask* iTaskManager::getTask(uint64 taskID)
    {
        iTask* result = nullptr;

        auto iter = _tasks.find(taskID);
        if (iter != _tasks.end())
        {
            result = (*iter).second;
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

    void iTaskManager::createPhysicsContextThread()
    {
        iPhysicsContextThread* workerThread = new iPhysicsContextThread();

        _physicsContextThreads.push_back(workerThread);

        workerThread->run(ThreadDelegate(this, &iTaskManager::workWithPhysicsContext));
    }

    bool iTaskManager::createRenderContextThread(iWindow *window)
    {
        iRenderContextThread* workerThread = new iRenderContextThread(window);
        if (workerThread->isValid())
        {
            ThreadContext context;
            context._window = window;

            _renderContextThreadsMutex.lock();
            _renderContextThreads[workerThread] = context;
            _renderContextThreadsMutex.unlock();

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
        _mutex.lock();

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

        _mutex.unlock();

        _renderContextThreadsMutex.lock();
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
        _renderContextThreadsMutex.unlock();
    }

    void iTaskManager::workWithRenderContext(iThread* thread)
    {
        iTask* taskTodo = nullptr;
        ThreadContext* context = nullptr;

        _renderContextThreadsMutex.lock();
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
        _renderContextThreadsMutex.unlock();

        while (iTaskManager::isRunning() && !context->_stopThread)
        {
            taskTodo = nullptr;

            _mutex.lock();
            if (_renderContextTasksQueued.size())
            {
                taskTodo = (*_renderContextTasksQueued.begin());
                _renderContextTasksQueued.pop_front();
                _renderContextTasksRunning.push_back(taskTodo);
            }
            _mutex.unlock();

            if (taskTodo)
            {
                taskTodo->run();

                _mutex.lock();
                list<iTask*>::iterator findIter = find(_renderContextTasksRunning.begin(), _renderContextTasksRunning.end(), taskTodo);
                if (findIter != _renderContextTasksRunning.end())
                {
                    _renderContextTasksRunning.erase(findIter);

                    if (taskTodo->isRepeating())
                    {
                        addTaskToQueue(taskTodo);
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
                _mutex.unlock();

            }
            else
            {
                _sleep(0);
            }
        }
    }

    void iTaskManager::workWithPhysicsContext(iThread* thread)
    {
        iTask* taskTodo = nullptr;

        while (iTaskManager::isRunning())
        {
            taskTodo = nullptr;

            _mutex.lock();
            if (_physicsContextTasksQueued.size())
            {
                taskTodo = (*_physicsContextTasksQueued.begin());
                _physicsContextTasksQueued.pop_front();
                _physicsContextTasksRunning.push_back(taskTodo);
            }
            _mutex.unlock();

            if (taskTodo)
            {
                taskTodo->run();

                _mutex.lock();
                list<iTask*>::iterator findIter = find(_physicsContextTasksRunning.begin(), _physicsContextTasksRunning.end(), taskTodo);
                if (findIter != _physicsContextTasksRunning.end())
                {
                    _physicsContextTasksRunning.erase(findIter);

                    if (taskTodo->isRepeating())
                    {
                        addTaskToQueue(taskTodo);
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

                _mutex.unlock();
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

            _mutex.lock();
            if (_tasksQueued.size())
            {
                taskTodo = (*_tasksQueued.begin());
                _tasksQueued.pop_front();
                _tasksRunning.push_back(taskTodo);
            }
            _mutex.unlock();

            if (taskTodo)
            {
                taskTodo->run();

                _mutex.lock();
                list<iTask*>::iterator findIter = find(_tasksRunning.begin(), _tasksRunning.end(), taskTodo);
                if (findIter != _tasksRunning.end())
                {
                    _tasksRunning.erase(findIter);

                    if (taskTodo->isRepeating())
                    {
                        addTaskToQueue(taskTodo);
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

                _mutex.unlock();
            }
            else
            {
                _sleep(0);
            }
        }
    }

    void iTaskManager::addTaskToQueue(iTask* task)
    {
        switch (task->getContext())
        {
        case iTaskContext::Default:
            _tasksQueued.push_back(task);
            _tasksQueued.sort([](const iTask* a, const iTask* b) { return a->_priority < b->_priority; });
            break;

        case iTaskContext::RenderContext:
            _renderContextTasksQueued.push_back(task);
            _renderContextTasksQueued.sort([](const iTask* a, const iTask* b) { return a->_priority < b->_priority; });
            break;

        case iTaskContext::PhysicsContext:
            _physicsContextTasksQueued.push_back(task);
            _physicsContextTasksQueued.sort([](const iTask* a, const iTask* b) { return a->_priority < b->_priority; });
            break;
        }
    }

    uint64 iTaskManager::addTask(iTask* task)
    {
        uint64 taskID = 0;
        con_assert(task != nullptr, "zero pointer");

        if (task != nullptr)
        {
            taskID = task->getID();

            _mutex.lock();
            auto iter = _tasks.find(taskID);
            if (iter == _tasks.end())
            {
                _tasks[taskID] = task;
                addTaskToQueue(task);
            }
            else
            {
                con_warn("task already managed by task manager (id:" << taskID << ")");
            }
            _mutex.unlock();
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
