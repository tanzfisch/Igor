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
// (c) Copyright 2012-2022 by Martin Loga
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

#ifndef __IAUX_SPHERE_H__
#define __IAUX_SPHERE_H__

#include <iaux/math/iaVector3.h>

namespace igor
{

    /*! sphere

    \todo mode to IgorAux
    */
    template <class T>
    class IGOR_API_EXPORT_ONLY iaSphere
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
        iaSphere(iaVector3<T> center, T radius);

        /*! does nothing
        */
        iaSphere() = default;

        /*! does nothing
        */
        virtual ~iaSphere() = default;

        /*! merges on spehre in to an other

        \param sphere the sphere to merge with
        */
        __IGOR_INLINE__ void merge(iaSphere<T> &sphere);

        /*! compares of two spheres are equal
        */
        __IGOR_INLINE__ bool operator==(const iaSphere<T> &sphere) const;

        /*! compares of two spheres are not equal
        */
        __IGOR_INLINE__ bool operator!=(const iaSphere<T> &sphere) const;

    private:
        /*! containment test with an other sphere

        \param sphere the other sphere to test with
        \returns true: if sphere is completely inside sphere; false: if not
        */
        __IGOR_INLINE__ bool contains(const iaSphere<T> &sphere);
    };

#include <iaux/data/iaSphere.inl>

    /*! float32 precision of sphere
    */
    typedef iaSphere<float32> iSpheref;

    /*! float64 precision of sphere
    */
    typedef iaSphere<float64> iSphered;

    /*! uint64 precision of sphere
    */
    typedef iaSphere<int64> iSphereI;

} // namespace igor

#endif // __IAUX_SPHERE_H__