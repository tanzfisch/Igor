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

#ifndef __IAUX_RANDOM__
#define __IAUX_RANDOM__

#include <iaux/math/iaRandomNumberGenerator.h>

namespace iaux
{

    /*! wrapper for one random number generator 

    if you need individual random number generators use iaRandomNumberGenerator instead of iaRandom
    */
    class IAUX_API iaRandom
    {
    public:
        /*! sets the seed

        \param seed the seed
        */
        static void setSeed(uint64 seed);

        /*! \returns next random number
        */
        static uint64 getNext();

        /*! \returns next random number [min, max]

        \param min the min value returned
        \param max the max value returned
        */
        static int64 getNextRange(int64 min, int64 max);

        /*! \returns next random number with exponential distribution [min, max]

        high chances towards min and lower chances towards max

        \param min the min value returned
        \param max the max value returned
        \param lambda this value determines where the median of the distribution curve lies before it gets scaled back to the min max range
        */
        static int64 getNextRangeExponentialDecrease(int64 min, int64 max, float64 lambda = 2.0);          

        /*! \returns next random number with exponential distribution [min, max]

        high chances towards min and lower chances towards max

        \param min the min value returned
        \param max the max value returned
        \param lambda this value determines where the median of the distribution curve lies before it gets scaled back to the min max range
        */
        static int64 getNextRangeExponentialIncrease(int64 min, int64 max, float64 lambda = 2.0);              

        /*! \returns next random number [0, range-1]

        \param range the range of values
        */
        static int64 getNextRange(int64 range);

        /*! \returns next random number as float 0.0-1.0
        */
        static float64 getNextFloat();

        /*! \returns next random number as float min-max

        \param min the min value returned
        \param max the max value returned
        */
        static float64 getNextFloatRange(float64 min, float64 max);
    };

} // namespace iaux

#endif // __IAUX_RANDOM__
