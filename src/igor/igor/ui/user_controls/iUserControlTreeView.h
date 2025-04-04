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

#ifndef IGOR_USERCONTROL_TREEVIEW_H
#define IGOR_USERCONTROL_TREEVIEW_H

#include <igor/ui/user_controls/iUserControl.h>

#include <igor/data/iItemData.h>
#include <igor/ui/layouts/iWidgetBoxLayout.h>
#include <igor/ui/widgets/iWidgetButton.h>

namespace igor
{

    /*! widget click event
     */
    IGOR_EVENT_DEFINITION(iClickTreeView, const iWidgetPtr);

    /*! context menu event
     */
    IGOR_EVENT_DEFINITION(iContextMenuTreeView, const iWidgetPtr);

    /*! tree view widget
     */
    class IGOR_API iUserControlTreeView : public iUserControl
    {

    public:
        /*! ctor initializes member variables

        \param parent the optional parent
        */
        iUserControlTreeView(const iWidgetPtr parent = nullptr);

        /*! clean up
         */
        virtual ~iUserControlTreeView();

        /*! sets tree items

        caller keeps ownership

        \param itemData item data container
        */
        void setItems(iItemData *itemData);

        /*! \returns selected item path
         */
        const iaString &getSelectedItemPath() const;

        /*! \returns the click event
         */
        iClickTreeViewEvent &getClickEvent();

        /*! \returns context menu event
         */
        iContextMenuTreeViewEvent &getContextMenuTreeViewEvent();

        /*! clears the widget back to default
         */
        void clear() override;

    protected:
        /*! box layout
         */
        iWidgetBoxLayoutPtr _vboxLayout = nullptr;

        /*! click event
         */
        iClickTreeViewEvent _clickEvent;

        /*! context menu event
         */
        iContextMenuTreeViewEvent _contextMenuTreeViewEvent;

        /*! selected item path
         */
        iaString _selectedItemPath;

        /*! hold on to all widgets
         */
        std::vector<iWidgetButtonPtr> _allInteractiveWidgets;

        /*! handle click events from our buttons

        \param source the widget that was clicked
        */
        void onClick(const iWidgetPtr source);

        /*! called when context menu is to be opened

        \param source the widget that was clicked
        */
        void onContextMenu(const iWidgetPtr source);

        /*! initializes ui
         */
        void initUI();

        /*! updates ui from tree items

        \param item the current item to update from
        \param itemPath the path of the current item
         */
        virtual void updateUI(iItem *item, const iaString &itemPath);
    };

    /*! widget tree view pointer definition
     */
    typedef iUserControlTreeView *iUserControlTreeViewPtr;

} // namespace igor

#endif // IGOR_USERCONTROL_TREEVIEW_H