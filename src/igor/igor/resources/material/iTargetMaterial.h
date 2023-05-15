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

#ifndef __IGOR_TARGETMATERIAL__
#define __IGOR_TARGETMATERIAL__

#include <igor/iDefines.h>
#include <igor/resources/texture/iTextureOld.h>

#include <iaux/data/iaColor3.h>
using namespace iaux;

#include <map>
#include <memory>

namespace igor
{
    /*! target material pointer definition
     */
    class iTargetMaterial;
    typedef std::shared_ptr<iTargetMaterial> iTargetMaterialPtr;

    /*! contains all material information that belong to the surface of an object

    Whereas iMaterial contains all the information realted to shader and environment ie lights etc.
    */
    class IGOR_API iTargetMaterial
    {

        friend class iTargetMaterialDeleter;

    public:
        /*! \returns a newly created target material
         */
        static iTargetMaterialPtr create();

        /*! set a texture for a given texture unit

        \param texture the texture to add
        \param texunit the texture unit
        */
        void setTexture(iTextureOldPtr texture, uint32 texunit);

        /*! \returns texture for given texture unit

        \param texunit the given texture unit
        */
        iTextureOldPtr getTexture(uint32 texunit) const;

        /*! \returns true if target material has texture for given texture unit

        \param texunit the given texture unit
        */
        bool hasTextureUnit(uint32 texunit) const;

        /*! \returns all the textures in the target material
         */
        const std::map<uint32, iTextureOldPtr> &getTextures() const;

        /*! sets the tiling configuration that can be use in a shader

        \param tiling column and row count of tiles
        */
        void setTilingConfig(const iaVector2f &tiling);

        /*! \returns tiling config
         */
        const iaVector2f &getTilingConfig() const;

        /*! \returns true if mesh has textures and texture coordinates
         */
        bool hasTextures() const;

        /*! set emmisive color

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

    private:

        /*! texture unit to texture map
         */
        std::map<uint32, iTextureOldPtr> _textures;

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
        iaVector2f _tilingConfig;

        /*! if true target material will be displayed oriented along it's velocity axis
         */
        bool _velocityOriented;

        /*! initializes members
         */
        iTargetMaterial();

        /*! releases resources
         */
        ~iTargetMaterial();
    };

} // namespace igor

#endif