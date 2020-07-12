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
// contact: igorgameengine@protonmail.com

#ifndef __iTEXTURE__
#define __iTEXTURE__

#include <iaux/data/iaString.h>
using namespace iaux;

#include <igor/iDefines.h>

#include <memory>

namespace igor
{

    struct iRendererTexture;

    /*! represents a texture resource
    */
    class Igor_API iTexture
    {

        friend class iTextureResourceFactory;
        friend class iRenderer;

    public:
        /*! true if there is actually a texture present
        */
        bool isValid();

        /*! true if the texture was processed

        processed does not mean that it was loaded correctly 
        it means that we are done trying loading it
        */
        bool isProcessed();

        /*! true if it is a dummy texture
        */
        bool isDummy();

        /*! returns the width
        */
        int32 getWidth();

        /*! returns the height
        */
        int32 getHeight();

        /*! returns the bits per pixel
        */
        int32 getBpp();

        /*! returns the color format
        */
        iColorFormat getColorFormat();

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
        iaString getFilename();

        /*! does nothing
        */
        virtual ~iTexture();

    private:
        /*! true if there was actually a texture loaded
		*/
        bool _valid = false;

        /*! if true the texture is considered loaded regardless if it was a success or not
        */
        bool _processed = false;

        /*! true if it is a dummy texture
		*/
        bool _dummy = true;

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

        /*! renderer specific texture handle
		*/
        iRendererTexture *_rendererTexture = nullptr;

        /*! ctor

		initializes member variables
		*/
        iTexture(iaString name, iResourceCacheMode cacheMode, iTextureBuildMode buildMode, iTextureWrapMode wrapMode);
    };

    /*! definition of texture shared pointer
	*/
    typedef std::shared_ptr<iTexture> iTexturePtr;

}; // namespace igor

#endif