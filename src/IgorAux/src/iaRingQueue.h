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
// (c) Copyright 2014-2016 by Martin Loga
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
// contact: martinloga@gmx.de  

#ifndef __iaRINGQUEUE__
#define __iaRINGQUEUE__

#include <iaDefines.h>

#include <mutex>
using namespace std;

namespace IgorAux
{

    template <typename TValue>
    class iaRingQueueBase
    {

    public:

        iaRingQueueBase(uint64 size);

        virtual ~iaRingQueueBase();

    protected:

        uint64 _size = 0;

        TValue* _buffer = nullptr;

        uint64 _head = 0;
        uint64 _tail = 0;

    };

    template <typename TValue>
    iaRingQueueBase<TValue>::iaRingQueueBase(uint64 size)
    {
        _size = size;
        _buffer = new TValue[_size];
    }

    template <typename TValue>
    iaRingQueueBase<TValue>::~iaRingQueueBase()
    {
        delete[] _buffer;
    }

    enum class iaRingQueueUseMutex
    {
        On,
        Off
    };

    template <typename TValue, iaRingQueueUseMutex Src, iaRingQueueUseMutex Dst>
    class iaRingQueue : public iaRingQueueBase<TValue>
    {
    public:

        iaRingQueue(uint64 size);

        void push(const TValue& value);

        bool pop(TValue& dstValue);

    };

    template <typename TValue, iaRingQueueUseMutex Src, iaRingQueueUseMutex Dst>
    iaRingQueue<TValue, Src, Dst>::iaRingQueue(uint64 size)
        :iaRingQueueBase<TValue>(size)
    {
        // nothing to do
    }

    template <typename TValue, iaRingQueueUseMutex Src, iaRingQueueUseMutex Dst>
    void iaRingQueue<TValue, Src, Dst>::push(const TValue& value)
    {
        // nothing to do
    }

    template <typename TValue, iaRingQueueUseMutex Src, iaRingQueueUseMutex Dst>
    bool iaRingQueue<TValue, Src, Dst>::pop(TValue& dstValue)
    {
        // nothing to do
        return false;
    }

    template <typename TValue>
    class iaRingQueue<TValue, iaRingQueueUseMutex::Off, iaRingQueueUseMutex::Off> : public iaRingQueueBase<TValue>
    {
    public:

        iaRingQueue(uint64 size);

        void push(const TValue& value);

        bool pop(TValue& dstValue);

    };

    template <typename TValue>
    iaRingQueue<TValue, iaRingQueueUseMutex::Off, iaRingQueueUseMutex::Off>::iaRingQueue(uint64 size)
        :iaRingQueueBase<TValue>(size)
    {
        // nothing to do
    }

    template <typename TValue>
    void iaRingQueue<TValue, iaRingQueueUseMutex::Off, iaRingQueueUseMutex::Off>::push(const TValue& value)
    {
        if ((_head + 1) % _size == _tail)
        {
            con_err("ring buffer overflow");
        }
        else
        {
            _buffer[_head] = value;
            _head = (_head + 1) % _size;
        }
    }

    template <typename TValue>
    bool iaRingQueue<TValue, iaRingQueueUseMutex::Off, iaRingQueueUseMutex::Off>::pop(TValue& dstValue)
    {
        bool result = false;

        if (_head != _tail)
        {
            dstValue = _buffer[_tail];
            _tail = (_tail + 1) % _size;
            result = true;
        }

        return result;
    }

    template <typename TValue>
    class iaRingQueue<TValue, iaRingQueueUseMutex::On, iaRingQueueUseMutex::On> : public iaRingQueueBase<TValue>
    {
    public:

        iaRingQueue(uint64 size);

        void push(const TValue& value);

        bool pop(TValue& dstValue);

    private:

        mutex _pushMutex;
        mutex _popMutex;

    };

    template <typename TValue>
    iaRingQueue<TValue, iaRingQueueUseMutex::On, iaRingQueueUseMutex::On>::iaRingQueue(uint64 size)
        :iaRingQueueBase<TValue>(size)
    {
        // nothing to do
    }

    template <typename TValue>
    void iaRingQueue<TValue, iaRingQueueUseMutex::On, iaRingQueueUseMutex::On>::push(const TValue& value)
    {
        _pushMutex.lock();
        if ((_head + 1) % _size == _tail)
        {
            con_err("ring buffer overflow");
        }
        else
        {
            _buffer[_head] = value;
            _head = (_head + 1) % _size;
        }
        _pushMutex.unlock();
    }

    template <typename TValue>
    bool iaRingQueue<TValue, iaRingQueueUseMutex::On, iaRingQueueUseMutex::On>::pop(TValue& dstValue)
    {
        bool result = false;

        _popMutex.lock();
        if (_head != _tail)
        {
            dstValue = _buffer[_tail];
            _tail = (_tail + 1) % _size;
            result = true;
        }
        _popMutex.unlock();

        return result;
    }

    template <typename TValue>
    class iaRingQueue<TValue, iaRingQueueUseMutex::On, iaRingQueueUseMutex::Off> : public iaRingQueueBase<TValue>
    {
    public:

        iaRingQueue(uint64 size);

        void push(const TValue& value);

        bool pop(TValue& dstValue);

    private:

        mutex _pushMutex;

    };

    template <typename TValue>
    iaRingQueue<TValue, iaRingQueueUseMutex::On, iaRingQueueUseMutex::Off>::iaRingQueue(uint64 size)
        :iaRingQueueBase<TValue>(size)
    {
        // nothing to do
    }

    template <typename TValue>
    void iaRingQueue<TValue, iaRingQueueUseMutex::On, iaRingQueueUseMutex::Off>::push(const TValue& value)
    {
        _pushMutex.lock();
        if ((_head + 1) % _size == _tail)
        {
            con_err("ring buffer overflow");
        }
        else
        {
            _buffer[_head] = value;
            _head = (_head + 1) % _size;
        }
        _pushMutex.unlock();
    }

    template <typename TValue>
    bool iaRingQueue<TValue, iaRingQueueUseMutex::On, iaRingQueueUseMutex::Off>::pop(TValue& dstValue)
    {
        bool result = false;

        if (_head != _tail)
        {
            dstValue = _buffer[_tail];
            _tail = (_tail + 1) % _size;
            result = true;
        }

        return result;
    }

    template <typename TValue>
    class iaRingQueue<TValue, iaRingQueueUseMutex::Off, iaRingQueueUseMutex::On> : public iaRingQueueBase<TValue>
    {
    public:

        iaRingQueue(uint64 size);

        void push(const TValue& value);

        bool pop(TValue& dstValue);

    private:

        mutex _popMutex;

    };

    template <typename TValue>
    iaRingQueue<TValue, iaRingQueueUseMutex::Off, iaRingQueueUseMutex::On>::iaRingQueue(uint64 size)
        :iaRingQueueBase<TValue>(size)
    {
        // nothing to do
    }

    template <typename TValue>
    void iaRingQueue<TValue, iaRingQueueUseMutex::Off, iaRingQueueUseMutex::On>::push(const TValue& value)
    {
        if ((_head + 1) % _size == _tail)
        {
            con_err("ring buffer overflow");
        }
        else
        {
            _buffer[_head] = value;
            _head = (_head + 1) % _size;
        }
    }

    template <typename TValue>
    bool iaRingQueue<TValue, iaRingQueueUseMutex::Off, iaRingQueueUseMutex::On>::pop(TValue& dstValue)
    {
        bool result = false;

        _popMutex.lock();
        if (_head != _tail)
        {
            dstValue = _buffer[_tail];
            _tail = (_tail + 1) % _size;
            result = true;
        }
        _popMutex.unlock();

        return result;
    }

};
#endif