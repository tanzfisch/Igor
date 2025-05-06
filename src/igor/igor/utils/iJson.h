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

// https://wiki.lspace.org/Igor ;-)
#ifndef IGOR_JSON_H
#define IGOR_JSON_H

#include <igor/data/iAACube.h>

#include <iaux/data/iaUUID.h>
#include <iaux/math/iaVector2.h>
#include <iaux/math/iaVector3.h>
#include <iaux/math/iaVector4.h>
#include <iaux/data/iaColor3.h>
#include <iaux/data/iaColor4.h>
#include <iaux/data/iaRectangle.h>

#include <json.hpp>
using json = nlohmann::json;

namespace igor
{
    /*! helper class to access json values
     */
    class iJson
    {
    public:
        template <typename T>
        static const T getValue(const json &jsonElement, const iaString &elementName, const T &defaultValue);

        static json parse(const iaString &filename);
    };

    /*! iAACubef to json
     */
    void to_json(json &j, const iAACubef &cube);

    /*! json to iAACubef
     */
    void from_json(const json &j, iAACubef &cube);  

    /*! iAACubed to json
     */
    void to_json(json &j, const iAACubed &cube);

    /*! json to iAACubed
     */
    void from_json(const json &j, iAACubed &cube);     

#include <igor/utils/iJson.inl>
}

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

    /*! iaColor3f to json
     */
    void to_json(json &j, const iaColor3f &color);

    /*! json to iaColor3f
     */
    void from_json(const json &j, iaColor3f &color);

    /*! iaColor4f to json
     */
    void to_json(json &j, const iaColor4f &color);

    /*! json to iaColor4f
     */
    void from_json(const json &j, iaColor4f &color);

    /*! iaRectanglef to json
     */
    void to_json(json &j, const iaRectanglef &rect);

    /*! json to iaRectanglef
     */
    void from_json(const json &j, iaRectanglef &rect);

    /*! iaRectangled to json
     */
    void to_json(json &j, const iaRectangled &rect);

    /*! json to iaRectangled
     */
    void from_json(const json &j, iaRectangled &rect);
}


#endif // IGOR_JSON_H