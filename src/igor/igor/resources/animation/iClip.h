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

#include <igor/resources/animation/iAnimation.h>

#include <iaux/math/iaEasing.h>
using namespace iaux;

#include <vector>

namespace igor
{
    class iClip;

    /*! evaluation pointer definition
     */
    typedef std::shared_ptr<iClip> iClipPtr;

    /*! clip to time an animation
     */
    class IGOR_API iClip
    {

    public:
        /*! create clip with given duration

        \param duration the duration of the clip
        \param animations list of animations to bind to this clip
        \param looped if true clip is looped
        \param randomStart if true clip starts with a random offset
         */
        static iClipPtr createClip(const iaTime &duration = iaTime::fromSeconds(1.0),
                                   const std::vector<iAnimationPtr> &animations = std::vector<iAnimationPtr>(),
                                   bool looped = true, bool randomStart = false);

        /*! does nothing
         */
        ~iClip() = default;

        /*! adds animation to this clip

        \param animation the to add
        */
        void addAnimation(iAnimationPtr animation);

        /*! removes animation from this clip

        \param animation the to remove
        */
        void removeAnimation(iAnimationPtr animation);

        /*! \returns animations under this clip
         */
        const std::vector<iAnimationPtr> &getAnimations() const;

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

        /*! sets random start time flag

        This flag can be used to have an animation playback with a random offset so copies of an animation don't appear too artificially synchronous
        */
        void setRandomStart(bool randomStart);

        /*! \returns true if random start is active
         */
        bool hasRandomStart() const;

        /*! \returns value generally between 0.0-1.0

        Where 0.0 represents the start and 1.0 the stop time
        If the time given is before start the returns value is 0.0
        If the time given is after stop the returns value is 1.0

        \param startTime the time this clip was started to playback
        \param time the time we want to normalize
        */
        float64 getNormalizedTime(const iaTime &startTime, const iaTime &time);

    private:
        /*! interpolation function
         */
        iaEasing::iaEasingFunction _easingFunction = iaEasing::iaEasingFunction::Linear;

        /*! duration time of evaluation
         */
        iaTime _duration = iaTime::fromSeconds(1.0);

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
        bool _looped = true;

        /*! random start flag
         */
        bool _randomStart = false;

        /*! animation under this clip
         */
        std::vector<iAnimationPtr> _animations;

        /*! init members

        \param duration the duration of the clip
         */
        iClip(const iaTime &duration);
    };

} // namespace igor

#endif // __IGOR_CLIP__
