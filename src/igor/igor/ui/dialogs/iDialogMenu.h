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

#ifndef IGOR_DIALOGMENU_H
#define IGOR_DIALOGMENU_H

#include <igor/ui/dialogs/iDialog.h>
#include <igor/ui/actions/iAction.h>
#include <igor/ui/layouts/iWidgetBoxLayout.h>

namespace igor
{

    class iWidgetMenu;
    typedef iWidgetMenu *iWidgetMenuPtr;

    /*! menu dialog

    technically a dialog it is used as menu, sub menu or context menu
	*/
    class IGOR_API iDialogMenu : public iDialog
    {

        friend class iWidgetMenu;

    public:
        /*! init members

		\param parent the parent of this widget
		*/
        iDialogMenu(const iWidgetPtr parent = nullptr);

        /*! de initializes gui
		*/
        virtual ~iDialogMenu() = default;

        /*! adds action to menu

        only adds actions that are registered to the action manager
        actions are not owned by the menu

        \param action the action to be added
        \param context the action cintext
        \param enabled if true widget will be enabled
        */
        void addAction(const iActionPtr action, const iActionContextPtr context = nullptr, bool enabled = true);

        /*! same as add actions just by action name

        requires that the action we are searching for was already registered to the action manager

        \param actionName name of the action to be added
        \param context the action cintext
        \param enabled if true widget will be enabled
        */
        void addAction(const iaString &actionName, const iActionContextPtr context = nullptr, bool enabled = true);

        /*! \returns true if dialog has actions on it
        */
        bool hasActions() const;

        /*! add callback entry to menu

        \param delegate the delegate that will be called on click
        \param title the title or brief description
        \param description the full description (used as tooltip)
        \param iconAlias an alias or id for texture resource
        \param enabled if true widget will be enabled
        */
        void addCallback(iClickDelegate delegate, const iaString &title, const iaString &description = "", const iaString &iconAlias = "", bool enabled = true);

        /*! adds a menu to the menu

        \param menu the menu to add
        */
        void addMenu(const iWidgetMenuPtr menu);

        /*! adds a spacer
        */
        void addSeparator();

        /*! shows the dialog on screen

        \param dialogCloseDelegate the delegate to call after the dialog was closed
        \param modal if true open this dialog as modal
        */
        virtual void open(iDialogCloseDelegate dialogCloseDelegate = iDialogCloseDelegate(), bool modal = false) override;

        /*! clears the widget back to default
        */
        void clear() override;

    private:
        /*! main layout
        */
        iWidgetBoxLayoutPtr _vboxLayout = nullptr;

        /*! true if dialog has actions in it
        */
        bool _hasActions = false;

        /*! handle mouse off click event

		\param source the source of that event
		*/
        void onMouseOffClick(const iWidgetPtr source);

        /*! called when action was clicked
        */
        void onActionClick(const iWidgetPtr source);

        /*! initializes the gui
		*/
        void init();
    };

    /*! dialog menu pointer definition
    */
    typedef iDialogMenu *iDialogMenuPtr;

} // namespace igor

#endif // IGOR_DIALOGMENU_H