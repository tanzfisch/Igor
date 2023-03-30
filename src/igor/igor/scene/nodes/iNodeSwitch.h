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

#ifndef __IGOR_NODESWITCH__
#define __IGOR_NODESWITCH__

#include <igor/scene/nodes/iNode.h>

#include <iaux/data/iaString.h>
using namespace iaux;

#include <map>

namespace igor
{

    /*! the switch node. Has multiple children like every node but only one can be active at a time
    */
    class IGOR_API iNodeSwitch : public iNode
    {

        friend class iNodeManager;

    public:
        /*! sets the active child by name

        if name is not found all children are deactivated

        \param name the name of the child
        */
        void setActiveChild(const iaString &name);

        /*! sets the active child by object

        if node pointer is nullptr all children are deactivated

        \param node pointer to node object
        */
        void setActiveChild(iNodePtr node);

        /*! sets the active child by id

        if id is not found all children are deactivated

        \param id node id
        */
        void setActiveChild(uint32 id);

    private:
        /*! called after a node was copied

        \param nodeIDMap map with old node ids to new node ids
        */
        void onPostCopyLink(std::map<uint32, uint32> &nodeIDMap);

        /*! initializes memeber varialbes
        */
        iNodeSwitch();

        /*! copy ctor
        */
        iNodeSwitch(iNodeSwitch *node);

        /*! does nothing
        */
        virtual ~iNodeSwitch() = default;
    };

    /*! pointer definition of switch node
    */
    typedef iNodeSwitch* iNodeSwitchPtr;

}; // namespace igor

#endif
