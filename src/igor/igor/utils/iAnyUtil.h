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
// (c) Copyright 2012-2026 by Martin A. Loga
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
#ifndef IGOR_ANY_UTIL_H
#define IGOR_ANY_UTIL_H

#include <igor/iDefines.h>

#include <iaux/data/iaString.h>

#include <any>
#include <unordered_map>
#include <functional>
#include <typeindex>

namespace igor
{

    /*! types known to iAnyUtil
    */
    enum class iAnyUtilType
    {
        Unknown,
        Void,
        Bool,
        uint8,
        int8,
        uint16,
        int16,
        uint32,
        int32,
        uint64,
        int64,
        float32,
        float64,
        std_string,
        iaString,
        iaVector2f,
        iaVector2d,
        iaVector2i,
        iaVector2I,
        iaVector3f,
        iaVector3d,
        iaVector3i,
        iaVector3I,
        iaVector4f,
        iaVector4d,
        iaVector4i,
        iaVector4I
    };

    class IGOR_API iAnyUtil
    {
    public:
        /*! \returns instance of singleton
         */
        static iAnyUtil &getInstance();

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

        /*! turn any value in to string

        works only for known types

        \param value the any value
        \returns string of value
        */
        static iaString toString(const std::any &value);

        /*! turn type info in to string

        \param typeInfo the type info
        \returns string of type info
        */
        static iaString toString(const std::type_info &typeInfo);

        /*! \returns enum for given type info

        \param typeInfo the given type info
        */
        static iAnyUtilType toEnum(const std::type_info &typeInfo);

    private:
        /*! registered types
         */
        std::unordered_map<std::type_index, std::function<bool(const std::any &, const std::any &)>> _comparators;

        /*! init
         */
        iAnyUtil();
    };

#include <igor/utils/iAnyUtil.inl>

}

#endif // IGOR_ANY_UTIL_H