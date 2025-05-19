//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin A. Loga
//
// This library is free software; you can redistribute it and or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
//
// contact: igorgameengine@protonmail.com
//

#ifndef IAUX_EVENT_H
#define IAUX_EVENT_H

#include <iaux/system/iaConsole.h>

#include <mutex>
#include <vector>
#include <algorithm>
#include <memory>

namespace iaux
{
    /*! a delegate that wraps a function or method
     */
    template <typename R, typename... Args>
    class iaDelegate
    {
    public:
        /*! does nothing
         */
        iaDelegate() = default;

        /*! initializes with a function pointer
         */
        iaDelegate(R (*function)(Args...))
        {
            _internal = (InternalDefaultCall *)malloc(sizeof(InternalDefaultCall));
            new (_internal) InternalDefaultCall(function);
        }

        /*! initializes with a method and instance
         */
        template <typename T>
        iaDelegate(T *instance, R (T::*method)(Args...))
        {
            con_assert(instance != nullptr, "zero pointer");
            _internal = (InternalThisCall<T> *)malloc(sizeof(InternalThisCall<T>));
            new (_internal) InternalThisCall<T>(instance, method);
        }

        /*! copy ctor

        \param other the other delegate
         */
        iaDelegate(const iaDelegate &other)
        {
            if (other._internal == nullptr)
            {
                _internal = nullptr;
                return;
            }

            _internal = other._internal->clone();
        }

        /*! cleanup
         */
        ~iaDelegate()
        {
            clear();
        }

        /*! clears delegate and makes it invalid
         */
        void clear()
        {
            if (_internal != nullptr)
            {
                _internal->~InternalBase();
                free(_internal);
                _internal = nullptr;
            }
        }

        /*! get info in to stream

        \param[out] stream the stream to put the infor in
        */
        void getInfo(std::wostream &stream) const
        {
            stream << std::hex << this << " " << _internal << std::dec;
        }

        /*! executes the bound function or method
         */
        R operator()(Args... args) const
        {
            con_assert_sticky(_internal != nullptr, "invalid state");

            return _internal->execute(std::forward<Args>(args)...);
        }

        /*! assignment operator

        \param other the other delegate
        */
        const iaDelegate &operator=(const iaDelegate &other)
        {
            clear();

            if (other._internal != nullptr)
            {
                _internal = other._internal->clone();
            }

            return *this;
        }

        /*! tests if two delegates are equal

        \param delegate the other delegate to test against
        */
        bool operator==(const iaDelegate<R, Args...> &delegate) const
        {
            if (_internal == nullptr || delegate._internal == nullptr)
            {
                return false;
            }

            return (_internal->compare(delegate._internal));
        }

        /*! tests if two delegates are not equal

        \param delegate the other delegate to test against
        */
        bool operator!=(const iaDelegate<R, Args...> &delegate) const
        {
            if (_internal == nullptr || delegate._internal == nullptr)
            {
                return true;
            }

            return !(_internal->compare(delegate._internal));
        }

        bool isValid() const
        {
            if (_internal == nullptr)
            {
                return false;
            }

            return _internal->isValid();
        }

    private:
        class InternalBase
        {
        public:
            virtual R execute(Args... args) const = 0;
            virtual InternalBase *clone() const = 0;
            virtual bool compare(const InternalBase *delegate) const = 0;
            virtual int getType() const = 0;
            virtual bool isValid() const = 0;
        };

        class InternalDefaultCall : public InternalBase
        {
        public:
            InternalDefaultCall(R (*function)(Args...))
            {
                _function = function;
            }

            R execute(Args... args) const override
            {
                return _function(std::forward<Args>(args)...);
            }

            InternalBase *clone() const override
            {
                InternalDefaultCall *result = (InternalDefaultCall *)malloc(sizeof(InternalDefaultCall));
                new (result) InternalDefaultCall(_function);
                return result;
            }

            bool compare(const InternalBase *delegate) const override
            {
                if (getType() != delegate->getType())
                {
                    return false;
                }

                R(*function)
                (Args...) = ((InternalDefaultCall *)delegate)->_function;
                return (_function == function);
            }

            int getType() const override
            {
                return 1;
            }

            bool isValid() const override
            {
                if (_function == nullptr)
                {
                    return false;
                }

                return true;
            }

        protected:
            R(*_function)
            (Args...);
        };

        template <typename T>
        class InternalThisCall : public InternalBase
        {
        public:
            InternalThisCall(T *instance, R (T::*method)(Args...))
                : _instance(instance), _method(method)
            {
            }

            R execute(Args... args) const override
            {
                return (_instance->*_method)(std::forward<Args>(args)...);
            }

            InternalBase *clone() const override
            {
                InternalThisCall<T> *result = (InternalThisCall<T> *)malloc(sizeof(InternalThisCall<T>));
                new (result) InternalThisCall<T>(this->_instance, this->_method);
                return result;
            }

            bool compare(const InternalBase *delegate) const override
            {
                if (getType() != delegate->getType())
                {
                    return false;
                }

                if (_instance != ((InternalThisCall<T> *)delegate)->_instance)
                {
                    return false;
                }

                R(T::*method)
                (Args...) = ((InternalThisCall<T> *)delegate)->_method;
                return (_method == method);
            }

            int getType() const override
            {
                return 0;
            }

            bool isValid() const override
            {
                if (_instance == nullptr || _method == nullptr)
                {
                    return false;
                }

                return true;
            }

        protected:
            T *_instance = nullptr;
            R(T::*_method)
            (Args...);
        };

        InternalBase *_internal = nullptr;
    };

    /*! stream operator

    \param stream the destination
    \param text the string to stream
    \returns the resulting stream
    */
    template <typename T>
    std::wostream &operator<<(std::wostream &stream, const iaDelegate<T> &delegate)
    {
        delegate.getInfo(stream);

        return stream;
    }

    typedef iaDelegate<void> iaEventPoolDelegate;

    /*! pool of events that need to be triggered in main thread
    */
    class iaEventPool
    {
    public:

        /*! \returns instance of this pool
        */
        static iaEventPool &getInstance()
        {
            static iaEventPool instance;
            return instance;
        }

        /*! execute all events in pool
        */
        void execute()
        {
            con_assert(std::this_thread::get_id() == IGOR_MAIN_THREAD_ID, "only allowed to run in main thread");

            _mutex.lock();
            auto delegates = _delegates;
            _mutex.unlock();

            for (const auto &delegate : delegates)
            {
                delegate();
            }
        }

        /*! registers event to pool

        \param delegate the delegate representing event and function
        */
        void registerEvent(const iaEventPoolDelegate &delegate)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _delegates.push_back(delegate);
        }

        /*! unregisters event from pool

        \param delegate the delegate representing event and function
        */
        void unregisterEvent(const iaEventPoolDelegate &delegate)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            auto it = std::find(_delegates.begin(), _delegates.end(), delegate);
            if (it == _delegates.end())
            {
                return;
            }
            _delegates.erase(it);
        }

    private:
        /*! mutex to protect pool
        */
        std::mutex _mutex;

        /*! the pool
        */
        std::vector<iaEventPoolDelegate> _delegates;

        /*! does nothing
        */
        iaEventPool() = default;

        /*! does nothing
        */
        ~iaEventPool() = default;
    };

    /*! event container for delegates that executes delegates when triggered
     */
    template <typename R, typename... Args>
    class iaEvent
    {
    public:
        /*! unregister from pool in case it is needed
        */
        ~iaEvent()
        {
            iaEventPool::getInstance().unregisterEvent(iaEventPoolDelegate(this, &iaEvent<R, Args...>::processInMain));
        }

        /*! adds delegate to event

        \param delegate the delegate to add
        \param fireOnce if true delegate will be fired once and then removed
        \param mainThread fire only on main thread
        */
        void add(const iaDelegate<R, Args...> &delegate, bool fireOnce = false, bool mainThread = false)
        {
            _mutex.lock();
            if (!fireOnce)
            {
                if (mainThread)
                {
                    iaEventPool::getInstance().registerEvent(iaEventPoolDelegate(this, &iaEvent<R, Args...>::processInMain));

                    auto iter = std::find(_delegatesMainThread.begin(), _delegatesMainThread.end(), delegate);
                    if (iter == _delegatesMainThread.end())
                    {
                        _delegatesMainThread.push_back(delegate);
                    }
                }
                else
                {
                    auto iter = std::find(_delegates.begin(), _delegates.end(), delegate);
                    if (iter == _delegates.end())
                    {
                        _delegates.push_back(delegate);
                    }
                }
            }
            else
            {
                if (mainThread)
                {
                    iaEventPool::getInstance().registerEvent(iaEventPoolDelegate(this, &iaEvent<R, Args...>::processInMain));

                    auto iter = std::find(_fireOnceDelegatesMainThread.begin(), _fireOnceDelegatesMainThread.end(), delegate);
                    if (iter == _fireOnceDelegatesMainThread.end())
                    {
                        _fireOnceDelegatesMainThread.push_back(delegate);
                    }
                }
                else
                {
                    auto iter = std::find(_fireOnceDelegates.begin(), _fireOnceDelegates.end(), delegate);
                    if (iter == _fireOnceDelegates.end())
                    {
                        _fireOnceDelegates.push_back(delegate);
                    }
                }
            }
            _mutex.unlock();
        }

        /*! removes delegate from event

        \param delegate the delegate to remove
        */
        void remove(const iaDelegate<R, Args...> &delegate)
        {
            _mutex.lock();
            auto iter = std::find(_delegates.begin(), _delegates.end(), delegate);
            if (iter != _delegates.end())
            {
                _delegates.erase(iter);
            }
            iter = std::find(_fireOnceDelegates.begin(), _fireOnceDelegates.end(), delegate);
            if (iter != _fireOnceDelegates.end())
            {
                _fireOnceDelegates.erase(iter);
            }
            iter = std::find(_delegatesMainThread.begin(), _delegatesMainThread.end(), delegate);
            if (iter != _delegatesMainThread.end())
            {
                _delegatesMainThread.erase(iter);
            }
            iter = std::find(_fireOnceDelegatesMainThread.begin(), _fireOnceDelegatesMainThread.end(), delegate);
            if (iter != _fireOnceDelegatesMainThread.end())
            {
                _fireOnceDelegatesMainThread.erase(iter);
            }
            _mutex.unlock();
        }

        /*! blocks event from executing it's delegates

        \param blockEvent if true event is blocked
        */
        void block(bool blockEvent = true)
        {
            _blocked = blockEvent;
        }

        /*! unblocks event
         */
        void unblock()
        {
            _blocked = false;
        }

        /*! \returns true if event is blocked
         */
        bool isBlocked()
        {
            return _blocked;
        }

        /*! executes event
         */
        void operator()(Args... args)
        {
            if (_blocked)
            {
                return;
            }

            bool mainThread = std::this_thread::get_id() == IGOR_MAIN_THREAD_ID;
            _mutex.lock();
            if (!mainThread &&
                (!_fireOnceDelegatesMainThread.empty() ||
                 !_delegatesMainThread.empty()))
            {
                // store args for being executed in main thread
                _fireInMain.emplace_back(std::forward<Args>(args)...);
            }

            auto delegates = std::move(_fireOnceDelegates);
            delegates.insert(delegates.end(), _delegates.begin(), _delegates.end());
            _mutex.unlock();

            for (auto &delegate : delegates)
            {
                delegate(std::forward<Args>(args)...);
            }
        }

        /*! clears delegates from event
         */
        void clear()
        {
            _mutex.lock();
            _delegates.clear();
            _fireOnceDelegates.clear();
            _mutex.unlock();
        }

        /*! \returns true if event contains delegates
         */
        bool hasDelegates()
        {
            return (!_delegates.empty() || !_fireOnceDelegates.empty()) ? true : false;
        }

    private:
        iaMutex _mutex;
        std::vector<iaDelegate<R, Args...>> _delegates;
        std::vector<iaDelegate<R, Args...>> _fireOnceDelegates;
        std::vector<iaDelegate<R, Args...>> _delegatesMainThread;
        std::vector<iaDelegate<R, Args...>> _fireOnceDelegatesMainThread;
        std::vector<std::tuple<Args...>> _fireInMain;
        bool _blocked = false;

        void processInMain()
        {
            con_assert(std::this_thread::get_id() == IGOR_MAIN_THREAD_ID, "not in main thread");

            _mutex.lock();
            auto fire = std::move(_fireInMain);
            _mutex.unlock();

            for (const auto &entry : fire)
            {
                std::apply(&iaEvent::processDelegate, std::tuple_cat(std::make_tuple(this), entry));
            }

            if (_delegatesMainThread.empty() &&
                _fireOnceDelegatesMainThread.empty())
            {
                iaEventPool::getInstance().unregisterEvent(iaEventPoolDelegate(this, &iaEvent<R, Args...>::processInMain));
            }
        }

        void processDelegate(Args &&...args)
        {
            _mutex.lock();
            auto delegates = std::move(_fireOnceDelegatesMainThread);
            delegates.insert(delegates.end(), _delegatesMainThread.begin(), _delegatesMainThread.end());
            _mutex.unlock();

            for (auto &delegate : delegates)
            {
                delegate(std::forward<Args>(args)...);
            }
        }
    };

/*! helper function to specify an event and its corresponding delegate

\param NAME the name of the event
\param ... parameter types. first parameter is expected to be the return type
*/
#define IGOR_EVENT_DEFINITION(NAME, ...)            \
    typedef iaEvent<void, __VA_ARGS__> NAME##Event; \
    typedef iaDelegate<void, __VA_ARGS__> NAME##Delegate;

#define IGOR_DELEGATE_DEFINITION(NAME, ...)            \
    typedef iaDelegate<void, __VA_ARGS__> NAME##Delegate;

#define IGOR_EVENT_DEFINITION_NO_ARGS(NAME) \
    typedef iaEvent<void> NAME##Event;      \
    typedef iaDelegate<void> NAME##Delegate;

} // iaux

#endif // IAUX_EVENT_H