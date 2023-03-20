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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IGOR_PERLINNOISE_H__
#define __IGOR_PERLINNOISE_H__

#include <igor/iDefines.h>

#include <iaux/math/iaVector3.h>
#include <iaux/math/iaRandomNumberGenerator.h>
using namespace iaux;

namespace igor
{

    /*! perlin noise

    most of the implementation comes from the folowing sources
    source: http://flafla2.github.io/2014/08/09/perlinnoise.html
    source: https://gist.github.com/Flafla2/f0260a861be0ebdeef76
    Thanks Adrian Biagioli
    */
    class IGOR_API iPerlinNoise
    {

    public:
        /*! makes pseudo infinite access possible
        */
        static const int64 RANDOM_NUMBERS_COUNT = 256;

        /*! generates pseudo random numbers

        \param seed the seed used for generating the random numbers
        */
        void generateBase(uint32 seed);

        /*! \returns noise value at specified position

        3d version

        \param pos specified position
        */
        float64 getValue(const iaVector3d &pos);

        /*! \returns noise value at specified position

        1d version

        \param pos specified position
        */
        float64 getValue(const float64 pos);

        /*! \returns noise value of multiple octaves at specified position in a range of 0.0-1.0

        \param pos specified position
        \param octaves octaves count
        \param persistence multiplicator of amplitude between each octave (default 0.5)
        */
        float64 getValue(const iaVector3d &pos, int64 octaves, float64 persistence = 0.5);

        /*! \returns noise value of multiple octaves at specified position in a range of 0.0-1.0

        \param pos specified position
        \param octaves octaves count
        \param persistence multiplicator of amplitude between each octave (default 0.5)
        */
        float64 getValue(const float64 pos, int64 octaves, float64 persistence = 0.5);

        /*! generates random numbers
        */
        iPerlinNoise();

        /*! does nothing
        */
        ~iPerlinNoise() = default;

    private:
        /*! buffer with random numbers

        double the size for simpler access code
        */
        uint8 p[RANDOM_NUMBERS_COUNT * 2];

        /*! random number generator
        */
        iaRandomNumberGenerator _rand;

        /*! linear interpolation between two values

        \param a first value
        \param b second value
        \param x interpolation factor
        */
        __IGOR_INLINE__ float64 lerp(float64 a, float64 b, float64 x);

        /*! increments value by one inclusive wrapping at random number buffer boundaries

        \param num number to increment
        \returns increased number
        */
        __IGOR_INLINE__ int64 inc(int64 num);

        /*! smooth fading function

        \returns fading value used for linear interpolation
        \param t value between 0.0 - 1.0
        */
        __IGOR_INLINE__ float64 fade(float64 t);

        /*! lookup gradient for specified hash number (comes from pseudo random number buffer)

        \param hash the lookup hash
        \param x x position within unit cube
        \param y y position within unit cube
        \param z z position within unit cube
        \returns gradient
        */
        __IGOR_INLINE__ float64 grad(int64 hash, float64 x, float64 y, float64 z);
    };

#include <igor/generation/iPerlinNoise.inl>

} // namespace igor

#endif // __IGOR_PERLINNOISE3D_H__