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
// (c) Copyright 2014-2015 by Martin Loga
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
// contact: martinloga@gmx.de  

#ifndef __iMATRIX__
#define __iMATRIX__

#include <iaVector4.h>

#include <fstream>
#include <cmath>
#include <ostream>
#include <iomanip>
using namespace std;

namespace IgorAux
{

	/*! 4x4 Matrix
	*/
	template <class T> class IgorAux_API_Template iaMatrix
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
		__IGOR_INLINE__ void operator+= (iaMatrix &a);

		/*! subtraction
		*/
		__IGOR_INLINE__ void operator-=(iaMatrix &a);

        /*! comparison of two matrices

        \param a the second matrix to compare this matrix with
        \returns true if matrices are equal
        */
        __IGOR_INLINE__ bool operator== (const iaMatrix<T> &a) const;

        /*! negated comparison of two matrices

        \param a the second matrix to compare this matrix with
        \returns true if matrices are not equal
        */
        __IGOR_INLINE__ bool operator!= (const iaMatrix<T> &a) const;

		/*! matrix vector3 multiplication
		*/
		__IGOR_INLINE__ iaVector3<T> operator*(iaVector3<T> &a);

		/*! matrix vector4 multiplication
		*/
		__IGOR_INLINE__ iaVector4<T> operator*(iaVector4<T> &a);

		/*! matrix multiplication
		*/
		__IGOR_INLINE__ iaMatrix operator*(iaMatrix &m);

		/*! matrix multiplication
		*/
		__IGOR_INLINE__ void operator*=(iaMatrix &m);

		/*! matrix component by index

		\param i only indexes from 0 to 15 are allowed
		*/
		__IGOR_INLINE__ T& operator[](int i);

		/*! initializes matrix with id matrix
		*/
		__IGOR_INLINE__ void identity(void);

		/*! returns the determinant of the matrix
		*/
		__IGOR_INLINE__ T determinant();

		/*! returns the determinant of the 3x3 part of the matrix
		*/
		__IGOR_INLINE__ T determinant3x3();

		/*! calculates the inverse of the matrix and overwrites the original value
		*/
		__IGOR_INLINE__ bool invert();

		__IGOR_INLINE__ void transpose();

		/*! calculates a view matrix

		\param eye eye point of the camera
		\param coi center of interesst
		\param top the ipvector of the camera
		*/
		__IGOR_INLINE__ void lookAt(iaVector3<T> &eye, iaVector3<T> &coi, iaVector3<T> &top);

		/*! calculates a frustum matrix

		\param left the left value
		\param right the right value
		\param bottom the bottom value
		\param top the top value
		\param nearplain the near plain distance
		\param farplain the far plain distance
		*/
		__IGOR_INLINE__ void frustum( T left, T right, T bottom, T top, T nearplain, T farplain);

		/*! calculates a othogonal matrix

		\param left the left value
		\param right the right value
		\param bottom the bottom value
		\param top the top value
		\param nearplain the near plain distance
		\param farplain the far plain distance
		*/
		__IGOR_INLINE__ void ortho( T left, T right, T bottom, T top, T nearplain, T farplain);

		/*! translate the matrix by vector 3d

		\param a translation vector 
		*/
		__IGOR_INLINE__ void translate(const iaVector3<T> &a);

		/*! translate the matrix by vector 3d

		\param x x component
		\param y y component
		\param z z component
		*/
		__IGOR_INLINE__ void translate(T x, T y, T z);

		/*! scale matrix by vector

		\param s scale vector
		*/
		__IGOR_INLINE__ void scale(iaVector3<T> &s);
		__IGOR_INLINE__ void scale(T x, T y, T z);

		/*! translate the matrix along it's own orientation vectors

		\param distance distance to translate the matrix
		\param axis axis to translate along
		*/
		__IGOR_INLINE__ void move(T distance, iaAxis axis);

		/*! rotates the matrix around a specified orientation vector of the matrix

		\param angle angle to rotate in radians
		\param axis axis to rotate around
		*/
		__IGOR_INLINE__ void rotate(T angle, iaAxis axis);
		
		/*! returns pointer to the data
		*/
		__IGOR_INLINE__ const T* getData() const;

        /*! returns pointer to the data
        */
        __IGOR_INLINE__ T* getData();

		/*! set data
		*/
		__IGOR_INLINE__ void setData(T* data);

		/*! initializes the matrix with the id matrix
		*/
		__IGOR_INLINE__ iaMatrix();

		/*! initializes matrix with a data set

		\param data matrix 
		*/
		__IGOR_INLINE__ iaMatrix(const T data[16]);

		/*! does nothing
		*/
		__IGOR_INLINE__ ~iaMatrix();
	};

	#include <iaMatrix.inl>

	typedef iaMatrix<float32> iaMatrixf;
	typedef iaMatrix<float64> iaMatrixd;

};

#endif