// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iThread.h>

#include <iaConsole.h>

namespace Igor
{

	iThread::~iThread()
	{
        if (nullptr != _thread)
        {
            delete _thread;
        }
	}

	void iThread::init()
	{
		// base implementation does nothing here
	}

	void iThread::deinit()
	{
		// base implementation does nothing here
	}

    iThreadState iThread::getState()
    {
        return _currentState;
    }

    void* threadFunc(void* data)
	{
		iThread* thread = (iThread*)data;
        thread->_currentState = iThreadState::Init;
		thread->init();
        thread->_currentState = iThreadState::Running;
        thread->_threadDelegate(thread);
        thread->_currentState = iThreadState::Deinit;
		thread->deinit();
        thread->_currentState = iThreadState::Done;
		return 0;
	}

	void iThread::run(ThreadDelegate threadDelegate)
	{
        _threadDelegate = threadDelegate;

        _thread = new thread(threadFunc, (void*)this);
	}

    void iThread::join()
    {
        if (_thread->joinable())
        {
            _thread->join();
        }
        else
        {
            con_err("thread not joinable");
        }
    }

};