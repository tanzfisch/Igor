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
#ifndef IGOR_ANY_H
#define IGOR_ANY_H

#include <igor/iDefines.h>

#include <any>
#include <unordered_map>
#include <functional>
#include <typeindex>

namespace igor
{

    class IGOR_API iAny
    {
    public:
        /*! \returns instance of singleton
         */
        static iAny &getInstance();

        /*! compare two any

        \param a first any
        \param b second any
        \returns true if types and values match
        */
        bool compare(const std::any &a, const std::any &b) const;

        /*! add types for comparison
        */
        template <typename T>
        void add();

    private:
        /*! registered types
         */
        std::unordered_map<std::type_index, std::function<bool(const std::any &, const std::any &)>> _comparators;

        /*! init
         */
        iAny();
    };

#include <igor/utils/iAny.inl>

}

#endif // IGOR_UTILS_H