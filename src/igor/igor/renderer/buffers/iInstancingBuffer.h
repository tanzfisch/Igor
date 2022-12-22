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

#ifndef __IGOR_INSTANCING_BUFFER__
#define __IGOR_INSTANCING_BUFFER__

#include <igor/renderer/buffers/iVertexArray.h>

#include <memory>

namespace igor
{

    class iInstancingBuffer;

    /*! instancing buffer pointer definition
     */
    typedef std::shared_ptr<iInstancingBuffer> iInstancingBufferPtr;

    /*! instancing buffer is a convenience wrapper arround a vertex buffer and an vertex array to help with instancing
     */
    class iInstancingBuffer
    {
        friend class iInstancingBufferDeleter;

    public:

        /*! \returns a newly created vertex buffer

        \param maxInstances the maximum amount if instances
        \param layout the layout of data used for instancing
        */
        static iInstancingBufferPtr create(uint32 maxInstances, const iBufferLayout &layout);

        /*! adds instance data. This can be the date of just one instance or many.

        \param size the size of the data in bytes. Must be multiple of the layout size/stride
        \param data the data to add
        */
        void addInstance(uint32 size, const void* data);

        /*! clear the instancing data
        */
        void clear();

        /*! \returns vertex array for rendering after updating the buffer with the instancing data
        */
        iVertexArrayPtr getVertexArray();

    private:

        /*! the vertex array
        */
        iVertexArrayPtr _vertexArray;

        /*! the vertex buffer which contains the instancing data
        */
        iVertexBufferPtr _vertexBuffer;

        /*! size of instancing data buffer in bytes
        */
        uint32 _instanceDataSize;

        /*! the instancing data
        */
        uint8* _instancingData;

        /*! the next address to add more instances
        */
        uint8* _instancingDataPtr;

        /*! init instancing buffer

        \param maxInstances the maximum amount if instances
        \param layout the layout of data used for instancing
        */
        iInstancingBuffer(uint32 maxInstances, const iBufferLayout &layout);

        /*! release buffer
         */
        virtual ~iInstancingBuffer();
    };

}

#endif // __IGOR_INSTANCING_BUFFER__