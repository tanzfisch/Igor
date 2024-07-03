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
//    (c) Copyright 2012-2024 by Martin Loga
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

#ifndef __IGOR_RECTANGLE__
#define __IGOR_RECTANGLE__

#include <igor/iDefines.h>

namespace igor
{

    /*! 2d rectangle
     */
    template <class T>
    class IGOR_API_EXPORT_ONLY iRectangle
    {

    public:
        /*! does nothing
         */
        iRectangle() = default;

        /*! ctor with parameters

        \param x horizontal position
        \param y vertical position
        \param width width
        \param height height
        */
        iRectangle(T x, T y, T width = 0, T height = 0)
        {
            _x = x;
            _y = y;
            _width = width;
            _height = height;
        }

        /*! copy ctor

        \param rect the rectangle to copy
        */
        iRectangle(const iRectangle<T> &rect)
        {
            _x = rect._x;
            _y = rect._y;
            _width = rect._width;
            _height = rect._height;
        }

        /*! adjust the size of the rectangle

        \param x the horizontal position adjustment
        \param y the vertical position adjustment
        \param width the horizontal size adjustment
        \param height the vertical size adjustment

        */
        void adjust(T x, T y, T width, T height);

        /*! \returns horrizonal position
         */
        const T getX() const;

        /*! \returns vertical position
         */
        const T getY() const;

        /*! \returns rectangle width
         */
        const T getWidth() const;

        /*! \returns rectangle height
         */
        const T getHeight() const;

        /*! \returns horrizonal position
         */
        const T getLeft() const;

        /*! \returns horizontal position of right edge
         */
        const T getRight() const;

        /*! \returns vertical position
         */
        const T getTop() const;

        /*! \returns vertical position of bottom edge
         */
        const T getBottom() const;

        /*! \returns center of rectangle
        */
        const iaVector2<T> getCenter() const;

        /*! sets horizontal position

        \param x horizontal position
        */
        void setX(T x);

        /*! sets vertical position

        \param y vertical position
        */
        void setY(T y);

        /*! sets width of rectangle

        \param width width of rectangle
        */
        void setWidth(T width);

        /*! sets height of rectangle

        \param height height of rectangle
        */
        void setHeight(T height);

        /*! sets width and height of rectangle

        \param height height of rectangle
        \param width width of rectangle
        */
        void setSize(T width, T height);

        /*! sets all values at once

        \param x the horizontal position
        \param y the vertical position
        \param width the horizontal size
        \param height the vertical size
        */
        void set(T x, T y, T width, T height);

        /*! assignment operator

        \param rectangle the rectangle to assign
        \returns resulting rectangle
        */
        iRectangle<T> operator=(const iRectangle<T> &rectangle);

        /*! horizontal position
         */
        T _x = (T)0;

        /*! vertical position
         */
        T _y = (T)0;

        /*! height of rectangle
         */
        T _width = (T)0;

        /*! height of rectangle
         */
        T _height = (T)0;
    };

#include <igor/data/iRectangle.inl>

    /*! float32 precision of rectangle
     */
    typedef iRectangle<float32> iaRectanglef;

    /*! float64 precision of rectangle
     */
    typedef iRectangle<float64> iaRectangled;

    /*! int32 precision of rectangle
     */
    typedef iRectangle<int32> iaRectanglei;

    /*! int64 precision of rectangle
     */
    typedef iRectangle<int64> iaRectangleI;
}; // namespace igor

#endif // __IGOR_RECTANGLE__
