// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/system/iaThread.h>

#include <iaux/system/iaConsole.h>

namespace iaux
{

    iaIDGenerator32 iaThread::_idGenerator;

    iaThread::iaThread()
    {
        _id = _idGenerator.createID() + 1;
    }

    iaThread::~iaThread()
    {
        if (nullptr != _thread)
        {
            delete _thread;
        }
    }

    void iaThread::init()
    {
        // nothing to do
    }

    void iaThread::deinit()
    {
        // nothing to do
    }

    iaThreadState iaThread::getState() const
    {
        return _currentState;
    }

    iaID32 iaThread::getID() const
    {
        return _id;
    }

    void* threadFunc(void* data)
    {
        iaThread* thread = (iaThread*)data;
        std::hash<std::thread::id> hashFunc;
        size_t hashValue = hashFunc(std::this_thread::get_id());

        iaConsole::getInstance().lock();
        iaConsole::getInstance()._threadIDs[hashValue] = thread->getID();
        iaConsole::getInstance().unlock();
        

        thread->_currentState = iaThreadState::Init;
        thread->init();
        thread->_currentState = iaThreadState::Running;
        thread->_threadDelegate(thread);
        thread->_currentState = iaThreadState::Deinit;
        thread->deinit();
        thread->_currentState = iaThreadState::Done;

        iaConsole::getInstance().lock();
        iaConsole::getInstance()._threadIDs.erase(hashValue);
        iaConsole::getInstance().unlock();
        return 0;
    }

    void iaThread::run(iThreadCallbackDelegate threadDelegate)
    {
        _threadDelegate = threadDelegate;

        _thread = new std::thread(threadFunc, (void*)this);
    }

    void iaThread::join()
    {
        if (_thread->joinable())
        {
            _thread->join();
        }
    }

}; // namespace iaux
