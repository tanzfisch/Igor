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

#ifndef __IGOR_CLIP__
#define __IGOR_CLIP__

#include <igor/iDefines.h>

#include <iaux/math/iaEasing.h>
#include <iaux/system/iaTime.h>
using namespace iaux;

#include <vector>

namespace igor
{

    /*! clip to time an animation
     */
    class IGOR_API iClip
    {

    public:
        /*! does nothing
         */
        iClip() = default;

        /*! does nothing
         */
        ~iClip() = default;

        /*! sets start time of evaluation

        \param start the start time for evaluation
        */
        void setStart(const iaTime &start);

        /*! \returns start time of evaluation
         */
        const iaTime &getStart() const;

        /*! sets stop time of evaluation

        \param stop the stop time for evaluation
        */
        void setStop(const iaTime &stop);

        /*! \returns stop time of evaluation
         */
        const iaTime &getStop() const;

        /*! sets stop based on current start plus duration

        \param duration the duration to be set
        */
        void setDuration(const iaTime &duration);

        /*! \returns duration
         */
        iaTime getDuration() const;

        /*! sets the interpolation function

        only has an effect on deriving classes that respect the easing function like iClipTransform

        \param interpolationFunction the interpolation function type to set
        */
        void setEasingFunction(iaEasing::iaEasingFunction interpolationFunction);

        /*! \returns interpolation function type
         */
        iaEasing::iaEasingFunction getEasingFunction() const;

        /*! sets if this evaluation runs in an endless loop
         */
        void setLooped(bool loop = true);

        /*! \returns true if evaluation is in loop mode
         */
        bool isLooped() const;

        /*! \returns value generally between 0.0-1.0 

        Where 0.0 represents the starts and 1.0 the stop time
        If the time given is before start the returns value is 0.0
        If the time given is after stop the returns value is 1.0

        \param time the time given
        */
        float64 getNormalizedTime(const iaTime &time);

    private:
        /*! interpolation function
         */
        iaEasing::iaEasingFunction _easingFunction = iaEasing::iaEasingFunction::Linear;

        /*! start time of evaluation
         */
        iaTime _start = iaTime::fromSeconds(0.0);

        /*! stop time of evaluation
         */
        iaTime _stop = iaTime::fromSeconds(1.0);

        /*! amplitude of elastic easing function
         */
        float64 _amplitude = 0.5;

        /*! period of elastic easing function
         */
        float64 _period = 0.25;

        /*! overshoot of back easing function
         */
        float64 _overshoot = 1.70158;

        /*! looped flag
         */
        bool _looped = false;
    };

    /*! evaluation pointer definition
     */
    typedef iClip *iClipPtr;

} // namespace igor

#endif // __IGOR_CLIP__
