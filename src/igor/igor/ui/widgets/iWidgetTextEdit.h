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

#ifndef __IGOR_WIDGETTEXTEDIT__
#define __IGOR_WIDGETTEXTEDIT__

#include <igor/ui/widgets/iWidget.h>

namespace igor
{

    /*! text edit widget

    for now non interactive
	*/
    class IGOR_API iWidgetTextEdit : public iWidget
    {

    public:
        /*! initializes member variables

		\param parent optional parent
		*/
        iWidgetTextEdit(const iWidgetPtr parent = nullptr);

        /*! does nothing
		*/
        virtual ~iWidgetTextEdit() = default;

        /*! sets the text

		\param text the text to be set
		*/
        void setText(const iaString &text);

        /*! \returns the text
		*/
        const iaString &getText() const;

    private:
        /*! the text
		*/
        iaString _text;

        /*! updates size based on it's content
		*/
        void calcMinSize() override;

        /*! draws the widget
         */
        void draw() override;
    };

    /*! text widget pointer definition
    */
    typedef iWidgetTextEdit* iWidgetTextEditPtr;

} // namespace igor

#endif // __IGOR_WIDGETTEXTEDIT__
