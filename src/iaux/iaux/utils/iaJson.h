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

#ifndef IAUX_JSON_H
#define IAUX_JSON_H

#include <iaux/data/iaUUID.h>
#include <iaux/math/iaVector2.h>
#include <iaux/math/iaVector3.h>
#include <iaux/math/iaVector4.h>

#include <json.hpp>
using json = nlohmann::json;

namespace iaux
{
    /*! iaUUID to json
     */
    void to_json(json &j, const iaUUID &uuid);

    /*! json to iaUUID
     */
    void from_json(const json &j, iaUUID &uuid);

    /*! iaString to json
     */
    void to_json(json &j, const iaString &text);

    /*! json to iaString
     */
    void from_json(const json &j, iaString &text);

    /*! iaVector2f to json
     */
    void to_json(json &j, const iaVector2f &vec);

    /*! json to iaVector2f
     */
    void from_json(const json &j, iaVector2f &vec);

    /*! iaVector3f to json
     */
    void to_json(json &j, const iaVector3f &vec);

    /*! json to iaVector3f
     */
    void from_json(const json &j, iaVector3f &vec);

    /*! iaVector4f to json
     */
    void to_json(json &j, const iaVector4f &vec);

    /*! json to iaVector4f
     */
    void from_json(const json &j, iaVector4f &vec);        

    /*! iaVector2d to json
     */
    void to_json(json &j, const iaVector2d &vec);

    /*! json to iaVector2d
     */
    void from_json(const json &j, iaVector2d &vec);

    /*! iaVector3d to json
     */
    void to_json(json &j, const iaVector3d &vec);

    /*! json to iaVector3d
     */
    void from_json(const json &j, iaVector3d &vec);

    /*! iaVector4d to json
     */
    void to_json(json &j, const iaVector4d &vec);

    /*! json to iaVector4d
     */
    void from_json(const json &j, iaVector4d &vec);        
}

#endif // IAUX_JSON_H
