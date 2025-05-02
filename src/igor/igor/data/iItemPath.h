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

#ifndef IGOR_ITEM_PATH_H
#define IGOR_ITEM_PATH_H

#include <igor/data/iItem.h>

namespace igor
{
    /*! item path definition
     */
    class iItemPath
    {
    public:
        /*! init empty item path
         */
        iItemPath() = default;

        /*! init path from string

        \param path the given path
        */
        iItemPath(const iaString &path);

        /*! copy ctor
         */
        iItemPath(const std::vector<iaString> &path);

        /*! \returns path data
         */
        const std::vector<iaString> &getPath() const;

        /*! appends path to item path

        \param path the path to append
        */
        void operator+=(const iaString &path);

        /*! compare two item paths being equal

        \returns true if equal
        \param other the other item path
        */
        bool operator==(const iItemPath &other) const;

        /*! compare two item paths being not equal

        \returns true if not equal
        \param other the other item path
        */
        bool operator!=(const iItemPath &other) const;

        /*! \returns path as string using '/' as delimiter
         */
        const iaString toString() const;

    private:
        std::vector<iaString> _path;
    };

}

#endif // IGOR_ITEM_PATH_H
