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
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef __IGOR_NODELODSWITCH__
#define __IGOR_NODELODSWITCH__

#include <igor/scene/nodes/iNode.h>

#include <iaux/data/iaString.h>
#include <iaux/math/iaVector3.h>
using namespace iaux;

#include <map>

namespace igor
{

    class iNodeLODTrigger;

    /*! the lod switch node switches it's child nodes active depending how far a registered LOD trigger node is away

    the LOS switch node can react on multiple LOD trigger nodes

    child goes active when _min <= child distance < _max
    */
    class IGOR_API iNodeLODSwitch : public iNode
    {

        friend class iNodeManager;
        friend class iNodeVisitorUpdateTransform;

        /*! threashold when to switch to a specific LOD

        overlaping is allowed
        */
        struct Threshold
        {
            float32 _min = 0.0f;
            float32 _max = 0.0f;
        };

    public:
        /*! sets the active threashold for a specified lod level (in this case a node)

        \param node node that represents a lod level
        \param min min distance the specified node is visible
        \param max max distance the specified node is visible
        */
        void setThresholds(iNodePtr node, float32 min, float32 max);

        /*! sets the active threashold for a specified lod level (in this case a nodes name)

        \param nodeName name of node that represents a lod level
        \param min min distance the specified node is visible
        \param max max distance the specified node is visible
        */
        void setThresholds(const iaString &nodeName, float32 min, float32 max);

        /*! sets the active threashold for a specified lod level (in this case the nodes id)

        \param nodeID id of node that represents a lod level
        \param min min distance the specified node is visible
        \param max max distance the specified node is visible
        */
        void setThresholds(uint32 nodeID, float32 min, float32 max);

        /*! adds a trigger that has an effect on this lod switch node

        \param trigger the trigger to be added
        */
        void addTrigger(iNodeLODTrigger *trigger);

        /*! removes a trigger that had an effect on this lod switch node

        \param trigger the trigger to be removed
        */
        void removeTrigger(iNodeLODTrigger *trigger);

        /*! adds a trigger that has an effect on this lod switch node by ID

        \param trigger the trigger to be added
        */
        void addTrigger(uint32 triggerID);

        /*! removes a trigger that had an effect on this lod switch node by ID

        \param trigger the trigger to be removed
        */
        void removeTrigger(uint32 triggerID);

        /*! calculates lowest distance to triggers and updates children accordingly
        */
        void update();

        /*! \returns world position of lod switch node
        */
        const iaVector3d &getWorldPosition() const;

    private:
        /*! list of triggers that have an effect on this switch node
        */
        std::vector<uint32> _triggers;

        /*! quadric distance thresholds for child nodes
        */
        std::map<iNodePtr, Threshold> _thresholds;

        /*! current absolute position
        */
        iaVector3d _worldPosition;

        /*! called after a node was copied

        \param nodeIDMap map with old node ids to new node ids
        */
        void onPostCopyLink(std::map<uint32, uint32> &nodeIDMap);

        /*! update by distance

        \param distance the min distance to a trigger
        */
        void update(float32 distance);

        /*! sets the current world aka absolute position

        \param pos new position to be set
        */
        void onUpdateTransform(iaMatrixd &matrix);

        /*! unregisters switch node from scene if it belonged to a scene before
        */
        virtual void onPreSetScene();

        /*! registers to new scene
        */
        virtual void onPostSetScene();

        /*! initializes memeber varialbes
        */
        iNodeLODSwitch();

        /*! copy ctor
        */
        iNodeLODSwitch(iNodeLODSwitch *node);

        /*! dtor
		*/
        virtual ~iNodeLODSwitch();
    };

}; // namespace igor

#endif
