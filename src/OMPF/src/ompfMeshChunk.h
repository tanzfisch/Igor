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
// contact: martinloga@gmx.de  

#ifndef __MESHCHUNK__
#define __MESHCHUNK__

#include <ompfBaseChunk.h>

#include <iaColor3.h>
#include <iaVector3.h>
using namespace IgorAux;

#include <map>
using namespace std;

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

	public:

        /*! init base class
        */
		ompfMeshChunk();

        /*! release some memory
        */
		virtual ~ompfMeshChunk();

        /*! \returns size of chunk in bytes

        \param settings the settings the size calculation is based on
        */
        virtual uint32 getSize(const ompfSettings& settings);

        /*! sets ambient color of target material of mesh

        \param ambient the ambient color
        */
        void setAmbient(const iaColor3c& ambient);

        /*! \returns ambient color
        */
        const iaColor3c& getAmbient() const;

        /*! sets diffuse color of target material of mesh

        \param diffuse the diffuse color
        */
        void setDiffuse(const iaColor3c& diffuse);

        /*! \returns diffuse color
        */
        const iaColor3c& getDiffuse() const;

        /*! sets specular color of target material of mesh

        \param specular the specular color
        */
        void setSpecular(const iaColor3c& specular);

        /*! \returns specular color
        */
        const iaColor3c& getSpecular() const;

        /*! sets emissiv color of target material of mesh

        \param emissiv the emissiv color
        */
        void setEmissive(const iaColor3c& emissiv);

        /*! \returns emissive color
        */
        const iaColor3c& getEmissive() const;

        /*! sets shininess of target material of mesh

        \param shininess the shininess factor
        */
        void setShininess(const float32 shininess);

        /*! \returns the shininess factor
        */
        float32 getShininess() const;

		/*! sets the alpha value

		\param shininess the shininess factor
		*/
		void setAlpha(const float32 alpha);

		/*! \returns the alpha value
		*/
		float32 getAlpha() const;

        /*! sets amount of normals per vertex

        \param count the amount of normals per vertex
        */
        void setNormalsPerVertex(uint8 count);

        /*! \returns amount of normals per vertex
        */
        uint8 getNormalsPerVertex() const;

        /*! sets amount of colors per vertex

        \param count the amount of colors per vertex
        */
        void setColorsPerVertex(uint8 count);

        /*! \returns amount of colors per vertex
        */
        uint8 getColorsPerVertex() const;

        /*! sets the amount of texture coordinates per vertex

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
        void setVertexData(const char* data, uint32 size);

        /*! \returns pointer to vertex data
        */
        const void* getVertexData() const;

        /*! sets index data

        \param data pointer to index data
        \param size the size of index data in bytes
        */
        void setIndexData(const char* data, uint32 size);

        /*! \returns pointer to index data
        */
        const void* getIndexData() const;

        /*! \returns vertex data size
        */
        uint32 getVertexDataSize() const;

        /*! \returns index data size
        */
        uint32 getIndexDataSize() const;

        /*! calculates the vertex size

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
        void setTexture(const iaString& texture, uint32 texunit);

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

        /*! ambient color
        */
        iaColor3c _ambient;

        /*! diffuse color
        */
        iaColor3c _diffuse;

        /*! specular color
        */
        iaColor3c _specular;
        
        /*! emissiv color
        */
        iaColor3c _emissive;

        /*! shininess factor
        */
        float32 _shininess = 0;

		/*! alpha value
		*/
		float32 _alpha = 1.0;

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
        void* _vertexData = nullptr;

        /*! vertex data size
        */
        uint32 _vertexDataSize = 0;

        /*! index count
        */
        uint32 _indexCount = 0;

        /*! pointer to index data
        */
        void* _indexData = nullptr;

        /*! index data size in bytes
        */
        uint32 _indexDataSize = 0;

        /*! material chunk id
        */
        uint32 _materialChunkID = OMPFDefaultConfiguration::INVALID_CHUNK_ID;

        /*! texture map with textures per texture unit
        */
        map<uint32, iaString> _textures;

        /*! writes chunk to stream

        \param stream destination stream
        \param settings the settings how to write the chunk
        */
        virtual bool write(ofstream& stream, const ompfSettings& settings);

        /*! reads chunk from stream

        \param stream source stream
        \param settings the settings how to read the chunk
        */
        virtual bool read(ifstream& stream, ompfSettings& settings);

	};

}

#endif