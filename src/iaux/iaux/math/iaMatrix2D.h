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

#ifndef __IAUX_MATRIX2D__
#define __IAUX_MATRIX2D__

#include <iaux/math/iaVector2.h>
#include <iaux/math/iaVector3.h>

#include <fstream>
#include <cmath>
#include <ostream>
#include <iomanip>

namespace iaux
{

    /*! 3x3 Matrix
	*/
    template <class T>
    class IAUX_API_EXPORT_ONLY iaMatrix2D
    {
    public:
        /*! internal data
		*/
        T _m[9];

        /*! addition
		*/
        void operator+=(iaMatrix2D &a);

        /*! subtraction
		*/
        void operator-=(iaMatrix2D &a);

        /*! comparison of two matrices

		\param a the second matrix to compare this matrix with
		\returns true if matrices are equal
		*/
        bool operator==(const iaMatrix2D<T> &a) const;

        /*! negated comparison of two matrices

		\param a the second matrix to compare this matrix with
		\returns true if matrices are not equal
		*/
        bool operator!=(const iaMatrix2D<T> &a) const;

        /*! matrix vector2 multiplication
		*/
        iaVector2<T> operator*(iaVector2<T> &a);

        /*! matrix vector3 multiplication
		*/
        iaVector3<T> operator*(iaVector3<T> &a);

        /*! matrix multiplication
		*/
        iaMatrix2D operator*(iaMatrix2D &m);

        /*! matrix multiplication
		*/
        void operator*=(iaMatrix2D &m);

        /*! matrix component by index

		\param i only indexes from 0 to 8 are allowed
		*/
        T &operator[](const int i);

        /*! initializes matrix with id matrix
		*/
        void identity();

        /*! returns the determinant of the 3x3 part of the matrix
		*/
        T determinant();

        /*! calculates the inverse of the matrix and overwrites the original value
		*/
        void transpose();

        /*! translate the matrix by vector dd

		\param a translation vector
		*/
        void translate(const iaVector2<T> &a);

        /*! translate the matrix by vector 2d

		\param x x component
		\param y y component
		*/
        void translate(T x, T y);

        /*! scale matrix by vector

		\param s scale vector
		*/
        void scale(iaVector2<T> &s);

        /*! scale matrix by vector

		\param x x component of vector
		\param y y component of vector
		*/
        void scale(T x, T y);

        /*! rotates the matrix

		\param angle angle to rotate in radians
		*/
        void rotate(T angle);

        /*! returns pointer to the data
		*/
        const T *getData() const;

        /*! returns pointer to the data
		*/
        T *getData();

        /*! set data
		*/
        void setData(T *data);

        /*! converts the vector in to given type of vector

		\returns matrix for given type
		*/
        template <class T2>
        iaMatrix2D<T2> convert();

        /*! initializes the matrix with the id matrix
		*/
        iaMatrix2D();

        /*! does nothing
		*/
        ~iaMatrix2D();
    };

#include <iaux/math/iaMatrix2D.inl>

    /*! float32 3x3 matrix
	*/
    typedef iaMatrix2D<float32> iaMatrix3x3f;

    /*! float64 3x3 matrix
	*/
    typedef iaMatrix2D<float64> iaMatrix3x3d;

}; // namespace iaux

#endif // __IAUX_MATRIX2D__
