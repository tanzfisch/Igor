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
//    (c) Copyright 2012-2025 by Martin Loga
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

#ifndef __IAUX_CIRCLE__
#define __IAUX_CIRCLE__

#include <iaux/math/iaVector2.h>

namespace iaux
{

    /*! 2d circle
     */
    template <class T>
    class IAUX_API_EXPORT_ONLY iaCircle
    {

    public:
        /*! does nothing
         */
        iaCircle() = default;

        /*! ctor with parameters

        \param x horizontal center position
        \param y vertical center position
        \param radius radius
        */
        iaCircle(T x, T y, T radius = 1.0)
        {
            _center.set(x, y);
            _radius = radius;
        }

        /*! ctor with parameters

        \param center center position
        \param radius radius
        */
        iaCircle(const iaVector2<T> &center, T radius = static_cast<T>(1.0))
        {
            _center = center;
            _radius = radius;
        }

        /*! copy ctor

        \param circle the circle to copy
        */
        iaCircle(const iaCircle<T> &circle)
        {
            _center = circle._center;
            _radius = circle._radius;
        }

        /*! \returns horizontal position
         */
        const T getX() const;

        /*! \returns vertical position
         */
        const T getY() const;

        /*! \returns rectangle width
         */
        const T getRadius() const;

        /*! \returns center of circle
         */
        const iaVector2<T> &getCenter() const;

        /*! sets horizontal position

        \param x horizontal position
        */
        void setX(T x);

        /*! sets vertical position

        \param y vertical position
        */
        void setY(T y);

        /*! sets radius of circle

        \param radius radius of circle
        */
        void setRadius(T radius);

        /*! sets center of circle

        \param center the new center of the circle
         */
        void setCenter(const iaVector2<T> &center);

        /*! set with parameters

        \param x horizontal center position
        \param y vertical center position
        \param radius radius
        */
        void set(T x, T y, T radius);

        /*! set with parameters

        \param center center position
        \param radius radius
        */
        void set(const iaVector2<T> &center, T radius);

        /*! assignment operator

        \param circle the circle to assign
        \returns resulting circle
        */
        iaCircle<T> operator=(const iaCircle<T> &circle);

        /*! \returns true if two circles are equal

        \param circle the circle to compare with
        */
        bool operator==(const iaCircle<T> &circle) const;

        /*! \returns true if circle are not equal

        \param circle the circle to compare with
        */
        bool operator!=(const iaCircle<T> &circle) const;

        /*! center position
         */
        iaVector2<T> _center;

        /*! radius of circle
         */
        T _radius = static_cast<T>(1.0);
    };

#include <iaux/data/iaCircle.inl>

    /*! float32 precision of rectangle
     */
    typedef iaCircle<float32> iaCirclef;

    /*! float64 precision of rectangle
     */
    typedef iaCircle<float64> iaCircled;

    /*! int32 precision of rectangle
     */
    typedef iaCircle<int32> iaCirclei;

    /*! int64 precision of rectangle
     */
    typedef iaCircle<int64> iaCircleI;
    
}; // namespace igor

#endif // __IAUX_RECTANGLE__
