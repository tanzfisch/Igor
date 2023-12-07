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

#ifndef __IGOR_CHECKBOXWIDGET__
#define __IGOR_CHECKBOXWIDGET__

#include <igor/ui/widgets/iWidget.h>

#include <vector>

namespace igor
{

    class iTextureFont;

    /*! check box widget
	*/
    class IGOR_API iWidgetCheckBox : public iWidget
    {

    public:
        /*! ctor initializes member variables

		\param parent the optional parent
		*/
        iWidgetCheckBox(const iWidgetPtr parent = nullptr);

        /*! does nothing
		*/
        virtual ~iWidgetCheckBox() = default;

        /*! beginns a radio button group

		every check box that is created afterwards will be added to the same radio button group
		*/
        static void beginRadioButtonGroup();

        /*! ends the radio button group
		*/
        static void endRadioButtonGroup();

        /*! set text of check box
		*/
        void setText(const iaString &buttontext);

        /*! \retruns text of check box
		*/
        const iaString &getText() const;

        /*! sets the checked flag

		\param check bolean value to set if check box is checked
		*/
        void setChecked(bool check = true);

        /*! \returns true: if checked
		*/
        bool isChecked();

    protected:
        /*! check box text
		*/
        iaString _text;

        /*! boolean if check box is checked or not
		*/
        bool _checked = false;

        /*! handles mouse key up events

        \param event the mouse key up event
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool onMouseKeyUp(iEventMouseKeyUp &event) override;

        /*! if check box is part of a radio button group. this is the group
		*/
        std::vector<iWidgetCheckBox *> _radioButtons;

        /*! flag to determine if the next check box created is part of a radio button group
		*/
        static bool _generatingRadioButtonGroup;

        /*! radio button stack while creating a radio button group
		*/
        static std::vector<iWidgetCheckBox *> _currentRadioButtons;

        /*! updates size based on it's content
		*/
        void calcMinSize() override;

        /*! draws the widget
         */
        void draw() override;
    };

    /*! checkbox pointer definition
    */
    typedef iWidgetCheckBox *iWidgetCheckBoxPtr;
} // namespace igor

#endif // __IGOR_CHECKBOXWIDGET__
