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

#ifndef __iSPHERE__
#define __iSPHERE__

#include <iFrustum.h>
#include <iaConsole.h>

namespace Igor
{

    /*! sphere
    */
	template <class T>
	class Igor_API_Template iSphere
	{

	public:

        /*! center position of sphere
        */
		iaVector3<T> _center;

        /*! radius of sphere
        */
		T _radius = static_cast<T>(0);

        /*! constructor that initializes member variables

        \param center center position of sphere
        \param radius radius of sphere
        */
		iSphere(iaVector3<T> center, T radius);

        /*! does nothing
        */
		iSphere() = default;

        /*! does nothing
        */
        virtual ~iSphere() = default;

        /*! intersection test againts an other sphere

        \param sphere the other sphere to test with
        \returns true: if spheres intersect each other; false: if not
        */
        __IGOR_INLINE__ bool intersects(iSphere<T> &sphere);

        /*! containment test with an other sphere
        
        \param sphere the other sphere to test with
        \returns true: if sphere is completely inside sphere; false: if not
        */
        __IGOR_INLINE__ bool contains(iSphere<T> &sphere);

        /*! merges on spehre in to an other

        \param sphere the sphere to merge with
        */
        __IGOR_INLINE__ void merge(iSphere<T> &sphere);

        /*! checks if sphere is in fron of a plane

        \returns true: if spehre is in front of plane: false: if not
        */
		__IGOR_INLINE__ bool inFrontOf(iPlane<T> &plane);

        /*! intersection test agains a frustum

        \param frustum the frustum to test it with
        \returns true: if sphere intersects with frustum: false: if not
        */
		__IGOR_INLINE__ bool intersects(iFrustum<T> &frustum);
	};

	#include <iSphere.inl>

	typedef iSphere<float32> iSpheref;
    typedef iSphere<float64> iSphered;

}

#endif