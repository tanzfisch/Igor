//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef IGOR_AABOX_H
#define IGOR_AABOX_H

#include <igor/iDefines.h>

#include <iaux/math/iaVector3.h>
using namespace iaux;

namespace igor
{

    /*! Axis aligned box
	*/
    template <class T>
    class IGOR_API_EXPORT_ONLY iAABox
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
        iAABox(const iaVector3<T> &center, const iaVector3<T> &halfWidths);

        /*! creates iAABox from min max values

        \param min min vector for aa box
        \param max max vector for aa box
        */
        static iAABox<T> fromMinMax(const iaVector3<T> &min, const iaVector3<T> &max);

        /*! sets min max values of aabox

        \param min min vector for aa box
        \param max max vector for aa box
        */
        void setMinMax(const iaVector3<T> &min, const iaVector3<T> &max);

        /*! returns min, max values

        \param[out] min the min value
        \param[out] max the max value
        */
        void getMinMax(iaVector3<T> &min, iaVector3<T> &max);
    };

#include <igor/data/iAABox.inl>

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

} // namespace igor

#endif // IGOR_AABOX_H