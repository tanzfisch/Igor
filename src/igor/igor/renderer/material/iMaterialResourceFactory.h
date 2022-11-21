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

#ifndef __IGOR_MATERIALRESOURCEFACTORY__
#define __IGOR_MATERIALRESOURCEFACTORY__

#include <igor/resources/module/iModule.h>
#include <igor/renderer/material/iMaterial.h>

namespace igor
{
    /*! material resource factory
     */
    class IGOR_API iMaterialResourceFactory : public iModule<iMaterialResourceFactory>
    {
        friend class iModule<iMaterialResourceFactory>;

    public:
        /*! loads material from file. If the same material was already loaded it will return from cache

        \param filename name of file to load
        \param cacheMode default ist mipmapped
        \returns shared pointer to material
        */
        iMaterialPtr loadMaterial(const iaString &filename, iResourceCacheMode cacheMode = iResourceCacheMode::Cache);

        /*! creates new material with default settings and no shader on it
         */
        iMaterialPtr createMaterial(const iaString &name = "");

        /*! \returns default material
         */
        const iMaterialPtr &getDefaultMaterial() const;

        /*! \returns material by id
         */
        // TODO const iMaterialPtr &getMaterial(UUID) const;

        /*! works like a garbage collector.

        Interrates through all materials and checks how many references every material has. If reference count
        goes down to 1 then the material will be released.
        */
        void flush();

    private:

        /*! the default material
        */
        iMaterialPtr _defaultMaterial;

        /*! initialisation of members 3rd party lib
         */
        iMaterialResourceFactory();

        /*! clean up and error handling
         */
        virtual ~iMaterialResourceFactory();
    };

}; // namespace igor

#endif // __IGOR_MATERIALRESOURCEFACTORY_H__
