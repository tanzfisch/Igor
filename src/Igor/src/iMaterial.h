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
// (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __iMATERIAL__
#define __iMATERIAL__

#include <iaString.h>
using namespace IgorAux;

#include <iRenderStateSet.h>
#include <iShader.h>

#include <vector>
#include <memory>
using namespace std;

namespace Igor
{

	class iNodeLight;
	class iNodeCamera;

    /*! defines visual material

    \todo need a unique material ID. maybe a hash code
    */
    class Igor_API iMaterial
	{

        friend class iRenderer;
        friend class iMaterialGroup;
        friend class iMaterialResourceFactory;

    public:

        /*! invalid material ID
        */
        static const int32 INVALID_MATERIAL_ID = 0;

        /*! early render order value
        */
        static const int32 RENDER_ORDER_EARLY = 100;

        /*! default render order value
        */
        static const int32 RENDER_ORDER_DEFAULT = 200;

        /*! late render order value
        */
        static const int32 RENDER_ORDER_LATE = 300;

        /*! min render order value
        */
        static const int32 RENDER_ORDER_MIN = 0;

        /*! max render order value
        */
        static const int32 RENDER_ORDER_MAX = 400;


        /*! \returns name of material
        */
        iaString getName();

        /*! sets name of material

        \param name name to be set
        */
        void setName(iaString name);

        /*! \returns render state set of this material
        */
        iRenderStateSet& getRenderStateSet();

        /*! add shader source file

        \param filename shader code file
        \param type type of shader code to add
        */
        void addShaderSource(iaString filename, iShaderObjectType type);

        /*! \returns all shader sources that where added before
        */
        const vector<iShaderSource>& getShaderSources() const;

        /*! compiles the shader including all added sources
        */
        void compileShader();

        /*! \returns render order
        */
        int32 getOrder();

        /*! set render order

        \param order the higher the value the later it get's rendered (default is iMaterial::RENDER_ORDER_DEFAULT)
        */
        void setOrder(int32 order = iMaterial::RENDER_ORDER_DEFAULT);

	private:

        /*! oder that material groups get sorted by

        default value is iMaterial::RENDER_ORDER_DEFAULT
        */
        int32 _order = iMaterial::RENDER_ORDER_DEFAULT;

        /*! name of the material.

        must be unique! but this implementation has no future
        */
		iaString _name = L"";

        /*! pointer to shader (optional)
        */
        iShader* _shader = nullptr;

        /*! render states set
        */
        iRenderStateSet _renderStateSet;

        /*! does nothing
        */
        iMaterial();

        /*! deletes shader if one was used
        */
        ~iMaterial();

        /*! activate material
        */
        void activateShader();

        /*! deactivate material
        */
        void deactivateShader();

        /*! stes shader to be used in this material
        */
        void setShader(iShader* shader);

        /*! \returns shader in use
        */
        iShader* getShader();

	};

}

#endif

