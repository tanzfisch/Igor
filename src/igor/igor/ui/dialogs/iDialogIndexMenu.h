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

#ifndef __IGOR_DIALOGINDEXMENU_H__
#define __IGOR_DIALOGINDEXMENU_H__

#include <igor/ui/dialogs/iDialog.h>

#include <iaux/system/iaEvent.h>
#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{

    iaEVENT(iDialogIndexMenuCloseEvent, iDialogIndexMenuCloseDelegate, (int32 index), (index));

    /*! pull down menu

	\deprecated remove this
	*/
    class IGOR_API iDialogIndexMenu : public iDialog
    {

    public:
        /*! init members

		\param parent the parent of this widget
		*/
        iDialogIndexMenu(const iWidgetPtr parent = nullptr);

        /*! deinitializes gui
		*/
        virtual ~iDialogIndexMenu() = default;

        /*! opens dialog

		leave the pictures list empty if you don't want pictures or call the alternative implementation of show

		\param dialogCloseDelegate delegate for closing event
		\param texts the texts to put in the selection list
		*/
        void open(iDialogCloseDelegate dialogCloseDelegate, std::vector<iaString> &texts);

        /*! opens dialog

		leave the pictures list empty if you don't want pictures or call the alternative implementation of show

		\param dialogCloseDelegate delegate for closing event
		\param texts the texts to put in the selection list
		\param pictures paths to textures used as icons next to the text
		*/
        void open(iDialogCloseDelegate dialogCloseDelegate, std::vector<iaString> &texts, std::vector<iaString> &pictures);

        /*! \returns selected menu entry index

        returns -1 if nothing was selected
        */
        int32 getSelectionIndex() const;

    private:
        /*! the close event
		*/
        iDialogIndexMenuCloseEvent _selectBoxCloseEvent;

        /*! the return value of the selection box

		-1 stands for cancel
		*/
        int32 _returnValue = -1;

        /*! handles change event

		\param source the source of the event (should be the grid)
		*/
        void onChange(const iWidgetPtr source);

        /*! handle mouse off click event

		\param source the source of that event
		*/
        void onMouseOffClick(const iWidgetPtr source);

        /*! initializes the gui

		\param texts the texts for the menu
		*/
        void initGUI(std::vector<iaString> &texts);

        /*! initializes the gui

		\param texts the texts for the menu
		\param pictures the pictures for the menu
		*/
        void initGUI(std::vector<iaString> &texts, std::vector<iaString> &pictures);
    };

    /*! dialog menu pointer definition
    */
    typedef iDialogIndexMenu *iDialogIndexMenuPtr;

} // namespace igor

#endif // __IGOR_DIALOGINDEXMENU_H__