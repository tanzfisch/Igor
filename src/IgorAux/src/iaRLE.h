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
// (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __iaRLE__
#define __iaRLE__

#include <iaDefines.h>
#include <iaConsole.h>
#include <list>
using namespace std;

namespace IgorAux
{

    /*! one bock in rle
    */
    template <typename TValue, typename TIndex> 
    struct IgorAux_API_Template iaRLEBlock
    {
        TIndex _length = static_cast<TIndex>(0);
        TValue _value = static_cast<TValue>(0);
    };

    /*! run lenght encoded buffer

    \todo a set value range method would be nice
    */
    template <typename TValue, typename TIndex> 
    class IgorAux_API_Template iaRLE
    {
    public:

        /*! sets size of rle buffer

        \param size in amount of storable values not bytes
        */
        void setSize(TIndex size);

        /*! resets the memory to 0
        */
        void clear();

        /*! sets value at given index

        no range check

        \param index index position in buffer
        \param value the value to set
        */
        __IGOR_INLINE__ void setValue(TIndex index, TValue value);

        /*! sets value in a range of indexes

        \param indexFrom index to start from
        \param indexTo index to end with
        \param value tehe value to set
        \todo not implemented
        */
//        __IGOR_INLINE__ void setValue(TIndex index, TIndex length, TValue value);

        /*! \returns value at given index

        no range check

        \param index index position of value to return
        */
        __IGOR_INLINE__ TValue getValue(TIndex index) const;

        /*! \retruns list of blocks
        */
        const vector<iaRLEBlock<TValue, TIndex>>& getBlocks() const;

    private:

        /*! size of buffer
        */
        TIndex _size = 0;

        /*! blocks storing the RLE information
        */
        vector<iaRLEBlock<TValue, TIndex>> _blocks;
    };

#include <iaRLE.inl>

}

#endif