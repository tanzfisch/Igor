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

#ifndef __IGOR_WIDGETMENU__
#define __IGOR_WIDGETMENU__

#include <igor/ui/actions/iAction.h>
#include <igor/ui/dialogs/iDialogMenu.h>

#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetSpacer.h>
#include <igor/ui/widgets/iWidgetPicture.h>

namespace igor
{

    /*! widget menu pointer definition
    */
    class iWidgetMenu;
    typedef iWidgetMenu *iWidgetMenuPtr;

    /*! menu widget
    */
    class IGOR_API iWidgetMenu : public iWidget
    {

        friend class iWidgetMenuBar;
        friend class iDialogMenu;

    public:
        /*! initializes gui elements

        \param title set the title of this menu
        \param parent parent widget
        */
        iWidgetMenu(const iaString &title = "", const iWidgetPtr parent = nullptr);

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

        requires that the action we are searching for was already registered to the action manager

        \param actionName name of the action to be added
        */
        void addAction(const iaString &actionName, const iActionContextPtr context = nullptr);

        /*! add callback entry to menu

        \param delegate the delegate that will be called on click
        \param title the title or brief description
        \param description the full description (used as tooltip)
        \param iconAlias an alias or id for texture resource
        */
        void addCallback(iClickDelegate delegate, const iaString &title, const iaString &description = "", const iaString &iconAlias = "");

        /*! adds a menu to the menu

        \param menu the menu to add
        */
        void addMenu(const iWidgetMenuPtr menu);

        /*! add separator to menu
        */
        void addSeparator();

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

#endif // __IGOR_WIDGETMENU__
