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
// (c) Copyright 2012-2019 by Martin Loga
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

#ifndef __iaTRANSFORM__
#define __iaTRANSFORM__

#include <iaMatrix.h>

namespace IgorAux
{

	/*! Transform based on the components translate, rotate, scale.
	The intention is to keep the components separate for better control.
	*/
	template <class T>
	class IgorAux_API_Template iaTransform
	{

	public:

		/*! translate component
		*/
		iaVector3<T> _translate;

		/*! rotatation component in rad
		*/
		iaVector3<T> _rotate;

		/*! scale component
		*/
		iaVector3<T> _scale;

		/*! add a transfomr to a transform

		\param rsv the transform to add on top
		*/
		__IGOR_INLINE__ void operator+= (iaTransform& rsv);

		/*! comparison of two transforms

		\param rsv the right side value
		\returns true if transforms are equal
		*/
		__IGOR_INLINE__ bool operator== (const iaTransform<T>& rsv) const;

		/*! returns the transforms matrix based on it's components
		\param[out] matrix the resulting matrix
		*/
		__IGOR_INLINE__ void getMatrix(iaMatrix<T>& matrix) const;

		/*! decompose matrix and set transform

		\param matrix the matrix to decompose
		*/
		__IGOR_INLINE__ void setMatrix(const iaMatrix<T>& matrix);

		/*! sets the transform the the equivalent of an identity matrix
		*/
		__IGOR_INLINE__ void identity();

		/*! translate the transform by vector

		\param a translation vector
		*/
		__IGOR_INLINE__ void translate(const iaVector3<T>& a);

		/*! translate the transform by vector

		\param x x component
		\param y y component
		\param z z component
		*/
		__IGOR_INLINE__ void translate(T x, T y, T z);

		/*! scale transform by vector

		\param scale the scale vector
		*/
		__IGOR_INLINE__ void scale(const iaVector3<T>& scale);

		/*! scales the transform

		\param x scale in x axis
		\param y scale in y axis
		\param z scale in z axis
		*/
		__IGOR_INLINE__ void scale(T x, T y, T z);

		/*! rotates the transform around a specified orientation vector of the matrix

		\param angle angle to rotate in radians
		\param axis axis to rotate around
		*/
		__IGOR_INLINE__ void rotate(T angle, iaAxis axis);

		/*! rotates transform in three axis

		\param vec the rotation vector
		*/
		__IGOR_INLINE__ void rotate(const iaVector3<T>& vec);

		/*! rotates transform in three axis

		\param x x axis
		\param y y axis
		\param z z axis
		*/
		__IGOR_INLINE__ void rotate(T x, T y, T z);

		/*! initializes the matrix with the id matrix
		*/
		iaTransform();

		/*! ctor set wit matrix to decompose

		\param matrix the matrix to decompose
		*/
		iaTransform(const iaMatrix<T>& matrix);

		/*! initializes the matrix with the id matrix

		\param translate translation component
		\param rotate the rotate component
		\param scale the scale component
		*/
		iaTransform(const iaVector3<T>& translate, const iaVector3<T>& rotate, const iaVector3<T>& scale);

		/*! does nothing
		*/
		__IGOR_INLINE__ ~iaTransform();
	};

#include <iaTransform.inl>

	/*! float32 transform
	*/
	typedef iaTransform<float64> iaTransformf;

	/*! float64 transform
	*/
	typedef iaTransform<float64> iaTransformd;

};

#endif
