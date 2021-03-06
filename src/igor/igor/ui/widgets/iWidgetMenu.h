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
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __IGOR_WIDGETMENU_H__
#define __IGOR_WIDGETMENU_H__

#include <igor/ui/dialogs/iDialog.h>
#include <igor/ui/actions/iAction.h>

#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{

    class iWidgetLabel;
    typedef iWidgetLabel *iWidgetLabelPtr;
    class iDialogMenu;
    typedef iDialogMenu *iDialogMenuPtr;
    class iWidgetMenu;
    typedef iWidgetMenu *iWidgetMenuPtr;
    class iWidgetSpacer;
    typedef iWidgetSpacer *iWidgetSpacerPtr;
    class iWidgetPicture;
    typedef iWidgetPicture *iWidgetPicturePtr;

    /*! menu widget
    */
    class IGOR_API iWidgetMenu : public iWidget
    {

        friend class iWidgetMenuBar;
        friend class iDialogMenu;

    public:
        /*! initializes gui elements
        */
        iWidgetMenu(const iWidgetPtr parent = nullptr);

        /*! clean up
        */
        virtual ~iWidgetMenu();

        /*! sets the menu title

        \param title the menu title
        */
        void setTitle(const iaString &title);

        /*! \returns the menu's title
        */
        const iaString &getTitle() const;

        /*! adds action to menu

        only adds actions that are registered to the action manager
        actions are not owned by the menu

        \param action the action to be added
        */
        void addAction(const iActionPtr action, const iActionContextPtr context = nullptr);

        /*! same as add actions just by action name

        requires that the action we are searchign for was already registered to the action manager

        \param actionName name of the action to be added
        */
        void addAction(const iaString &actionName, const iActionContextPtr context = nullptr);

        /*! adds a menu to the menu

        \param menu the menu to add
        */
        void addMenu(const iWidgetMenuPtr menu);

    private:
        /*! menu title
        */
        iWidgetLabelPtr _title = nullptr;

        /*! spacer
        */
        iWidgetSpacerPtr _spacer = nullptr;

        /*! picture
        */
        iWidgetPicturePtr _picture = nullptr;

        /*! menu body
        */
        iDialogMenuPtr _dialogMenu = nullptr;

        /*! ID of parent in menu hierarchy
        */
        iWidgetID _menuParent = IGOR_INVALID_ID;

        /*! defines the menu parent

        \param menuParent the menu parent
        */
        void setMenuParent(iWidgetPtr menuParent);

        /*! called when submenu closed

        \param returnState the return state of the closed sub menu
        */
        void onSubMenuClosed(iDialogReturnState returnState);

        /*! called when the parent of this widget changed
        */
        void onParentChanged() override;

        /*! called after sub menu was closed
        */
        void onDialogClose(iDialogPtr dialog);

        /*! initialize GUI elements
        */
        void init();

        /*! updates the widget
        */
        void update();

        /*! handles when menu was clicked on
        */
        void onClick(const iWidgetPtr source);
    };

    /*! menu widget pointer definition
	*/
    typedef iWidgetMenu *iMenuPtr;
} // namespace igor

#endif // __IGOR_WIDGETMENU_H__
