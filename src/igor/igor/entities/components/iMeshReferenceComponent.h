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
// along with this program.If not, see <http://www.gnu.org/licenses/>.
//
// contact: igorgameengine@protonmail.com

#ifndef IGOR_MESH_REFERENCE_COMPONENT_H
#define IGOR_MESH_REFERENCE_COMPONENT_H

#include <igor/entities/iEntity.h>
#include <igor/resources/model/iModel.h>

namespace igor
{
    /*! mesh reference component
     */
    class iMeshReferenceComponent : public iEntityComponent
    {
        friend class iPrefabIO;

    public:
        /*! default ctor
         */
        iMeshReferenceComponent();

        /*! ctor

        \param model the model to refer to
        \param meshPaths the paths to the meshes inside the model to refer to (ie "foo:bar:myMesh")
         */
        iMeshReferenceComponent(iModelPtr model, const std::vector<iaString> &meshPaths = std::vector<iaString>());

        /*! creates instance of this component type
        */
        static iEntityComponent* createInstance();

        /*! \returns model
         */
        iModelPtr getModel() const;

        /*! set model

        \param model the model to set
        */
        void setModel(iModelPtr model);

        /*! sets paths to meshes in model

        \param meshPaths the given mesh paths
        */
        void setMeshPaths(const std::vector<iaString> &meshPaths);

        /*! \returns mesh path
         */
        const std::vector<iaString> &getMeshPaths() const;

    private:
        /*! reference to model
         */
        iModelPtr _model;

        /*! the mesh paths to all meshes we want to reference. when empty we take all meshes
         */
        std::vector<iaString> _meshPaths;

        /*! \returns a copy of this component
         */
        iEntityComponentPtr getCopy() override;
    };

    /*! mesh reference pointer definition
     */
    typedef iMeshReferenceComponent *iMeshReferenceComponentPtr;
}

#endif // IGOR_MESH_REFERENCE_COMPONENT_H