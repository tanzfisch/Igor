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

#ifndef MICA_OUTLINER_H
#define MICA_OUTLINER_H

#include "../MicaDefines.h"

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
    void onTreeViewSelectionChanged(const iWidgetPtr source);

    /*! handles context menu for tree view

    \param source the source widget of this event
    */
    void onContextMenuTreeView(const iWidgetPtr source);

    /*! populate the entity tree
     */
    void populateTree();

    /*! populate the entity tree
     */
    void populateTree(iItemPtr item, iEntityPtr entity);

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

    /*! called when given entity's name changed

    \param entity the given entity
    */
    void onEntityNameChanged(iEntityPtr entity);

    /*! called when hierarchy of given scene changed

    \param scene the given scene
    */
    void onHierarchyChanged(iEntityScenePtr scene);

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
    bool onProjectLoaded(iEventProjectLoaded &event);

    /*! called when project was unloaded
     */
    bool onProjectUnloaded(iEventProjectUnloaded &event);

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

    /*! save prefab event

    \param source the event source
    */
    void onSavePrefab(const iWidgetPtr source);

    /*! populate outliner with sub scenes

    \param children list of entities that represent sub scenes or prefabs
    \param active if true this subscene will be displayed as active
    */
    void populateSubScenes(const std::vector<iEntityPtr> &children, bool active);

    /*! handles incoming generic event

    \param event the event
    */
    bool onEvent(iEvent &event) override;

    /*! handle selection change
     */
    void onSelectionChanged(const iEntitySceneID &sceneID, const std::vector<iEntityID> &entities);
};

#endif // MICA_OUTLINER_H
