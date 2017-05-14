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
// (c) Copyright 2012-2017 by Martin Loga
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

#include <iDefines.h>
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

        /*! sets min max values of aabox

        \param min min vector for aa box
        \param max max vector for aa box
        */
        void set(const iaVector3<T>& min, const iaVector3<T>& max);

	};

#include <iAABox.inl>

    /*! float32 specialization of iAABox
    */
	typedef iAABox<float32> iAABoxf;

    /*! float32 specialization of iAABox
    */
    typedef iAABox<float64> iAABoxd;

    /*! int32 specialization of iAABox
    */
    typedef iAABox<int32> iAABoxi;

    /*! int64 specialization of iAABox
    */
    typedef iAABox<int64> iAABoxI;

}

#endif
