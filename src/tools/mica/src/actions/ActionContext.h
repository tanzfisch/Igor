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
// (c) Copyright 2014-2020 by Martin Loga
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

#ifndef __ACTIONCONTEXT_H__
#define __ACTIONCONTEXT_H__

#include "../Outliner.h"

#include <igor/ui/actions/iActionContext.h>
#include <igor/scene/nodes/iNode.h>
using namespace igor;

/*! mica action context
*/
class Igor_API ActionContext : public iActionContext
{

public:
    /*! init members
    */
    ActionContext(const std::vector<iNodeID> &nodes, iNodeID rootNode, Outliner *outliner)
        : _nodes(nodes), _rootNode(rootNode), _outliner(outliner)
    {
    }

    /*! \returns nodes of action context
    */
    __IGOR_INLINE__ const std::vector<iNodeID> &getNodes() const
    {
        return _nodes;
    }

    /*! returns root node of mica graph view
    */
    __IGOR_INLINE__ iNodeID getRootNode() const
    {
        return _rootNode;
    }

    /*! returns the outliner
    */
    Outliner *getOutliner() const
    {
        return _outliner;
    }

private:
    /*! the nodes to do an action with
    */
    std::vector<iNodeID> _nodes;

    /*! the root node of the mica workspace
    */
    iNodeID _rootNode = iNode::INVALID_NODE_ID;

    /*! reference to the outliner
    */
    Outliner *_outliner = nullptr;
};

#endif // __ACTIONS_H__
