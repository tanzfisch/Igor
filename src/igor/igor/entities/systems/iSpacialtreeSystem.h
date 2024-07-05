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

#ifndef IGOR_QUADTREE_SYSTEM_H
#define IGOR_QUADTREE_SYSTEM_H

#include <igor/entities/iEntitySystem.h>

#include <igor/data/iQuadtree.h>
#include <igor/data/iOctree.h>

namespace igor
{
    /*! quadtree system
     */
    class iSpacialtreeSystem : public iEntitySystem
    {
    public:

        /*! init types
        */
        iSpacialtreeSystem();

		/*! updates system

		\param context the update context
		 */
		void onUpdate(const iEntitySceneUpdateContext &context) override;

		/*! \returns processing stage this system want's to run in
		*/
		iEntitySystemStage getStage() const override;

    private:

        /*! a view on some entities with positions
        */
        iEntityViewPtr _quadtreePositionView;        

        /*! a view on some entities with circles
        */
        iEntityViewPtr _quadtreeCircleView;

        /*! updated quadtree data
        */
        void onUpdateQuadtree(iQuadtreed &quadtree);

        /*! updated octree data
        */
        void onUpdateQuadtree(iOctreed &octree);

    };

} // igor

#endif // IGOR_QUADTREE_SYSTEM_H