// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "FirstSteps.h"

template <typename... Args>
using iFunctionPointer = std::function<void(Args...)>;

template <class T, typename... Args>
using iMethodFunctionPointer = std::function<void(T, Args...)>;

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

void foo(int i) { std::cout << "foo " << i << std::endl; }

class Bar
{
public:
    void bar(int i) { std::cout << "Bar::bar " << i << std::endl; }

    static void baar(int i) { std::cout << "Bar::baar " << i << std::endl; }
};

int main(void)
{
    Bar bar;

    iaDelegate<int> delegate1(&bar, &Bar::bar);
    delegate1(10);

    iaDelegate delegate2(&Bar::baar);
    delegate2(20);

    iaDelegate delegate3(&foo);
    delegate3(30);

    iaEvent<int> event;
    event.append(delegate1);
    event(40);
    event.append(delegate2);
    event(50);
    event.append(delegate3);
    event(60);

    return 0;

    // call this before you call anything else of Igor
    igor::startup();

    // we are not creating a window here just to show that we don't need one

    // create example and add it to the layer stack
    igor::iApplication::getInstance().addLayer(new FirstSteps());

    // run example
    igor::iApplication::getInstance().run();

    // call this after you are done using Igor
    igor::shutdown();

    return 0;
}
