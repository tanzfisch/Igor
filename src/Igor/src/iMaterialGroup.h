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
// along with this program.If not, see <http://www.gnu.org/licenses/>.
// 
// contact: martinloga@gmx.de  

#ifndef __iMATERIALGROUP__
#define __iMATERIALGROUP__

#include <iDefines.h>
#include <iaMatrix.h>
#include <iInstancer.h>

#include <map>
#include <vector>
#include <memory>


namespace Igor
{

    class iMeshBuffers;

    /*! structure for handling instanced rendered nodes
    */
    struct iInstancedNodes
    {
        /*! render node ids
        */
        std::vector<uint64> _renderNodeIDs;

        /*! instance of corresponding instancer
        */
        iInstancer* _instancer = nullptr;
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

        /*! adds node to material group

        \param renderNode node to be added
        \param instancing if true instancing is used by the material associated with this group
        */
        void addRenderNode(uint64 renderNodeID, bool instancing);

        /*! removes render node from material group

        \param renderNode node to be removed
        */
        void removeRenderNode(uint64 renderNodeID, bool instancing);

        /*! \returns copy of render nodes in this group
        */
        std::vector<uint64> getRenderNodes() const;

        /*! \returns copy of instanced render node meshs in this group
        */
        std::map<std::shared_ptr<iMeshBuffers>, iInstancedNodes> getInstancedRenderNodes() const;

    private:

        /*! render node IDs registred to this material
        */
        std::vector<uint64> _renderNodeIDs;

        /*! render nodes registred to this material that are also using instancing
        */
        std::map<std::shared_ptr<iMeshBuffers>, iInstancedNodes> _instancedRenderNodes;

	};

}

#endif

