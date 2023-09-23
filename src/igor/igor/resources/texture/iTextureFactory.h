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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IGOR_TEXTURE_FACTORY__
#define __IGOR_TEXTURE_FACTORY__

#include <igor/resources/iFactory.h>
#include <igor/resources/texture/iTexture.h>

namespace igor
{

    class iPixmap;

    /*! this factory creates texture resources
     */
    class iTextureFactory : public iFactory
    {
        friend class iResourceManager;
        friend class iPixmap;

    private:
        /*! init members
         */
        iTextureFactory();

        /*! \returns true if resource parameters are supported by this factory

        \param parameters the given resource parameters
        */
        bool matchingType(const iParameters &parameters) const override;

        /*! \returns resource type specific hash data
         */
        iaString getHashData(const iParameters &parameters) const override;

        /*! creates a resource object

        \param parameters the resource parameters
        \returns loaded or created new resource
        */
        iResourcePtr createResource(const iParameters &parameters) override;

        /*! loads the resource

        \param resource the resource to load
        \returns true if loading the resource was successful
        */
        bool loadResource(iResourcePtr resource) override;

        /*! unloads the resource

        \param resource the resource to unload
        */
        void unloadResource(iResourcePtr resource) override;

        /*! mutex to protect the image lib interface
         */
        static iaMutex _mutexImageLibrary;

        /*! loads texture from given file

        \param filename the given filename
        \param texture the texture resource
        */
        bool loadTexture(const iaString &filename, iTexturePtr texture);

        /*! generates some simple textures

        \param texture the texture resource
        \param parameters parameters to generate a texture
        */
        bool generateTexture(iTexturePtr texture, const iParameters &parameters);

        /*! use pixmap as textures

        \param pixmap the pixmap to load in to the texture
        \param texture the texture resource
        */
        bool pixmapToTexture(iPixmapPtr pixmap, iTexturePtr texture);

        /*! loads pixmap from file

        \param filename the file to load
        \returns pixmap
        */
        static iPixmapPtr loadPixmap(const iaString &filename);
    };

}; // namespace igor

#endif // __IGOR_TEXTURE_FACTORY__