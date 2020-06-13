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

#ifndef __iDIALOGDECISIONBOX__
#define __iDIALOGDECISIONBOX__

#include <igor/ui/dialogs/iDialog.h>

#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{

	class iWidgetCheckBox;

	/*! the decision box dialog
	*/
	class Igor_API iDialogDecisionBox : public iDialog
	{

	public:
		/*! does nothing
		*/
		iDialogDecisionBox() = default;

		/*! deinitializes gui
		*/
		~iDialogDecisionBox() = default;

		/*! \returns the widgets type
        */
		virtual iWidgetType getWidgetType() const override;

		/*! show/open the decision box

        \param dialogCloseDelegate delegate to handle the close desicion box event
		\param message the message
		\param radioButtonTexts besides the message you can add a selection of radio buttons
		\param preSelection optional preselection of radio buttons
		*/
		void open(iDialogCloseDelegate dialogCloseDelegate, const iaString &message, std::initializer_list<iaString> radioButtonTexts, int32 preSelection = -1);

		/*! \returns selection index

        -1 for no selection
        */
		int32 getSelection() const;

	private:
		/*! radio buttons
		*/
		std::vector<iWidgetCheckBox *> _radioButtons;

		/*! handles ok button clicked event

		\param source the ok button it self
		*/
		void onOK(const iWidgetPtr source);

		/*! handles cancel button clicked event

		\param source the cancel button it self
		*/
		void onCancel(const iWidgetPtr source);

		/*! initializes the gui elements
		*/
		void initGUI(const iaString &message, std::initializer_list<iaString> radioButtonTexts, int32 preSelection);
	};

} // namespace igor

#endif