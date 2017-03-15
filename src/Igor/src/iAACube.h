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
// (c) Copyright 2014-2016 by Martin Loga
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

#ifndef __iAACUBE__
#define __iAACUBE__

#include <iFrustum.h>
#include <iSphere.h>
#include <iaVector3.h>

namespace Igor
{

	/*! Axis aligned cube
	*/
	template <class T>
	class Igor_API_Template iAACube
	{

	public:

        /*! center position of cube
        */
		iaVector3<T> _center;

        /*! half of the edge lenght
        */
		T _halfEdgeLength = static_cast<T>(0);

        /*! does nothing
        */
		iAACube() = default;

        /*! init members constructor

        \param center center position of cube
        \param halfEdgeLenght half of the edge lenght of the cube
        */
		iAACube(const iaVector3<T>& center, T halfEdgeLenght);

        /*! intersection test against plane

        \param plane the plane to test against
        */
        __IGOR_INLINE__ bool inFrontOf(const iPlane<T> &plane);

        /*! intersection test against a frustum

        \param frustum the frustum to test against
        */
        __IGOR_INLINE__ bool intersects(const iFrustum<T> &frustum);

        /*! intersection test with a sphere

        \param sphere the sphere ti test against
        */
        __IGOR_INLINE__ bool intersects(const iSphere<T> &sphere);
	};

#include <iAACube.inl>

    /*! float32 specialization of iAACube
    */
	typedef iAACube<float32> iAACubef;

    /*! float32 specialization of iAACube
    */
    typedef iAACube<float64> iAACubed;

}

#endif