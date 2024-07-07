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

    /*! init user interface
     */
    void initGUI();

    /*! handles click in tree view

    \param source the source widget of this event
    */
    void onClickTreeView(const iWidgetPtr source);

    /*! populate the entity tree
     */
    void populateTree();

    /*! populate scene
     */
    void populateScene(iEntityScenePtr scene, iItemPtr sceneItem);

    void onEntityCreated(iEntityPtr scene);
    void onEntityDestroyed(iEntityPtr scene);
};

#endif // MICA_OUTLINER_H
