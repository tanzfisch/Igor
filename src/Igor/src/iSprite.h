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

#include <iTexture.h>

#include <iaVector2.h>
#include <iaConsole.h>
using namespace IgorAux;

#include <memory>
#include <vector>

namespace Igor
{

	class iTexture;

    /*! used to have 2d sprites
    */
	class Igor_API iSprite
    {

    public:

		/*! a frame defines an area within the texture that later can be rendered
		*/
		struct Frame
		{
			/*! position of frame in texture coordinates
			*/
			iaVector2f _pos;

			/*! size of frame in texture coordinates
			*/
			iaVector2f _size;
			
			/*! origin of frame in "texel" coordinates

			can be used to position on screen
			*/
			iaVector2f _origin;
		};

		/*! adds a frame to the sprite

		\returns index of the added frame
		\param pos position of frame
		\param size the size of the frame
		\param origin the origin of the frame
		\param pixel if true values are in pixel coordinates if false than 0.0-1.0
		*/
		uint32 addFrame(const iaVector2f& pos, const iaVector2f& size, const iaVector2f& origin, bool pixel = true);

        /*! \returns texture
        */
        iTexturePtr getTexture() const;

		/*! sets txture used by this sprite

		\param texture the texture to use
		*/
		void setTexture(iTexturePtr texture);

		/*! \returns the amount of frames
		*/
		uint32 getFrameCount() const;

		/*! returns frame for given index

		no range check!

		\param index the frame index to return
		\returns the specified frame
		*/
		const Frame& getFrame(uint32 index) const;

        /*! ctor initializes member variables

        \param texture texture used for this sprite
        */
        iSprite(iTexturePtr texture);

        /*! does nothing
        */
        ~iSprite() = default;

	private:

		/*! the sprite sets
		*/
		std::vector<Frame> _frames;

        /*! texture in use
        */
		iTexturePtr _texture;

	};

#include <iSprite.inl>

};

#endif
