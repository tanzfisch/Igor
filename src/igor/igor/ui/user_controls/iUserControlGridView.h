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

#ifndef __IGOR_USERCONTROL_GRIDVIEW__
#define __IGOR_USERCONTROL_GRIDVIEW__

#include <igor/ui/user_controls/iUserControl.h>
#include <igor/ui/widgets/iWidgetBoxLayout.h>
#include <igor/data/iItem.h>

namespace igor
{

    /*! widget click event
     */
    IGOR_EVENT_DEFINITION(iClickGridView, void, const iWidgetPtr);

    /*! tree view widget
     */
    class IGOR_API iUserControlGridView : public iUserControl
    {

    public:
        /*! ctor initializes member variables

        \param parent the optional parent
        */
        iUserControlGridView(const iWidgetPtr parent = nullptr);

        /*! sets tree items

        caller keeps ownership of items

        \param items the root item of the tree items
        */
        void setItems(iItem *items);

        /*! \returns the click event
        */
        iClickGridViewEvent& getClickEvent();

    protected:
        /*! items
         */
        iItem *_items = nullptr;

        /*! the click event
        */
        iClickGridViewEvent _clickEvent;

        iWidgetBoxLayoutPtr _vboxLayout = nullptr;

        /*! handle click events from our buttons

        \param source the widget that was clicked
        */
        void onClick(const iWidgetPtr source);

        /*! initializes ui
         */
        void initUI();
    };

    /*! widget tree view pointer definition
     */
    typedef iUserControlGridView *iUserControlGridViewPtr;

} // namespace igor

#endif // __IGOR_USERCONTROL_GRIDVIEW__
