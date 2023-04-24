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

#ifndef __IAUX_EVENT_H__
#define __IAUX_EVENT_H__

#include <iaux/system/iaConsole.h>

#include <vector>
#include <algorithm>

namespace iaux
{

    /*! function pointer definition
     */
    template <typename R, typename... Args>
    using iFunctionPointer = std::function<R(Args...)>;

    /*! method pointer definition
     */
    template <class T, typename R, typename... Args>
    using iMethodPointer = std::function<R(T, Args...)>;

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
            _internal = (InternalThisCall<T> *)malloc(sizeof(InternalThisCall<T>));
            new (_internal) InternalThisCall<T>(instance, method);
        }

        /*! executes the bound function or method
         */
        R operator()(Args... args) const
        {
            con_assert_sticky(_internal != nullptr, "invalid state");

            return _internal->execute(std::forward<Args>(args)...);
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

    private:
        class InternalBase
        {
        public:
            virtual R execute(Args... args) const = 0;
            virtual InternalBase *clone() const = 0;
            virtual bool compare(const InternalBase *delegate) const = 0;
            virtual int getType() const = 0;
        };

        class InternalDefaultCall : public InternalBase
        {
        public:
            InternalDefaultCall(iFunctionPointer<R, Args...> function)
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

                iFunctionPointer<R, Args...> function = ((InternalDefaultCall *)delegate)->_function;
                return (_function.template target<R(Args...)>() == function.template target<R(Args...)>());
            }

            int getType() const override
            {
                return 1;
            }

        protected:
            iFunctionPointer<R, Args...> _function;
        };

        template <typename T>
        class InternalThisCall : public InternalBase
        {
        public:
            InternalThisCall(T *instance, iMethodPointer<T *, R, Args...> method)
            {
                _method = method;
                _instance = instance;
            }

            R execute(Args... args) const override
            {
                return _method(_instance, std::forward<Args>(args)...);
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

                iMethodPointer<T *, R, Args...> method = ((InternalThisCall *)delegate)->_method;
                return (_method.template target<R(T *, Args...)>() == method.template target<R(T *, Args...)>());
            }

            int getType() const override
            {
                return 0;
            }

        protected:
            T *_instance = nullptr;
            iMethodPointer<T *, R, Args...> _method;
        };

        InternalBase *_internal;
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

/*! helper function to specify events and their corresponding delegates

It will generate type names from the given name adding an 'i' as prefix and "Event" or "Delegate" as postfix.

It expects at least a return type

\param NAME the name of the event
\param ... parameter types. first parameter is expected to be the return type
*/
#define IGOR_EVENT_DEFINITION(NAME, ...) \
    typedef iaEvent<__VA_ARGS__> NAME##Event; \
    typedef iaDelegate<__VA_ARGS__> NAME##Delegate;

} // iaux

#endif // __IAUX_EVENT_H__l