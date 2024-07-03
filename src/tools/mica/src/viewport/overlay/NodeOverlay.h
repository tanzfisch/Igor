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

#ifndef __NODEOVERLAY__
#define __NODEOVERLAY__

#include "../../Workspace.h"

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
    virtual void setNodeID(uint64 nodeID);

    /*! sets node overlay active

    \param active true to set node overlay active
    */
    virtual void setActive(bool active);

    /*! \returns true id overlay is active
    */
    bool isActive() const;

    /*! sets overlay mode

    \param mode the new overlay mode
    */
    virtual void setOverlayMode(OverlayMode mode);

    /*! \returns true if mode in combination with node type can be handled by this node overlay

    \param mode the overlay mod
    \param nodeKind kind of node
    \param nodeType type of node
    */
    virtual bool accepts(OverlayMode mode, iNodeKind nodeKind, iNodeType nodeType) = 0;

    /*! handles mouse key down event

    \param event the mouse key down event
    \returns true if consumed
    */
    virtual bool onMouseKeyDownEvent(iEventMouseKeyDown &event);

    /*! handles mouse key up event

    \param event the mouse key up event
    \returns true if consumed
    */
    virtual bool onMouseKeyUpEvent(iEventMouseKeyUp &event);

    /*! handles mouse move event

    \param event the mouse move event
    \returns true if consumed
    */
    virtual bool onMouseMoveEvent(iEventMouseMove &event);

    /*! called when key was pressed

    \param event the event to handle
    */
    virtual bool onKeyDown(iEventKeyDown &event);

    /*! triggered when selection in scene changed

    \param event the event handle
    */
    virtual bool onSceneSelectionChanged(iEventSceneSelectionChanged &event);

    /*! \returns id of controled node
     */
    uint64 getNodeID() const;

    /*! \returns current overlay mode
     */
    OverlayMode getOverlayMode() const;

    /*! \returns mica workspace
     */
    WorkspacePtr getWorkspace() const;

    /*! \returns the node overlay scene
     */
    iScenePtr getScene() const;

    /*! \returns the view
     */
    iViewPtr getView() const;

private:
    /*! id of node connected to this node overlay
     */
    uint64 _nodeID = iNode::INVALID_NODE_ID;

    /*! the overlay mode
     */
    OverlayMode _overlayMode = OverlayMode::None;

    /*! mica workspace
     */
    WorkspacePtr _workspace;

    /*! the node overlay scene
     */
    iScenePtr _scene;

    /*! the view
     */
    iViewPtr _view = nullptr;

    /*! if overlay is active or not
    */
    bool _active = false;
};

/*! node overlay pointer definition
 */
typedef std::shared_ptr<NodeOverlay> NodeOverlayPtr;

#endif // __NODEOVERLAY__
