//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/                     ) )     ((
//                                           (_(       \)
//    (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __IAUX_BITS_H__
#define __IAUX_BITS_H__

#include <iaux/iaDefines.h>

namespace iaux
{

    /*! bit field

    bit 0 is the least significant bit    

    \deprecated use std::bitset instead
    */
    template <class T>
    class IgorAux_API_Template iaBits
    {

    public:
        /*! init with zeros
        */
        iaBits();

        /*! copy ctor
        */
        iaBits(T values);

        /*! resets nth bit to specified value

        \param bit the nth bit
        \param value the values to set
        */
        void set(uint64 bit, bool value);

        /*! sets all data

        \param values the new data
        */
        void setData(T data);

        /*! \returns the internal data
        */
        T getData() const;

        /*! sets nth bit

        \param bit the nth bit
        */
        void set(uint64 bit);

        /*! resets nth bit

        \param bit the nth bit
        */
        void reset(uint64 bit);

        /*! \returns bit value at specified position

        \param bit the nth bit
        */
        void toggle(uint64 bit);

        /*! \returns bit value at specified position

        \param bit the nth bit. 
        */
        bool get(uint64 bit) const;

    private:
        /*! the bit field
        */
        T _bitField;
    };

#include <iaBits.inl>

    /*! 8 bit field
    */
    typedef iaBits<uint8> iaBits8;

    /*! 16 bit field
    */
    typedef iaBits<uint16> iaBits16;

    /*! 32 bit field
    */
    typedef iaBits<uint32> iaBits32;

    /*! 64 bit field
    */
    typedef iaBits<uint64> iaBits64;

}; // namespace iaux

#endif // __IAUX_BITS_H__
