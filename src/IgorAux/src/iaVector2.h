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

#ifndef __iVECTOR2__
#define __iVECTOR2__

#include <iaDefines.h>

#include <cmath>
#include <ostream>
using namespace std;

namespace IgorAux
{

	/*! 2 dimensional vector
	*/
	template <class T>
	class IgorAux_API_Template iaVector2
	{
	public:

		/*! x component
		*/
		T _x = static_cast<T>(0);

		/*! y component
		*/
		T _y = static_cast<T>(0);

		/*! vector addition

		\param vec vector to be added to this vector
		\returns resulting vector
		*/
		__IGOR_INLINE__ iaVector2<T> operator+ (const iaVector2<T>& vec) const;

		/*! vector addition and assignment

		\param vec vector to be added to this vector
		*/
		__IGOR_INLINE__ void operator+= (const iaVector2<T>& vec);

		/*! vector subtraction

		\param vec vector to be subtracted from this vector
		\returns resulting vector
		*/
		__IGOR_INLINE__ iaVector2<T> operator- (const iaVector2<T>& vec) const;

		/*! vector subtraction and assignment

		\param vec vector to be subtracted from this vector
		*/
		__IGOR_INLINE__ void operator-= (const iaVector2<T>& vec);

		/*! vector assignment

		\param vec vector to be assigned to this vector
		*/
		__IGOR_INLINE__ iaVector2<T> operator= (const iaVector2<T>& vec);

		/*! multiplication of vector with scalar

		\param scalar value to multiply with this vector
		\returns resulting vector
		*/
		__IGOR_INLINE__ iaVector2<T> operator* (T scalar) const;

		/*! multiplication of vector with scalar and assignment

		\param scalar value to multiply with this vector
		*/
		__IGOR_INLINE__ void operator*= (T scalar);

		/*! division of vector with scalar

		\param scalar value to devide this vector by
		\returns resulting vector
		*/
		__IGOR_INLINE__ iaVector2<T> operator/  (T scalar) const;

		/*! division of vector with scalar and assignment

		\param scalar value to devide this vector by
		*/
		__IGOR_INLINE__ void operator/= (T scalar);

		/*! comparison of two vectors

		no deviation accepted

		\returns true: if excactly equal; false: if not equal
		*/
		__IGOR_INLINE__ bool operator== (const iaVector2<T>& vec) const;

		/*! comparison of two vectors

		no deviation accepted

		\returns true: if not equal; false: if excactly equal
		*/
		__IGOR_INLINE__ bool operator!= (const iaVector2<T>& vec) const;

		/*! dot product between two vectors

		\param vec vector to multiply this vector with
		\returns dot product of both vectors
		*/
		__IGOR_INLINE__ T operator*  (const iaVector2<T>& vec) const;

		/*! \returns value by index (const version)

		\param index the index of the value to be returned
		*/
		__IGOR_INLINE__ const T& operator[] (int index) const;

		/*! \returns value by index

		\param index the index of the value to be returned
		*/
		__IGOR_INLINE__ T& operator[] (int index);

		/*! \returns pointer to data
		*/
		__IGOR_INLINE__ T* getData();

		/*! \returns pointer to data (const version)
		*/
		__IGOR_INLINE__ const T* getData() const;

		/*! set values

		\param x x value to set
		\param y y value to set
		*/
		__IGOR_INLINE__ void set(T x, T y);

		/*! \returns length of vector
		*/
		__IGOR_INLINE__ T length(void) const;

		/*! \returns quadratic length of vector
		*/
		__IGOR_INLINE__ T length2(void) const;

		/*! \returns distance between positions

		\param vec the second position to calculate the distance to
		*/
		__IGOR_INLINE__ T distance(iaVector2<T>& vec) const;

		/*! \returns quadratic distance between positions

		\param vec the second position to calculate the distance to
		*/
		__IGOR_INLINE__ T distance2(iaVector2<T>& vec) const;

		/*! \returns angle in rad relative to X-Axis
		*/
		__IGOR_INLINE__ T angle() const;

		/*! \returns angle in rad between two vectors

		\param vec second vector to calculate the angle with
		*/
		__IGOR_INLINE__ T angle(iaVector2<T>& vec) const;

		/*! rotates a vector in XY plane

		\param angle the angle in rad to rotate the vector with
		*/
		__IGOR_INLINE__ void  rotateXY(T angle);

		/*! normalizes the vector
		*/
		__IGOR_INLINE__ void  normalize(void);

		/*! conjugates (or negates) the vector
		*/
		__IGOR_INLINE__ void  negate(void);

		/*! converts the vector in to given type of vector

		\returns vector for given type
		*/
		template<class T2>
		__IGOR_INLINE__ iaVector2<T2> convert() const;

		/*! does nothing
		*/
		iaVector2() = default;

		/*! initializes the vector with x y components

		\param x x component
		\param y y component
		*/
		iaVector2(T x, T y);

		/*! does nothing
		*/
		virtual ~iaVector2() = default;
	};

#include <iaVector2.inl>

	/*! float32 2d vector
	*/
	typedef iaVector2<float32> iaVector2f;

	/*! float64 2d vector
	*/
	typedef iaVector2<float64> iaVector2d;

	/*! int32 2d vector
	*/
	typedef iaVector2<int32> iaVector2i;

	/*! int64 2d vector
	*/
	typedef iaVector2<int64> iaVector2I;

};

#endif
