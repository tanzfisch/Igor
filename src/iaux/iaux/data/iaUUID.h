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
// (c) Copyright 2012-2022 by Martin Loga
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

#ifndef __IAUX_UUID__
#define __IAUX_UUID__

#include <iaux/data/iaString.h>

namespace iaux
{

    /*! uuid (probably not following any standard but good enough for me)
     */
    class IAUX_API iaUUID
    {
    public:
        /*! does nothing
         */
        iaUUID() = default;

        /*! copy ctor

        \param other the other value
        */
        iaUUID(const iaUUID &other);

        /*! ctor with value

        \param value the value to use as uuid
        */
        iaUUID(const iaString &value);

        /*! \returns new created uuid
         */
        static iaUUID create();

        /*! \returns uuid value
         */
        const iaString &getValue() const;

        /*! \returns true if both uuids are equal

        \param other the other uuid
        */
        bool operator==(const iaUUID &other) const;

        /*! \returns true if uuids are not equal

        \param other the other uuid
        */
        bool operator!=(const iaUUID &other);        

        /*! \returns true if uuid is valid/initialized
         */
        bool isValid() const;

    private:
        /*! uuid value
         */
        iaString _value;
    };

    /*! print uuid in console

    \param stream the stream to write to
    \param uuid the uuid to write
    \returns stream it self
    */
    IAUX_API std::wostream &operator<<(std::wostream &stream, const iaUUID &uuid);

}

/*! so can be used as key in a map
 */
template <>
struct std::hash<iaux::iaUUID>
{
    std::size_t operator()(const iaux::iaUUID &uuid) const
    {
        return static_cast<std::size_t>(uuid.getValue().getHashValue());
    }
};

#endif // __IAUX_UUID__