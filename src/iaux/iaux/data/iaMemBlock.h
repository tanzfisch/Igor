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

#ifndef __IAUX_MEMBLOCK__
#define __IAUX_MEMBLOCK__

#include <iaux/iaDefines.h>

namespace iaux
{

    /*! just a block of memory
    */
    class IgorAux_API iaMemBlock
    {
    public:
        /*! does nothing
        */
        iaMemBlock();

        /*! frees memory
        */
        virtual ~iaMemBlock();

        /*! frees memory
        */
        void clearData();

        /*! sets data

        makes local copy of data

        \param buffer pointer to new data
        \param size the size in bytes
        */
        void setData(const char *buffer, uint64 size);

        /*! copies local data and returns it

        \param[out] buffer the buffer to copy the data in
        \param size size of destination buffer. must be bigger or equal than src buffer
        */
        void getData(char *buffer, uint64 size) const;

        /*! \returns pointer to memory
        */
        char *getDataPointer() const;

        /*! \retruns true if memory is allocated
        */
        bool hasData();

        /*! \returns size of memory block
        */
        uint64 getDataSize();

    private:
        /*! pointer to memory
        */
        char *_data = nullptr;

        /*! size of memory
        */
        uint64 _dataSize = 0;
    };

} // namespace iaux

#endif