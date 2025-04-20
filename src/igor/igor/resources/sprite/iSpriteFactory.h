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
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef IGOR_SPRITE_FACTORY_H
#define IGOR_SPRITE_FACTORY_H

#include <igor/resources/iFactory.h>
#include <igor/resources/sprite/iSprite.h>

namespace igor
{

    /*! this factory creates sprite resources
     */
    class iSpriteFactory : public iFactory
    {
        friend class iResourceManager;

    private:
        /*! init members
         */
        iSpriteFactory();

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

        /*! load sprite from file

        \param filename the filename
        \param sprite the target sprite
        */
        bool load(const iaString &filename, iSpritePtr sprite);
    };

}; // namespace igor

#endif // IGOR_SPRITE_FACTORY_H