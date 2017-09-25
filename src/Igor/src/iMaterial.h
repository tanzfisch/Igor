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

    /*! shader source and type definition
    */
    struct iShaderSource
    {
        /*! source file name
        */
        iaString _filename;

        /*! shader type
        */
        iShaderObjectType _type;
    };

    /*! defines visual material

    \todo need a unique material ID. maybe a hash code
    */
    class Igor_API iMaterial
	{

        friend class iRenderer;
        friend class iMaterialGroup;
        friend class iMaterialResourceFactory;

    public:

        static constexpr const char* UNIFORM_LIGHT_ORIENTATION = "igor_lightOrientation";
        static constexpr const char* UNIFORM_LIGHT_AMBIENT = "igor_lightAmbient";
        static constexpr const char* UNIFORM_LIGHT_DIFFUSE = "igor_lightDiffuse";
        static constexpr const char* UNIFORM_LIGHT_SPECULAR = "igor_lightSpecular";
        static constexpr const char* UNIFORM_EYE_POSITION = "igor_eyePosition";
        static constexpr const char* UNIFORM_MODEL_VIEW_PROJECTION = "igor_modelViewProjection";
        static constexpr const char* UNIFORM_MODEL = "igor_model";

        static constexpr const char* UNIFORM_MATERIAL_AMBIENT = "igor_matAmbient";
        static constexpr const char* UNIFORM_MATERIAL_DIFFUSE = "igor_matDiffuse";
        static constexpr const char* UNIFORM_MATERIAL_SPECULAR = "igor_matSpecular";
        static constexpr const char* UNIFORM_MATERIAL_SHININESS = "igor_matShininess";
        static constexpr const char* UNIFORM_MATERIAL_EMISSIVE = "igor_matEmissive";

        static constexpr const char* SAMPLER_TEXTURE0 = "igor_matTexture0";
        static constexpr const char* SAMPLER_TEXTURE1 = "igor_matTexture1";
        static constexpr const char* SAMPLER_TEXTURE2 = "igor_matTexture2";
        static constexpr const char* SAMPLER_TEXTURE3 = "igor_matTexture3";

        /*! invalid material ID
        */
        static const int64 INVALID_MATERIAL_ID = 0;

        /*! default render order value
        */
        static const int32 RENDER_ORDER_DEFAULT = 200;

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
        vector<iShaderSource> getShaderSources() const;

        /*! compiles the shader including all added sources
        */
        void compileShader();

        /*! destroyes shaders and removes all information about them
        */
        void clearShader();

        /*! \returns render order
        */
        int32 getOrder();

        /*! set render order

        \param order the higher the value the later it get's rendered (default is iMaterial::RENDER_ORDER_DEFAULT)
        */
        void setOrder(int32 order = iMaterial::RENDER_ORDER_DEFAULT);

	private:

        bool _hasDirectionalLight = false;

        bool _hasEyePosition = false;

        bool _hasModelViewProjectionMatrix = false;

        bool _hasModelMatrix = false;

        bool _hasTargetMaterial = false;

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

        /*! shader sources
        */
        vector<iShaderSource> _shaderSources;

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

        /*! \returns shader in use
        */
        iShader* getShader();

	};

}

#endif


