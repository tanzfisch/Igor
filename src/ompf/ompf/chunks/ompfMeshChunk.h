//
//  ##### #   # ####  #####
//  #   # ## ## #   # #
//  #   # # # # ####  ####
//  #   # #   # #     #
//  ##### #   # #     #
//
//  (c) Copyright by Martin Loga
//
// This library is free software; you can redistribute it and or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
//
// contact: igorgameengine@protonmail.com

#ifndef OMPF_MESH_CHUNK_H
#define OMPF_MESH_CHUNK_H

#include <ompf/chunks/ompfBaseChunk.h>

#include <iaux/data/iaColor3.h>
#include <iaux/math/iaVector3.h>
using namespace iaux;

#include <map>

namespace OMPF
{

    /*! ompf mesh type

    \todo for now there are only triangles
    */
    enum class OMPF_API OMPFMeshType
    {
        Triangles = 0
    };

    /*! ompf mesh chunk
     */
    class OMPF_API ompfMeshChunk : public ompfBaseChunk
    {

        friend class OMPF;

    public:
        /*! init base class
         */
        ompfMeshChunk();

        /*! release some memory
         */
        virtual ~ompfMeshChunk();

        /*! sets amount of normals per vertex (usually 0 or 1)

        \param count the amount of normals per vertex
        */
        void setNormalsPerVertex(uint8 count);

        /*! \returns amount of normals per vertex
         */
        uint8 getNormalsPerVertex() const;

        /*! sets amount of colors per vertex (usually 0 or 1)

        \param count the amount of colors per vertex
        */
        void setColorsPerVertex(uint8 count);

        /*! \returns amount of colors per vertex
         */
        uint8 getColorsPerVertex() const;

        /*! sets the amount of texture coordinates per vertex (usually 0 or 1)

        \param count the amount of texture coordinates per vertex
        */
        void setTexCoordPerVertex(uint8 count);

        /*! \returns the amount of texture coordinates per vertex
         */
        uint8 getTexCoordPerVertex() const;

        /*! sets vertex count

        \param count the amount of vertexes
        */
        void setVertexCount(uint32 count);

        /*! \returns the vertex count
         */
        uint32 getVertexCount() const;

        /*! sets the amount of indexes

        \param count the amount of indexes
        */
        void setIndexCount(uint32 count);

        /*! \returns the indexes count
         */
        uint32 getIndexCount() const;

        /*! sets vertex data

        \param data pointer to raw vertex data
        \param size the size in bytes
        */
        void setVertexData(const char *data, uint32 size);

        /*! \returns pointer to vertex data
         */
        const void *getVertexData() const;

        /*! sets index data

        \param data pointer to index data
        \param size the size of index data in bytes
        */
        void setIndexData(const char *data, uint32 size);

        /*! \returns pointer to index data
         */
        const void *getIndexData() const;

        /*! \returns vertex data size
         */
        uint32 getVertexDataSize() const;

        /*! \returns index data size
         */
        uint32 getIndexDataSize() const;

        /*! calculates the size of a single vertex

        \returns the vertex size
        */
        uint32 getVertexSize() const;

        /*! sets the mesh type

        \param type the mesh type
        */
        void setMeshType(OMPFMeshType type);

        /*! \returns the mesh type
         */
        OMPFMeshType getMeshType() const;

        /*! sets a texture for specified texture unit

        \param texture the texture
        \param texunit the texture unit specified
        */
        void setTexture(const iaString &texture, uint32 texunit);

        /*! \returns texture for specified texture unit

        \param texunit the texture unit
        */
        iaString getTexture(uint32 texunit) const;

        /*! \returns amount of textures in use
         */
        uint32 getTextureCount() const;

        /*! sets material chund id

        \param id the material chunk id
        */
        void setMaterialChunkID(uint32 id);

        /*! \returns material chunk id
         */
        uint32 getMaterialChunkID() const;

    private:
        /*! normal count per vertex
         */
        uint8 _normalsPerVertex = 0;

        /*! color count per vertex
         */
        uint8 _colorsPerVertex = 0;

        /*! texture coordinates count per vertex
         */
        uint8 _texCoordPerVertex = 0;

        /*! mesh type
         */
        OMPFMeshType _meshType = OMPFMeshType::Triangles;

        /*! vertex count
         */
        uint32 _vertexCount = 0;

        /*! pointer to vertex data
         */
        void *_vertexData = nullptr;

        /*! vertex data size
         */
        uint32 _vertexDataSize = 0;

        /*! index count
         */
        uint32 _indexCount = 0;

        /*! pointer to index data
         */
        void *_indexData = nullptr;

        /*! index data size in bytes
         */
        uint32 _indexDataSize = 0;

        /*! material chunk id
         */
        uint32 _materialChunkID = OMPFDefaultConfiguration::INVALID_CHUNK_ID;

        /*! \returns size of chunk in bytes

        \param settings the settings the size calculation is based on
        */
        uint32 getSize(const ompfSettings &settings) override;

        /*! writes chunk to stream

        \param stream destination stream
        \param settings the settings how to write the chunk
        */
        bool write(std::ofstream &stream, const ompfSettings &settings) override;

        /*! reads chunk from stream

        \param stream source stream
        \param settings the settings how to read the chunk
        */
        bool read(std::ifstream &stream, ompfSettings &settings) override;
    };

}

#endif // OMPF_MESH_CHUNK_H