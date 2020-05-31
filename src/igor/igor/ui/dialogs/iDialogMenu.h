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
// contact: martinloga@gmx.de

#ifndef __iDIALOG_MENU__
#define __iDIALOG_MENU__

#include <igor/ui/dialogs/iDialog.h>

namespace Igor
{

    class iWidgetGrid;
    typedef iWidgetGrid *iWidgetGridPtr;
    class iWidgetMenu;
    typedef iWidgetMenu *iWidgetMenuPtr;
    class iAction;
    typedef iAction *iActionPtr;

    /*! menu dialog

    technically a dialog it is used as menu, sub menu or context menu
	*/
    class Igor_API iDialogMenu : public iDialog
    {

        friend class iWidgetMenu;

    public:
        /*! does nothing
		*/
        iDialogMenu();

        /*! deinitializes gui
		*/
        virtual ~iDialogMenu() = default;

        /*! \returns the widgets type
        */
        virtual iWidgetType getWidgetType() const override;

        /*! adds action to menu

        only adds actions that are registered to the action manager
        actions are not owned by the menu

        \param action the action to be added
        */
        void addAction(const iActionPtr action);

        /*! same as add actions just by action name

        requires that the action we are searchign for was already registered to the action manager

        \param actionName name of the action to be added
        */
        void addAction(const iaString &actionName);

        /*! adds a menu to the menu

        \param menu the menu to add
        */
        void addMenu(const iWidgetMenuPtr menu);

        /*! shows the dialog on screen

        \param dialogCloseDelegate the delegate to call after the dialog was closed
        */
        virtual void open(iDialogCloseDelegate dialogCloseDelegate) override;

    private:
        /*! main grid
        */
        iWidgetGridPtr _grid = nullptr;

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

} // namespace Igor

#endif