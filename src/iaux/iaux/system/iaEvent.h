//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IAUX_EVENT__
#define __IAUX_EVENT__

#include <iaux/system/iaConsole.h>

#include <vector>
#include <algorithm>

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

            con_endl("ctor 1 " << std::hex << _internal << " this " << this << std::dec);
        }

        /*! initializes with a method and instance
         */
        template <typename T>
        iaDelegate(T *instance, R (T::*method)(Args...))
        {
            _internal = (InternalThisCall<T> *)malloc(sizeof(InternalThisCall<T>));
            new (_internal) InternalThisCall<T>(instance, method);

            con_endl("ctor 2 " << std::hex << _internal << " this " << this << std::dec);
        }

        /*! copy ctor
         */
        iaDelegate(const iaDelegate *other)
        {
            _internal = other._internal->clone();
            con_endl("copy ctor " << std::hex << _internal << " this " << this << std::dec);
        }

        /*! cleanup
         */
        ~iaDelegate()
        {
            con_endl("dtor " << std::hex << _internal << " this " << this << std::dec);
            clear();
        }

        /*! clears delegate and makes it invalid
         */
        void clear()
        {
            con_endl("clear " << std::hex << _internal << " this " << this << std::dec);

            if (_internal != nullptr)
            {
                _internal->~InternalBase();
                free(_internal);
                _internal = nullptr;
            }
        }

        /*! executes the bound function or method
         */
        R operator()(Args... args) const
        {
            con_assert_sticky(_internal != nullptr, "invalid state");

            con_endl("run " << std::hex << _internal << " this " << this << std::dec);

            return _internal->execute(std::forward<Args>(args)...);
        }

        /*! assignment operator

        \param other the other delegate
        */
        const iaDelegate &operator=(const iaDelegate &other)
        {
            clear();
            _internal = other._internal->clone();
            con_endl("assignment " << std::hex << _internal << " this " << this << std::dec);
            return *this;
        }

        /*! tests whether or not two delegates are equal

        \param delegate the other delegate to test against
        */
        bool operator==(const iaDelegate<R, Args...> &delegate) const
        {
            if (_internal == nullptr)
            {
                return (delegate._internal == nullptr);
            }

            return (_internal->compare(delegate._internal));
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

    /*! helper class to determine return type
     */
    template <typename R, typename... Args>
    class iaEventReturnHandler
    {
    public:
        using ReturnType = std::vector<R>;

        static ReturnType processDelegate(const iaDelegate<R, Args...> &delegate, Args... args)
        {
            return {delegate(std::forward<Args>(args)...)};
        }
    };

    /*! helper class for void return type
     */
    template <typename... Args>
    class iaEventReturnHandler<void, Args...>
    {
    public:
        using ReturnType = void;

        static void processDelegate(const iaDelegate<void, Args...> &delegate, Args... args)
        {
            delegate(std::forward<Args>(args)...);
        }
    };

    /*! event container for delegates that executes delegates when triggered
     */
    template <typename R, typename... Args>
    class iaEvent
    {
    public:
        using ReturnHandler = iaEventReturnHandler<R, Args...>;
        using ReturnType = typename ReturnHandler::ReturnType;

        /*! adds delegate to event

        \param delegate the delegate to add
        */
        void add(const iaDelegate<R, Args...> &delegate)
        {
            _mutex.lock();
            auto iter = std::find(_delegates.begin(), _delegates.end(), delegate);
            if (iter == _delegates.end())
            {
                _delegates.push_back(delegate);
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
        ReturnType operator()(Args... args)
        {
            if (_blocked)
            {
                if constexpr (!std::is_same_v<ReturnType, void>)
                {
                    return {};
                }
                else
                {
                    return;
                }
            }

            _mutex.lock();
            auto delegates = _delegates;
            _mutex.unlock();

            if constexpr (!std::is_same_v<ReturnType, void>)
            {
                ReturnType results;
                for (auto &delegate : delegates)
                {
                    auto result = ReturnHandler::processDelegate(delegate, args...);
                    results.insert(results.end(), result.begin(), result.end());
                }
                return results;
            }
            else
            {
                for (auto &delegate : delegates)
                {
                    ReturnHandler::processDelegate(delegate, args...);
                }
            }
        }

        /*! clears delegates from event
         */
        void clear()
        {
            _mutex.lock();
            _delegates.clear();
            _mutex.unlock();
        }

        /*! \returns true if event contains delegates
         */
        bool hasDelegates()
        {
            return _delegates.size() ? true : false;
        }

    protected:
        iaMutex _mutex;
        std::vector<iaDelegate<R, Args...>> _delegates;
        bool _blocked = false;
    };

/*! helper function to specify an event and its corresponding delegate

\param NAME the name of the event
\param ... parameter types. first parameter is expected to be the return type
*/
#define IGOR_EVENT_DEFINITION(NAME, ...)      \
    typedef iaEvent<__VA_ARGS__> NAME##Event; \
    typedef iaDelegate<__VA_ARGS__> NAME##Delegate;

} // iaux

#endif // __IAUX_EVENT__l