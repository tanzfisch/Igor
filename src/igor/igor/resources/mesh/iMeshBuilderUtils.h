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

#ifndef __IGOR_MESHBUILDERUTILS__
#define __IGOR_MESHBUILDERUTILS__

#include <igor/resources/mesh/iMeshBuilder.h>
#include <igor/resources/mesh/iMesh.h>

namespace igor
{

    namespace iMeshBuilderUtils
    {

        /*! adds a cylinder to the given mesh builder instance

        \param meshBuilder the given mesh builder instance
        \param radius the radius of the cylinder
        \param height the height of the cylinder
        \param segments the segment count of the cylinder
        \param hasCaps if true the cylinder will have caps on top and bottom
        */
        void IGOR_API addCylinder(iMeshBuilder &meshBuilder, float32 radius, float32 height, uint32 segments = 16, bool hasCaps = true);

        /*! adds a sphere to the given mesh builder instance

		\param meshBuilder the given mesh builder instance
		\param radius the radius of the cylinder
		\param segments the segment count of the cylinder
		*/
        void IGOR_API addSphere(iMeshBuilder &meshBuilder, float32 radius, uint32 segments = 16);

        /*! adds a box to the given mesh builder instance

        \param meshBuilder the given mesh builder instance
        \param width width of the box
        \param height height of the box
        \param depth depth of the box
        */
        void IGOR_API addBox(iMeshBuilder &meshBuilder, float32 width, float32 height, float32 depth);

        /*! adds a cone to the given mesh builder instance

        \param meshBuilder the given mesh builder instance
        \param radius the radius of the cones base
        \param height the height of the cone
        \param segments the segment count of the cone
        */
        void IGOR_API addCone(iMeshBuilder &meshBuilder, float32 radius, float32 height, uint32 segments = 16);

        /*! adds a circle to the given mesh builder instance

        \param meshBuilder the given mesh builder instance
        \param radius the radius of the circle
        \param segments the segment count of the circle
        */
        void IGOR_API addCircle(iMeshBuilder &meshBuilder, float32 radius, uint32 segments = 16);

        /*! adds a ring to the given mesh builder instance

        \param meshBuilder the given mesh builder instance
        \param innerRadius the inner radius of the ring
        \param outerRadius the outer radius of the ring
        \param segments the segment count of the ring
        */
        void IGOR_API addRing(iMeshBuilder &meshBuilder, float32 innerRadius, float32 outerRadius, uint32 segments = 16);

        /*! adds a rectangle in XZ plane

        \param meshBuilder the given mesh builder instance
        \param x x position of plane
        \param z z position of plane
        \param sizeX size in x axis
        \param sizeZ size in z axis
        */
        void IGOR_API addRectangle(iMeshBuilder &meshBuilder, float32 x, float32 z, float32 sizeX, float32 sizeZ);

        /*! adds a mesh to the mesh builder

		\param meshBuilder the given mesh builder instance
		\param mesh the mesh to be added
		*/
        void IGOR_API addMesh(iMeshBuilder &meshBuilder, const iMeshPtr mesh);

    } // namespace iMeshBuilderUtils

}; // namespace igor

#endif
