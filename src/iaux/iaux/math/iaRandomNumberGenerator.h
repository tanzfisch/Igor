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
// (c) Copyright 2012-2025 by Martin Loga
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

#ifndef __IAUX_RANDOMNUMBERGENERATOR__
#define __IAUX_RANDOMNUMBERGENERATOR__

#include <iaux/system/iaConsole.h>

#include <random>

namespace iaux
{

    /*! pseudo random number generator
     */
    class IAUX_API iaRandomNumberGenerator
    {

    public:
        /*! init seed
         */
        iaRandomNumberGenerator(uint64 seed = 1337);

        /*! does nothing
         */
        ~iaRandomNumberGenerator() = default;

        /*! sets the seed

        \param seed the seed
        */
        void setSeed(uint64 seed);

        /*! \returns next random number
         */
        uint64 getNext();

        /*! \returns next random number [min, max]

        \param min the min value returned
        \param max the max value returned
        */
        int64 getNextRange(int64 min, int64 max);

        /*! \returns next random number with exponential distribution [min, max]

        high chances towards min and lower chances towards max

        \param min the min value returned
        \param max the max value returned
        \param lambda this value determines where the median of the distribution curve lies before it gets scaled back to the min max range
        */
        int64 getNextRangeExponentialDecrease(int64 min, int64 max, float64 lambda);        

        /*! \returns next random number with exponential distribution [min, max]

        high chances towards max and lower chances towards min

        \param min the min value returned
        \param max the max value returned
        \param lambda this value determines where the median of the distribution curve lies before it gets scaled back to the min max range
        */
        int64 getNextRangeExponentialIncrease(int64 min, int64 max, float64 lambda);         

        /*! \returns next random number [0, range-1]

        \param range the range of values
        */
        int64 getNextRange(int64 range);

        /*! \returns next random number as float 0.0-1.0
         */
        float64 getNextFloat();

        /*! \returns next random number as float min-max

        \param min the min value returned
        \param max the max value returned
        */
        float64 getNextFloatRange(float64 min, float64 max);

    private:

        /*! random number engine
        */
        std::mt19937_64 _engine;

        /*! random number distribution
        */
        std::uniform_int_distribution<uint64> _distribution;

    };

} // namespace iaux

#endif // __IAUX_RANDOMNUMBERGENERATOR__
