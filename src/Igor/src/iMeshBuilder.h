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

#ifndef __iMESHBUILDER__
#define __iMESHBUILDER__

#include <iaConsole.h>
using namespace IgorAux;

#include <iaVector2.h>
#include <iSphere.h>
#include <iAABox.h>
#include <iaVector4.h>
#include <iaColor3.h>
#include <iaColor4.h>
#include <iTexture.h>

#include <map>
#include <memory>
//using namespace stdext;

#include <vector>
#include <unordered_map>
using namespace std;

namespace Igor
{

    class iMesh;

    /*! triangle defined by three indexes to vertexes
    */
    struct Igor_API iIndexedTriangle
    {
        /*! index a
        */
        uint32 _a;

        /*! index b
        */
        uint32 _b;

        /*! index c
        */
        uint32 _c;
    };

    /*! builds up a mesh and spits out iMesh

    \todo how about applying transformations directly on the data before generating buffers from ìt?
    */
    class Igor_API iMeshBuilder
    {

        /*! hash function to quickly find corresponding vertex positions
        */
        class VectorHasher
        {
        public:
            size_t operator() (iaVector3f const& key) const
            {
                return ((*(uint64*)&key._x)<<1) ^ ((*(uint64*)&key._y)<<2) ^ ((*(uint64*)&key._y) << 3);
            }
        };

        class VectorEqualFn
        {
        public:
            bool operator() (iaVector3f const& t1, iaVector3f const& t2) const
            {
                return t1 == t2;
            };
        };

    public:

        /*! does nothing
        */
        iMeshBuilder();

        /*! does nothing
        */
        ~iMeshBuilder();

        /*! calculating the bounding sphere

        \todo implement bouncing bubble algorithm
        */
        void calcBoundingSphere(iSphered& sphere) const;

        /*! calculating the bounding box
        */
        void calcBoundingBox(iAABoxd& bbox) const;

        /*! adds vertex to data

        \param vertex the vertex to add
        \returns index of vertex added
        */
        uint32 addVertex(const iaVector3f& vertex);

        /*! adds vertex to data

        ignores the 4th component

        \param vertex the vertex to add
        \returns index of vertex added
        */
        uint32 addVertex(const iaVector4f& vertex);

        /*! snappes vertex position to a raster before adding to the mesh

        \param vertex vertex position to add
        */
        uint32 addVertexSnapped(const iaVector3f& vertex);

        /*! sets normal of last added vertex

        \param normal the normal to add
        */
        void setNormal(uint32 index, const iaVector3f& normal);

        /*! accumulates normal values for specified index

        \param index the index of vertex to manipulate
        \param normal the normal vector to accumulate
        */
        void accumulateNormal(uint32 index, const iaVector3f& normal);

        /*! sets color of last added vertex

        \param color the color to add
        */
        void setColor(uint32 index, const iaColor4f& color);

        /*! sets texture coordinate of last added vertex

        \param texCoord the texture coordinate to add
        \param unit the texture unit to add the texture coodinate to
        */
        void setTexCoord(uint32 index, const iaVector2f& texCoord, uint32 unit);

        /*! adds indexes of a triangle to the data

        \param indexA first point of triangle
        \param indexB second point of triangle
        \param indexC third point of triangle
        \returns index of triangle created
        */
        uint32 addTriangle(const uint32 indexA, const uint32 indexB, const uint32 indexC);

        /*! set's if added vertexes will be joined when heaving same position

        can only be set before the first vertex was added

        \param joinVertexes on off flag 
        */
        void setJoinVertexes(bool joinVertexes = true);

        /*! normalizes all normals
        */
        void normalizeNormals();

        /*! \returns join vertexes flag
        */
        bool getJoinVertexes();

        /*! sets the snap grid size

        \param gridSize the snap grid size
        */
        void setGridSize(float32 gridSize);

        /*! \returns snap grid size
        */
        float32 getGridSize();

        /*! \returns vertex count
        */
        uint32 getVertexCount() const;

        /*! \returns triangle count
        */
        uint32 getTrianglesCount() const;

        /*! \returns texture unit count
        */
        uint32 getTextureUnitCount() const;

        /*! \returns list of vertex positions
        */
        const vector<iaVector3f>& getVertexes() const;

        /*! \returns list of vertex normals
        */
        const vector<iaVector3f>& getNormals() const;

        /*! \returns list of indexed triangles
        */
        const vector<iIndexedTriangle>& getTriangles() const;

        /*! \returns true: if mesh has _normals; false: mesh has no _normals
        */
        bool hasNormals() const;

        /*! \returns true if data contains colors
        */
        bool hasColors() const;

        /*! calculates normals for mesh

        \param sharpEdges if true sharp edges will cause plit of neighbor triangles for more realistic representation
        */
        void calcNormals(bool sharpEdges = false);

        /*! creates ans returns a mesh based on current data

        \returns shared pointer to iMesh
        */
        shared_ptr<iMesh> createMesh();

        /*! creates ans returns a mesh based on current data but filtered by triangle list

        \param triangles list of triangle IDs that end up in the mesh
        \returns shared pointer to iMesh
        */
        shared_ptr<iMesh> createMesh(vector<uint32> triangles);

        /*! clears data
        */
        void clear();

        /*! copies the triangles in the index list to an other iMeshBuilder

        \param srcTriangleIndexes list of triangle indexes to copy
        */
        void copyTriangles(vector<uint32> srcTriangleIndexes, iMeshBuilder& dst);

        /*! calculates spherical texture coordinates based on vertex normal orientation

        \param texunit texture unit to put the result in
        \param scale scaling the texture coordinates
        */
        void calcSphericalTexcoordsByNormals(int texunit, float32 scale = 1.0f);

        /*! calculates spherical texture coordinate based on vertex position relative to center

        \param center the center the calculation bases on
        \param texunit the texture unit to put the result in
        \param scale scaling the texture coordinates
        */
        void calcSphericalTexcoordsByPosition(const iaVector3f& center, int texunit, float32 scale = 1.0f);

        /*! calculates planar texture coordinates from specified axis aligned direction

        \param center it kind of works like an offset
        \param direction the axis in which we project the texture coordinates
        \param texunit the destination texture unit
        \param scale the texture coordinate scale
        */
        void calcPlanarTextureCoordinates(const iaVector3f& center, iaAxis direction, int texunit, float32 scale = 1.0f);

    private:

        /*! map for vertex positions
        */
        unordered_map<iaVector3f, uint32, VectorHasher, VectorEqualFn> _indexMap;

        /*! if true all vertexes that have the same position will be joined
        */
        bool _joinVertexes = true;

        /*! snap grid size
        */
        float32 _gridSize = 0.0f;

        /*! the vertices of the mesh
        */
        vector<iaVector3f> _vertexes;

        /*! the normals of the mesh
        */
        vector<iaVector3f> _normals;

        /*! the colors of the mesh
        */
        vector<iaColor4f> _colors;

        /*! the texture coordinates of the mesh
        */
        map<uint32, vector<iaVector2f>> _texCoords;

        /*! the triangles of the mesh
        */
        vector<iIndexedTriangle> _triangles;

        /*! fills iMesh with data

        \param mesh the mesh to fill the data in
        */
        void compile(iMesh* mesh);

        /*! fills iMesh with data filtered by triangles list

        \param[in,out] mesh the mesh to fill the data in
        \param triangles list of triangles that are supposed to end up in the mesh
        */
        void compile(iMesh* mesh, vector<uint32> triangles);

        /*! checks boundaries of generated data
        */
        bool checkConsistency();

        /*! tries to find vertex indexes by their position

        \param vertexA location of vertex A
        \param[out] indexA index of vertex A
        \param vertexB location of vertex B
        \param[out] indexB index of vertex B
        \param vertexC location of vertex C
        \param[out] indexC index of vertex C
        */
        void getIndexOfVertexes(const iaVector3f& vertexA, int64& indexA, const iaVector3f& vertexB, int64& indexB, const iaVector3f& vertexC, int64& indexC);

    };

};

#endif

