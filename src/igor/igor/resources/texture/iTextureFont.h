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

#ifndef __iFONT__
#define __iFONT__

#include <iaux/data/iaString.h>
using namespace IgorAux;

#include <igor/data/iRectangle.h>
#include <igor/resources/texture/iTexture.h>

#include <vector>
#include <memory>

namespace Igor
{

	class iPixmap;

	/*! defines how the spacing is between letters and digits
    */
	enum class iFontType
	{
		Standard,
		FixedWidth,
		FixedDigitWidth
	};

	/*! masks specific channel to determine the background within the texture font
    */
	enum class iColorMask
	{
		Red,
		Green,
		Blue,
		Alpha
	};

	/*! dimensions of a character in the texture
	*/
	class iCharacterDimensions
	{

	public:
		iRectanglef rect;
		float32 relRenderWidth;
	};

	/*! texture font
	*/
	class Igor_API iTextureFont
	{

	private:
		/*! true if the font is a valid font
		*/
		bool valid;

		/*! 
		*/
		void makeFixedWidth();

		/*! 
		*/
		void makeFixedDigitWidth();

		/*! 
		*/
		void modifyWidth(iCharacterDimensions &character, float32 newWidth, float32 newRelRenderWidth);

		/*! texture
		*/
		iTexturePtr _texture;

		/*! pixmap of the the texture
		*/
		iPixmap *_pixmap;

		/*! character set of the font
		*/
		std::vector<iCharacterDimensions> _characters;

	public:
		/*! calculates the width of the given iaString using this font

		\param text the text to calculate the width
		\param size size of the font
		*/
		float32 measureWidth(iaString text, float32 size);

		/*! calculates the height of a iaString with line breaks

		\param text the text to calculate with
		\param size font size
		\param max_width with to make line break
		\param line_height factor for the line height (default value is 1.15)
		*/
		float32 measureHeight(iaString text, float32 size, float32 max_width, float32 line_height = 1.15f);

		/*! returns true if the font was loading correctly
		*/
		bool isValid();

		/*! returns pointer to the font texture

		\return pointer to font texture
		*/
		iTexturePtr getTexture();

		/*! returns character set

		\return vector of characters
		*/
		std::vector<iCharacterDimensions> &getCharacters();

		/*! ctor

		\param font_file filename of font texture
		\param font_type font type
		\param mask_channel use this color channel to detect the size of characters
		\param mask_threashold this threashold dertermines what texel belongs to the character and wich not
		*/
		iTextureFont(iaString font_file, iFontType font_type = iFontType::FixedDigitWidth, iColorMask mask_channel = iColorMask::Alpha, float32 mask_threashold = 0);

		/*! cleans up
		*/
		~iTextureFont();
	};

}; // namespace Igor

#endif
