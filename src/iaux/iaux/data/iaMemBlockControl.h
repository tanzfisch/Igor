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
// contact: igorgameengine@protonmail.com

#ifndef __IAUX_MEMREADER__
#define __IAUX_MEMREADER__

#include <iaux/iaDefines.h>
#include <iaux/data/iaString.h>
using namespace iaux;

namespace iaux
{

    /*! memory block with some convinient methods

    \todo write buffer functionality
    */
    class IgorAux_API iaMemBlockControl
    {

    public:
        /*! does nothing
        */
        iaMemBlockControl() = default;

        /*! does nothing
        */
        ~iaMemBlockControl() = default;

        /*! sets the buffer to control or manipulate

        \param size new site of allocated buffer
        */
        void setBuffer(void *data, uint64 size);

        /*! \returns pointer to data
        */
        void *getData();

        /*! \returns size of data
        */
        uint64 getSize();

        /*! jups to specified position relative to beginning

        \param pos position relative to beginning
        */
        void seekBeg(uint64 pos);

        /*! jups to specified position relative to the end

        \param pos position relative to end
        */
        void seekEnd(uint64 pos);

        /*! jups to specified position relative to current position

        \param pos position relative to current position
        */
        void seekCur(uint64 pos);

        /*! \returns current position
        */
        uint64 tell();

        /*! \returns true: if end of buffer reached; false: if current position is still in buffer range
        */
        bool eof();

        /*! reads data from buffer and stores it in specified location.
        current location will be increased by the amount of bytes red

        \param destination pointer to destination buffer
        \patam count amount of bytes to copy
        \returns amount of actually read bytes
        */
        uint64 read(char *destination, uint64 count);

        /*! \returns uint8 value from buffer
        current location will be increased by the amount of bytes red.
        No range check.
        */
        uint8 readUInt8();

        /*! \returns int8 value from buffer
        current location will be increased by the amount of bytes red.
        No range check.
        */
        int8 readInt8();

        /*! \returns uint16 value from buffer
        current location will be increased by the amount of bytes red.
        No range check.
        */
        uint16 readUInt16();

        /*! \returns int16 value from buffer
        current location will be increased by the amount of bytes red.
        No range check.
        */
        int16 readInt16();

        /*! \returns uint32 value from buffer
        current location will be increased by the amount of bytes red.
        No range check.
        */
        uint32 readUInt32();

        /*! \returns int32 value from buffer
        current location will be increased by the amount of bytes red.
        No range check.
        */
        int32 readInt32();

        /*! \returns uint64 value from buffer
        current location will be increased by the amount of bytes red.
        No range check.
        */
        uint64 readUInt64();

        /*! \returns int64 value from buffer
        current location will be increased by the amount of bytes red.
        No range check.
        */
        int64 readInt64();

        /*! \returns float32 value from buffer
        current location will be increased by the amount of bytes red.
        No range check.
        */
        float32 readFloat32();

        /*! \returns float64 value from buffer
        current location will be increased by the amount of bytes red.
        No range check.
        */
        float64 readFloat64();

    private:
        /*! pointer to actual data
        */
        void *_data = nullptr;

        /*! current position in data
        */
        uint64 _pos = 0;

        /*! size of memory allocated
        */
        uint64 _size = 0;
    };

} // namespace iaux

#endif