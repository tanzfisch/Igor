//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin Loga
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

#ifndef __IGOR_INDEX_BUFFER__
#define __IGOR_INDEX_BUFFER__

#include <igor/iDefines.h>

#include <memory>

namespace igor
{

    /*! vertex buffer pointer definition
     */
    class iIndexBuffer;
    typedef std::shared_ptr<iIndexBuffer> iIndexBufferPtr;

    /*! index buffer aka index buffer object
     */
    class iIndexBuffer
    {
        friend class iIndexBufferDeleter;
        friend class iVertexArray;

    public:
        /*! \returns a newly created index buffer

        \param cound index count
        \param indices the index data
        */
        static iIndexBufferPtr create(uint32 count, const uint32 *indices = nullptr);

        /*! bin this buffer
         */
        void bind();

        /*! \returns index count of this buffer
         */
        uint32 getIndexCount() const;

        /*! sets data on buffer

        Ideally only use this if the buffer is dynamic but generally it should always be safe to use

        \param size the size in bytes
        \param indexData the index data to set
        */
        void setData(uint32 size, const void *indexData);

        /*! \returns true if this is a dynamic buffer
         */
        bool isDynamic() const;

    private:
        /*! handle to internal buffer object
         */
        uint32 _indexBufferObject;

        /*! index count
         */
        uint32 _indexCount;

        /*! if true this is a dynamic buffer
         */
        bool _dynamic;

        /*! ctor with parameters

        \param count the count if indices in the buffer
        \param indices the index data
         */
        iIndexBuffer(uint32 count, const uint32 *indices);

        /*! releases resources
         */
        virtual ~iIndexBuffer();
    };

}

#endif // __IGOR_INDEX_BUFFER__