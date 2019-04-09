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

#ifndef __iSPRITE__
#define __iSPRITE__

#include <iaVector2.h>
#include <iTexture.h>

#include <memory>
#include <vector>
using namespace std;

namespace Igor
{

	class iTexture;

    /*! used to have 2d sprites
    */
	class Igor_API iSprite
    {

    public:

        /*! set texture coordinates

        \param tex0 texcoordinate of vertex 0
        \param tex1 texcoordinate of vertex 1
        \param tex2 texcoordinate of vertex 2
        \param tex3 texcoordinate of vertex 3
        \param pixel true: texcoordinated in pixel; false: texcoordinates in 0.0-1.0
        */
        void setTexcoord(const iaVector2f& tex0, const iaVector2f& tex1, const iaVector2f& tex2, const iaVector2f& tex3);

        /*! sets origin of the sprite

        \param coi center of interesst
        */
        void setOrigin(const iaVector2f& pos);

        /*! \returns texture
        */
        __IGOR_INLINE__ shared_ptr<iTexture> getTexture() const;

        /*! \returns texture coordinates by index

        \param index the index to return the texture coordinates from
        */
        __IGOR_INLINE__ const iaVector2f& getTexCoord(uint32 index) const;

        /*! \returns center of interesst
        */
        __IGOR_INLINE__ const iaVector2f& getOrigin() const;

        /*! \returns width
        */
        __IGOR_INLINE__ float32 getWidth() const;

        /*! \returns height
        */
        __IGOR_INLINE__ float32 getHeight() const;

        /*! ctor initializes member variables

        \param texture texture used for this sprite
        */
        iSprite(shared_ptr<iTexture> texture);

        /*! does nothing
        */
        ~iSprite() = default;

	private:

        /*! texture in use
        */
		shared_ptr<iTexture> _texture;

        /*! texture coordinates
        */
		iaVector2f _texCoord[4];

        /*! relative origin
        */
		iaVector2f _origin;

        /*! width of sprite
        */
		float32 _width = 0;

        /*! height of sprite
        */
		float32 _height = 0;

	};

#include <iSprite.inl>

};

#endif
