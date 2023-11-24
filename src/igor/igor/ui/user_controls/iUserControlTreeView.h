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

#ifndef __IGOR_USERCONTROL_TREEVIEW__
#define __IGOR_USERCONTROL_TREEVIEW__

#include <igor/ui/user_controls/iUserControl.h>

namespace igor
{

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

    protected:

        /*! initializes ui
        */
        void initUI();

        /*! draws the widget
         */
        void draw() override;
    };

    /*! widget tree view pointer definition
     */
    typedef iUserControlTreeView *iUserControlTreeViewPtr;

} // namespace igor

#endif // __IGOR_USERCONTROL_TREEVIEW__
