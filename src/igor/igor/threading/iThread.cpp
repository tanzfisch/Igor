// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/threading/iThread.h>

#include <igor/physics/iPhysics.h>
#include <iaux/system/iaConsole.h>

namespace igor
{

    iThread::~iThread()
    {
        if (nullptr != _thread)
        {
            delete _thread;
        }
    }

    uint64 iThread::getWorld() const
    {
        return _worldID;
    }

    void iThread::init()
    {
        _worldID = iPhysics::getInstance().createWorld()->getID();
    }

    void iThread::deinit()
    {
        iPhysics::getInstance().destroyWorld(_worldID);
    }

    iThreadState iThread::getState()
    {
        return _currentState;
    }

    void *threadFunc(void *data)
    {
        iThread *thread = (iThread *)data;
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

        _thread = new std::thread(threadFunc, (void *)this);
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

}; // namespace igor
