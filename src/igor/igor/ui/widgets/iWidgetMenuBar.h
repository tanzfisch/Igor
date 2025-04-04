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
// (c) Copyright 2012-2025 by Martin Loga
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

#ifndef __IGOR_WIDGETMENUBAR__
#define __IGOR_WIDGETMENUBAR__

#include <igor/ui/widgets/iWidget.h>
#include <igor/ui/actions/iAction.h>
#include <igor/ui/layouts/iWidgetGridLayout.h>
#include <igor/ui/widgets/iWidgetMenu.h>

#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{

    /*! menu bar widget
    */
    class IGOR_API iWidgetMenuBar : public iWidget
    {

    public:
        /*! init gui
        */
        iWidgetMenuBar(const iWidgetPtr parent = nullptr);

        /*! does nothing
        */
        virtual ~iWidgetMenuBar() = default;

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

        /*! adds a menu to the menu bar

        \param menu the menu to add
        */
        void addMenu(const iWidgetMenuPtr menu);

        /*! unselects the menu bar
        */
        void unselect();

    private:
        /*! menu grid
        */
        iWidgetGridLayoutPtr _grid;

        /*! initializes ui elements
        */
        void init();
    };

    /*! menu widget pointer definition
	*/
    typedef iWidgetMenuBar *iWidgetMenuBarPtr;

} // namespace igor

#endif // __IGOR_WIDGETMENUBAR__
