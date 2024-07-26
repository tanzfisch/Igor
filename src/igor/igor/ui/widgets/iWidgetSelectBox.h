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

#ifndef __IGOR_WIDGETSELECTBOX__
#define __IGOR_WIDGETSELECTBOX__

#include <igor/ui/widgets/iWidget.h>

#include <any>

namespace igor
{

    class iDialog;
    typedef iDialog *iDialogPtr;
    class iTextureFont;
    class iWidgetManager;
    class iDialogIndexMenu;

    /*! select box widget
	*/
    class IGOR_API iWidgetSelectBox : public iWidget
    {

    public:
        /*! ctro initializes member variables

		\param parent optional parent
		*/
        iWidgetSelectBox(const iWidgetPtr parent = nullptr);

        /*! clean up
		*/
        virtual ~iWidgetSelectBox();

        /*! adds entry to select box

		\param entryText the entry text
		\param userData optional user data (use same type of user data for every entry)
		*/
        void addItem(const iaString &entryText, const std::any &userData = std::any());

        /*! clears all entries

		Attention! if you used user data you have to delete it your self
		*/
        void clear();

        /*! sets selection by index

        \param index the index to set
		*/
        void setSelection(uint32 index);

        /*! sets selection by user data

        works on ly same type of user data for every entry was used

        \param userData the user data to set
        */
        void setSelection(const std::any &userData);

        /*! \returns amount of select box entries
		*/
        uint32 getItemCount() const;

        /*! \returns index of selected entry
		*/
        uint32 getSelectedIndex() const;

        /*! \returns text of selected entry
		*/
        iaString getSelectedValue() const;

        /*! \returns user data pointer of selected entry
		*/
        const std::any getSelectedUserData() const;

    private:
        /*! list of select box entries
		*/
        std::vector<std::pair<iaString, std::any>> _entries;

        /*! current selected index

		-1 stand for no selection
		*/
        int32 _currentSelection = -1;

        /*! true: if the select box is unfoldet
		*/
        bool _openSelectBox = false;

        /*! select box dialog used for the unfold effect
		*/
        iDialogIndexMenu *_selectBox = nullptr;

        /*! appearance state of the button
		*/
        iWidgetState _buttonAppearanceState = iWidgetState::Standby;

        /*! triggered when selection box closed

        \param dialog source of this event
		*/
        void onSelectBoxClosed(iDialogPtr dialog);

        /*! handles incoming mouse key down events

        \param event mouse key down event
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool onMouseKeyDown(iEventMouseKeyDown &event) override;

        /*! handles mouse key up events

        \param event the mouse key up event
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool onMouseKeyUp(iEventMouseKeyUp &event) override;

        /*! handles incoming mouse move events

        \param event mouse move event
        */
        void onMouseMove(iEventMouseMove &event) override;

        /*! updates size based on it's content
		*/
        void calcMinSize() override;

        /*! draws the widget
         */
        void draw() override;
    };

    /*! select box pointer definition
    */
    typedef iWidgetSelectBox* iWidgetSelectBoxPtr;

} // namespace igor

#endif // __IGOR_WIDGETSELECTBOX__
