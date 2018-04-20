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

#ifndef __iTEXTURERESOURCEFACTORY__
#define __iTEXTURERESOURCEFACTORY__

#include <iTexture.h>

#include <iaSingleton.h>
#include <iaString.h>
#include <iaMutex.h>
using namespace IgorAux;

#include <list>
#include <map>
#include <memory>
using namespace std;

namespace Igor
{

    class iPixmap;
    struct iRendererTexture;

    /*! handles texture resources

    \bug not all textures released when used by iNodeModel and iNodeMesh
    */
    class Igor_API iTextureResourceFactory : public iaSingleton<iTextureResourceFactory>
    {
        friend class iaSingleton<iTextureResourceFactory>;

    public:

        /*! \returns shared pointer to genereated dummy texture
        */
        iTexturePtr getDummyTexture();

        /*! requests a file to be loaded asynchronously.

        \param filename name of file to load
        \param buildMode default ist mipmapped
        \param wrapMode wrap mode of texture
        \returns shared pointer to texture
        */
        iTexturePtr requestFile(const iaString& filename, iResourceCacheMode cacheMode = iResourceCacheMode::Free, iTextureBuildMode buildMode = iTextureBuildMode::Mipmapped, iTextureWrapMode wrapMode = iTextureWrapMode::Repeat);

        /*! load texture synchronously

        \param filename name of file to load
        \param mode default ist mipmapped
        \returns shared pointer to texture
        */
        iTexturePtr loadFile(const iaString& filename, iResourceCacheMode cacheMode = iResourceCacheMode::Free, iTextureBuildMode buildMode = iTextureBuildMode::Mipmapped, iTextureWrapMode wrapMode = iTextureWrapMode::Repeat);

        /*! works like a garbage collector.

        Interrates through all textures and checks how many references every texture has. If reference count
        goes down to 1 then the texture will be released. If reference count is greater 1 and the texture was
        not loaded yet and the texture will be loaded.

        \param cacheModeLevel level of cache mode to be released
        */
        void flush(iResourceCacheMode cacheModeLevel = iResourceCacheMode::Free);

        /*! if a flush in a different thread is currently running. this will prevent loading new data from disk and in consequence make it stop earlier
        */
        void interruptFlush();

        /*! generates texture form memory using the iPixmap class

        \param pixmap memory with pixmap data
        \param pixmapname artificial filename used as identifier (has to be a uNiQue)
        \param mode default ist mipmapped
        */
        iTexturePtr loadFromPixmap(iPixmap* pixmap, const iaString& pixmapname, iResourceCacheMode cacheMode = iResourceCacheMode::Free, iTextureBuildMode buildMode = iTextureBuildMode::Mipmapped, iTextureWrapMode wrapMode = iTextureWrapMode::Repeat);

        /*! synchronously loading a texture and returning a iPixmap

        \param filename name of file to load
        \returns pointer to pixmap
        */
        iPixmap* loadFileAsPixmap(const iaString& filename);

    private:

        /*! mutex to protect the texture manager access
        */
        iaMutex _mutex;

        /*! flag to interrupt flush cross threads
        */
        bool _interrupLoading = false;

        /*! mutex to protect the DevIL interface
        */
		iaMutex _mutexImageLibrary;

        /*! map of textures
        */
        map<int64, iTexturePtr> _textures;

        /*! generated dummy texture
        */
        iTexturePtr _dummyTexture = nullptr;

        /*! releases all textures
        */
        void deinit();

        /*! generates a dummy texture
        */
        void init();

        /*! actually loads a texture

        \param texture [in out] contains src information and texture
        */
        void loadTexture(iTexturePtr texture);

        /*! calculates hash value based on filename or name and build mode

        \param name the file name or just virtual name of the texture
        \param mode the build mode of the texture
        */
        int64 calcHashValue(const iaString& name, iResourceCacheMode cacheMode, iTextureBuildMode buildMode, iTextureWrapMode);

        /*! initialisation of members 3rd party lib
        */
        iTextureResourceFactory();

        /*! clean up and error handling
        */
        virtual ~iTextureResourceFactory();

    };

};

#endif

