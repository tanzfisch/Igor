// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iThread.h>
#include <iPhysics.h>

#include <iWindow.h>
#include <iaConsole.h>

namespace Igor
{

    iThread::iThread(iWindow* window)
    {
        con_assert(nullptr != window, "window is missing");
        _window = window;

        if (!(_renderContext = wglCreateContext(_window->getDeviceContext())))
        {
            con_err_win("can't create render context");
        }
        else
        {
            if (!wglShareLists(_window->getRenderContext(), _renderContext))
            {
                con_err_win("can't share lists");
            }
        }
    }

	iThread::~iThread()
	{
        if (nullptr != _thread)
        {
            delete _thread;
        }
	}

	void iThread::init()
	{
        // init render context
        if (!wglMakeCurrent(_window->getDeviceContext(), _renderContext))
        {
            con_err_win("can't make render context current");
        }

        // init physics context
        _worldID = iPhysics::getInstance().createWorld()->getID();
	}

	void iThread::deinit()
	{
        // deinit render context
        if (!wglMakeCurrent(nullptr, nullptr))
        {
            con_err_win("can't set current to zero");
            return;
        }

        if (!wglDeleteContext(_renderContext))
        {
            con_err_win("can't delete render context");
            return;
        }

        // deinit physics context
        iPhysics::getInstance().destroyWorld(_worldID);
        _worldID = iPhysicsWorld::INVALID_WORLD_ID;
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

    uint64 iThread::getWorld() const
    {
        return _worldID;
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
