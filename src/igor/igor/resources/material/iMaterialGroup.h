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

#ifndef __IGOR_MATERIALGROUP_H__
#define __IGOR_MATERIALGROUP_H__

#include <igor/iDefines.h>
#include <iaux/math/iaMatrix.h>
#include <igor/renderer/iInstancer.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/scene/nodes/iNode.h>
#include <igor/scene/nodes/iNodeRender.h>
#include <igor/resources/mesh/iMeshBuffers.h>
#include <igor/resources/material/iTargetMaterial.h>

#include <unordered_map>
#include <vector>
#include <memory>

namespace igor
{

    struct iInstancingData
    {
        iInstancer *_instancer = nullptr;
        iTargetMaterialPtr _targetMaterial = nullptr;
    };

    /*! material group describes a group of render nodes that use the same material
    */
    class iMaterialGroup
    {

    public:

        /*! does nothing
        */
        iMaterialGroup() = default;

        /*! clean up
        */
        virtual ~iMaterialGroup();

        /*! \returns true if material is instanced
        */
        bool isInstanced() const;

        /*! \returns true if group has nodes
        */
        bool hasNodes() const;

        /*! clear node list
        */
        void clear();

        /*! adds node to material group

        \param renderNode node to be added
        */
        void addRenderNode(iNodeRenderPtr renderNode);

        /*! \returns copy of render nodes in this group
        */
        const std::vector<iNodeRenderPtr> &getRenderNodes() const;

        /*! \returns copy of instanced render node meshs in this group
        */
        const std::unordered_map<iMeshBuffersPtr, iInstancingData> &getInstancedRenderNodes() const;

    private:

        /*! render node IDs registred to this material
        */
        std::vector<iNodeRenderPtr> _renderNodes;

        /*! render nodes registred to this material that are also using instancing
        */
        std::unordered_map<iMeshBuffersPtr, iInstancingData> _instancedRenderNodes;
    };

} // namespace igor

#endif // __IGOR_MATERIALGROUP_H__
