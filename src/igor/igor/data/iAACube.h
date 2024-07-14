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

#ifndef IGOR_AACUBE_H
#define IGOR_AACUBE_H

#include <igor/iDefines.h>

#include <iaux/math/iaVector3.h>
using namespace iaux;

namespace igor
{

    /*! Axis aligned cube
	*/
    template <class T>
    class IGOR_API_EXPORT_ONLY iAACube
    {
    public:
        /*! does nothing
        */
        iAACube() = default;

        /*! init members constructor

        \param center center position of cube
        \param halfEdgeLenght half of the edge length of the cube
        */
        iAACube(const iaVector3<T> &center, T halfEdgeLenght);

        /*! \returns the center
        */
        const iaVector3<T>& getCenter() const;

        /*! \returns the center
        */
        const T& getHalfEdge() const;

        /*! center position of cube
        */
        iaVector3<T> _center;

        /*! half of the edge length
        */
        T _halfEdgeLength = static_cast<T>(0);
    };

#include <igor/data/iAACube.inl>

    /*! float32 specialization of iAACube
    */
    typedef iAACube<float32> iAACubef;

    /*! float32 specialization of iAACube
    */
    typedef iAACube<float64> iAACubed;

} // namespace igor

#endif // IGOR_AACUBE_H