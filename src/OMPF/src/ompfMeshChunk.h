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

    /*!

    \todo for now there are only triangles
    */
    enum class OMPF_API OMPFMeshType
    {
        Triangles = 0
    };

	class OMPF_API ompfMeshChunk : public ompfBaseChunk
	{

	public:

		ompfMeshChunk();
		virtual ~ompfMeshChunk();

        virtual uint32 getSize(const ompfSettings& settings);

        void setAmbient(const iaColor3c& ambient);
        const iaColor3c& getAmbient() const;
        void setDiffuse(const iaColor3c& diffuse);
        const iaColor3c& getDiffuse() const;
        void setSpecular(const iaColor3c& specular);
        const iaColor3c& getSpecular() const;
        void setEmissive(const iaColor3c& emissiv);
        const iaColor3c& getEmissive() const;
        void setShininess(const float32 shininess);
        float32 getShininess() const;

        void setNormalsPerVertex(uint8 count);
        uint8 getNormalsPerVertex() const;
        void setColorsPerVertex(uint8 count);
        uint8 getColorsPerVertex() const;
        void setTexCoordPerVertex(uint8 count);
        uint8 getTexCoordPerVertex() const;

        void setVertexCount(uint32 count);
        uint32 getVertexCount() const;

        void setIndexCount(uint32 count);
        uint32 getIndexCount() const;

        void setVertexData(const char* data, uint32 size);
        const char* getVertexData() const;

        void setIndexData(const char* data, uint32 size);
        const char* getIndexData() const;

        uint32 getVertexDataSize() const;
        uint32 getIndexDataSize() const;

        uint32 calcVertexSize() const;

        void setMeshType(OMPFMeshType type);
        OMPFMeshType getMeshType() const;

        void setTexture(const iaString& texture, uint32 texunit);
        iaString getTexture(uint32 texunit) const;

        uint32 getTextureCount() const;

        void setBoundingSphere(const iaVector3f& pos, float32 radius);
        void getBoundingSphere(iaVector3f& pos, float32& radius) const;

        void setMaterialChunkID(uint32 id);
        uint32 getMaterialChunkID() const;

    private:

        iaColor3c _ambient;
        iaColor3c _diffuse;
        iaColor3c _specular;
        iaColor3c _emissive;
        float32 _shininess = 0;

        uint8 _normalsPerVertex = 0;
        uint8 _colorsPerVertex = 0;
        uint8 _texCoordPerVertex = 0;
        OMPFMeshType _meshType = OMPFMeshType::Triangles;

        uint32 _vertexCount = 0;

        char* _vertexData = nullptr;
        uint32 _vertexDataSize = 0;

        uint32 _indexCount = 0;

        char* _indexData = nullptr;
        uint32 _indexDataSize = 0;

        uint32 _materialChunkID = OMPFDefaultConfiguration::InvalidChunkID;

        map<uint32, iaString> _textures;

        iaVector3f _boundingSpherePos;
        float32 _boundingSphereRadius = 0.0f;

        virtual bool write(ofstream& file, const ompfSettings& settings);
        virtual bool read(ifstream& file, ompfSettings& settings);

	};

}

#endif