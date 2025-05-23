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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IGOR_NODELODTRIGGER__
#define __IGOR_NODELODTRIGGER__

#include <igor/scene/nodes/iNode.h>

#include <iaux/data/iaString.h>
#include <iaux/math/iaVector3.h>
using namespace iaux;

#include <map>

namespace igor
{

    /*! LOD trigger node

    every LOD switch node needs a LOD trigger node in order to switch
    It can be only one for the whole scene or it can be multiple ones for individual LOD switch nodes.
    */
    class IGOR_API iNodeLODTrigger : public iNode
    {

        friend class iNodeManager;
        friend class iNodeVisitorUpdateTransform;

    public:
        /*! \returns world position of trigger
        */
        const iaVector3d &getWorldPosition() const;

    protected:
        /*! sets the current world aka absolute position

        \param pos new position to be set
        */
        void onUpdateTransform(iaMatrixd &matrix);

        /*! unregisters trigger from scene if it belonged to a scene before
        */
        virtual void onPreSetScene();

        /*! registers to new scene
        */
        virtual void onPostSetScene();

    private:
        /*! current world position of lod trigger
        */
        iaVector3d _worldPosition;

        /*! called after a node was copied

        \param nodeIDMap map with old node ids to new node ids
        */
        void onPostCopyLink(std::map<uint32, uint32> &nodeIDMap);

        /*! initializes memeber varialbes
        */
        iNodeLODTrigger();

        /*! copy ctor
        */
        iNodeLODTrigger(iNodeLODTrigger *node);

        /*! does nothing
        */
        virtual ~iNodeLODTrigger();
    };

}; // namespace igor

#endif
