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

#ifndef __NODEOVERLAY__
#define __NODEOVERLAY__

#include "../Workspace.h"

#include <memory>

/*! overlay modes
 */
enum class OverlayMode
{
    None,
    Translate,
    Scale,
    Rotate
};

/*! 3d overlay ui element for scene nodes

    to modify position, orientation and scale of objects in the scene
    to display additional information when nodes are selected
*/
class NodeOverlay
{

    friend class OverlayLayer;

public:
    /*! initialize node overlay

    \param view the view to use
    \param scene the scene to use
    \param workspace the mica workspace
    */
    NodeOverlay(iViewPtr view, iScenePtr scene, WorkspacePtr workspace);

    /*! cleanup
     */
    ~NodeOverlay();

    /*! sets the node to control by ID

    \param nodeID id of node to control
    */
    void setNodeID(uint64 nodeID);

    /*! \returns id of controled node
     */
    uint64 getNodeID() const;

    /*! sets node overlay visible

    \param visible true to set node overlay visible
    */
    virtual void setVisible(bool visible);

    /*! \returns true if node overlay is visible
     */
    virtual bool isVisible() const;

    /*! \returns true the node overlay is selected
     */
    virtual bool isSelected() const;

    /*! sets the mode of the node overlay

    \param nodeOverlayMode the node overlay mode
    */
    virtual void setOverlayMode(OverlayMode nodeOverlayMode);

    /*! \returns the current node overlay mode
     */
    OverlayMode getOverlayMode() const;

    /*! selects node inside of node overlay if present

    \returns true if node overlay has control and can select given nodeID
    */
    virtual bool select(iNodeID nodeID);

    /*! unselect what ever may be selected
     */
    virtual void unselect();

protected:
    /*! mica workspace
     */
    WorkspacePtr _workspace;

    /*! the node overlay scene
     */
    iScenePtr _scene;

    /*! the view
     */
    iViewPtr _view = nullptr;

    /*! on mouse moved callback

    \param from the mouse moved from here
    \param to the mouse moved to here
    */
    virtual void onMouseMoved(const iaVector2i &from, const iaVector2i &to);

private:
    /*! id of node connected to this node overlay
     */
    uint64 _nodeID = iNode::INVALID_NODE_ID;

    /*! if true node overlay is visible
     */
    bool _visible = false;

    /*! current user node overlay mode
     */
    OverlayMode _userNodeOverlayMode = OverlayMode::None;
};

/*! node overlay pointer definition
 */
typedef NodeOverlay *NodeOverlayPtr;

#endif // __NODEOVERLAY__
