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
// (c) Copyright 2014-2016 by Martin Loga
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
#include <iMaterial.h>
#include <iaMatrix.h>
#include <iInstancer.h>

#include <map>
#include <memory>
using namespace std;

namespace Igor
{

    class iNodeRender;
    class iMeshBuffers;
    class iNodeMesh;

    /*! material group describes a group of render nodes that user the same material
    */
    class Igor_API iMaterialGroup
	{

		friend class iMaterialResourceFactory;
        friend class iRenderEngine;
        friend class iNodeRender;

        /*! structure for handling instanced rendered nodes
        */
        struct Instanced
        {
            vector<iNodeMesh*> _nodes;
            iInstancer* _instancer = nullptr;
        };

    public:

        /*! \returns pointer to internal material definition
        */
        iMaterial* getMaterial();

        /*! \returns render order
        */
        int32 getOrder();

        /*! set render order

        \param order the higher the value the later it get's rendered (default is iMaterial::DEFALT_ORDER)
        */
        void setOrder(int32 order = iMaterial::RENDER_ORDER_DEFAULT);

        /*! \returns material group id
        */
        uint32 getID();

	private:

        /*! material group or material id
        */
		uint32 _id = iMaterial::INVALID_MATERIAL_ID;

        /*! next material group id
        */
		static uint32 _nextId;

        /*! the actuall material
        */
		iMaterial _material;

        /*! render nodes registred to this material
        */
        vector<iNodeRender*> _renderNodes;

        /*! render nodes registred to this material that are also using instancing
        */
        map<shared_ptr<iMeshBuffers>, Instanced> _instancedRenderNodes;

        /*! adds node to material group

        \param renderNode node to be added

        \todo when does a mesh get removed from this group? reference count?
        */
        void addRenderNode(iNodeRender* renderNode);

		/*! removes render node from material group
		
		\param renderNode node to be removed
		*/
        void removeRenderNode(iNodeRender* renderNode);

        /*! init id
        */
        iMaterialGroup();
        
        /*! clean up
        */
        virtual ~iMaterialGroup();

	};
}

#endif

