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
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef IGOR_MESH_RENDER_COMPONENT_H
#define IGOR_MESH_RENDER_COMPONENT_H

#include <igor/entities/iEntity.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/resources/material/iMaterial.h>

namespace igor
{
    /*! mesh render component
     */
    class iMeshRenderComponent : public iEntityComponent
    {
        friend class iPrefabIO;

    public:

        /*! default ctor
        */
        iMeshRenderComponent();

        /*! ctor
         */
        iMeshRenderComponent(iMeshPtr mesh, iMaterialPtr material = nullptr);

        /*! \returns mesh for render
         */
        iMeshPtr getMesh() const;

        /*! sets mesh

        \param mesh the mesh to set
        */
        void setMesh(iMeshPtr mesh);

        /*! \returns material
         */
        iMaterialPtr getMaterial() const;

        /*! sets material

        \param material the material to set
        */
        void setMaterial(iMaterialPtr material);

    private:
        /*! the mesh to render
         */
        iMeshPtr _mesh;

        /*! the material to render with
         */
        iMaterialPtr _material;

        /*! callback for loading component

        \param entity the entity this component relates to
        \param[out] asyncLoad if true try again if unsuccessful
        \returns true when loading was successful
        */
        bool onLoad(iEntityPtr entity, bool &asyncLoad) override;

        /*! callback for unload component
         */
        void onUnLoad(iEntityPtr entity) override;

        /*! \returns a copy of this component
         */
        iEntityComponentPtr getCopy() override;
    };

    /*! mesh render component pointer definition
    */
    typedef iMeshRenderComponent* iMeshRenderComponentPtr;
}

#endif // IGOR_MESH_RENDER_COMPONENT_H