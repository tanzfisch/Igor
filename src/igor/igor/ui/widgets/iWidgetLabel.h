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

#ifndef __IGOR_WIDGETLABEL_H__
#define __IGOR_WIDGETLABEL_H__

#include <igor/ui/widgets/iWidget.h>

#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{

	/*! label widget
	*/
	class Igor_API iWidgetLabel : public iWidget
	{

	public:
		/*! ctor initializes member variables

		\param parent optional parent
		*/
		iWidgetLabel(const iWidgetPtr parent = nullptr);

		/*! does nothing
		*/
		virtual ~iWidgetLabel() = default;

		/*! sets maximum text width

		text that needs more space will automatically turn in to multi line text

		\param width max width
		*/
		void setMaxTextWidth(int32 width);

		/*! \returns max text width
		*/
		int32 getMaxTextWidth();

		/*! sets text

		\param text the label text
		*/
		void setText(const iaString &text);

		/*! \returns label text
		*/
		const iaString &getText() const;

	protected:
		/*! label text
		*/
		iaString _text;

		/*! max label width
		*/
		int32 _maxTextWidth = 0;

	private:
		/*! updates size based on it's content
		*/
		void calcMinSize() override;

		/*! draws the widget
		*/
		void draw();
	};

	/*! widget label pointer definition
	*/
	typedef iWidgetLabel *iWidgetLabelPtr;
} // namespace igor

#endif // __IGOR_WIDGETLABEL_H__
