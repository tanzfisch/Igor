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
//    (c) Copyright 2012-2019 by Martin Loga
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

#ifndef __IGOR_AUX_BITS__
#define __IGOR_AUX_BITS__

#include <iaDefines.h>

namespace IgorAux
{

    /*! bit field

    0 is the least significant bit
    */
    template <class T> class IgorAux_API_Template iaBits
    {

    public:

        iaBits();

        iaBits(T values);

        /*! resets nth bit to specified value

        \param bit the nth bit
        \param value the values to set
        */
        __IGOR_INLINE__ void set(uint64 bit, bool value);

        /*! overwrites all values

        \param values the new values
        */
        __IGOR_INLINE__ void setValues(T values);

        /*! sets nth bit

        \param bit the nth bit
        */
        __IGOR_INLINE__ void set(uint64 bit);

        /*! resets nth bit

        \param bit the nth bit
        */
        __IGOR_INLINE__ void reset(uint64 bit);

        /*! \returns bit value at specified position

        \param bit the nth bit
        */
        __IGOR_INLINE__ void toggle(uint64 bit);

        /*! \returns bit value at specified position

        \param bit the nth bit. 
        */
        __IGOR_INLINE__ bool get(uint64 bit) const;

        /*! \returns all values
        */
        __IGOR_INLINE__ T getValues() const;

    private:

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
};

#endif
