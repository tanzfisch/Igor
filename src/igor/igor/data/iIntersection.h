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

#ifndef __IGOR_INTERSECTION_H__
#define __IGOR_INTERSECTION_H__

#include <igor/data/iRectangle.h>
#include <igor/data/iAABox.h>
#include <igor/data/iSphere.h>
#include <igor/data/iFrustum.h>
#include <igor/data/iAACube.h>
#include <igor/data/iRay.h>

#include <iaux/math/iaVector2.h>
#include <iaux/math/iaVector3.h>
using namespace iaux;

namespace igor
{

    /*! collection of intersection functions

    \todo read http://noonat.github.io/intersect/
    */
    class IGOR_API iIntersection
    {

    public:
        /*! intersection test sphere with spehre

        \param sphereA first sphere
        \param sphereB second sphere
        \returns true if spheres collide
        */
        template <typename T>
        static bool intersects(const iSphere<T> &sphereA, const iSphere<T> &sphereB);

        /*! containment test sphere in spehre

        \param sphereA ?
        \param sphereB ?
        \returns true if ?
        */
        template <typename T>
        static bool contains(const iSphere<T> &sphereA, const iSphere<T> &sphereB);

        /*! tests if sphere is in front of plane
        
        \param sphere the sphere to test
        \param plane the plane to test
        \returns true if sphere is partially or fully in front of plane
        */
        template <typename T>
        static bool inFrontOf(const iSphere<T> &sphere, const iPlane<T> &plane);

        /*! tests if sphere intersects with frustum

        \param spehre the sphere to test
        \param frustum the frustum to test with
        \returns true if sphere intersects with frustum
        */
        template <typename T>
        static bool intersects(const iSphere<T> &sphere, const iFrustum<T> &frustum);

        /*! tests if cube is in front of a plane

        \param cube the cube to test
        \param plane the plane to test with
        \returns true if the cube is in front of the plane
        */
        template <typename T>
        static bool inFrontOf(const iAACube<T> &cube, const iPlane<T> &plane);

        /*! tests if a cube intersects with a sphere

        \param cube the cube to test with
        \param sphere the sphere to test with
        \returns true if cube and sphere intersecting
        */
        template <typename T>
        static bool intersects(const iAACube<T> &cube, const iSphere<T> &sphere);

        /*! tests if a cube intersects with a frustum

        \param cube the cube to test with
        \param frustum the frustum to test with
        \returns true if cube and frustum intersecting
        */
        template <typename T>
        static bool intersects(const iAACube<T> &cube, const iFrustum<T> &frustum);

        /*! tests if a vector intersects with a cube

        \param vec the vector to test with
        \param cube the cube to test with
        \returns true if the vector intersects with the cube
        */
        template <typename T>
        static bool intersects(const iaVector3<T> &vec, const iAACube<T> &cube);

        /*! tests if two axis aligned cubes are intersecting

        \param cubeA the first cube to test with
        \param cubeB the second cube to test with
        \returns true if the cubes intersect with each other
        */
        template <typename T>
        static bool intersects(const iAACube<T> &cubeA, const iAACube<T> &cubeB);

        /*! tests if two axis aligned boxes are intersecting

        \param boxA the first box to test with
        \param boxB the second box to test with
        \returns true if the boxes intersect with each other
        */
        template <typename T>
        static bool intersects(const iAABox<T> &boxA, const iAABox<T> &boxB);

        /*! tests if a vector intersects with a axis aligned box

        \param vec the vector to test with
        \param box the box to test with
        \returns true if the vector intersects with the box
        */
        template <typename T>
        static bool intersects(const iaVector3<T> &vec, const iAABox<T> &box);

        /*! intersection test between rectangle and point
        
        \param point the point to test
        \param rectangle the rectangle to test
        \returns true if intersecting
        */
        template <typename T>
        static bool intersects(iaVector2<T> point, iRectangle<T> rectangle);

        /*! intersection test between plane and ray

        \param plane the plane to test with
        \param ray the plane to test with
        \param[out] intersection the calculated intersection point
        \returns true if intersecting
        */
        template <typename T>
        static bool intersects(iPlane<T> plane, iRay<T> ray, iaVector3<T> &intersection);
    };

#include <igor/data/iIntersection.inl>

}; // namespace igor

#endif // __IGOR_INTERSECTION_H__
