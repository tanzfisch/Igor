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

#ifndef __iMESHNODE__
#define __iMESHNODE__

#include <iNodeVolume.h>
#include <iaColor3.h>
using namespace IgorAux;

namespace Igor
{

    class iMesh;
    class iMeshBuffers;
    class iTargetMaterial;

	__IGOR_DISABLE_WARNING__(4275) // the interface of iNodeVolume is not fully public on purpose

    /*! represents a mesh within the scene

    \todo remove as many friends as possible
    */
	class Igor_API iNodeMesh : public iNodeVolume
	{

        friend class iNodeFactory;
		friend class iNodeModel;
        friend class iMaterialGroup;
        friend class iModelDataIOOMPF;

    public:

        /*! \returns shared pointer to mesh buffers
        */
        shared_ptr<iMeshBuffers> getMeshBuffers();

        /*! \returns shared pointer to mesh
        */
        shared_ptr<iMesh> getMesh();

        /*! sets mesh for mesh node

        \todo why is this public?

        \param mesh shared pointer to mesh
        */
        void setMesh(shared_ptr<iMesh> mesh);

        /*! \returns emissive color
        */
        iaColor3f getEmissive() const;

        /*! \returns ambient color
        */
        iaColor3f getAmbient() const;

        /*! \returns specular color
        */
        iaColor3f getSpecular() const;

        /*! \returns diffuse color
        */
        iaColor3f getDiffuse() const;

        /*! \returns shininess
        */
        float32 getShininess() const;

        /*! \returns vertexes count
        */
        uint32 getVertexCount();

        /*! \returns triangles count
        */
        uint32 getTrianglesCount();

        /*! \returns indexes count
        */
        uint32 getIndexesCount();

        /*! \returns target material
        */
        iTargetMaterial* getTargetMaterial();

        /*! configures target material

        \param targetMaterial the target material configuration to copy
        */
        void setTargetMaterial(const iTargetMaterial* const targetMaterial);

        /*! sets the keep mesh flag

        if the keep mesh flag is true. meshs will not be released after creation of buffers for rendering
        this is needed if the mesh is supposed to be manipulated or exported later

        \param keepMesh keep mesh flag. if true mesh will not be released after buffer creation
        */
        void setKeepMesh(bool keepMesh = true);

        /*! \retruns keep mesh flag. if true mesh will not be released after buffer creation
        */
        bool getKeepMesh() const;

	private:

        /*! flag where to keep mesh after creating buffers or not
        */
        bool _keepMesh = false;

        /*! target material
        */
        iTargetMaterial* _targetMaterial = nullptr;

        /*! shared pointer to mesh buffers
        */
        shared_ptr<iMeshBuffers> _meshBuffers = nullptr;

        /*! shared pointer to mesh
        */
        shared_ptr<iMesh> _mesh = nullptr;

        /*! sets mesh buffers for mesh node

        \param meshBuffers shared pointer to mesh buffers
        */
        void setMeshBuffers(shared_ptr<iMeshBuffers> meshBuffers);

        /*! draw mesh
        */
        virtual void draw();

        /*! initializes member variables
        */
        iNodeMesh();

        /*! copy ctor
        */
        iNodeMesh(iNodeMesh* node);

        /*! release target material
        */
        virtual ~iNodeMesh();

	};

	__IGOR_ENABLE_WARNING__(4275)

}

#endif
