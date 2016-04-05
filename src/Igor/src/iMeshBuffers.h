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
// (c) Copyright 2014-2016 by Martin Loga
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

#ifndef __iMESHBUFFERS__
#define __iMESHBUFFERS__

#include <iDefines.h>

namespace Igor
{

    class iMeshBuffers
    {

    public:

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

        /*! dtor clean up
        */
        ~iMeshBuffers();

    private:

        /*! vertex buffer object ID
        */
        uint32 _vertexBufferObject = 0;

        /*! index buffer object ID
        */
        uint32 _indexBufferObject = 0;

        /*! vertex array object ID
        */
        uint32 _vertexArrayObject = 0;

        /*! instance array object ID
        */
        uint32 _instanceArrayObject = 0;

        /*! indexes count
        */
        uint32 _indexesCount = 0;

        /*! vertex count
        */
        uint32 _vertexCount = 0;

        /*! triangle count
        */
        uint32 _trianglesCount = 0;

    };

#include <iMeshBuffers.inl>

}

#endif