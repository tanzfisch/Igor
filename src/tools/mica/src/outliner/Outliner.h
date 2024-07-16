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

#ifndef MICA_OUTLINER_H
#define MICA_OUTLINER_H

#include <igor/igor.h>
using namespace igor;

/*! outliner

    a hierarchy of scenes and their entities
 */
class Outliner : public iDialog
{

    friend class iWidgetManager;

public:
    /*! init ui
     */
    Outliner();

private:
    /*! main layout
     */
    iWidgetBoxLayoutPtr _layout = nullptr;

    /*! tree view
     */
    iUserControlTreeViewPtr _treeView = nullptr;

    /*! tree view data
     */
    std::unique_ptr<iItemData> _itemData;

    /*! the context menu
     */
    iDialogMenu _contextMenu;

    /*! resource to do actions on
     */
    iResourceID _contextResourceID;

    /*! init user interface
     */
    void initGUI();

    /*! handles click in tree view

    \param source the source widget of this event
    */
    void onClickTreeView(const iWidgetPtr source);

    /*! handles context menu for tree view

    \param source the source widget of this event
    */
    void onContextMenuTreeView(const iWidgetPtr source);

    /*! populate the entity tree
     */
    void populateTree();

    /*! populate the entity tree
    */
    void populateTree(iItemPtr item, iEntityScenePtr scene);

    /*! drag move handle

    \param drag the drag data
    \param mousePos the current mouse pos
    */
    void onDragMove(iDrag &drag, const iaVector2f &mousePos) override;

    /*! drop handle

    \param drag the drag data
    \param mousePos the current mouse pos
    */
    void onDrop(const iDrag &drag, const iaVector2f &mousePos) override;

    /*! called when entity was created

    \param entity the entity that was created
    */
    void onEntityCreated(iEntityPtr entity);

    /*! called when entity is about to be destroyed

    \param entity the entity that is about to be destroyed
    */
    void onEntityDestroyed(iEntityPtr entity);

    /*! called when scene was added

    \param sceneID the resource id of the scene
    */
    void onSceneAdded(const iResourceID &sceneID);

    /*! called when scene was removed

    \param sceneID the resource id of the scene
    */
    void onSceneRemoved(const iResourceID &sceneID);

    /*! called when project was loaded
     */
    void onProjectLoaded();

    /*! called when project was unloaded
     */
    void onProjectUnloaded();    

    /*! called when user want's to load a scene
     */
    void onLoadScene(const iWidgetPtr source);

    /*! called when scene was loaded
     */
    void onResourceLoaded(iResourceID resourceID);

    /*! called when user want's to unload a scene
     */
    void onUnloadScene(const iWidgetPtr source);

    /*! called when widget was queued for refresh in last frame
     */
    void onRefresh() override;
};

#endif // MICA_OUTLINER_H
