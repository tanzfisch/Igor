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

#ifndef __iAABOX__
#define __iAABOX__

#include <iaVector3.h>
using namespace IgorAux;

namespace Igor
{

	/*! Axis aligned box 
	*/
	template <class T>
	class Igor_API_Template iAABox
	{

	public:

        /*! center of of box
        */
		iaVector3<T> _center;

        /*! half widths for all axis
        */
        iaVector3<T> _halfWidths;

        /*! does nothing
        */
        iAABox() = default;

        /*! init members constructor

        \param center center of box
        \param halfWidths halfWidths of box in all axis
        */
        iAABox(const iaVector3<T>& center, const iaVector3<T>& halfWidths);

        void set(const iaVector3<T>& min, const iaVector3<T>& max);

        /*! intersection test agains an other box

        \param box the other box to test against
        */
		__IGOR_INLINE__ bool intersects(const iAABox<T> &box);

        /*! intersection test with point

        \param vec vector that defines the point to test with
        */
        __IGOR_INLINE__ bool intersects(const iaVector3<T> &vec);

	};

#include <iAABox.inl>

    /*! float32 specialization of iAABox
    */
	typedef iAABox<float32> iAABoxf;

    /*! float32 specialization of iAABox
    */
    typedef iAABox<float64> iAABoxd;

}

#endif