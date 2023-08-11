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

#ifndef __IAUX_MATRIX__
#define __IAUX_MATRIX__

#include <iaux/math/iaVector4.h>
#include <iaux/math/iaQuaternion.h>
#include <iaux/system/iaConsole.h>

#include <fstream>
#include <cmath>
#include <iomanip>
#include <iostream>

namespace iaux
{

    /*! right handed 4x4 Matrix 

	data by name

	_right._x | _top._x | _depth._x | _pos.x
	_right._y | _top._y | _depth._y | _pos.y
	_right._z | _top._z | _depth._z | _pos.z
	_w0       | _w1     | _w2       | _w3

	data by index (index equals to position in memory)

	 0 |  4 |  8 | 12
	 1 |  5 |  9 | 13
	 2 |  6 | 10 | 14
	 3 |  7 | 11 | 15

	corresponds to

	0,0 | 1,0 | 2,0 | 3,0
	0,1 | 1,1 | 2,1 | 3,1
	0,2 | 1,2 | 2,2 | 3,2
	0,3 | 1,3 | 2,3 | 3,3

	shear

	1      | 0      | 0     | 0
	z (xy) | 1      | 0     | 0
	y (xz) | x (yz) | 1     | 0
	0      | 0      | 0     | 1

	translate

	1 | 0 | 0 | x
	0 | 1 | 0 | y
	0 | 0 | 1 | z
	0 | 0 | 0 | 1

	scale

	x | 0 | 0 | 0
	0 | y | 0 | 0
	0 | 0 | z | 0
	0 | 0 | 0 | 1

	rotate X

	1 | 0      | 0       | 0
	0 | cos(a) | -sin(a) | 0
	0 | sin(a) | cos(a)  | 0
	0 | 0      | 0       | 1

	rotate Y

	cos(a)  | 0 | sin(a) | 0
	0       | 1 | 0      | 0
	-sin(a) | 0 | cos(a) | 0
	0       | 0 |        | 1

	rotate Z

	cos(a)  | -sin(a) | 0 | 0
	sin(a)  | cos(a)  | 0 | 0
	0       | 0       | 1 | 0
	0       | 0       | 0 | 1

	*/
    template <class T>
    class IAUX_API_EXPORT_ONLY iaMatrix
    {
    public:
        /*! internal data
		*/
        iaVector3<T> _right;
        T _w0 = static_cast<T>(0);
        iaVector3<T> _top;
        T _w1 = static_cast<T>(0);
        iaVector3<T> _depth;
        T _w2 = static_cast<T>(0);
        iaVector3<T> _pos;
        T _w3 = static_cast<T>(0);

        /*! addition
		*/
        void operator+=(iaMatrix &a);

        /*! subtraction
		*/
        void operator-=(iaMatrix &a);

        /*! comparison of two matrices

		\param a the second matrix to compare this matrix with
		\returns true if matrices are equal
		*/
        bool operator==(const iaMatrix<T> &a) const;

        /*! negated comparison of two matrices

		\param a the second matrix to compare this matrix with
		\returns true if matrices are not equal
		*/
        bool operator!=(const iaMatrix<T> &a) const;

        /*! matrix vector3 multiplication
		*/
        iaVector3<T> operator*(iaVector3<T> &a) const;

        /*! matrix vector3 multiplication
		*/
        iaVector3<T> operator*(const iaVector3<T> &a) const;

        /*! matrix vector4 multiplication
		*/
        iaVector4<T> operator*(iaVector4<T> &a) const;

        /*! matrix vector4 multiplication
		*/
        iaVector4<T> operator*(const iaVector4<T> &a) const;

        /*! matrix multiplication
		*/
        iaMatrix operator*(const iaMatrix &m);

        /*! matrix multiplication
		*/
        void operator*=(const iaMatrix &m);

        /*! matrix component by index

		\param i only indexes from 0 to 15 are allowed
		*/
        T &operator[](int i);

        T operator[](int i) const;

        /*! initializes matrix with id matrix
		*/
        void identity();

        /*! returns the determinant of the matrix
		*/
        T determinant();

        /*! returns the determinant of the 3x3 part of the matrix
		*/
        T determinant3x3();

        /*! calculates the inverse of the matrix and overwrites the original value
		*/
        bool invert();

        /*! transposes the matrix
		*/
        void transpose();

        /*! calculates a view matrix

		\param eye eye point of the camera
		\param coi center of interesst
		\param top the ipvector of the camera
		*/
        void lookAt(const iaVector3<T> &eye, const iaVector3<T> &coi, const iaVector3<T> &top);

        /*! calculates a frustum matrix

		\param left the left value
		\param right the right value
		\param bottom the bottom value
		\param top the top value
		\param nearPlain the near plain distance
		\param farPlain the far plain distance
		*/
        void frustum(T left, T right, T bottom, T top, T nearPlain, T farPlain);

        /*! calculates a perspective projection matrix

		\param fov field of view in degrees
		\param aspect aspect ratio
		\param nearPlain near clipping plane distance
		\param farPlain far clipping plane distance
		*/
        void perspective(T fov, T aspect, T nearPlain, T farPlain);

        /*! calculates a othogonal projection matrix

		\param left the left value
		\param right the right value
		\param bottom the bottom value
		\param top the top value
		\param nearPlain the near plain distance
		\param farPlain the far plain distance
		*/
        void ortho(T left, T right, T bottom, T top, T nearPlain, T farPlain);

        /*! calculates a orthogonal matrix based on one direction

		does only change the orientation of this matrix
		the vector does not have top be normalized

		\param depth the direction the matrix bases on
		*/
        void grammSchmidt(iaVector3<T> &depth);

        /*! calculates a orthogonal matrix based on a plane with primary direction depth

		does only change the orientation of this matrix
		the vectors do not have to be perpendicular
		the vectors do not have to be normalized

		\param depth the direction the matrix bases on
		\param top the second vector to define the plane
		*/
        void grammSchmidt(iaVector3<T> &depth, iaVector3<T> &top);

        /*! translate the matrix by vector 3d

		\param a translation vector
		*/
        void translate(const iaVector3<T> &a);

        /*! translate the matrix by vector 3d

		\param x x component
		\param y y component
		\param z z component
		*/
        void translate(T x, T y, T z);

        /*! scale matrix by vector

		\param s scale vector
		*/
        void scale(const iaVector3<T> &s);

        /*! scales the matrix

		\param x scale in x axis
		\param y scale in y axis
		\param z scale in z axis
		*/
        void scale(T x, T y, T z);

        /*! translate the matrix along it's own orientation vectors

		\param distance distance to translate the matrix
		\param axis axis to translate along
		*/
        void move(T distance, iaAxis axis);

        /*! translate the matrix along it's own orientation vectors by given vector

		\param v the given vector
		*/
		void move(const iaVector3<T> &v);

        /*! rotates the matrix around a specified orientation vector of the matrix

		\param angle angle to rotate in radians
		\param axis axis to rotate around
		*/
        void rotate(T angle, iaAxis axis);

        /*! rotates matrix in three axis

		\param vec the rotation vector
		*/
        void rotate(const iaVector3<T> &vec);

        /*! rotates matrix in three axis

		\param x x axis
		\param y y axis
		\param z z axis
		*/
        void rotate(T x, T y, T z);

        /*! shears matrix in three axis

		\param vec the shear vector
		*/
        void shear(const iaVector3<T> &vec);

        /*! shears matrix in three axis

		\param xy shear in xy plane
		\param xz shear in yz plane
		\param yz shear in yz plane
		*/
        void shear(T xy, T xz, T yz);

        /*! \returns pointer to the data
		*/
        const T *getData() const;

        /*! \returns pointer to the data
		*/
        T *getData();

        /*! set data
		*/
        void setData(const T *data);

        /*! converts the vector in to given type of vector

		\returns matrix for given type
		*/
        template <class T2>
        iaMatrix<T2> convert();

        /*! initializes the matrix with the id matrix
		*/
        iaMatrix();

        /*! initializes matrix with a data set

		\param data matrix
		*/
        iaMatrix(const T data[16]);

        /*! does nothing
		*/
        ~iaMatrix();
    };

    /*! stream operator

    \param stream the destination
    \param m the matrix to stream
    \returns the resulting stream
    */
    template <class T>
	IAUX_API std::wostream &operator<<(std::wostream &stream, const iaMatrix<T> &m);

#include <iaux/math/iaMatrix.inl>

    /*! float32 4x4 matrix
	*/
    typedef iaMatrix<float32> iaMatrixf;

    /*! float64 4x4 matrix
	*/
    typedef iaMatrix<float64> iaMatrixd;

}; // namespace iaux

#endif // __IAUX_MATRIX__
