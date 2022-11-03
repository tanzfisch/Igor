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

#ifndef __IGOR_VERTEX_ARRAY__
#define __IGOR_VERTEX_ARRAY__

#include <igor/renderer/buffers/iVertexBuffer.h>
#include <igor/renderer/buffers/iIndexBuffer.h>

#include <vector>

namespace igor
{

    class iVertexArray;

    /*! vertex array pointer definition
     */
    typedef std::shared_ptr<iVertexArray> iVertexArrayPtr;

    /*! vertex array aka vertex array object
     */
    class iVertexArray
    {
    public:

        /*! \returns a newly created vertex array
        */
        static iVertexArrayPtr create(); 
        
        /*! initializes vertex array
         */
        iVertexArray();

        /*! releases vertex array
         */
        virtual ~iVertexArray();

        /*! bind this buffer
         */
        void bind() const;

        /*! unbind any buffer
         */
        static void unbind();

        /*! adds a vertex buffer to the vertex array

        \param vertexBuffer the vertex buffer to be added
        */
        void addVertexBuffer(const iVertexBufferPtr &vertexBuffer);

        /*! sets the index buffer

        \param indexBuffer the index buffer to be set
        */
        void setIndexBuffer(const iIndexBufferPtr &indexBuffer);

        /*! \returns list of all vertex buffers
        */
        const std::vector<iVertexBufferPtr> &getVertexBuffers() const;

        /*! \returns index buffer
        */
        const iIndexBufferPtr &getIndexBuffer() const;

    private:

        /*! internal handle for the vertex array object
        */
        uint32 _vertexArrayObject;

        /*! components counter
        */
        uint32 _totalComponentCount = 0;

        /*! list of vertex buffers
        */
        std::vector<iVertexBufferPtr> _vertexBuffers;

        /*! the index buffer
        */
        iIndexBufferPtr _indexBuffer;
    };

}

#endif // __IGOR_VERTEX_ARRAY__