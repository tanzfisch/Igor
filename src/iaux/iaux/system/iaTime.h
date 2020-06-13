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
// contact: martinloga@gmx.de

#ifndef __IAUX_TIME__
#define __IAUX_TIME__

#include <iaux/iaDefines.h>

#include <iostream>

namespace iaux
{

    /*! stores time
    */
    class IgorAux_API iaTime
    {

    public:
        // init with time zero
        iaTime() = default;

        // copy constructor
        iaTime(const iaTime &other);

        /*! init with micro seconds
        \param microSeconds time in micro seconds
        */
        iaTime(int64 microSeconds);

        /*! \returns current time
        */
        static iaTime now();

        /*! \returns zero time
        */
        static iaTime zero();

        /*! \returns time object based on given seconds

        \param time time in seconds
        */
        static iaTime fromSeconds(float64 time);

        /*! \returns time object based on given milli seconds

        \param time time in milli seconds
        */
        static iaTime fromMilliseconds(float64 time);

        /*! adds time given time to this time

        \param time given time
        */
        void operator+=(const iaTime &time);

        /*! adds time given time to this time and returns result

        \param time given time
        \returns new time
        */
        iaTime operator+(const iaTime &time) const;

        /*! subtracts time given time from this time

        \param time given time
        */
        void operator-=(const iaTime &time);

        /*! subtracts time given time from this time and returns result

        \param time given time
        \returns new time
        */
        iaTime operator-(const iaTime &time) const;

        /*! multiplies this time with scalar 

        \param scalar the value to multiply the time with
        */
        void operator*=(float64 scalar);

        /*! multiplies time with scalar

        \param scalar the value to multiply the time with
        \returns the new time
        */
        iaTime operator*(float64 scalar) const;

        /*! \returns true if both times are equal
        */
        bool operator==(const iaTime &time) const;

        /*! \returns true if both times are not equal
        */
        bool operator!=(const iaTime &time) const;

        /*! \returns true if this time is smaller then given time

        \param time the given time
        */
        bool operator<(const iaTime &time) const;

        /*! \returns true if this time is greater then given time

        \param time the given time
        */
        bool operator>(const iaTime &time) const;

        /*! \returns true if this time is smaller or equal then given time

        \param time the given time
        */
        bool operator<=(const iaTime &time) const;

        /*! \returns true if this time is greater or equal then given time

        \param time the given time
        */
        bool operator>=(const iaTime &time) const;

        /*! \returns time in micro seconds
        */
        int64 getMicrosenconds() const;

        /*! \returns time in seconds
        */
        float64 getSeconds() const;

        /*! \returns time in milliseconds
        */
        float64 getMilliseconds() const;

    private:
        // time in micro seconds
        int64 _microSeconds = 0;
    };

    /*! stream operator e.g. for cosole output

    \param ostr the out stream handle
    \param time the time to print in stream
    */
    __IGOR_INLINE__ std::wostream &operator<<(std::wostream &ostr, const iaTime &time)
    {
        if (time.getSeconds() > 1.0)
        {
            ostr << time.getSeconds() << L"s";
        }
        else
        {
            ostr << time.getMilliseconds() << L"ms";
        }
        return ostr;
    } // namespace iaux

} // namespace iaux

#endif // __IAUX_TIME__
