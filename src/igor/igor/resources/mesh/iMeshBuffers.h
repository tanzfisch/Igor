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

#ifndef __IGOR_MESHBUFFERS_H__
#define __IGOR_MESHBUFFERS_H__

#include <igor/iDefines.h>

#include <memory>

namespace igor
{

    /*! set of opengl buffer IDs
    */
    class iMeshBuffers
    {

    public:
        /*! definition of invalid buffer id
        */
        static const uint32 INVALID_BUFFER_ID = 0;

        /*! sets vertex array object

        \param vao vertex array object
        */
        void setVertexArrayObject(uint32 vao);

        /*! \returns vertex array object id
        */
        __IGOR_INLINE__ uint32 getVertexArrayObject() const;

        /*! sets index buffer object

        \param ibo index buffer object
        */
        void setIndexBufferObject(uint32 ibo);

        /*! \returns index buffer object id
        */
        __IGOR_INLINE__ uint32 getIndexBufferObject() const;

        /*! sets vertex buffer object

        \param vbo vertex buffer object
        */
        void setVertexBufferObject(uint32 vbo);

        /*! \returns vertex buffer object id
        */
        __IGOR_INLINE__ uint32 getVertexBufferObject() const;

        /*! \returns indexes count
        */
        __IGOR_INLINE__ uint32 getIndexesCount() const;

        /*! sets index count

        \param count indexes count
        */
        void setIndexesCount(uint32 count);

        /*! \returns vertex count
        */
        __IGOR_INLINE__ uint32 getVertexCount() const;

        /*! sets vertex count

        \param count vertex count
        */
        void setVertexCount(uint32 count);

        /*! \returns triangles count
        */
        __IGOR_INLINE__ uint32 getTrianglesCount() const;

        /*! set triangles count

        \param count the triangles count
        */
        void setTrianglesCount(uint32 count);

        /*! \returns true if buffers are initialized
        */
        bool isReady();

        /*! sets buffers ready
        */
        void setReady(bool ready = true);

        /*! dtor clean up
        */
        ~iMeshBuffers();

    private:
        /*! flag if buffers area ready to use
        */
        bool _ready = false;

        /*! vertex buffer object ID
        */
        uint32 _vertexBufferObject = INVALID_BUFFER_ID;

        /*! index buffer object ID
        */
        uint32 _indexBufferObject = INVALID_BUFFER_ID;

        /*! vertex array object ID
        */
        uint32 _vertexArrayObject = INVALID_BUFFER_ID;

        /*! instance array object ID
        */
        uint32 _instanceArrayObject = INVALID_BUFFER_ID;

        /*! indexes count
        */
        uint32 _indexesCount = INVALID_BUFFER_ID;

        /*! vertex count
        */
        uint32 _vertexCount = INVALID_BUFFER_ID;

        /*! triangle count
        */
        uint32 _trianglesCount = INVALID_BUFFER_ID;
    };

#include <igor/resources/mesh/iMeshBuffers.inl>

    /*! mesh buffers pointer definition
    */
    typedef std::shared_ptr<iMeshBuffers> iMeshBuffersPtr;

} // namespace igor

#endif // __IGOR_MESHBUFFERS_H__