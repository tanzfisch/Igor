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

#ifndef IGOR_USERCONTROL_VECTOR_H
#define IGOR_USERCONTROL_VECTOR_H

#include <igor/ui/user_controls/iUserControl.h>

#include <igor/ui/widgets/iWidgetLineTextEdit.h>

namespace igor
{

    /*! vector 3 user control
     */
    class IGOR_API iUserControlVector : public iUserControl
    {

    public:
 
        /*! ctor initializes member variables

        \param dimensions n dimensions (value 1 or greater)
        \param parent the optional parent
        */
        iUserControlVector(uint32 dimensions, const iWidgetPtr parent = nullptr);

        /*! clean up
         */
        ~iUserControlVector() = default;

        /*! \returns value for given index

        \param index the dimension index
         */
        float64 getValue(uint32 index) const;

        /*! set value for given index

        \param index the dimension index
        \param value the value to set on given index
        */
        void setValue(uint32 index, float32 value);

    private:
        /*! text fields
         */
        std::vector<iWidgetLineTextEditPtr> _textValues;

        /*! dimensions
        */
        uint32 _dimensions;

        /*! initializes the gui
         */
        void initGUI();

        /*! called when any value was changed
        */
        void onValueChanged(iWidgetPtr source);
    };

    /*! user control vector pointer definition
     */
    typedef iUserControlVector *iUserControlVectorPtr;

} // namespace igor

#endif // IGOR_USERCONTROL_VECTOR3_H