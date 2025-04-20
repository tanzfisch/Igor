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

#ifndef IGOR_PREFABIO_H
#define IGOR_PREFABIO_H

#include <igor/resources/prefab/iPrefab.h>

#include <igor/utils/iJson.h>

namespace igor
{
    /*! reader for Igor prefab files
     */
    class IGOR_API iPrefabIO
    {
    public:
        /*! reads prefab file and configures given prefab with it

        \param filename path to prefab file
        \param prefab the prefab to configure with
        */
        static bool read(const iaString &filename, const iPrefabPtr &prefab);

        /*! writes prefab to file

        \param filename path to new prefab file
        \param prefab the prefab to use
        */
        static bool write(const iaString &filename, const iPrefabPtr &prefab);

    private:

        /*! read entity from json

        \param entityJson the json object
        \param scene the scene to create the entity in
        */
        static void readEntity(iEntityScenePtr scene, const json &entityJson);

        /*! connects children with their parent entities

        \param entityJson the json object
        \param scene the scene to works with
        */
        static void connectEntity(iEntityScenePtr scene, const json &entityJson);

        /*! write entity to json

        \param entityJson the json object
        \param entity the entity to write
        */
        static void writeEntity(json &entityJson, iEntityPtr entity);
    };

} // namespace igor

#endif // IGOR_PREFABIO_H