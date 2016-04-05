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
// (c) Copyright 2014-2016 by Martin Loga
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

#ifndef __iPIXMAP__
#define __iPIXMAP__

#include <iaString.h>
using namespace IgorAux;

#include <iDefines.h>

namespace Igor
{

    /*! pixmap is an editable texture

    in order to use it for rendering one have to transform it in to a iTexture

    \see iTextureResourceFactory::loadFromPixmap

    \todo manythings not implemented anymore since switch from OpenIL to stb_image
    */
	class Igor_API iPixmap
	{
		friend class iTextureResourceFactory;

    public:

        /*! \returns width of pixmap
        */
        uint32 getWidth();

        /*! \returns height of pixmap
        */
        uint32 getHeight();

        /*! \returns bytes per pixel
        */
        uint32 getBytesPerPixel();

        /*! \returns color format in use
        */
        iColorFormat getColorFormat();

        /*! sets pixmap data

        \param data 
        */
        void setData(uint8 *data);
        uint8* getData();

        uint32 getPixel(uint32 x, uint32 y);
        bool setPixel(uint32 x, uint32 y, uint32 color);

        uint8 getPixel(uint32 x, uint32 y, uint8 byte);
        bool setPixel(uint32 x, uint32 y, uint8 byte, uint8 color);
        bool setPixelRGB(uint32 x, uint32 y, uint8 r, uint8 g, uint8 b);
        bool setPixelRGBA(uint32 x, uint32 y, uint8 r, uint8 g, uint8 b, uint8 a);

        bool exportFile(const iaString filename, bool overwrite = true);

        iPixmap(uint32 width, uint32 height, iColorFormat color_format);
        ~iPixmap();

	private:

		uint32 textureID;

		uint32 image_width;
		uint32 image_height;
		uint32 bytes_per_pixel;
		uint32 format;							// OpenIL iColorFormat
		iColorFormat color_format;				// Igor iColorFormat
		uint8 *data;

		bool created;

		iPixmap();

	};
};

#endif