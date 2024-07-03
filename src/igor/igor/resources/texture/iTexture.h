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

#ifndef __IGOR_TEXTURE__
#define __IGOR_TEXTURE__

#include <iaux/data/iaString.h>
using namespace iaux;

#include <igor/resources/iResource.h>
#include <igor/resources/texture/iPixmap.h>

#include <memory>

namespace igor
{

    /*! represents a texture resource

    available parameters:

    - inherits all parameters from iResource
    - generate: leads to generating a texture procedurally (type: bool)
      + pattern: type of pattern to use for procedurally generation (type: iTexturePattern)
      + primary: primary color to use (type: iaColor4f)
      + secondary: secondary color to use (type: iaColor4f)
      + width: width of texture (type: uint32)
      + height: height of texture (type: uint32)
    - pixmap: use data from given pixmap for texture (type: iPixmapPtr)
    - wrapMode: texture wrap mode (type: iTextureWrapMode, default: Repeat)
    - buildMode: texture wrap mode (type: iTextureBuildMode, default: Mipmapped)
    

     */
    class IGOR_API iTexture : public iResource
    {
        friend class iTextureFactory;

    public:

        /*! \returns true if it is unprocessed or invalid. Then the renderer will use a fallback texture
         */
        bool useFallback() const;

        /*! \returns the width in texels
         */
        int32 getWidth() const;

        /*! \returns the height in texels
         */
        int32 getHeight() const;

        /*! \returns the bits per pixel (texel)
         */
        int32 getBpp() const;

        /*! \returns the mip map level count
         */
        uint32 getMipMapLevels() const;

        /*! \returns the color format
         */
        iColorFormat getColorFormat() const;

        /*! \returns true if data contains alpha channel and values != 1.0
         */
        bool hasTransparency() const;

        /*! \returns the texture build mode
         */
        iTextureBuildMode getBuildMode() const;

        /*! \returns texture wrap mode
         */
        iTextureWrapMode getWrapMode() const;

        /*! binds and activates texture to given texture unit

        \param textureUnit the given texture unit
        */
        void bind(uint32 textureUnit);

        /*! calculates mip map levels

        \param width the width of the texture
        \param height the height of the texture
        \returns the mip map levels
        */
        static uint32 calcMipMapLevels(int32 width, int32 height);

    private:

        /*! true if it is a dummy texture
         */
        bool _useFallback = true;

        /*! build mode. initialized in ctor
         */
        iTextureBuildMode _buildMode;

        /*! wrap mode. initialized in ctor
         */
        iTextureWrapMode _wrapMode;

        /*! the width
         */
        int32 _width = 256;

        /*! the height
         */
        int32 _height = 256;

        /*! bits per pixel
         */
        int32 _bpp = 0;

        /*! color format
         */
        iColorFormat _colorFormat = iColorFormat::Undefined;

        /*! mip map levels
         */
        uint32 _mipMapLevels = 1;

        /*! renderer specific texture handle
         */
        uint32 _textureID = 0;

        /*! if true data has alpha channel with values != 1.0
         */
        bool _hasTrans = false;

        /*! initializes member variables

        \param parameters the parameters of this texture
        */
        iTexture(const iParameters &parameters);

        /*! sets data on texture

        \param width width of the texture
        \param height height of the texture
        \param bytepp bytes per pixel
        \param format color format of texture
        \param data pointer to the actual data used for the texture
        \param buildMode generation mode of texture like mimapping or not
        \param wrapMode wrap mode of texture
        */
        void setData(int32 width, int32 height, int32 bytepp, iColorFormat format, unsigned char *data, iTextureBuildMode buildMode, iTextureWrapMode wrapMode);
    };

    /*! definition of texture shared pointer
     */
    typedef std::shared_ptr<iTexture> iTexturePtr;

}; // namespace igor

#endif // __IGOR_TEXTURE__