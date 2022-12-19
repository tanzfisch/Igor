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

#ifndef __IGOR_MATERIAL__
#define __IGOR_MATERIAL__

#include <igor/renderer/iRenderStateSet.h>
#include <igor/renderer/shaders/iShaderProgram.h>

#include <iaux/data/iaUUID.h>
#include <iaux/data/iaColor3.h>
#include <iaux/data/iaColor4.h>

#include <memory>

namespace igor
{

    /*! material pointer definition
     */
    class iMaterial;
    typedef std::shared_ptr<iMaterial> iMaterialPtr;

    /*! material ID definition
     */
    typedef iaUUID iMaterialID;

    /*! material definition
     */
    class IGOR_API iMaterial
    {
        friend class iRenderer;
        friend class iMaterialDeleter;

    public:
        /*! default render order value
         */
        static const int32 RENDER_ORDER_DEFAULT = 200;

        /*! min render order value
         */
        static const int32 RENDER_ORDER_MIN = 0;

        /*! max render order value
         */
        static const int32 RENDER_ORDER_MAX = 400;

        /*! \returns a newly created material
         */
        static iMaterialPtr create();

        /*! \returns a newly created material from file
         */
        static iMaterialPtr create(const iaString &filename);

        /*! sets shader program to be used

        \param shaderProgram the shader program to be used
        */
        void setShaderProgram(const iShaderProgramPtr &shaderProgram);

        /*! \returns the shader program
         */
        iShaderProgramPtr getShaderProgram() const;

        /*! sets name of material

        \param name name to be set
        */
        void setName(const iaString &name);

        /*! \returns name of material
         */
        const iaString &getName() const;

        /*! \returns material id
         */
        const iMaterialID &getID() const;

        /*! sets material id

        \param materialID the material id
        */
        void setID(const iMaterialID &materialID);

        /*! defines the value of a specific render state

        \param state render state
        \param value render state value
        */
        void setRenderState(const iRenderState state, const iRenderStateValue value);

        /*! returns the value of a specific render state

        \param state render state
        \return render state value
        */
        iRenderStateValue getRenderState(const iRenderState state) const;

        /*! \returns render order
         */
        int32 getOrder() const;

        /*! set render order

        \param order the higher the value the later it get's rendered (default is iMaterial_old::RENDER_ORDER_DEFAULT)
        */
        void setOrder(int32 order = iMaterial::RENDER_ORDER_DEFAULT);

        /*! \returns true if shader has directional light
         */
        bool hasDirectionalLight() const;

        /*! \returns true if shader has eye position
         */
        bool hasEyePosition() const;

        /*! \returns true if shader contains model view projection matrix
         */
        bool hasModelViewProjectionMatrix() const;

        /*! \returns true if shader contains model matrix
         */
        bool hasModelMatrix() const;

        /*! \returns true if shader contains model view matrix
         */
        bool hasModelViewMatrix() const;

        /*! \returns true if shader contains view projection matrix
         */
        bool hasViewProjectionMatrix() const;

        /*! \returns true if shader contains target material
         */
        bool hasTargetMaterial() const;

        /*! \returns true if shader contains solid color property
         */
        bool hasSolidColor() const;

        /*! \returns true if gien texture unit is used

        \param texUnit the given texture unit
        */
        bool hasTextureUnit(uint32 texUnit) const;

        /*! \returns true if this material is ready for use
         */
        bool isValid() const;

    private:
        /*! name of the material.
         */
        iaString _name = L"iMaterial";

        /*! the shader program
         */
        iShaderProgramPtr _shaderProgram;

        /*! render states set
         */
        iRenderStateSet _renderStateSet;

        /*! material id
         */
        iMaterialID _materialID;

        /*! oder that material groups get sorted by

        default value is iMaterial_old::RENDER_ORDER_DEFAULT
        */
        int32 _order = iMaterial::RENDER_ORDER_DEFAULT;

        /*! if true shader has directional light
         */
        bool _hasDirectionalLight = false;

        /*! if true shader has eye position
         */
        bool _hasEyePosition = false;

        /*! if true shader contains model view projection matrix
         */
        bool _hasModelViewProjectionMatrix = false;

        /*! if true shader contains model matrix
         */
        bool _hasModelMatrix = false;

        /*! if true shader contains model view matrix definition
         */
        bool _hasModelViewMatrix = false;

        /*! if true shader contains view projection matrix definition
         */
        bool _hasViewProjectionMatrix = false;

        /*! if true shader contains target material
         */
        bool _hasTargetMaterial = false;

        /*! if true shader contains solid color property
         */
        bool _hasSolidColor = false;

        /*! list of avail able texture units in shader
         */
        bool _hasTexture[MAX_TEXTURE_UNITS];

        /*! does nothing
         */
        iMaterial() = default;

        /*! does nothing
         */
        ~iMaterial();

        /*! bind this material
         */
        void bind();

        /*! unbind material
         */
        void unbind();

        /*! sets integer on given uniform

        \param uniform the uniform name
        \param value the value to set
        */
        void setInt(const iaString &uniform, int value);

        /*! sets float on given uniform

        \param uniform the uniform name
        \param value the value to set
        */
        void setFloat(const iaString &uniform, float32 value);

        /*! sets 2 dimensional vector on given uniform

        \param uniform the uniform name
        \param value the value to set
        */
        void setFloat2(const iaString &uniform, const iaVector2f &value);

        /*! sets 3 dimensional vector on given uniform

        \param uniform the uniform name
        \param value the value to set
        */
        void setFloat3(const iaString &uniform, const iaVector3f &value);

        /*! sets 4 dimensional vector on given uniform

        \param uniform the uniform name
        \param value the value to set
        */
        void setFloat4(const iaString &uniform, const iaVector4f &value);

        /*! sets 3 dimensional color vector on given uniform

        \param uniform the uniform name
        \param value the value to set
        */
        void setFloat3(const iaString &uniform, const iaColor3f &value);

        /*! sets 4 dimensional color vector on given uniform

        \param uniform the uniform name
        \param value the value to set
        */
        void setFloat4(const iaString &uniform, const iaColor4f &value);

        /*! sets 4x4 matrix on given uniform

        \param uniform the uniform name
        \param value the value to set
        */
        void setMatrix(const iaString &uniform, const iaMatrixf &value);
    };
}

#endif // __IGOR_MATERIAL__