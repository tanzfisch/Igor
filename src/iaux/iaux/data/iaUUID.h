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

#ifndef IAUX_UUID_H
#define IAUX_UUID_H

#include <iaux/data/iaString.h>

namespace iaux
{

    /*! uuid (probably not following any standard but good enough for me)
     */
    class IAUX_API iaUUID
    {
    public:
        /*! init valid random value
         */
        iaUUID();

        /*! copy ctor

        \param other the other value
        */
        iaUUID(const iaUUID &other);

        /*! ctor with value

        \param value the value to use as uuid
        */
        iaUUID(uint64 value);

        /*! \returns invalid uuid
         */
        static iaUUID getInvalid();

        /*! import from string

        expected format "0xffffffffffffffff" to read it as is
        every other type of string will be turned in the a hash value

        \param uuid the id in string form
        */
        iaUUID(const iaString &text);

        /*! \returns true if given text is a uuid

        \param text the given text
        */
        static bool isUUID(const iaString &text);

        /*! makes id invalid
         */
        void reset();

        /*! \returns true if both uuids are equal

        \param other the other uuid
        */
        bool operator==(const iaUUID &other) const;

        /*! \returns true if uuids are not equal

        \param other the other uuid
        */
        bool operator!=(const iaUUID &other);

        /*! = operator overwrites current string with new string

        \param text the new string
        \returns the new string
        */
        iaUUID operator=(const iaUUID &other);

        /*! = operator overwrites current value with new value

        \param value the new value
        \returns the new uuid
        */
        iaUUID operator=(uint64 value);

        /*! \returns true if uuid is valid/initialized
         */
        bool isValid() const;

        /*! \returns id as string
         */
        const iaString toString() const;

        /*! \returns value
         */
        operator uint64() const;

    private:
        /*! uuid value
         */
        uint64 _value = IGOR_INVALID_ID;
    };

    /*! print uuid in console

    \param stream the stream to write to
    \param uuid the uuid to write
    \returns stream it self
    */
    IAUX_API std::wostream &operator<<(std::wostream &stream, const iaUUID &uuid);

    /*! print list of uuid in console

    \param stream the stream to write to
    \param uuids the uuids to write
    \returns stream it self
    */
   IAUX_API std::wostream &operator<<(std::wostream &stream, const std::vector<iaUUID> &uuids);
}

/*! so can be used as key in a map
 */
template <>
struct std::hash<iaux::iaUUID>
{
    std::size_t operator()(const iaux::iaUUID &uuid) const
    {
        return static_cast<std::size_t>(uuid);
    }
};

#endif // IAUX_UUID_H