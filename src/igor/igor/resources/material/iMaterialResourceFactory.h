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
#include <igor/resources/material/iMaterial.h>

#include <unordered_map>

namespace igor
{

    /*! material resource factory
     */
    class IGOR_API iMaterialResourceFactory : public iModule<iMaterialResourceFactory>
    {
        friend class iModule<iMaterialResourceFactory>;

    public:
        /*! creates material
        \param name the materials name which must be unique
        \param cache if true this material will be added to the cache
        \returns new material
        If the name already exists it will return a previously created material
        */
        iMaterialPtr createMaterial(const iaString &name, bool cache = true);

        /*! loads material from file. If the same material was already loaded it will return from cache

        \param filename name of file to load
        \param cache if true this material will be added to the cache
        \returns shared pointer to material
        */
        iMaterialPtr loadMaterial(const iaString &filename, bool cache = true);

        /*! \returns default material
         */
        const iMaterialPtr &getDefaultMaterial() const;

        /*! \returns colorID material
         */
        const iMaterialPtr &getColorIDMaterial() const;

        /*! returns a list of all materials sorted by order

        \param[out] the materials list
        */
        void getMaterials(std::vector<iMaterialPtr> &materials);

        /*! \return material for given name

        \param name the given material name
        */
        iMaterialPtr getMaterial(const iaString &name);

        /*! \returns material for given material id
        
        \param materialID the given id
        */
        iMaterialPtr getMaterial(const iMaterialID &materialID);

        /*! creates default materials
         */
        void init();

        /*! release resources
         */
        void deinit();

        /*! works like a garbage collector.

        Interrates through all materials and checks how many references every material has. If reference count
        goes down to 1 then the material will be released.
        */
        void flush();        

    private:

        /*! mutex to protect the materials map
         */
        iaMutex _mutexMaterial;

        /*! map of materials
         */
        std::unordered_map<int64, iMaterialPtr> _materials;

        /*! the default material
         */
        iMaterialPtr _defaultMaterial;

        /*! the colorID material
         */
        iMaterialPtr _colorIDMaterial;

        /*! does nothing
         */
        iMaterialResourceFactory() = default;

        /*! does nothing
         */
        virtual ~iMaterialResourceFactory() = default;
    };

}; // namespace igor

#endif // __IGOR_MATERIALRESOURCEFACTORY_H__
