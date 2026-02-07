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
// (c) Copyright 2012-2026 by Martin A. Loga
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

#ifndef IGOR_USERCONTROL_PARAMETER_H
#define IGOR_USERCONTROL_PARAMETER_H

#include <igor/ui/user_controls/iUserControl.h>

#include <igor/ui/widgets/iWidgetLineTextEdit.h>
#include <igor/data/iAny.h>

namespace igor
{

    /*! parameter user control
     */
    class IGOR_API iUserControlParameter : public iUserControl
    {

    public:
        /*! ctor initializes member variables

        \param type the data type
        \param parent the optional parent
        */
        iUserControlParameter(const iAnyType &type, const iWidgetPtr parent = nullptr);

        /*! clean up
         */
        ~iUserControlParameter() = default;

        /*! \returns value
         */
        const iAny &getValue() const;

        /*! sets value
        */
        void setValue(const iAny &value);

        /*! \returns name of parameter
        */
        const iaString& getName() const;

        /*! sets name of parameter

        \param name the name to set
        */
        void setName(const iaString &name);

    private:
        /*! initializes the gui
         */
        void onInitUI(const iAnyType &type);

        /*! called when any value was changed
         */
        void onValueChanged(iWidgetPtr source);
    };

    /*! user control parameter pointer definition
     */
    typedef iUserControlParameter *iUserControlParameterPtr;

} // namespace igor

#endif // IGOR_USERCONTROL_PARAMETER_H