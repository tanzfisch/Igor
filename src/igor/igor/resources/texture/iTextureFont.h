//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef __IGOR_TEXTURE_FONT__
#define __IGOR_TEXTURE_FONT__

#include <igor/resources/texture/iTexture.h>

#include <iaux/data/iaRectangle.h>
#include <iaux/data/iaString.h>
using namespace iaux;

#include <vector>
#include <memory>

namespace igor
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
    struct iCharacterDimensions
    {
        iaRectanglef _characterRect;
        float32 _characterOffset;
    };

    class iTextureFont;

    /*! texture font pointer definition
     */
    typedef std::shared_ptr<iTextureFont> iTextureFontPtr;

    /*! texture font
     */
    class IGOR_API iTextureFont
    {
    public:
        /*! \returns a newly created font

        \param texture the texture to use (needs to be loaded already)
        \param type font type
        \param colorMask use this color channel to detect the size of characters
        \param colorMaskThreshold this threshold determines  what texel belongs to the character and wich not
        */
        static iTextureFontPtr create(iTexturePtr texture, iFontType type = iFontType::FixedDigitWidth, iColorMask colorMask = iColorMask::Alpha, float32 colorMaskThreshold = 0.5f);

        /*! cleans up
         */
        ~iTextureFont();

        /*! calculates the width of the given iaString using this font

        \param text the text to calculate the width
        \param size size of the font
        */
        float32 measureWidth(const iaString &text, float32 size);

        /*! calculates the height of a iaString with line breaks

        \param text the text to calculate with
        \param size font size
        \param maxWidth with to make line break
        \param lineHeight factor for the line height (default value is 1.15)
        */
        float32 measureHeight(const iaString &text, float32 size, float32 maxWidth, float32 lineHeight = 1.15f);

        /*! returns true if the font was loading correctly
         */
        bool isValid() const;

        /*! returns pointer to the font texture

        \return pointer to font texture
        */
        iTexturePtr getTexture() const;

        /*! returns character set

        \return vector of characters
        */
        const std::vector<iCharacterDimensions> &getCharacters() const;

    private:
        /*! true if the font is a valid font
         */
        bool _valid;

        /*! texture
         */
        iTexturePtr _texture;

        /*! pixmap of the the texture
         */
        iPixmapPtr _pixmap;

        /*! character set of the font
         */
        std::vector<iCharacterDimensions> _characters;

        /*! ctor

        \param texture the texture to use
        \param type font type
        \param colorMask use this color channel to detect the size of characters
        \param colorMaskThreshold this threshold determines  what texel belongs to the character and wich not
        */
        iTextureFont(iTexturePtr texture, iFontType type = iFontType::FixedDigitWidth, iColorMask colorMask = iColorMask::Alpha, float32 colorMaskThreshold = 0.0f);

        /*! setup font type to fixed width
         */
        void makeFixedWidth();

        /*! setup font type to fixed digit width
         */
        void makeFixedDigitWidth();

        /*! modify width of given character

        \param character the character to modify
        \param newWidth the new newWidth
        \param newOffset the new character offset
         */
        void modifyWidth(iCharacterDimensions &character, float32 newWidth, float32 newOffset);
    };

}; // namespace igor

#endif // __IGOR_TEXTURE_FONT__