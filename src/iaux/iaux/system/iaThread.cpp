// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <iaux/system/iaThread.h>

#include <iaux/system/iaConsole.h>

namespace iaux
{
    std::map<size_t, iaID32> iaThread::_threadIDs;
    iaMutex iaThread::_mutex;    

    iaIDGenerator32 iaThread::_idGenerator;

    iaThread::iaThread(const iaString &type)
    {
        _id = _idGenerator.getNextID();
        _type = type;
    }

    iaThread::~iaThread()
    {
        if (nullptr != _thread)
        {
            delete _thread;
        }
    }

    const iaString &iaThread::getType() const
    {
        return _type;
    }

    void iaThread::init()
    {
        con_trace("start thread id:" << std::hex << getID() << std::dec << " type:" << getType());
    }

    void iaThread::deinit()
    {
        con_trace("stop thread id:" << std::hex << getID() << std::dec << " type:" << getType());
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

        iaThread::_mutex.lock();
        auto iter = _threadIDs.find(hashValue);
        iaThread::_mutex.unlock();

        if (iter != _threadIDs.end())
        {
            return iter->second;
        }

        // this must be the main thread
        return 1;
    }

    bool iaThread::isMainThread()
    {
        return std::this_thread::get_id() == IGOR_MAIN_THREAD_ID;
    }

    void *threadFunc(void *data)
    {
        iaThread *thread = (iaThread *)data;
        std::hash<std::thread::id> hashFunc;
        size_t hashValue = hashFunc(std::this_thread::get_id());

        iaThread::_mutex.lock();
        iaThread::_threadIDs[hashValue] = thread->getID();
        iaThread::_mutex.unlock();

        thread->_currentState = iaThreadState::Init;
        thread->init();
        thread->_currentState = iaThreadState::Running;
        thread->_threadDelegate(thread);
        thread->_currentState = iaThreadState::Deinit;
        thread->deinit();
        thread->_currentState = iaThreadState::Done;

        iaThread::_mutex.lock();
        iaThread::_threadIDs.erase(hashValue);
        iaThread::_mutex.unlock();
        return 0;
    }

    void iaThread::run(iThreadCallbackDelegate threadDelegate)
    {
        _threadDelegate = threadDelegate;

        _thread = new std::thread(threadFunc, (void *)this);
    }

    void iaThread::join()
    {
        if (_thread->joinable())
        {
            _thread->join();
        }
    }

}; // namespace iaux
