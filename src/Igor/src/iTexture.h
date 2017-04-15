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
// (c) Copyright 2012-2017 by Martin Loga
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

#ifndef __iTEXTURE__
#define __iTEXTURE__

#include <iaString.h>
using namespace IgorAux;

#include <iDefines.h>

namespace Igor
{

    class iRendererTexture;
    class iTexture;

	class Igor_API iTexture
	{

		friend class iTextureResourceFactory;
        friend class iRenderer;

    public:

        /*! true if there is actually a texture present
        */
        bool isValid();

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

	private:

		/*! true if there is actually a texture loaded
		*/
		bool _valid = false;

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
		iRendererTexture* _rendererTexture = nullptr;

		/*! ctor

		initializes member variables
		*/
		iTexture(iaString name, iResourceCacheMode cacheMode, iTextureBuildMode buildMode, iTextureWrapMode wrapMode);

        /*! dtor

        does nothing
        */
        virtual ~iTexture();

        /*! so the shared_ptr is able to call the private destructor indirectly

        Thanks to http://nealabq.com/blog/2008/11/28/factory/
        */
        __IGOR_DISABLE_WARNING__(4091);
        static struct private_deleter
        {
            void operator()(iTexture* p)
            {
                delete p;
            }
        };
        __IGOR_ENABLE_WARNING__(4091);
		
	};

};

#endif