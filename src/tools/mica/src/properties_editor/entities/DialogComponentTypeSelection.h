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

#ifndef IGOR_DIALOG_SELECT_COMPONENT_TYPE_H
#define IGOR_DIALOG_SELECT_COMPONENT_TYPE_H

#include <igor/igor.h>

namespace igor
{
    /*!  component type selection
     */
    class IGOR_API DialogComponentTypeSelection : public iDialog
    {

    public:
        /*! init members

        \param parent the parent of this widget
        */
        DialogComponentTypeSelection(const iWidgetPtr parent = nullptr);

        /*! de initializes gui
         */
        ~DialogComponentTypeSelection() = default;

        /*! initializes gui and opens the message box dialog

        \param dialogCloseDelegate the close delegate
        */
        void open(iDialogCloseDelegate dialogCloseDelegate);

    private:
        /*! init ui
         */
        void initGUI();
    };

    /*! pointer definition for message box
     */
    typedef DialogComponentTypeSelection *DialogComponentTypeSelectionPtr;

} // namespace igor

#endif // IGOR_DIALOG_SELECT_COMPONENT_TYPE_H