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

#ifndef __IGOR_WIDGETGROUPBOX__
#define __IGOR_WIDGETGROUPBOX__

#include <igor/ui/widgets/iWidget.h>

#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{

    class iUserControl;

    /*! group box widget

	\todo collapsable group box would be nice/interessting
	*/
    class IGOR_API iWidgetGroupBox : public iWidget
    {

    public:
        /*! ctor initializes member variables

		\param parent optional parent
		*/
        iWidgetGroupBox(const iWidgetPtr parent = nullptr);

        /*! does nothing
		*/
        ~iWidgetGroupBox() = default;

        /*! sets text of widget

		\param text new text
		*/
        void setText(const iaString &text);

        /*! \returns text of widget
		*/
        const iaString &getText() const;

        /*! sets border size

		\param border size of border
		*/
        void setBorder(int32 border);

        /*! \returns border size of widget
		*/
        int32 getBorder();

        /*! setter for header only mode

		\param headerOnly if true there will only the header be visible
		*/
        void setHeaderOnly(bool headerOnly = true);

        /*! \returns if true: header only mode is on
		*/
        bool getHeaderOnly() const;

    private:
        /*! if true only the header of the group box will be drawn
		*/
        bool _headerOnly = false;

        /*! border size
		*/
        int32 _border = 5;

        /*! group box text
		*/
        iaString _text;

        /*! draws the widget
         */
        void draw() override;

        /*! updates size based on it's content
		*/
        void calcMinSize() override;
    };

    /*! widget group box pointer definition
	*/
    typedef iWidgetGroupBox *iWidgetGroupBoxPtr;
} // namespace igor

#endif // __IGOR_WIDGETGROUPBOX__
