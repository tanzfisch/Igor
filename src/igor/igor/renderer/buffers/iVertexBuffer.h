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
// (c) Copyright 2012-2022 by Martin Loga
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

#ifndef __IGOR_VERTEX_BUFFER__
#define __IGOR_VERTEX_BUFFER__

#include <igor/renderer/buffers/iBufferLayout.h>

#include <memory>

namespace igor
{

    class iVertexBuffer;

    /*! vertex buffer pointer definition
     */
    typedef std::shared_ptr<iVertexBuffer> iVertexBufferPtr;

    /*! vertex buffer aka vertex buffer object
     */
    class iVertexBuffer
    {
        friend class iVertexBufferDeleter;

    public:

        /*! \returns a newly created vertex buffer

        \param size buffer size in bytes
        \param vertexData the vertex data
        */
        static iVertexBufferPtr create(uint32 size, const void *vertexData = nullptr);

        /*! bind this buffer
         */
        void bind() const;

        /*! unbind any buffer
         */
        static void unbind();

        /*! sets data on buffer

        Ideally only use this if the buffer is dynamic but generally it should always be safe to use

        \param size data size in bytes
        \param vertexData the vertex data
        */
        void setData(uint32 size, const void *vertexData);

        /*! sets buffer layout

        \param layout the layout to set
        */
        void setLayout(const iBufferLayout &layout);

        /*! \returns buffer layout
         */
        const iBufferLayout &getLayout() const;

        /*! \returns true if this is a dynamic buffer
         */
        bool isDynamic() const;

        /*! \returns vertex count
        */
        uint32 getVertexCount() const;

        /*! \returns vertex size
        */
        uint32 getVertexSize() const;

        /*! \returns buffer size in byte
        */
        uint32 getBufferSize() const;

    private:
        /*! internal buffer id
         */
        uint32 _vertexBufferObject;

        /*! the buffer layout
         */
        iBufferLayout _layout;

        /*! if true this is a dynamic buffer
         */
        bool _dynamic;        

        /*! the buffe size in byte
        */
        uint32 _bufferSize;

        /*! init vertex buffer

        \param size buffer size in bytes
        \param vertexData the vertex data
        */
        iVertexBuffer(uint32 size, const void *vertexData);

        /*! release buffer
         */
        virtual ~iVertexBuffer();        
    };

}

#endif // __IGOR_VERTEX_BUFFER__