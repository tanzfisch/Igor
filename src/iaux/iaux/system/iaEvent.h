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
// Originally this code was written by Markus Schulz for nexusPIL

#ifndef __IAUX_EVENT_H__
#define __IAUX_EVENT_H__

#include <iaux/system/iaMutex.h>
#include <iaux/system/iaSlot.h>

#include <vector>
#include <algorithm>

namespace iaux
{

/*! function pointer definition
*/
template <typename... Args>
using iFunctionPointer = std::function<void(Args...)>;

/*! method pointer definition
*/
template <class T, typename... Args>
using iMethodFunctionPointer = std::function<void(T, Args...)>;

/*! a delegate that wraps a function or method (including instance to an object)
*/
template <typename... Args>
class iaDelegate
{
public:
    iaDelegate(void (*function)(Args...))
    {
        _internal = (InternalDefaultCall *)malloc(sizeof(InternalDefaultCall));
        new (_internal) InternalDefaultCall(function);
    }

    template <typename T>
    iaDelegate(T *instance, void (T::*method)(Args...))
    {
        _internal = (InternalThisCall<T> *)malloc(sizeof(InternalThisCall<T>));
        new (_internal) InternalThisCall<T>(instance, method);
    }

    void operator()(Args &&...args) const
    {
        if (_internal == nullptr)
        {
            return;
        }
        return _internal->execute(std::forward<Args>(args)...);
    }

    bool operator==(const iaDelegate<Args...> &delegate) const
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
        virtual void execute(Args &&...args) const = 0;
        virtual InternalBase *clone() const = 0;
        virtual bool compare(const InternalBase *delegate) = 0;
        virtual int getType() const = 0;
    };

    class InternalDefaultCall : public InternalBase
    {
    public:
        InternalDefaultCall(iFunctionPointer<Args...> function)
        {
            _function = function;
        }

        void execute(Args &&...args) const override
        {
            return _function(std::forward<Args>(args)...);
        }

        InternalBase *clone() const override
        {
            InternalDefaultCall *result = (InternalDefaultCall *)malloc(sizeof(InternalDefaultCall));
            new (result) InternalDefaultCall(_function);
            return result;
        }

        bool compare(const InternalBase *delegate) override
        {
            if (getType() != delegate->getType())
            {
                return false;
            }

            iFunctionPointer<Args...> function = ((InternalDefaultCall *)delegate)->_function;
            return (_function.template target<void (*)(Args...)>() == function.template target<void (*)(Args...)>());
        }

        int getType() const override
        {
            return 1;
        }

    protected:
        iFunctionPointer<Args...> _function;
    };

    template <typename T>
    class InternalThisCall : public InternalBase
    {
    public:
        InternalThisCall(T *instance, iMethodFunctionPointer<T *, Args...> method)
        {
            _method = method;
            _instance = _instance;
        }

        void execute(Args &&...args) const override
        {
            return _method(_instance, std::forward<Args>(args)...);
        }

        InternalBase *clone() const override
        {
            InternalThisCall<T> *result = (InternalThisCall<T> *)malloc(sizeof(InternalThisCall<T>));
            new (result) InternalThisCall<T>(this->_instance, this->_method);
            return result;
        }

        bool compare(const InternalBase *delegate) override
        {
            if (getType() != delegate->getType())
            {
                return false;
            }

            if (_instance != ((InternalThisCall<T> *)delegate)->_instance)
            {
                return false;
            }

            iMethodFunctionPointer<T *, Args...> method = ((InternalThisCall *)delegate)->_method;
            return (_method.template target<void (*)(Args...)>() == method.template target<void (*)(Args...)>());
        }

        int getType() const override
        {
            return 0;
        }

    protected:
        T *_instance;
        iMethodFunctionPointer<T *, Args...> _method;
    };

    InternalBase *_internal;
};

/*! event that calls a bunch of added delegates when executed
*/
template <typename... Args>
class iaEvent
{

public:
    void append(const iaDelegate<Args...> &delegate)
    {
        _mutex.lock();
        auto iter = std::find(_delegates.begin(), _delegates.end(), delegate);
        if (iter == _delegates.end())
        {
            _delegates.push_back(delegate);
        }
        _mutex.unlock();
    }

    void remove(const iaDelegate<Args...> &delegate)
    {
        _mutex.lock();
        auto iter = std::find(_delegates.begin(), _delegates.end(), delegate);
        if (iter != _delegates.end())
        {
            _delegates.erase(iter);
        }
        _mutex.unlock();
    }

    void block(bool blockEvent = true)
    {
        _blocked = blockEvent;
    }

    void unblock()
    {
        _blocked = false;
    }

    bool isBlocked()
    {
        return _blocked;
    }

    void operator()(Args &&...args)
    {
        if (_blocked)
        {
            return;
        }

        _mutex.lock();
        auto delegates = _delegates;
        _mutex.unlock();
        for (auto delegate : delegates)
        {
            delegate(std::forward<Args>(args)...);
        }
    }

    void clear()
    {
        _mutex.lock();
        _delegates.clear();
        _mutex.unlock();
    }

    bool hasDelegates()
    {
        return _delegates.size() ? true : false;
    }

protected:
    iaMutex _mutex;
    std::vector<iaDelegate<Args...>> _delegates;
    bool _blocked = false;
};

} // iaux

#endif // __IAUX_EVENT_H__