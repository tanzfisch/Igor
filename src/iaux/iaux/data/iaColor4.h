//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/                     ) )     ((
//                                           (_(       \)
//    (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __IAUX_COLOR4__
#define __IAUX_COLOR4__

#include <iaux/iaDefines.h>

#include <fstream>
#include <algorithm>
#include <math.h>

namespace iaux
{

    /*! color class the represents the red, green, blue and alpha channel

    for floating point variant we expect values in range from 0.0 - 1.0
    for integer variant we expect values in range from 0.0 - 255.0

    \todo move code in to inline file
    */
    template <class T>
    class IgorAux_API_Template iaColor4
    {
    public:
        /*! red color channel
        */
        T _r;

        /*! green color channel
        */
        T _g;

        /*! blue color channel
        */
        T _b;

        /*! alpha color channel
        */
        T _a;

        /*! ctor initialize member variables
        */
        iaColor4()
        {
            _r = static_cast<T>(0);
            _g = static_cast<T>(0);
            _b = static_cast<T>(0);
            _a = static_cast<T>(0);
        }

        /*! ctor initialize member variables with parameters

        \param r red component
        \param g green component
        \param b blue component
        \param a alpha component
        */
        iaColor4(T r, T g, T b, T a)
        {
            _r = r;
            _g = g;
            _b = b;
            _a = a;
        }

        /*! set member variables with parameters

        \param r red component
        \param g green component
        \param b blue component
        \param a alpha component
        */
        void set(T r, T g, T b, T a)
        {
            _r = r;
            _g = g;
            _b = b;
            _a = a;
        }

        /*! \returns pointer to data
        */
        T *getData()
        {
            return &_r;
        }

        /*! assignment operator with other color

        \param color the color to assign
        */
        __IGOR_INLINE__ iaColor4<T> operator=(const iaColor4<T> &color)
        {
            iaColor4<T> result;
            result._r = _r = color._r;
            result._g = _g = color._g;
            result._b = _b = color._b;
            result._a = _a = color._a;

            return result;
        }

        /*! add operator

        \param color the color to add
        */
        __IGOR_INLINE__ void operator+=(const iaColor4<T> &color)
        {
            _r += color._r;
            _g += color._g;
            _b += color._b;
            _a += color._a;
        }

        /*! subtract operator

        \param color the color to subtract
        */
        __IGOR_INLINE__ void operator-=(const iaColor4<T> &color)
        {
            _r -= color._r;
            _g -= color._g;
            _b -= color._b;
            _a -= color._a;
        }

        /*! scale operator

        \param factor the value to scale with
        */
        void operator*=(T factor)
        {
            _r *= factor;
            _g *= factor;
            _b *= factor;
            _a *= factor;
        }

        /*! linear interpolation between two colors with a factor

        \param color1 first color
        \param color2 second color
        \param w factor to interpolate with
        */
        void lerp(iaColor4<T> &color1, iaColor4<T> &color2, float32 w)
        {
            _r = color1._r * w + color2._r * (1.0f - w);
            _g = color1._g * w + color2._g * (1.0f - w);
            _b = color1._b * w + color2._b * (1.0f - w);
            _a = color1._a * w + color2._a * (1.0f - w);
        }

        /*! converts RGB to HSV

        \param rgb the rgb color
        \returns hsv color
        */
        static iaColor4<T> convertRGBtoHSV(iaColor4<T> &rgb)
        {
            iaColor4<T> result;

            bool maxR = false;
            bool maxG = false;
            bool maxB = false;
            T max = 0;

            if (rgb._r > rgb._g)
            {
                if (rgb._r > rgb._b)
                {
                    maxR = true;
                    max = rgb._r;
                }
                else
                {
                    maxB = true;
                    max = rgb._b;
                }
            }
            else
            {
                if (rgb._g > rgb._b)
                {
                    maxG = true;
                    max = rgb._g;
                }
                else
                {
                    maxB = true;
                    max = rgb._b;
                }
            }

            T min = std::min(std::min(rgb._r, rgb._g), rgb._b);
            T delta = max - min;

            if (delta == 0)
            {
                result._r = 0;
            }
            else if (maxR)
            {
                const float64 tmp = static_cast<float64>(((rgb._g - rgb._b) / delta));
                result._r = static_cast<T>(60.0 * fabs(fmod(tmp, 6.0)));
            }
            else if (maxG)
            {
                result._r = static_cast<T>(60.0 * (((rgb._b - rgb._r) / delta) + 2.0));
            }
            else if (maxB)
            {
                result._r = static_cast<T>(60.0 * (((rgb._r - rgb._g) / delta) + 4.0));
            }

            result._r /= 360.0;

            if (max == 0.0)
            {
                result._g = 0.0;
            }
            else
            {
                result._g = delta / max;
            }

            result._b = max;

            result._a = rgb._a;

            return result;
        }

        /*! converts HSV to RGB

        \param hsv the hsv color
        \returns rgb color
        */
        static iaColor4<T> convertHSVtoRGB(iaColor4<T> &hsv)
        {
            iaColor4<T> result;

            T h = hsv._r * static_cast<T>(360.0);
            T s = hsv._g;
            T v = hsv._b;

            T c = v * s;
            T x = c * (1 - static_cast<T>(fabs(fmod((static_cast<double>(h) / 60.0), 2.0) - 1.0)));

            T m = v - c;

            if (h < 60)
            {
                result._r = c;
                result._g = x;
                result._b = 0;
            }
            else if (h < 120)
            {
                result._r = x;
                result._g = c;
                result._b = 0;
            }
            else if (h < 180)
            {
                result._r = 0;
                result._g = c;
                result._b = x;
            }
            else if (h < 240)
            {
                result._r = 0;
                result._g = x;
                result._b = c;
            }
            else if (h < 300)
            {
                result._r = x;
                result._g = 0;
                result._b = c;
            }
            else if (h <= 360)
            {
                result._r = c;
                result._g = 0;
                result._b = x;
            }

            result._r += m;
            result._g += m;
            result._b += m;
            result._a = hsv._a;

            return result;
        }
    };

    /*! stream operator e.g. for cosole output

    \param ostr the out stream handle
    \param color the color to print in stream
    */
    template <class T>
    __IGOR_INLINE__ std::wostream &operator<<(std::wostream &ostr, const iaColor4<T> &color)
    {
        ostr << "(" << color._r << ", " << color._g << ", " << color._b << ", " << color._a << ")";
        return ostr;
    }

    /*! float32 color
    */
    typedef iaColor4<float32> iaColor4f;

    /*! uint8 color
    */
    typedef iaColor4<uint8> iaColor4c;

}; // namespace iaux

#endif