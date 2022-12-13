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

#ifndef __IGOR_TEXTURE__
#define __IGOR_TEXTURE__

#include <iaux/data/iaString.h>
using namespace iaux;

#include <igor/iDefines.h>

#include <memory>

namespace igor
{

    /*! definition of texture shared pointer
	*/
    class iTexture;
    typedef std::shared_ptr<iTexture> iTexturePtr;    

    /*! represents a texture resource
    */
    class IGOR_API iTexture
    {

        friend class iTextureResourceFactory;

    public:
    
        /*! \returns true if there is actually a texture present
        */
        bool isValid() const;

        /*! \returns true if the texture was processed
        */
        bool isProcessed() const;

        /*! \returns true if it is unprocessed or invalid. Then the renderer will use a fallback texture
        */
        bool useFallback() const;

        /*! \returns the width
        */
        int32 getWidth() const;

        /*! \returns the height
        */
        int32 getHeight() const;

        /*! \returns the bits per pixel
        */
        int32 getBpp() const;

        /*! \returns the mip map levels
        */
        uint32 getMipMapLevels() const;

        /*! \returns the color format
        */
        iColorFormat getColorFormat() const;

        /*! \returns true if data contains alpha channel and values != 1.0
        */
        bool hasTransparency() const;

        /*! \returns cache mode
        */
        iResourceCacheMode getCacheMode() const;

        /*! \returns the texture build mode
        */
        iTextureBuildMode getBuildMode() const;

        /*! \returns texture wrap mode
        */
        iTextureWrapMode getWrapMode() const;

        /*! returns the filename
        */
        const iaString &getFilename() const;

        /*! binds and activates texture to given texture unit

        \param textureUnit the given texture unit
        */
        void bind(uint32 textureUnit);

        /*! \returns texture id
        */
        uint32 getTextureID() const;

    private:
        /*! true if there was actually a texture loaded
		*/
        bool _valid = false;

        /*! if true the texture is considered loaded regardless if it was a success or not
        */
        bool _processed = false;

        /*! true if it is a dummy texture
		*/
        bool _useFallback = true;

        /*! the file name. initialized in ctor
		*/
        iaString _filename;

        /*! cache mode. initialized in ctor
        */
        iResourceCacheMode _cacheMode;

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

        /*! ctor

		initializes member variables
		*/
        iTexture(iaString name, iResourceCacheMode cacheMode, iTextureBuildMode buildMode, iTextureWrapMode wrapMode);

        /*! \returns a newly created texture

        \param name name of the texture (usually the filename)
        \param cacheMode caching mode
        \param buildMode build mode 
        \param wrapMode texture coordinates wrapping mode
         */
        static iTexturePtr create(iaString name, iResourceCacheMode cacheMode, iTextureBuildMode buildMode, iTextureWrapMode wrapMode);

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

}; // namespace igor

#endif