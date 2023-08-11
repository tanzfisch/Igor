// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/system/iaThread.h>

#include <iaux/system/iaConsole.h>

namespace iaux
{
    /*! maps thread IDs to thread names
        */
    static std::map<size_t, iaID32> _threadIDs;

    /*! mutex to protect thread ids
    */
    static iaMutex _mutex;

    iaIDGenerator32 iaThread::_idGenerator;

    iaThread::iaThread()
    {
        // starting with 1
        _id = _idGenerator.getNextID() + 1;
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

    iaID32 iaThread::getThisThreadID()
    {
        std::hash<std::thread::id> hashFunc;
        size_t hashValue = hashFunc(std::this_thread::get_id());

        auto iter = _threadIDs.find(hashValue);
        if (iter != _threadIDs.end())
        {
            return iter->second;
        }

        // this must be the main thread
        return 1;
    }

    void* threadFunc(void* data)
    {
        iaThread* thread = (iaThread*)data;
        std::hash<std::thread::id> hashFunc;
        size_t hashValue = hashFunc(std::this_thread::get_id());

        _mutex.lock();
        _threadIDs[hashValue] = thread->getID();
        _mutex.unlock();

        thread->_currentState = iaThreadState::Init;
        thread->init();
        thread->_currentState = iaThreadState::Running;
        thread->_threadDelegate(thread);
        thread->_currentState = iaThreadState::Deinit;
        thread->deinit();
        thread->_currentState = iaThreadState::Done;

        _mutex.lock();
        _threadIDs.erase(hashValue);
        _mutex.unlock();
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
