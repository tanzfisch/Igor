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

#ifndef __IAUX_SPHERE__
#define __IAUX_SPHERE__

#include <iaux/math/iaVector3.h>

namespace iaux
{

    /*! sphere
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
        T _radius = static_cast<T>(1.0);

        /*! does nothing
         */
        iaSphere() = default;

        /*! constructor that initializes member variables

        \param center center position of sphere
        \param radius radius of sphere
        */
        iaSphere(iaVector3<T> center, T radius);

        /*! does nothing
         */
        virtual ~iaSphere() = default;

        /*! merges one sphere in to an other

        \param sphere the sphere to merge with
        */
        void merge(iaSphere<T> &sphere);

        /*! \returns true if two spheres are equal

        \param sphere the sphere to compare with
        */
        bool operator==(const iaSphere<T> &sphere) const;

        /*! \returns true if spheres are not equal

        \param sphere the sphere to compare with
        */
        bool operator!=(const iaSphere<T> &sphere) const;

    private:
        /*! containment test with an other sphere

        \param sphere the other sphere to test with
        \returns true: if sphere is completely inside sphere; false: if not
        */
        bool contains(const iaSphere<T> &sphere);
    };

#include <iaux/data/iaSphere.inl>

    /*! float32 precision of sphere
     */
    typedef iaSphere<float32> iaSpheref;

    /*! float64 precision of sphere
     */
    typedef iaSphere<float64> iaSphered;

    /*! uint64 precision of sphere
     */
    typedef iaSphere<int64> iaSphereI;

} // namespace iaux

#endif // __IAUX_SPHERE__