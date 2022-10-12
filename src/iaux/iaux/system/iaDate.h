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
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __IAUX_DATE__
#define __IAUX_DATE__

#include <iaux/iaDefines.h>

namespace iaux
{

    /*! stores a date
     */
    class IAUX_API iaDate
    {
    public:

        /*! ctor
        */
        iaDate() = default;

        /*! ctor date

        \param day the day of the date
        \param month the month of the date
        \param year the year of the date
        */
        iaDate(uint32 day, uint32 month, uint32 year);

        /*! copy ctor
        */
        iaDate(const iaDate &other);

        /*! \returns todays date
        */
        static iaDate getToday();

        /*! \returns day
         */
        uint32 getDay() const;

        /*! \returns month
         */
        uint32 getMonth() const;

        /*! \returns year
         */
        uint32 getYear() const;

    private:

        /*! the day
        */
        uint32 _day = 0;

        /*! the month
        */
        uint32 _month = 0;

        /*! the year
        */
        uint32 _year = 0;
    };


} // namespace iaux

#endif // __IAUX_TIME__
