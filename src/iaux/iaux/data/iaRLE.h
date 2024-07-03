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
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef __IAUX_RLE__
#define __IAUX_RLE__

#include <iaux/system/iaConsole.h>

#include <vector>

namespace iaux
{

    /*! one bock in rle
    */
    template <typename TValue, typename TIndex>
    struct IAUX_API_EXPORT_ONLY iaRLEBlock
    {
        TIndex _length = static_cast<TIndex>(0);
        TValue _value = static_cast<TValue>(0);
    };

    /*! run length encoded buffer
    */
    template <typename TValue, typename TIndex>
    class IAUX_API_EXPORT_ONLY iaRLE
    {

    public:
        /*! init members

        \param size the size of this buffer
        */
        iaRLE(TIndex size);

        /*! init members
        */
        iaRLE() = default;

        /*! sets size of rle buffer

        \param size the size of this buffer
        \param clearValue optional clear value
        */
        void setSize(TIndex size, TValue clearValue = 0);

        /*! \returns size of RLE buffer
        */
        TIndex getSize() const;

        /*! resets the memory to 0
        */
        void clear(TValue clearValue = 0);

        /*! sets value at given index

        \param index index position in buffer
        \param value the value to set
        */
        void setValue(TIndex index, TValue value);

        /*! sets value in a range of indexes

        \param index index to start from
        \param length amount of values to set
        \param value the value to set
        */
        void setValue(TIndex index, TIndex length, TValue value);

        /*! \returns value at given index

        \param index index position of value to return
        */
        TValue getValue(TIndex index) const;

        /*! makes copy of RLE data
        \param data the source data
        \returns the destination data
        */
        iaRLE<TValue, TIndex> operator=(const iaRLE<TValue, TIndex> &data);

    private:
        /*! size of buffer
        */
        TIndex _size = 0;

        /*! blocks storing the RLE information
        */
        std::vector<iaRLEBlock<TValue, TIndex>> _blocks;
    };

#include <iaux/data/iaRLE.inl>

} // namespace iaux

#endif // __IAUX_RLE__