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

#ifndef IGOR_SHADER_FACTORY_H
#define IGOR_SHADER_FACTORY_H

#include <igor/resources/iFactory.h>
#include <igor/resources/shader/iShader.h>

namespace igor
{

    /*! this factory creates shader material resources
     */
    class iShaderMaterialFactory : public iFactory
    {
        friend class iResourceManager;

    private:
        /*! init members
         */
        iShaderMaterialFactory();

        /*! \returns resource type specific hash data
         */
        iaString getHashData(const iParameters &parameters) const override;

        /*! creates a resource object

        \param parameters the resource parameters
        \returns loaded or created new resource
        */
        iResourcePtr createResource(const iParameters &parameters) override;

        /*! creates an empty resource

        \returns new resource
        */
        iResourcePtr createResource() override;

        /*! loads the resource

        \param resource the resource to load
        \returns true if loading the resource was successful
        */
        bool loadResource(iResourcePtr resource) override;

        /*! unloads the resource

        \param resource the resource to unload
        */
        void unloadResource(iResourcePtr resource) override;

        /*! exports the resource based on it's parameters

        this needs to be a valid resource with a valid source parameter

        \param resource the resource to save
        \param filename optional filename if resource is to be saved in a new location
        \returns true if saving the resource was successful
        */
        bool saveResource(iResourcePtr resource, const iaString &filename = "") override;
    };

}; // namespace igor

#endif // IGOR_SHADER_FACTORY_H