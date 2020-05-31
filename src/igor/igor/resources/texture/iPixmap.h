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
// (c) Copyright 2012-2020 by Martin Loga
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

#include <iaux/data/iaString.h>
#include <iaux/data/iaColor4.h>
#include <iaux/data/iaColor3.h>
using namespace IgorAux;

#include <igor/iDefines.h>

namespace Igor
{

	/*! pixmap is an editable texture

    in order to use it for rendering one have to transform it in to a iTexture

    \see iTextureResourceFactory::loadFromPixmap
    */
	class Igor_API iPixmap
	{
		friend class iTextureResourceFactory;

	public:
		/*! \returns width of pixmap
        */
		uint32 getWidth() const;

		/*! \returns height of pixmap
        */
		uint32 getHeight() const;

		/*! \returns bytes per pixel
        */
		uint32 getBytesPerPixel() const;

		/*! \returns color format in use
        */
		iColorFormat getColorFormat() const;

		/*! sets pixmap data

        \param data 
        */
		void setData(uint8 *data);

		/*! \returns pointer to pixel data
		*/
		uint8 *getData() const;

		/*! \returns pixel value from given position

		\param x x coordinate
		\param x y coordinate
		*/
		uint32 getPixel(uint32 x, uint32 y) const;

		/*! sets pixel value at given position

		\param x x coordinate
		\param x y coordinate
		\param color the color to set
		*/
		void setPixel(uint32 x, uint32 y, uint32 color);

		/*! same as getPixel but returns type iaColor4f

		\param x x coordinate
		\param x y coordinate
		\param[out] color the returned color
		*/
		void getPixel(float64 x, float64 y, iaColor4f &color) const;

		/*! same as getPixel but uses bilinear filter to calculate returned color

		\param x x coordinate
		\param x y coordinate
		\param[out] color the returned color
		*/
		void getPixelBiLinear(float64 x, float64 y, iaColor4f &color) const;

		/*! same as getPixel but returns type iaColor3f

		\param x x coordinate
		\param x y coordinate
		\param[out] color the returned color
		*/
		void getPixel(float64 x, float64 y, iaColor3f &color) const;

		/*! same as getPixel but returns type iaColor4c

		\param x x coordinate
		\param x y coordinate
		\param[out] color the returned color
		*/
		void getPixel(float64 x, float64 y, iaColor4c &color) const;

		/*! same as getPixel but returns type iaColor3c

		\param x x coordinate
		\param x y coordinate
		\param[out] color the returned color
		*/
		void getPixel(float64 x, float64 y, iaColor3c &color) const;

		/*! \returns value of color channel at given position

		\param x x coordinate
		\param x y coordinate
		\param colorChannel the chosen color channel
		*/
		uint8 getPixel(uint32 x, uint32 y, uint8 colorChannel) const;

		/*! sets color channel at given position 

		\param x x coordinate
		\param x y coordinate
		\param colorChannel the chosen color channel
		\param value the new color channel value
		*/
		void setPixel(uint32 x, uint32 y, uint8 colorChannel, uint8 value);

		/*! sets pixel color at given position

		\param x x coordinate
		\param x y coordinate
		\param r red color channel
		\param g green color channel
		\param b blur color channel
		*/
		void setPixelRGB(uint32 x, uint32 y, uint8 r, uint8 g, uint8 b);

		/*! sets pixel color at given position

		\param x x coordinate
		\param x y coordinate
		\param r red color channel
		\param g green color channel
		\param b blur color channel
		\param a alpha color channel
		*/
		void setPixelRGBA(uint32 x, uint32 y, uint8 r, uint8 g, uint8 b, uint8 a);

		/*! allocates memory based on size and colorformat

		\param width width of pixmap in pixel
		\param height height of pixmap in pixel
		\param colorFormat the color format of the pixmap
		*/
		iPixmap(uint32 width, uint32 height, iColorFormat colorFormat);

		/*! releases resources
		*/
		~iPixmap();

	private:
		/*! pixmap width
        */
		uint32 _width = 0;

		/*! pixmap height
        */
		uint32 _height = 0;

		/*! bytes per pixel
        */
		uint32 _bytesPerPixel = 0;

		/*! color format
        */
		iColorFormat _colorFormat = iColorFormat::Undefined;

		/*! raw data
        */
		uint8 *_data = nullptr;

		/*! does nothing
        */
		iPixmap() = default;
	};

	/*! pixmap pointer definition
	*/
	typedef iPixmap *iPixmapPtr;

}; // namespace Igor

#endif