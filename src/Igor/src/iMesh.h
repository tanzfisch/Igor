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
// (c) Copyright 2012-2017 by Martin Loga
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

#ifndef __iMESH__
#define __iMESH__

#include <iDefines.h>
#include <iSphere.h>

#include <iaColor3.h>
using namespace IgorAux;

#include <map>
using namespace std;

namespace Igor
{

    /*! internal mesh structure
    
    sorry it got verry opengl specific
    */
    class iMesh
    {

        friend class iRenderer;
        friend class iMeshBuilder;

    public:

        /*! initializes members
        */
        iMesh();

        /*! releases resources
        */
        ~iMesh();

        /*! set bounding sphere

        \param sphere the bounding sphere
        */
        void setBoundingSphere(const iSphered& sphere);

        /*! \returns the bounding spehre
        */
        __IGOR_INLINE__ const iSphered& getBoundingSphere() const;

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

        /*! \returns true if mesh has color deifnitions
        */
        __IGOR_INLINE__ bool hasColors() const;

        /*! \returns true if mesh has normals
        */
        __IGOR_INLINE__ bool hasNormals() const;

        /*! sets texture unit active or inactive

        \param texunit the texture unit index
        \param active flag to set the specified texture unit active or inactive
        */
        void setTexture(int texunit, bool active);

        /*! \returns true if mesh has textures and texture coordinates
        */
        __IGOR_INLINE__ bool hasTextures() const;

        /*! \returns texture unit count
        */
        __IGOR_INLINE__ uint32 getTextureUnitCount() const;

        /*! \returns true if mesh has specified texture unit
        */
        __IGOR_INLINE__ bool hasTextureUnit(uint32 unit) const;

        /*! sets index data

        \param data the data
        \param size the size in bytes of the data
        */
        void setIndexData(uint32* data, uint32 size);

        /*! deletes index buffer 
        */
        void resetIndexData();

        /*! \returns true if index data buffer is present
        */
        __IGOR_INLINE__ bool hasIndexData();

        /*! sets vertex data

        \param data the data
        \param size the size in bytes of the data
        */
        void setVertexData(float32* data, uint32 size);

        /*! deletes vertex buffer
        */
        void resetVertexData();

        /*! \returns true if vertex data buffer is present
        */
        __IGOR_INLINE__ bool hasVertexData();

        /*! sets size in byte of a vertex data set

        \param size size in bytes
        */
        void setVertexSize(uint32 size);

        /*! \returns pointer to index data
        */
        __IGOR_INLINE__ uint32* getIndexData() const;

        /*! \returns index data size in bytes
        */
        __IGOR_INLINE__ uint32 getIndexDataSize() const;

        /*! \returns pointer to vertex data
        */
        __IGOR_INLINE__ float32* getVertexData() const;

        /*! \returns vertex data size in bytes
        */
        __IGOR_INLINE__ uint32 getVertexDataSize() const;

        /*! \returns a single vertex data set size in bytes
        */
        __IGOR_INLINE__ uint32 getVertexSize() const;

        /*! sets if mesh has normals

        \param hasNormals flag to set if normals are present
         */
        void setHasNormals(bool hasNormals);

        /*! sets if mesh has colors

        \param hasColors flag to set if colors are present
        */
        void setHasColors(bool hasColors);

    private:

        /*! indexes count
        */
        uint32 _indexesCount = 0;

        /*! vertex count
        */
        uint32 _vertexCount = 0;

        /*! triangle count
        */
        uint32 _trianglesCount = 0;

        /*! if true mesh has normals
        */
        bool _hasNormals = false;

        /*! if true mesh has per vertex colors
        */
        bool _hasColors = false;

        /*! vertex size 

        depends if there are normals, colors or texture coordinates
        */
        uint32 _vertexSize = 0;

        /*! index data buffer
        */
        uint32* _indexData = nullptr;

        /*! index data buffer size
        */
        uint32 _indexDataSize = 0;

        /*! vertex data buffer
        */
        float32* _vertexData = nullptr;

        /*! vertex data buffer size
        */
        uint32 _vertexDataSize = 0;

        /*! map for texture units
        */
        map<uint32, bool> _textures;

        /*! bounding sphere
        */
        iSphered _sphere;
    };

#include <iMesh.inl>

}

#endif 