/*! This is a port from robertpenner.com/easing/

// Disclaimer for Robert Penner's Easing Equations license:
// TERMS OF USE - EASING EQUATIONS
// Open source under the BSD License.
// Copyright � 2001 Robert Penner
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted provided 
// that the following conditions are met :
// * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
//   in the documentation and /or other materials provided with the distribution.
// * Neither the name of the author nor the names of contributors may be used to endorse or promote products derived from this 
//   software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT
// SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef __ROBERTS_EASING_FUNCTIONS__
#define __ROBERTS_EASING_FUNCTIONS__

#include <iaux/iaDefines.h>

#include <cmath>

namespace iaux
{

    namespace iaEasing
    {

        /*! interpolation function type definitions
        */
        enum class iaEasingFunction
        {
            Linear,

            InSine,
            OutSine,
            InOutSine,
            OutInSine,

            InQuad,
            OutQuad,
            InOutQuad,
            OutInQuad,

            InCubic,
            OutCubic,
            InOutCubic,
            OutInCubic,

            InQuart,
            OutQuart,
            InOutQuart,
            OutInQuart,

            InQuint,
            OutQuint,
            InOutQuint,
            OutInQuint,

            InExpo,
            OutExpo,
            InOutExpo,
            OutInExpo,

            InCirc,
            OutCirc,
            InOutCirc,
            OutInCirc,

            /*InBack,
            OutBack,
            InOutBack,
            OutInBack,

            InElastic,
            OutElastic,
            InOutElastic,
            OutInElastic,*/

            InBounce,
            OutBounce,
            InOutBounce,
            OutInBounce
        };

        /*!
        t = elapsed time
        b = begin
        c = change == ending - beginning
        d = duration (total time)
        s = shift or overshoot
        a = amplitude
        p = period (0-1)
        */

        IGOR_INLINE float64 inSine(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            return -c * std::cos(t / d * (M_PI_2)) + c + b;
        }

        IGOR_INLINE float64 outSine(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            return c * std::sin(t / d * (M_PI_2)) + b;
        }

        IGOR_INLINE float64 inOutSine(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            return -c * 0.5 * (std::cos(M_PI * t / d) - 1.0) + b;
        }

        IGOR_INLINE float64 outInSine(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            if (t < d / 2)
                return outSine(t * 2.0, b, c * 0.5, d);
            else
                return inSine((t * 2.0) - d, b + c * 0.5, c * 0.5, d);
        }

        IGOR_INLINE float64 inQuad(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            t = t / d;
            return c * std::pow(t, 2.0) + b;
        }

        IGOR_INLINE float64 outQuad(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            t = t / d;
            return -c * t * (t - 2.0) + b;
        }

        IGOR_INLINE float64 inOutQuad(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            t = t / d * 2.0;
            if (t < 1)
                return c * 0.5 * std::pow(t, 2.0) + b;
            else
                return -c * 0.5 * ((t - 1.0) * (t - 3.0) - 1.0) + b;
        }

        IGOR_INLINE float64 outInQuad(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            if (t < d * 0.5)
                return outQuad(t * 2.0, b, c * 0.5, d);
            else
                return inQuad((t * 2.0) - d, b + c * 0.5, c * 0.5, d);
        }

        IGOR_INLINE float64 inCubic(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            t = t / d;
            return c * std::pow(t, 3.0) + b;
        }

        IGOR_INLINE float64 outCubic(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            t = t / d - 1.0;
            return c * (std::pow(t, 3.0) + 1.0) + b;
        }

        IGOR_INLINE float64 inOutCubic(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            t = t / d * 2.0;
            if (t < 1)
                return c * 0.5 * t * t * t + b;
            else
                t = t - 2.0;

            return c * 0.5 * (t * t * t + 2.0) + b;
        }

        IGOR_INLINE float64 outInCubic(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            if (t < d * 0.5)
                return outCubic(t * 2.0, b, c * 0.5, d);
            else
                return inCubic((t * 2.0) - d, b + c * 0.5, c * 0.5, d);
        }

        IGOR_INLINE float64 inQuart(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            t = t / d;
            return c * std::pow(t, 4.0) + b;
        }

        IGOR_INLINE float64 outQuart(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            t = t / d - 1.0;
            return -c * (std::pow(t, 4.0) - 1.0) + b;
        }

        IGOR_INLINE float64 inOutQuart(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            t = t / d * 2.0;
            if (t < 1)
                return c * 0.5 * std::pow(t, 4.0) + b;
            else
                t = t - 2.0;

            return -c * 0.5 * (std::pow(t, 4.0) - 2.0) + b;
        }

        IGOR_INLINE float64 outInQuart(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            if (t < d * 0.5)
                return outQuart(t * 2.0, b, c * 0.5, d);
            else
                return inQuart((t * 2.0) - d, b + c * 0.5, c * 0.5, d);
        }

        IGOR_INLINE float64 inQuint(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            t = t / d;
            return c * std::pow(t, 5.0) + b;
        }

        IGOR_INLINE float64 outQuint(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            t = t / d - 1.0;
            return c * (std::pow(t, 5.0) + 1.0) + b;
        }

        IGOR_INLINE float64 inOutQuint(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            t = t / d * 2.0;
            if (t < 1)
                return c * 0.5 * std::pow(t, 5.0) + b;
            else
                t = t - 2.0;

            return c * 0.5 * (std::pow(t, 5.0) + 2.0) + b;
        }

        IGOR_INLINE float64 outInQuint(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            if (t < d / 2)
                return outQuint(t * 2.0, b, c * 0.5, d);
            else
                return inQuint((t * 2.0) - d, b + c * 0.5, c * 0.5, d);
        }

        IGOR_INLINE float64 inExpo(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            if (t == 0)
                return b;
            else
                return c * std::pow(2.0, 10.0 * (t / d - 1.0)) + b - c * 0.001;
        }

        IGOR_INLINE float64 outExpo(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            if (t == d)
                return b + c;
            else
                return c * 1.001 * (-std::pow(2.0, -10.0 * t / d) + 1.0) + b;
        }

        IGOR_INLINE float64 inOutExpo(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            if (t == 0)
                return b;

            if (t == d)
                return b + c;

            t = t / d * 2.0;

            if (t < 1)
                return c * 0.5 * std::pow(2.0, 10.0 * (t - 1.0)) + b - c * 0.0005;
            else
                t = t - 1.0;

            return c * 0.5 * 1.0005 * (-std::pow(2.0, -10.0 * t) + 2.0) + b;
        }

        IGOR_INLINE float64 outInExpo(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            if (t < d * 0.5)
                return outExpo(t * 2.0, b, c * 0.5, d);
            else
                return inExpo((t * 2.0) - d, b + c * 0.5, c * 0.5, d);
        }

        IGOR_INLINE float64 inCirc(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            t = t / d;
            return(-c * (std::sqrt(1.0 - std::pow(t, 2.0)) - 1.0) + b);
        }

        IGOR_INLINE float64 outCirc(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            t = t / d - 1.0;
            return(c * std::sqrt(1.0 - std::pow(t, 2.0)) + b);
        }

        IGOR_INLINE float64 inOutCirc(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            t = t / d * 2.0;

            if (t < 1)
            {
                return -c * 0.5 * (std::sqrt(1.0 - t * t) - 1.0) + b;
            }
            else
            {
                t = t - 2.0;
            }

            return c * 0.5 * (std::sqrt(1.0 - t * t) + 1.0) + b;
        }

        IGOR_INLINE float64 outInCirc(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            if (t < d * 0.5)
            {
                return outCirc(t * 2.0, b, c * 0.5, d);
            }
            else
            {
                return inCirc((t * 2.0) - d, b + c * 0.5, c * 0.5, d);
            }
        }

        /*IGOR_INLINE float64 inBack(float64 t, float64 b = 0, float64 c = 1, float64 d = 1, float64 s = 1.70158)
        {
            t = t / d;
            return c * t * t * ((s + 1.0) * t - s) + b;
        }

        IGOR_INLINE float64 outBack(float64 t, float64 b = 0, float64 c = 1, float64 d = 1, float64 s = 1.70158)
        {
            t = t / d - 1.0;
            return c * (t * t * ((s + 1.0) * t + s) + 1.0) + b;
        }

        IGOR_INLINE float64 inOutBack(float64 t, float64 b = 0, float64 c = 1, float64 d = 1, float64 s = 1.70158)
        {
            s = s * 1.525;
            t = t / d * 2.0;

            if (t < 1)
            {
                return c * 0.5 * (t * t * ((s + 1.0) * t - s)) + b;
            }
            else
            {
                t = t - 2.0;
            }

            return c * 0.5 * (t * t * ((s + 1.0) * t + s) + 2.0) + b;
        }

        IGOR_INLINE float64 outInBack(float64 t, float64 b = 0, float64 c = 1, float64 d = 1, float64 s = 1.70158)
        {
            if (t < d * 0.5)
            {
                return outBack(t * 2.0, b, c * 0.5, d, s);
            }
            else
            {
                return inBack((t * 2.0) - d, b + c * 0.5, c * 0.5, d, s);
            }
        }

        IGOR_INLINE float64 inElastic(float64 t, float64 b = 0, float64 c = 1, float64 d = 1, float64 a = 0.5, float64 p = 0.25)
        {
            if (t == 0)
                return b;

            t = t / d;

            if (t == 1)
                return b + c;

            if (p == 0)
                p = d * 0.3;

            float64 s;

            if ((a == 0) || (a < std::abs(c)))
            {
                a = c;
                s = p * 0.25;
            }
            else
            {
                s = p / (2.0 * M_PI) * std::asin(c / a);
            }

            t = t - 1.0;

            return -(a * std::pow(2.0, 10.0 * t) * std::sin((t * d - s) * (2.0 * M_PI) / p)) + b;
        }

        IGOR_INLINE float64 outElastic(float64 t, float64 b = 0, float64 c = 1, float64 d = 1, float64 a = 0.5, float64 p = 0.25)
        {
            if (t == 0)
                return b;

            t = t / d;

            if (t == 1)
                return b + c;

            if (p == 0)
                p = d * 0.3;

            float64 s;

            if ((a == 0) || (a < std::abs(c)))
            {
                a = c;
                s = p * 0.25;
            }
            else
            {
                s = p / (2.0 * M_PI) * std::asin(c / a);
            }

            return a * std::pow(2.0, -10.0 * t) * std::sin((t * d - s) * (2.0 * M_PI) / p) + c + b;
        }

        IGOR_INLINE float64 inOutElastic(float64 t, float64 b = 0, float64 c = 1, float64 d = 1, float64 a = 0.5, float64 p = 0.25)
        {
            if (t == 0)
                return b;

            t = t / d * 2.0;

            if (t == 2)
                return b + c;

            if (p == 0)
                p = d * (0.3 * 1.5);

            float64 s;

            if (a < std::abs(c))
            {
                a = c;
                s = p * 0.25;
            }
            else
            {
                s = p / (2.0 * M_PI) * std::asin(c / a);
            }

            if (t < 1)
            {
                t = t - 1.0;
                return -0.5 * (a * std::pow(2.0, 10.0 * t) * std::sin((t * d - s) * (2.0 * M_PI) / p)) + b;
            }
            else
            {
                t = t - 1.0;
                return a * std::pow(2.0, -10.0 * t) * std::sin((t * d - s) * (2.0 * M_PI) / p) * 0.5 + c + b;
            }
        }

        IGOR_INLINE float64 outInElastic(float64 t, float64 b = 0, float64 c = 1, float64 d = 1, float64 a = 0.5, float64 p = 0.25)
        {
            if (t < d * 0.5)
            {
                return outElastic(t * 2.0, b, c * 0.5, d, a, p);
            }
            else
            {
                return inElastic((t * 2.0) - d, b + c * 0.5, c * 0.5, d, a, p);
            }
        }*/

        IGOR_INLINE float64 outBounce(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            t = t / d;
            if (t < 1 / 2.75)
            {
                return c * (7.5625 * t * t) + b;
            }
            else if (t < 2 / 2.75)
            {
                t = t - (1.5 / 2.75);
                return c * (7.5625 * t * t + 0.75) + b;
            }
            else if (t < 2.5 / 2.75)
            {
                t = t - (2.25 / 2.75);
                return c * (7.5625 * t * t + 0.9375) + b;
            }
            else
            {
                t = t - (2.625 / 2.75);
                return c * (7.5625 * t * t + 0.984375) + b;
            }
        }

        IGOR_INLINE float64 inBounce(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            return c - outBounce(d - t, 0.0, c, d) + b;
        }

        IGOR_INLINE float64 inOutBounce(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            if (t < d * 0.5)
            {
                return inBounce(t * 2.0, 0.0, c, d) * 0.5 + b;
            }
            else
            {
                return outBounce(t * 2.0 - d, 0.0, c, d) * 0.5 + c * 0.5 + b;
            }
        }

        IGOR_INLINE float64 outInBounce(float64 t, float64 b = 0, float64 c = 1, float64 d = 1)
        {
            if (t < d * 0.5)
            {
                return outBounce(t * 2.0, b, c * 0.5, d);
            }
            else
            {
                return inBounce((t * 2.0) - d, b + c * 0.5, c * 0.5, d);
            }
        }
    }
}

#endif // __ROBERTS_EASING_FUNCTIONS__
