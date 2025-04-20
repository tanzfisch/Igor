//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin A. Loga
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
// contact: igorgameengine@protonmail.com

#ifndef __IGOR_LOADEROBJ__
#define __IGOR_LOADEROBJ__

#include <igor/resources/model/loader/iModelDataIO.h>
#include <igor/resources/mesh/iMeshBuilder.h>

#include <iaux/data/iaString.h>
#include <iaux/math/iaVector4.h>
#include <iaux/math/iaVector2.h>
#include <iaux/data/iaColor3.h>
using namespace iaux;

#include <vector>
#include <map>

namespace igor
{

    /*! model data loader for the OBJ aka Wavefront format

    source: http://de.wikipedia.org/wiki/Wavefront_OBJ
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
            /*! list of vertices of this polygon
             */
            std::vector<OBJVertex> _vertexes;
        };

        /*! obj materil
         */
        struct OBJMaterial
        {
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

            OBJMaterial()
            {
                _ambient.set(0.4f, 0.4f, 0.4f);
                _diffuse.set(0.5f, 0.5f, 0.5f);
                _specular.set(0.6f, 0.6f, 0.6f);
                _shininess = 5.0f;
            }
        };

        /*! section describes part of the data framed by a combination of a group and material
         */
        struct Section
        {
            /*! name of material in use
             */
            iaString _materialName;

            /*! list of polygons in this group
             */
            std::vector<OBJPolygon> _polygons;

            /*! mesh builder
             */
            iMeshBuilder _meshBuilder;
        };

    public:
        /*! loades the data from filesystem and returns the result

        \param filename filename of file to load
        \return pointer to node tree
        */
        iNodePtr importData(const iParameters &parameters) override;

        /*! creates an instance of this class

        \returns new instance
        */
        static iModelDataIO *createInstance();

        /*!  initializes members
         */
        iModelDataIOOBJ();

        /*! does nothing
         */
        ~iModelDataIOOBJ() = default;

    private:
        /*! list of vertexes
         */
        std::vector<iaVector3f> _vertexes;

        /*! list of normals
         */
        std::vector<iaVector3f> _normals;

        /*! list of texture coordinates
         */
        std::vector<iaVector2f> _texcoord;

        /*! list of materials
         */
        std::map<iaString, iModelDataIOOBJ::OBJMaterial> _materials;

        /*! current groups
         */
        std::vector<iaString> _currentGroups;

        /*! current materials
         */
        iaString _currentMaterial;

        /*! generated mesh name id
         */
        uint32 _nextID = 1;

        /*! map of sections
         */
        std::map<iaString, iModelDataIOOBJ::Section> _sections;

        /*! list of current sections
         */
        std::vector<iaString> _currentSections;

        /*! source path of model
         */
        iaString _pathOfModel;

        /*! transfers data in to a mesh builder per section

        \param section the section to work with
        */
        void transferToMeshBuilder(iModelDataIOOBJ::Section &section);

        /*! analyse next attributes

        \param attributes list of strings containing attributes
        */
        bool analyseAttributes(std::vector<iaString> &attributes);

        /*! read vertex attributes

        \param attributes list of attributes
        */
        bool readVertex(std::vector<iaString> &attributes);

        /*! read normal attributes

        \param attributes list of attributes
        */
        bool readNormal(std::vector<iaString> &attributes);

        /*! read texture coordinate attributes

        \param attributes list of attributes
        */
        bool readTexcoord(std::vector<iaString> &attributes);

        /*! read group attributes

        \param attributes list of attributes
        */
        bool readGroup(std::vector<iaString> &attributes);

        /*! read face (aka polygon) attributes

        \param attributes list of attributes
        */
        bool readFace(std::vector<iaString> &attributes);

        /*! read material attributes

        \param attributes list of attributes
        */
        bool readMaterial(std::vector<iaString> &attributes);

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
        bool readShininess(std::vector<iaString> &attributes);

        /*! read ambient attributes

        \param attributes list of attributes
        */
        bool readAmbient(std::vector<iaString> &attributes);

        /*! read diffuse attributes

        \param attributes list of attributes
        */
        bool readDiffuse(std::vector<iaString> &attributes);

        /*! read specular attributes

        \param attributes list of attributes
        */
        bool readSpecular(std::vector<iaString> &attributes);

        /*! read texture attributes

        \param attributes list of attributes
        */
        bool readTexture(std::vector<iaString> &attributes);

        /*! open and read obj (wavefront) file

        \param filename file name to load and read
        */
        bool readFile(const iaString &filename);

        /*! read use material attributes (usemtl)

        \param attributes list of attributes
        */
        bool readUseMaterial(std::vector<iaString> &attributes);

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

        /*! \returns material by name

        \param materialName the material name
        */
        iModelDataIOOBJ::OBJMaterial *getMaterial(const iaString &materialName);

        /*! \returns vertex at given index

        \param index index of vertex to return
        */
        iaVector3f *getVertex(uint32 index);

        /*! \returns normal at given index

        \param index index of normal to return
        */
        iaVector3f *getNormal(uint32 index);

        /*! \returns texture coordinate at given index

        \param index index of texture coordinate to return
        */
        iaVector2f *getTexCoord(uint32 index);
    };

} // namespace igor

#endif
