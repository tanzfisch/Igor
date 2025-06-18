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

#ifndef IGOR_ENTITY_ID_PATH_H
#define IGOR_ENTITY_ID_PATH_H

#include <igor/entities/iEntityComponent.h>

namespace igor
{
    /*! entity id path definition
     */
    class iEntityIDPath
    {
    public:
        /*! does nothing
         */
        iEntityIDPath() = default;

        /*! copy ctor

        \param other the other id path
        */
        iEntityIDPath(const iEntityIDPath &other);

        /*! init from string

        \param path the string to init with
        */
        iEntityIDPath(const iaString &path);

        /*! \returns string version of ID path using '/' as delimiter
         */
        const iaString toString() const;

        /*! \returns path
         */
        const std::vector<iEntityID> &getPath() const;

        /*! appends path to item path

        \param path the path to append
        */
        void operator+=(const iEntityID &entityID);

        /*! compare two entityID paths being equal

        \returns true if equal
        \param other the other entityID path
        */
        bool operator==(const iEntityIDPath &other) const;

        /*! compare two entityID paths being not equal

        \returns true if not equal
        \param other the other entityID path
        */
        bool operator!=(const iEntityIDPath &other) const;

        /*! reverse path
         */
        void reverse();

    private:
        std::vector<iEntityID> _path;
    };

}

#endif // IGOR_ENTITY_ID_PATH_H
