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
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef __IGOR_DIALOGDECISIONBOX__
#define __IGOR_DIALOGDECISIONBOX__

#include <igor/ui/dialogs/iDialog.h>

#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{

    class iWidgetCheckBox;

    /*! the decision box dialog
	*/
    class IGOR_API iDialogDecisionBox : public iDialog
    {

    public:
        /*! does nothing
		*/
        iDialogDecisionBox(const iWidgetPtr parent = nullptr);

        /*! does nothing
		*/
        ~iDialogDecisionBox() = default;

        /*! show/open the decision box

        \param dialogCloseDelegate delegate to handle the close decision box event
        \param title the dialog title
		\param message the message
		\param radioButtonTexts besides the message you can add a selection of radio buttons
		\param preSelection optional preselection of radio buttons
		*/
        void open(iDialogCloseDelegate dialogCloseDelegate, const iaString &title, const iaString &message, std::initializer_list<iaString> radioButtonTexts, int32 preSelection = -1);

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
        void initGUI(const iaString &title, const iaString &message, std::initializer_list<iaString> radioButtonTexts, int32 preSelection);
    };

    /*! dialog decision box pointer definition
	*/
    typedef iDialogDecisionBox *iDialogDecisionBoxPtr;

} // namespace igor

#endif // __IGOR_DIALOGDECISIONBOX__