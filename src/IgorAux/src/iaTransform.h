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

	/*! Transform
	*/
	template <class T> class IgorAux_API_Template iaTransform
	{
	public:

        /*! rotatation component in rad
        */
		iaVector3<T> _rotate;

		/*! scale component
		*/
		iaVector3<T> _scale;

		/*! shear component
		*/
		iaVector3<T> _shear;

		/*! translate component
		*/
		iaVector3<T> _translate;

		__IGOR_INLINE__ void getMatrix(iaMatrix<T>& matrix);

		/*! initializes the matrix with the id matrix
		*/
		__IGOR_INLINE__ iaTransform();

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
