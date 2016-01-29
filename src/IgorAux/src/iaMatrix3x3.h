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

#ifndef __iMATRIX3X3__
#define __iMATRIX3X3__

#include <iaVector4.h>

#include <fstream>
#include <cmath>
#include <ostream>
#include <iomanip>
using namespace std;

namespace IgorAux
{

	/*! 3x3 Matrix
	*/
	template <class T> class IgorAux_API_Template iaMatrix3x3
	{
	public:

        /*! internal data
        */
		iaVector3<T> _right;
		iaVector3<T> _top;
		iaVector3<T> _depth;

		/*! addition
		*/
		__IGOR_INLINE__ void operator+= (iaMatrix3x3 &a);

		/*! subtraction
		*/
		__IGOR_INLINE__ void operator-=(iaMatrix3x3 &a);

        /*! comparison of two matrices

        \param a the second matrix to compare this matrix with
        \returns true if matrices are equal
        */
        __IGOR_INLINE__ bool operator== (const iaMatrix3x3<T> &a) const;

        /*! negated comparison of two matrices

        \param a the second matrix to compare this matrix with
        \returns true if matrices are not equal
        */
        __IGOR_INLINE__ bool operator!= (const iaMatrix3x3<T> &a) const;

		/*! matrix vector3 multiplication
		*/
		__IGOR_INLINE__ iaVector3<T> operator*(iaVector3<T> &a);

		/*! matrix vector4 multiplication
		*/
		__IGOR_INLINE__ iaVector4<T> operator*(iaVector4<T> &a);

		/*! matrix multiplication
		*/
		__IGOR_INLINE__ iaMatrix3x3 operator*(iaMatrix3x3 &m);

		/*! matrix multiplication
		*/
		__IGOR_INLINE__ void operator*=(iaMatrix3x3 &m);

		/*! matrix component by index

		\param i only indexes from 0 to 8 are allowed
		*/
		__IGOR_INLINE__ T& operator[](int i);

		/*! initializes matrix with id matrix
		*/
		__IGOR_INLINE__ void identity(void);

		/*! returns the determinant of the 3x3 part of the matrix
		*/
		__IGOR_INLINE__ T determinant();

		/*! calculates the inverse of the matrix and overwrites the original value
		*/
		// TODO nice to have __IGOR_INLINE__ bool invert();

		__IGOR_INLINE__ void transpose();

		/*! scale matrix by vector

		\param s scale vector
		*/
		__IGOR_INLINE__ void scale(iaVector3<T> &s);
		__IGOR_INLINE__ void scale(T x, T y, T z);

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
		__IGOR_INLINE__ iaMatrix3x3();

		/*! initializes matrix with a data set

		\param data matrix 
		*/
		__IGOR_INLINE__ iaMatrix3x3(const T data[16]);

		/*! does nothing
		*/
		__IGOR_INLINE__ ~iaMatrix3x3();
	};

	#include <iaMatrix3x3.inl>

	typedef iaMatrix3x3<float32> iaMatrix3x3f;
	typedef iaMatrix3x3<float64> iaMatrix3x3d;

};

#endif
