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

namespace igor
{

    /*! types known to iAny
     */
    enum class iAnyType
    {
        Unknown,
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
        std_wstring,
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
        iaVector4I,
        iItemPath,
        iResourceID
    };

    /*! \returns name of given enum value
    \param value the given value
    */
    iaString IGOR_API toString(const iAnyType &type);

    /*! a wrapper for std::any that is aware of igor types
     */
    class IGOR_API iAny
    {
    public:
        /*! default ctor
         */
        iAny() = default;

        /*! copy ctor
         */
        iAny(const iAny &value);

        template <class T>
        iAny(T &&value);

        /*! \returns value for given type
         */
        template <typename T>
        T getValue() const;

        /*! \returns value as string if known type

        this is for readability not for preserving information
         */
        iaString toString() const;

        /*! \returns type of any
         */
        iAnyType getType() const;

        /*! compare two any

        \param a first any
        \param b second any
        \returns true if types and values match
        */
        static bool compare(const iAny &a, const iAny &b);

        /*! \returns data
         */
        const std::any &getData() const;

        /*! \returns default any for given type

        \param type the given type
        */
        static std::any getAny(const iAnyType &type);

    private:
        /*! data
         */
        std::any _data;
    };

#include <igor/data/iAny.inl>

    /*! stream operator for iAny

    \param stream the stresm
    \param any the value
    */
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iAny &any);

}

#endif // IGOR_ANY_UTIL_H