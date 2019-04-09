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
// (c) Copyright 2012-2019 by Martin Loga
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
// auint32 with this program.If not, see <http://www.gnu.org/licenses/>.
// 
// contact: martinloga@gmx.de  

#ifndef __iLOADEROBJ__
#define __iLOADEROBJ__

#include <iaString.h>
using namespace IgorAux;

#include <iModelDataIO.h>
#include <iaVector4.h>
#include <iaVector2.h>
#include <iaColor3.h>

#include <vector>
using namespace std;

namespace Igor
{

    class iMeshBuilder;

	/*! model data loader for the OBJ aka Wavefront format

	source: http://de.wikipedia.org/wiki/Wavefront_OBJ

    \todo hopeless implementation. remove or rewrite asap
	*/
	class iModelDataIOOBJ : public iModelDataIO
	{

        /*! obj vertex
        */
		struct OBJVertex
		{
            /*! vertex index
            */
			int32 _v;

            /*! vertex normal index
            */
			int32 _vn;

            /*! vertex texture index
            */
			int32 _t;
		};

        /*! obj polygon
        */
		struct OBJPolygon
		{
            /*! material number
            */
			uint32 _materialNum;

            /*! list of vertices of this polygon
            */
			vector<OBJVertex> _vertexes;
		};

        /*! obj group of polygons (aka mesh)
        */
		struct OBJGroup
		{
            /*! name of group
            */
			iaString _name;

            /*! list of polygons in this group
            */
			vector<uint32> _polygons;
		};

        /*! obj materil
        */
		struct OBJMaterial
		{
            /*! name of material
            */
			iaString _name;

            /*! target ambient
            */
			iaColor3f _ambient;

            /*! target diffuse
            */
            iaColor3f _diffuse;

            /*! target specular
            */
            iaColor3f _specular;

            /*! target shininess
            */
            float32 _shininess;

            /*! texture
            */
			iaString _texture;
		};

    public:

        /*! loades the data from filesystem and returns the result

        \param filename filename of file to load
        \return pointer to node tree
        */
        virtual iNodePtr importData(const iaString& filename, iModelDataInputParameter* parameter);

        /*! creates an instance of this class

        \returns new instance
        */
        static iModelDataIO* createInstance();

        /*!  initializes members
        */
        iModelDataIOOBJ();

        /*! does nothing
        */
        ~iModelDataIOOBJ() = default;

	private:

        /*! list of vertexes
        */
		vector<iaVector3f> _vertexes;

        /*! list of normals
        */
		vector<iaVector3f> _normals;

        /*! list of texture coordinates
        */
		vector<iaVector2f> _texcoord;

        /*! list of polygons
        */
		vector<OBJPolygon> _polygons;

        /*! list of groups
        */
		vector<OBJGroup> _groups;

        /*! list of materials
        */
		vector<OBJMaterial> _materials;

        /*! current groups
        */
		vector<uint32> _currentGroups;

        /*! current materials
        */
        int32 _currentMaterial;

        /*! source path of model
        */
        iaString _pathOfModel;

        /*! transfers data in to a mesh builder per material

        \param meshBuilders list of mesh builders (one per material)
        */
        void transferToMeshBuilder(vector<iMeshBuilder*>& meshBuilders);

        /*! analyse next attributes

        \param attributes list of strings containing attributes
        */
		bool analyseAttributes(vector<iaString> &attributes);

        /*! read vertex attributes

        \param attributes list of attributes
        */
		bool readVertex(vector<iaString> &attributes);

        /*! read normal attributes

        \param attributes list of attributes
        */
        bool readNormal(vector<iaString> &attributes);

        /*! read texture coordinate attributes

        \param attributes list of attributes
        */
        bool readTexcoord(vector<iaString> &attributes);

        /*! read group attributes

        \param attributes list of attributes
        */
        bool readGroup(vector<iaString> &attributes);

        /*! read face (aka polygon) attributes

        \param attributes list of attributes
        */
        bool readFace(vector<iaString> &attributes);

        /*! read material attributes

        \param attributes list of attributes
        */
        bool readMaterial(vector<iaString> &attributes);

        /*! read vertex attributes

        \param attributes list of attributes in one string
        */
        iModelDataIOOBJ::OBJVertex readVertex(iaString attributes);

        /*! open and read material file

        \param filename file name of material definition
        */
		bool readMaterialFile(iaString filename);

        /*! read shininess attributes

        \param attributes list of attributes
        */
        bool readShininess(vector<iaString> &attributes);

        /*! read ambient attributes

        \param attributes list of attributes
        */
        bool readAmbient(vector<iaString> &attributes);

        /*! read diffuse attributes

        \param attributes list of attributes
        */
        bool readDiffuse(vector<iaString> &attributes);

        /*! read specular attributes

        \param attributes list of attributes
        */
        bool readSpecular(vector<iaString> &attributes);

        /*! read texture attributes

        \param attributes list of attributes
        */
        bool readTexture(vector<iaString> &attributes);

        /*! open and read obj (wavefront) file

        \param filename file name to load and read
        */
        bool readFile(const iaString& filename);

        /*! read use material attributes (usemtl)

        \param attributes list of attributes
        */
        bool readUseMaterial(vector<iaString> &attributes);

        /*! \returns material count
        */
		uint32 getMaterialCount();

        /*! \returns vertex count
        */
		uint32 getVertexCount();

        /*! \returns normals count
        */
		uint32 getNormalCount();

        /*! \returns tex coordinates count
        */
		uint32 getTexCoordCount();

        /*! \returns polygon count
        */
		uint32 getPolygonCount();

        /*! \returns material by ID (aka material number)
        
        \param materialnum the material ID
        */
		iModelDataIOOBJ::OBJMaterial* getMaterial(uint32 materialnum);

        /*! \returns vertex at given index

        \param index index of vertex to return
        */
		iaVector3f* getVertex(uint32 index);

        /*! \returns normal at given index

        \param index index of normal to return
        */
        iaVector3f* getNormal(uint32 index);

        /*! \returns texture coordinate at given index

        \param index index of texture coordinate to return
        */
        iaVector2f* getTexCoord(uint32 index);

        /*! \returns polygon with given index

        \param index index of polygon to return
        */
		iModelDataIOOBJ::OBJPolygon* getPolygon(uint32 index);

	};

}

#endif
