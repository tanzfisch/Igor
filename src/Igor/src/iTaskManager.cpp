// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iTaskManager.h>

#include <iTask.h>
#include <iApplication.h>
#include <iThread.h>
#include <iaConsole.h>
#include <iTask.h>
#include <iWindow.h>

namespace Igor
{
    bool iTaskManager::_running = false;

    iTaskManager::iTaskManager()
    {
    }

    iTaskManager::~iTaskManager()
    {
        iTaskManager::_running = false;

        _mutex.lock();
        {
            for (auto task : _tasksQueued)
            {
                delete task;
            }
            _tasksQueued.clear();

            con_debug_endl("waiting for " << _tasksRunning.size() << " tasks to finish");

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
        }
        _mutex.unlock();

        con_debug_endl("waiting for " << _threads.size() << " threads to join");

        for (auto thread : _threads)
        {
            thread.first->join();
            delete thread.first;
        }
        _threads.clear();

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

    uint32 iTaskManager::getQueuedTaskCount()
    {
        return _tasksQueued.size();
    }

    uint32 iTaskManager::getRunningTaskCount()
    {
        return _tasksRunning.size();
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

    void iTaskManager::createThreads(iWindow *window)
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
            createThread(window);
            _sleep(10);
        }
    }

    void iTaskManager::createThread(iWindow *window)
    {
        iThread* workerThread = new iThread(window);
        ThreadContext context;
        context._window = window;

        _mutex.lock();
        _threads[workerThread] = context;
        _mutex.unlock();

        workerThread->run(ThreadDelegate(this, &iTaskManager::work));
    }

    void iTaskManager::killThreads(iWindow *window)
    {
        _mutex.lock();

        for (auto task : _tasksQueued) // todo bug can't kill all of them here
        {
            delete task;
        }
        _tasksQueued.clear();

        auto taskIter = _tasksRunning.begin();
        while (taskIter != _tasksRunning.end())
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

                taskIter = _tasksRunning.erase(taskIter);
            }
            else
            {
                taskIter++;
            }
        }
        
        auto threadIter = _threads.begin();
        while (_threads.end() != threadIter)
        {
            if ((*threadIter).second._window == window)
            {
                (*threadIter).second._stopThread = true;
                (*threadIter).first->join();
                delete (*threadIter).first;
                threadIter = _threads.erase(threadIter);
            }
            else
            {
                ++threadIter;
            }
        }
        _mutex.unlock();
    }

    void iTaskManager::work(iThread* thread)
    {
        iTask* taskTodo = nullptr;
        ThreadContext* context = nullptr;

        _mutex.lock();
        auto threadIter = _threads.begin();
        while (_threads.end() != threadIter)
        {
            if ((*threadIter).first == thread)
            {
                context = &((*threadIter).second);
                break;
            }
            ++threadIter;
        }
        _mutex.unlock();

        while (iTaskManager::isRunning() && !context->_stopThread)
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
                taskTodo->setWorld(thread->getWorld());
                taskTodo->run();

                _mutex.lock();
                list<iTask*>::iterator findIter = find(_tasksRunning.begin(), _tasksRunning.end(), taskTodo);
                if (findIter != _tasksRunning.end())
                {
                    _tasksRunning.erase(findIter);

                    if (taskTodo->isRepeating())
                    {
                        _tasksQueued.push_back(taskTodo);
                        _tasksQueued.sort([](const iTask* a, const iTask* b) { return a->_priority < b->_priority; });
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
                _tasksQueued.push_back(task);
                _tasksQueued.sort([](const iTask* a, const iTask* b) { return a->_priority < b->_priority; });
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
