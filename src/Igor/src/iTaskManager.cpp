// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iTaskManager.h>

#include <iTask.h>
#include <iApplication.h>
#include <iThread.h>
#include <iRenderContextThread.h>
#include <iaConsole.h>
#include <iWindow.h>

namespace Igor
{
	bool iTaskManager::_running = false;

	iTaskManager::iTaskManager()
	{
		iTaskManager::_running = true;

		SYSTEM_INFO sysinfo;
		GetSystemInfo( &sysinfo );
		int32 numThreads = sysinfo.dwNumberOfProcessors + 1;

		if (numThreads < 4)
		{
            numThreads = 4;
		}

		for(int i=0;i<numThreads;++i)
		{
			createThread();
		}
	}

	iTaskManager::~iTaskManager()
	{
        iTaskManager::_running = false;

        _mutex.lock();
        // first clear all tasks left in queue
        for (auto task : _renderContextTasks)
        {
            delete task;
        }
        _renderContextTasks.clear();

        for (auto task : _tasks)
        {
            delete task;
        }
        _tasks.clear();

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

    uint32 iTaskManager::getQueuedTaskCount()
    {
        return _tasks.size();
    }

    uint32 iTaskManager::getRunningTaskCount()
    {
        return _tasksRunning.size();
    }

    uint32 iTaskManager::getQueuedRenderContextTaskCount()
    {
        return _renderContextTasks.size();
    }

    uint32 iTaskManager::getRunningRenderContextTaskCount()
    {
        return _renderContextTasksRunning.size();
    }

	bool iTaskManager::isRunning()
	{
		return _running;
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

        for (int i = 0; i<numThreads; ++i)
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

        for (auto task : _renderContextTasks)
        {
            delete task;
        }
        _renderContextTasks.clear();
        
        auto taskIter = _renderContextTasksRunning.begin();
        while(taskIter !=  _renderContextTasksRunning.end())
        {
            if ((*taskIter)->_window == window &&
                (*taskIter)->isRunning())
            {
                (*taskIter)->abort();

                while ((*taskIter)->isRunning())
                {
                    _sleep(1);
                }

                _taskFinished((*taskIter)->getID());
                delete (*taskIter);
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
			if(_renderContextTasks.size())
			{
				taskTodo = (*_renderContextTasks.begin());
				_renderContextTasks.pop_front();
				_renderContextTasksRunning.push_back(taskTodo);
			}
			_mutex.unlock();

			if(taskTodo)
			{
				taskTodo->run();

				_mutex.lock();
				list<iTask*>::iterator findIter = find(_renderContextTasksRunning.begin(), _renderContextTasksRunning.end(), taskTodo);
				if(findIter != _renderContextTasksRunning.end())
				{
					_renderContextTasksRunning.erase(findIter);

                    if (taskTodo->isRepeating())
                    {
                        addTaskIntern(taskTodo);
                    }
                    else
                    {
                        _taskFinished(taskTodo->getID());
                        delete taskTodo;
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

    void iTaskManager::work(iThread* thread)
	{
		iTask* taskTodo = nullptr;
		
        while (iTaskManager::isRunning())
		{
            taskTodo = nullptr;
			
			_mutex.lock();
			if(_tasks.size())
			{
                taskTodo = (*_tasks.begin());
				_tasks.pop_front();
                _tasksRunning.push_back(taskTodo);
			}
			_mutex.unlock();

            if (taskTodo)
			{
                taskTodo->run();

				_mutex.lock();
				list<iTask*>::iterator findIter = find(_tasksRunning.begin(), _tasksRunning.end(), taskTodo);
				if(findIter != _tasksRunning.end())
				{
					_tasksRunning.erase(findIter);

                    if (taskTodo->isRepeating())
                    {
                        addTaskIntern(taskTodo);
                    }
                    else
                    {
                        _taskFinished(taskTodo->getID());
                        delete taskTodo;
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

    void iTaskManager::addTaskIntern(iTask* task)
    {
        if (task->needsRenderContext())
        {
            _renderContextTasks.push_back(task);
            _renderContextTasks.sort([](const iTask* a, const iTask* b) { return a->_priority < b->_priority; });
        }
        else
        {
            _tasks.push_back(task);
            _tasks.sort([](const iTask* a, const iTask* b) { return a->_priority < b->_priority; });
        }
    }

	void iTaskManager::addTask(iTask* task)
	{
        con_assert(task != nullptr, "zero pointer");

        if (task != nullptr)
        {
            _mutex.lock();
            addTaskIntern(task);
            _mutex.unlock();
        }
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
