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

#ifndef IGOR_USERCONTROL_TREEVIEW_H
#define IGOR_USERCONTROL_TREEVIEW_H

#include <igor/ui/user_controls/iUserControl.h>

#include <igor/data/iItem.h>
#include <igor/ui/layouts/iWidgetBoxLayout.h>
#include <igor/ui/widgets/iWidgetButton.h>

#include <any>
#include <vector>

namespace igor
{

    /*! widget click event
     */
    IGOR_EVENT_DEFINITION(iClickTreeView, void, const iWidgetPtr);

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

        caller keeps ownership of items

        \param items the root item of the tree items
        */
        void setItems(iItem *items);

        /*! \returns selected item path
        */
        const iaString& getSelectedItemPath() const;

        /*! \returns the click event
        */
        iClickTreeViewEvent& getClickEvent();

    protected:
        /*! root of tree items
         */
        iItem *_root = nullptr;

        /*! box layout
         */
        iWidgetBoxLayoutPtr _vboxLayout = nullptr;

        /*! context menu event
         */
        iClickTreeViewEvent _clickEvent;        

        /*! selected item path
        */
        iaString _selectedItemPath;

        /*! hold on to all buttons so we can control if they are checked or not
        */
        std::vector<iWidgetButtonPtr> _allButtons;

        /*! handle click events from our buttons

        \param source the widget that was clicked
        */
        void onClick(const iWidgetPtr source);

        /*! initializes ui
         */
        void initUI();

        /*! updates ui from tree items
         */
        void updateUI(iItem *item, const iaString &itemPath, int indentation);
    };

    /*! widget tree view pointer definition
     */
    typedef iUserControlTreeView *iUserControlTreeViewPtr;

} // namespace igor

#endif // IGOR_USERCONTROL_TREEVIEW_H