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

#ifndef __iTARGETMATERIAL__
#define __iTARGETMATERIAL__

#include <iDefines.h>

#include <iaColor3.h>
using namespace IgorAux;

#include <map>
#include <memory>
using namespace std;

namespace Igor
{

    class iTexture;

    /*! contains all material information that belong to the surface of an object
    */
    class Igor_API iTargetMaterial
    {

        friend class iMaterialResourceFactory;

    public:

        /*! set a texture for a specific texture unit

        \param texture the texture to bind
        \param texunit the texture unit the texture will bond to
        */
        void setTexture(shared_ptr<iTexture> texture, int texunit);

        /*! \returns texture of specified texture unit

        \param texunit texture unit
        */
        shared_ptr<iTexture> getTexture(int texunit) const;

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
        void setEmissive(iaColor3f &e);

        /*! \returns emissive color
        */
        __IGOR_INLINE__ iaColor3f getEmissive() const;

        /*! set ambient color

        \param a ambient color
        */
        void setAmbient(iaColor3f &a);

        /*! \returns ambient color
        */
        __IGOR_INLINE__ iaColor3f getAmbient() const;

        /*! set specular color

        \param s specular color
        */
        void setSpecular(iaColor3f &s);

        /*! \returns specular color
        */
        __IGOR_INLINE__ iaColor3f getSpecular() const;

        /*! set diffuse color

        \param d diffuse color
        */
        void setDiffuse(iaColor3f &d);

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

    private:

        /*! map of _textures associated to the mesh
        */
        map<uint32, shared_ptr<iTexture>> _textures;

        /*! material _emissive value
        */
        iaColor3f _emissive;

        /*! material _ambient value
        */
        iaColor3f _ambient;

        /*! material _specular value
        */
        iaColor3f _specular;

        /*! material _diffuse value
        */
        iaColor3f _diffuse;

        /*! material _shininess value
        */
        float32 _shininess = 1;

        /*! initializes members
        */
        iTargetMaterial();

        /*! releases resources
        */
        ~iTargetMaterial();
    };

#include <iTargetMaterial.inl>

}

#endif