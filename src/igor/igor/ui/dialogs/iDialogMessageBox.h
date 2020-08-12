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
// contact: igorgameengine@protonmail.com

#ifndef __IGOR_DIALOGMESSAGEBOX_H__
#define __IGOR_DIALOGMESSAGEBOX_H__

#include <igor/ui/dialogs/iDialog.h>

#include <iaux/system/iaEvent.h>
#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{

    /*! message box button configuration
	*/
    enum class iMessageBoxButtons
    {
        Ok,
        CancelOk,
        YesNo,
        YesNoCancel
    };

    /*! the message box
	*/
    class IGOR_API iDialogMessageBox : public iDialog
    {

    public:
        /*! init members

		\param parent the parent of this widget
		*/
        iDialogMessageBox(const iWidgetPtr parent = nullptr);

        /*! deinitializes gui
		*/
        ~iDialogMessageBox() = default;

        /*! initializes gui and opens the message box dialog

		\param dialogCloseDelegate the close delegate
		\param message the message to display
		\param buttons the button configuration to use
		*/
        void open(iDialogCloseDelegate dialogCloseDelegate, iaString message, iMessageBoxButtons buttons = iMessageBoxButtons::Ok);

        /*! initializes gui and opens the message box dialog

		\param message the message to display
		\param buttons the button configuration to use
		*/
        void open(iaString message, iMessageBoxButtons buttons = iMessageBoxButtons::Ok);

    private:
        /*! handles ok button clicked event

		\param source the ok button it self
		*/
        void onOK(const iWidgetPtr source);

        /*! handles cancel button clicked event

		\param source the cancel button it self
		*/
        void onCancel(const iWidgetPtr source);

        /*! handles yes button clicked event

		\param source the yes button it self
		*/
        void onYes(const iWidgetPtr source);

        /*! handles no button clicked event

		\param source the no button it self
		*/
        void onNo(const iWidgetPtr source);
    };

} // namespace igor

#endif // __IGOR_DIALOGMESSAGEBOX_H__