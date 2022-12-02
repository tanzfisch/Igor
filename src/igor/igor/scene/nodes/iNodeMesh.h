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

#ifndef __IGOR_MESHNODE_H__
#define __IGOR_MESHNODE_H__

#include <igor/scene/nodes/iNodeVolume.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/resources/mesh/iMeshBuffers.h>

#include <iaux/data/iaColor3.h>
using namespace iaux;

namespace igor
{

    class iTargetMaterial;

    /*! represents a mesh within the scene

    \todo remove as many friends as possible
    */
    class IGOR_API iNodeMesh : public iNodeVolume
    {

        friend class iNodeManager;
        friend class iNodeModel;
        friend class iModelDataIOOMPF;

    public:

        /*! \returns shared pointer to mesh
        */
        iMeshPtr getMesh();

        /*! sets mesh for mesh node

        \param mesh shared pointer to mesh
        */
        void setMesh(iMeshPtr mesh);

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

        /*! \returns target material
        */
        iTargetMaterial *getTargetMaterial();

        /*! configures target material

        \param targetMaterial the target material configuration to copy
        */
        void setTargetMaterial(const iTargetMaterial *const targetMaterial);

        /*! sets the keep mesh flag

        if the keep mesh flag is true. meshs will not be released after creation of buffers for rendering
        this is needed if the mesh is supposed to be manipulated or exported later

        \param keepMesh keep mesh flag. if true mesh will not be released after buffer creation
        */
        void setKeepMeshData(bool keepMesh = true);

        /*! \retruns keep mesh flag. if true mesh will not be released after buffer creation
        */
        bool getKeepMeshData() const;

        /*! returns multiple lines of information about this node

		\param[out] info the returned information
		*/
        void getInfo(std::vector<iaString> &info) const override;

    private:
        /*! flag where to keep mesh after creating buffers or not
        */
        bool _keepMeshData = false;

        /*! target material
        */
        iTargetMaterial *_targetMaterial = nullptr;

        /*! shared pointer to mesh
        */
        iMeshPtr _mesh;

        /*! draw mesh
        */
        virtual void draw();

        /*! initializes member variables
        */
        iNodeMesh();

        /*! copy ctor
        */
        iNodeMesh(iNodeMesh *node);

        /*! release target material
        */
        virtual ~iNodeMesh();
    };

    /*! mesh node pointer definition
    */
    typedef iNodeMesh *iNodeMeshPtr;

} // namespace igor

#endif // __IGOR_MESHNODE_H__
