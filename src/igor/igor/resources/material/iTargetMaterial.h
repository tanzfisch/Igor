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

#ifndef __IGOR_TARGETMATERIAL__
#define __IGOR_TARGETMATERIAL__

#include <igor/iDefines.h>
#include <igor/resources/texture/iTexture.h>

#include <iaux/data/iaColor3.h>
using namespace iaux;

#include <map>
#include <memory>

namespace igor
{

    /*! contains all material information that belong to the surface of an object
    */
    class IGOR_API iTargetMaterial
    {

        friend class iMaterialResourceFactory_old;

    public:
        /*! set a texture for a specific texture unit

        \param texture the texture to bind
        \param texunit the texture unit the texture will bond to
        */
        void setTexture(iTexturePtr texture, int texunit);

        /*! \returns texture of specified texture unit

        \param texunit texture unit
        */
        iTexturePtr getTexture(int texunit) const;

        /*! \returns true if mesh has textures and texture coordinates
        */
        __IGOR_INLINE__ bool hasTextures() const;

        /*! \returns texture unit count
        */
        __IGOR_INLINE__ uint32 getTextureUnitCount() const;

        /*! \returns true if mesh has specified texture unit
        */
        __IGOR_INLINE__ bool hasTextureUnit(uint32 unit) const;

        /*! set emmisive color

        \param e emissive color
        */
        void setEmissive(const iaColor3f &e);

        /*! \returns emissive color
        */
        __IGOR_INLINE__ iaColor3f getEmissive() const;

        /*! set ambient color

        \param a ambient color
        */
        void setAmbient(const iaColor3f &a);

        /*! \returns ambient color
        */
        __IGOR_INLINE__ iaColor3f getAmbient() const;

        /*! set specular color

        \param s specular color
        */
        void setSpecular(const iaColor3f &s);

        /*! \returns specular color
        */
        __IGOR_INLINE__ iaColor3f getSpecular() const;

        /*! set diffuse color

        \param d diffuse color
        */
        void setDiffuse(const iaColor3f &d);

        /*! \returns diffuse color
        */
        __IGOR_INLINE__ iaColor3f getDiffuse() const;

        /*! set shininess

        \param s shininess value
        */
        void setShininess(float32 s);

        /*! \returns shininess
        */
        __IGOR_INLINE__ float32 getShininess() const;

        /*! sets alpha value of material
		*/
        void setAlpha(float32 alpha);

        /*! \returns alpha value of material
		*/
        __IGOR_INLINE__ float32 getAlpha() const;

    private:
        /*! map of textures associated to the mesh
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

        /*! initializes members
        */
        iTargetMaterial();

        /*! releases resources
        */
        ~iTargetMaterial();
    };

#include <igor/resources/material/iTargetMaterial.inl>

} // namespace igor

#endif