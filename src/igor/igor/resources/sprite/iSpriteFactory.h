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

#ifndef __IGOR_SPRITE_FACTORY__
#define __IGOR_SPRITE_FACTORY__

#include <igor/resources/iFactory.h>
#include <igor/resources/sprite/iSprite.h>

namespace igor
{

    /*! this factory creates sprite resources
     */
    class iSpriteFactory : public iFactory
    {

    private:
        /*! \returns the factory type

        this type is used to register with the resource manager
        */
        const iaString &getType() const override;

        /*! \returns true if resource parameters are supported by this factory

        \param parameters the given resource parameters
        */
        bool matchingType(const iParameters &parameters) const override;

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

        /*! reads atlas element

        \param atlas the atlas element
        \param sprite the target sprite
        */
        void readAtlas(TiXmlElement *atlas, iSpritePtr sprite);

        /*! load sprite from file

        \param filename the filename
        \param sprite the target sprite
        */
        bool loadSprite(const iaString &filename, iSpritePtr sprite);

    };

}; // namespace igor

#endif // __IGOR_SPRITE_FACTORY__