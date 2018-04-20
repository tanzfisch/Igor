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
#include <iaIDGenerator.h>
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
        friend class iMaterialResourceFactory;

    public:

        static constexpr const char* UNIFORM_SOLIDCOLOR = "igor_solidColor";

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

        static constexpr const char* SAMPLER_TEXTURE = "igor_matTexture";

        /*! invalid material ID
        */
        static const int64 INVALID_MATERIAL_ID = IGOR_INVALID_ID;

        /*! default render order value
        */
        static const int32 RENDER_ORDER_DEFAULT = 200;

        /*! maximum texture units supported in shaders
        */
        static const int32 MAX_TEXTURE_UNITS = 8;

        /*! min render order value
        */
        static const int32 RENDER_ORDER_MIN = 0;

        /*! max render order value
        */
        static const int32 RENDER_ORDER_MAX = 400;

        /*! \returns name of material
        */
        iaString getName();

        /*! \returns material ID
        */
        uint64 getID() const;

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
        int32 getOrder() const;

        /*! set render order

        \param order the higher the value the later it get's rendered (default is iMaterial::RENDER_ORDER_DEFAULT)
        */
        void setOrder(int32 order = iMaterial::RENDER_ORDER_DEFAULT);

        /*! \returns true if material is valid to use
        */
        bool isValid() const;

	private:

        /*! material id generator
        */
        static iaIDGenerator64 _idGenerator;

        /*! material id
        */
        uint64 _id = iMaterial::INVALID_MATERIAL_ID;

        /*! if false the material was already destroyed by the iMaterialResourceFactory
        */
        bool _isValid = false;

        /*! if true shader has directional light
        */
        bool _hasDirectionalLight = false;

        /*! id of shader property light orientation
        */
        int32 _lightOrientation;

        /*! id of shader property light ambient
        */
        int32 _lightAmbient;

        /*! id of shader property light diffuse
        */
        int32 _lightDiffuse;

        /*! id of shader property light specular
        */
        int32 _lightSpecular;

        /*! if true shader has eye position
        */
        bool _hasEyePosition = false;

        /*! id of shader property eye position
        */
        int32 _eyePosition;
        
        /*! if true shader contains model view projection matrix
        */
        bool _hasModelViewProjectionMatrix = false;

        /*! id of shader property model view projection matrix
        */
        int32 _mvp_matrix;

        /*! if true shader contains model matrix
        */
        bool _hasModelMatrix = false;

        /*! id of shader property model matrix
        */
        int32 _model_matrix;

        /*! if true shader contains target material
        */
        bool _hasTargetMaterial = false;

        /*! id of shader property material ambient
        */
        int32 _matAmbient;

        /*! id of shader property material diffuse
        */
        int32 _matDiffuse;

        /*! id of shader property material specular
        */
        int32 _matSpecular;

        /*! id of shader property material shininess
        */
        int32 _matShininess;

        /*! id of shader property material emissive
        */
        int32 _matEmissive;

        /*! if true shader contains solid color property
        */
        bool _hasSolidColor = false;

        /*! id of shader property solid color
        */
        int32 _matSolidColor;

        /*! list of avail able texture units in shader
        */
        bool _hasTexture[MAX_TEXTURE_UNITS];

        /*! ids of shader properties regarding texture units
        */
        int32 _matTexture[MAX_TEXTURE_UNITS];

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

        /*! so the shared_ptr is able to call the private destructor indirectly

        Thanks to http://nealabq.com/blog/2008/11/28/factory/
        */
        __IGOR_DISABLE_WARNING__(4091);
        static struct private_deleter
        {
            void operator()(iMaterial* p)
            {
                delete p;
            }
        };
        __IGOR_ENABLE_WARNING__(4091);

	};

    /*! definition of material pointer
    */
    typedef shared_ptr<iMaterial> iMaterialPtr;

}

#endif


