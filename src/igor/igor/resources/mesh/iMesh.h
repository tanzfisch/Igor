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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IGOR_MESH__
#define __IGOR_MESH__

#include <igor/iDefines.h>
#include <igor/data/iAABox.h>

#include <igor/renderer/buffers/iVertexArray.h>

#include <map>

namespace igor
{

    /*! definition of shared pointer mesh
     */
    class iMesh;
    typedef std::shared_ptr<iMesh> iMeshPtr;

    /*! internal mesh structure
     */
    class IGOR_API iMesh
    {

        friend class iMeshDeleter;

    public:
        /*! \returns a newly created mesh
         */
        static iMeshPtr create();

        /*! binds mesh vertex array for rendering
         */
        void bind();

        /*! \returns true if mesh is in a valid state
         */
        bool isValid() const;

        /*! set bounding box

        \param bbox the bounding box
        */
        void setBoundingBox(const iAABoxd &bbox);

        /*! \returns the bounding box
         */
        const iAABoxd &getBoundingBox() const;

        /*! \returns indexes count
         */
        uint32 getIndexCount() const;

        /*! sets index count

        \param count indexes count
        */
        void setIndexCount(uint32 count);

        /*! \returns vertex count
         */
        uint32 getVertexCount() const;

        /*! sets vertex count

        \param count vertex count
        */
        void setVertexCount(uint32 count);

        /*! \returns triangles count
         */
        uint32 getTrianglesCount() const;

        /*! set triangles count

        \param count the triangles count
        */
        void setTrianglesCount(uint32 count);

        /*! \returns true if mesh has color deifnitions
         */
        bool hasColors() const;

        /*! \returns true if mesh has normals
         */
        bool hasNormals() const;

        /*! sets texture unit active or inactive

        \param texunit the texture unit index
        \param active flag to set the specified texture unit active or inactive
        */
        void setTexture(int texunit, bool active);

        /*! \returns true if mesh has textures and texture coordinates
         */
        bool hasTextures() const;

        /*! \returns texture unit count
         */
        uint32 getTextureUnitCount() const;

        /*! \returns true if mesh has specified texture unit
         */
        bool hasTextureUnit(uint32 unit) const;

        /*! set texture coordinates per vertex count

        \param texCoord the texture coordinates count
        */
        void setTextureCoordinatesCount(uint32 texCoord);

        /*! \returns texture coordinates count
         */
        uint32 getTextureCoordinatesCount() const;

        /*! sets if mesh has normals

        \param hasNormals flag to set if normals are present
         */
        void setHasNormals(bool hasNormals);

        /*! sets if mesh has colors

        \param hasColors flag to set if colors are present
        */
        void setHasColors(bool hasColors);

        /*! sets index data

        can be called from non main thread

        \param indexData the index data
        \param indexDataSize the index data size
        \param vertexData the vertex data
        \param vertexDataSize the vertex data size
        \param keepRawData if true the raw data will not be deleted after passing it to the GPU
        */
        void setData(const void *indexData, uint32 indexDataSize, const void *vertexData, uint32 vertexDataSize, const iBufferLayout &layout, bool keepRawData = false);

        /*! sets keep data flag

        \param keepRawData if true the raw data will not be deleted after passing it to the GPU
        */
        void setKeepRawData(bool keepRawData = true);

        /*! \returns true if the raw data will not be deleted after passing it to the GPU
         */
        bool isKeepingRawData() const;

        /*! \returns true if mesh has raw data available
         */
        bool hasRawData() const;

        /*! \returns buffer layout
         */
        const iBufferLayout &getLayout() const;

        /*! \returns raw data if available
        Availability can be tested with hasRawData

        \param[out] indexData the index data pointer
        \param[out] indexDataSize the index data size
        \param[out] vertexData the vertex data pointer
        \param[out] vertexDataSize the vertex data size
        */
        void getRawData(void *&indexData, uint32 &indexDataSize, void *&vertexData, uint32 &vertexDataSize);

        /*! \returns vertex array
         */
        const iVertexArrayPtr &getVertexArray();

    private:
        /*! index count
         */
        uint32 _indexCount = 0;

        /*! vertex count
         */
        uint32 _vertexCount = 0;

        /*! triangle count
         */
        uint32 _trianglesCount = 0;

        /*! texture coordinates count per vertex
         */
        uint32 _textureCoordinatesCount = 0;

        /*! if true mesh has normals
         */
        bool _hasNormals = false;

        /*! if true mesh has per vertex colors
         */
        bool _hasColors = false;

        /*! map for texture units
         */
        std::map<uint32, bool> _textures;

        /*! bounding box
         */
        iAABoxd _bbox;

        /*! vertex array
         */
        iVertexArrayPtr _vertexArray;

        /*! index data buffer
         */
        uint8 *_indexData = nullptr;

        /*! index data buffer size
         */
        uint32 _indexDataSize = 0;

        /*! vertex data buffer
         */
        uint8 *_vertexData = nullptr;

        /*! vertex data buffer size
         */
        uint32 _vertexDataSize = 0;

        /*! the buffer layout
         */
        iBufferLayout _layout;

        /*! if true the mesh will hold on to a copy of vertex and index data
         */
        bool _keepRawData = false;

        /*! does nothing
         */
        iMesh() = default;

        /*! does nothing
         */
        ~iMesh();

        /*! creates vertex array
         */
        void createVertexArray();
    };

} // namespace igor

#endif // __IGOR_MESH__