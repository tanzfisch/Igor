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
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __IAUX_TRANSFORM_H__
#define __IAUX_TRANSFORM_H__

#include <iaux/math/iaMatrix.h>
#include <iaux/math/iaQuaternion.h>

namespace iaux
{

	/*! Transform based on the components translate, rotate, scale, shear and perspective
	*/
	template <class T>
	class IgorAux_API_Template iaTransform
	{

	public:
		/*! translate component
		*/
		iaVector3<T> _translate;

		/*! orientation component as quaternion
        */
		iaQuaternion<T> _orientation;

		/*! scale component
		*/
		iaVector3<T> _scale;

		/*! shear component
		*/
		iaVector3<T> _shear;

		/*! perspective
		*/
		iaVector4<T> _perspective;

		/*! \returns true if transform has shear differnt from 0,0,0
		*/
		bool hasShear() const;

		/*! \returns true if transform has scale different from 1,1,1
		*/
		bool hasScale() const;

		/*! \returns true if transform has translation different from 0,0,0
		*/
		bool hasTranslation() const;

		/*! \returns true if transform has rotation different from 0,0,0
		*/
		bool hasRotation() const;

		/*! returns the transforms matrix based on it's components
		\param[out] matrix the resulting matrix
		*/
		void getMatrix(iaMatrix<T> &matrix) const;

		/*! decompose matrix and set transform

		\param matrix the matrix to decompose
		*/
		void setMatrix(const iaMatrix<T> &matrix);

		/*! initializes the matrix with the id matrix
		*/
		iaTransform();

		/*! ctor set wit matrix to decompose

		\param matrix the matrix to decompose
		*/
		iaTransform(const iaMatrix<T> &matrix);

		/*! initializes the matrix with given components

		\param translate translation component
		\param orientation the orientation component
		\param scale the scale component
		*/
		iaTransform(const iaVector3<T> &translate, const iaQuaternion<T> orientation, const iaVector3<T> &scale);

		/*! initializes the matrix with given components

		\param translate translation component
		\param orientation the orientation component
		\param scale the scale component
		\param shear the shear component
		*/
		iaTransform(const iaVector3<T> &translate, const iaQuaternion<T> orientation, const iaVector3<T> &scale, const iaVector3<T> &_shear);

		/*! initializes the matrix with given components

		\param translate translation component
		\param orientation the orientation component
		\param scale the scale component
		\param shear the shear component
		\param perspective the perspective component
		*/
		iaTransform(const iaVector3<T> &translate, const iaQuaternion<T> orientation, const iaVector3<T> &scale, const iaVector3<T> &_shear, const iaVector4<T> &_perspective);

		/*! does nothing
		*/
		~iaTransform();
	};

	template <class T>
	iaTransform<T> lerp(const iaTransform<T> &a, const iaTransform<T> &b, T t);

#include <iaux/math/iaTransform.inl>

	/*! float32 transform
    */
	typedef iaTransform<float64> iaTransformf;

	/*! float64 transform
	*/
	typedef iaTransform<float64> iaTransformd;

}; // namespace iaux

#endif // __IAUX_TRANSFORM_H__
