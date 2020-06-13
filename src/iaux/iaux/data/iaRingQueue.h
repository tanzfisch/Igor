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
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __IAUX_RINGQUEUE__
#define __IAUX_RINGQUEUE__

#include <iaux/iaDefines.h>

namespace iaux
{

    /*! todo not in use yet and not tested yet
    */
    template <typename TValue>
    class iaRingQueue
    {

    public:
        iaRingQueue(uint64 size);

        virtual ~iaRingQueue();

        __IGOR_INLINE__ void push(const TValue &value);

        __IGOR_INLINE__ bool pop(TValue &dstValue);

    protected:
        uint64 _size = 0;

        TValue *_buffer = nullptr;

        uint64 _head = 0;
        uint64 _tail = 0;
    };

    template <typename TValue>
    iaRingQueue<TValue>::iaRingQueue(uint64 size)
    {
        con_assert(size > 0, "invalid param");
        if (size > 0)
        {
            _size = size;
            _buffer = new TValue[_size];
        }
        else
        {
            con_err("invalid param");
        }
    }

    template <typename TValue>
    iaRingQueue<TValue>::~iaRingQueue()
    {
        delete[] _buffer;
    }

    template <typename TValue>
    __IGOR_INLINE__ void iaRingQueue<TValue>::push(const TValue &value)
    {
        _buffer[_head % _size] = value;
        _head++;

        con_assert(_head % _size == _tail % _size, "ring buffer overflow");
        if (_head % _size == _tail % _size)
        {
            con_err("ring buffer overflow");
        }
    }

    template <typename TValue>
    __IGOR_INLINE__ bool iaRingQueue<TValue>::pop(TValue &dstValue)
    {
        bool result = false;

        if (_head % _size != _tail % _size)
        {
            dstValue = _buffer[_tail % _size];
            _tail++;
            result = true;
        }

        return result;
    }

}; // namespace iaux
#endif