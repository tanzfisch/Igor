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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef IGOR_MATERIAL_H
#define IGOR_MATERIAL_H

#include <igor/resources/texture/iTexture.h>
#include <igor/resources/shader_material/iShaderMaterial.h>

#include <iaux/data/iaColor3.h>
using namespace iaux;

#include <map>
#include <memory>

namespace igor
{
    /*! target material pointer definition
     */
    class iMaterial;
    typedef std::shared_ptr<iMaterial> iMaterialPtr;

    /*! material ID definition
     */
    typedef iaUUID iShaderMaterialID;

    /*! contains all material information that belong to the surface of an object

    Whereas iShaderMaterial contains all the information related to shader and environment ie lights etc.
    */
    class IGOR_API iMaterial : public iResource
    {

        friend class iMaterialFactory;

    public:
        /*! set a texture for a given texture unit

        \param texture the texture to add
        \param texunit the texture unit
        */
        void setTexture(iTexturePtr texture, uint32 texunit);

        /*! \returns texture for given texture unit

        \param texunit the given texture unit
        */
        iTexturePtr getTexture(uint32 texunit) const;

        /*! \returns true if target material has texture for given texture unit

        \param texunit the given texture unit
        */
        bool hasTextureUnit(uint32 texunit) const;

        /*! \returns all the textures in the target material
         */
        const std::map<uint32, iTexturePtr> &getTextures() const;

        /*! sets the tiling configuration that can be use in a shader

        \param tiling column and row count of tiles
        */
        void setTiling(const iaVector2f &tiling);

        /*! \returns tiling config
         */
        const iaVector2f &getTiling() const;

        /*! \returns true if mesh has textures and texture coordinates
         */
        bool hasTextures() const;

        /*! set emissive color

        \param e emissive color
        */
        void setEmissive(const iaColor3f &e);

        /*! \returns emissive color
         */
        iaColor3f getEmissive() const;

        /*! set ambient color

        \param a ambient color
        */
        void setAmbient(const iaColor3f &a);

        /*! \returns ambient color
         */
        iaColor3f getAmbient() const;

        /*! set specular color

        \param s specular color
        */
        void setSpecular(const iaColor3f &s);

        /*! \returns specular color
         */
        iaColor3f getSpecular() const;

        /*! set diffuse color

        \param d diffuse color
        */
        void setDiffuse(const iaColor3f &d);

        /*! \returns diffuse color
         */
        iaColor3f getDiffuse() const;

        /*! set shininess

        \param shininess the shininess value
        */
        void setShininess(float32 shininess);

        /*! \returns shininess
         */
        float32 getShininess() const;

        /*! sets alpha value of material
         */
        void setAlpha(float32 alpha);

        /*! \returns alpha value of material
         */
        float32 getAlpha() const;

        /*! sets velocity orientation

        \param enable if true orientation will be along velocity axis
        */
        void setVelocityOriented(bool enable);

        /*! \returns true if orientation will be along velocity axis
         */
        bool isVelocityOriented() const;

        /*! sets shader material

        \param shaderMaterial the shader material
        */
        void setShaderMaterial(const iShaderMaterialPtr &shaderMaterial);

        /*! \returns the shader material in use
        */
        iShaderMaterialPtr getShaderMaterial() const;

    private:

        /*! the shader material
        */
        iShaderMaterialPtr _shaderMaterial;

        /*! texture unit to texture map
         */
        std::map<uint32, iTexturePtr> _textures;

        /*! material emissive value
         */
        iaColor3f _emissive;

        /*! material ambient value
         */
        iaColor3f _ambient;

        /*! material specular value
         */
        iaColor3f _specular;

        /*! material diffuse value
         */
        iaColor3f _diffuse;

        /*! material shininess value
         */
        float32 _shininess;

        /*! alpha value
         */
        float32 _alpha;

        /*! tiling config
         */
        iaVector2f _tiling;

        /*! if true target material will be displayed oriented along it's velocity axis
         */
        bool _velocityOriented; // TODO does this belong here?

        /*! init material

        \param parameters parameters specifying the material
         */
        iMaterial(const iParameters &parameters);
    };

} // namespace igor

#endif // IGOR_MATERIAL_H