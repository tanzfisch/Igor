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
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef IGOR_PREFAB_H
#define IGOR_PREFAB_H

#include <igor/resources/iResource.h>

#include <igor/entities/iEntityScene.h>

namespace igor
{
    /*! prefab pointer definition
     */
    class iPrefab;
    typedef std::shared_ptr<iPrefab> iPrefabPtr;

    /*! prefab ID definition
     */
    typedef iaUUID iPrefabID;

    /*! contains a prefab or a whole scene
     */
    class IGOR_API iPrefab : public iResource
    {
        friend class iPrefabFactory;
        friend class iPrefabIO;

    public:
        // TODO something like this to load it in to an existing scene void loadInto(iEntityScenePtr scene, iEntityPtr entity);

        /*! \returns entity scene id of this prefab
         */
        iEntitySceneID getSceneID() const;

    private:
        /*! entity scene id held by prefab after loading
         */
        iEntitySceneID _sceneID = iEntitySceneID::getInvalid();

        /*! init prefab

        \param parameters parameters specifying the prefab
         */
        iPrefab(const iParameters &parameters);
    };

} // namespace igor

#endif // IGOR_PREFAB_H