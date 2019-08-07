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
// (c) Copyright 2012-2019 by Martin Loga
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

#ifndef __iWIDGETSCROLL__
#define __iWIDGETSCROLL__

#include <iWidget.h>

#include <iaMatrix.h>
using namespace IgorAux;

namespace Igor
{

	class iUserControl;

	/*! scroll widget

	basically allows to have a child that is bigger an ads some srcoll bars then

	\todo need to use actual buttons for the scroll bars, or a scroll bar widget
	*/
	class Igor_API iWidgetScroll : public iWidget
	{

	public:

		/*! ctor initializes membervariables
		*/
		iWidgetScroll();

		/*! does nothing
		*/
		~iWidgetScroll() = default;

		/*! sets the width or thikness of the scrollbars
		*/
		void setScollbarWidth(int32 width);

		/*! \returns scrollbar width
		*/
		int32 getScollbarWidth();

		/*! sets horizontal scroll value

		\param value new value for horizontal scroll
		*/
		void setHorizontalScroll(float32 value);

		/*! sets vertical scroll value

		\param value new value for vertical scroll
		*/
		void setVerticalScroll(float32 value);

	private:

		/*! scroll bar width
		*/
		int32 _scrollbarWidth = 15;

		/*! vertical scroll factor 0.0 - 1.0
		*/
		float32 _vscroll = 0;

		/*! horizontal scroll factor 0.0 - 1.0
		*/
		float32 _hscroll = 0;

		/*! vertical scrollbar active aka visible
		*/
		bool _vscrollActive = false;

		/*! horizontal scrollbar active aka visible
		*/
		bool _hscrollActive = false;

		/*! rectangle of horizontal scroll button
		*/
		iRectanglei _hscrollButton;

		/*! rectangle of vertical scroll button
		*/
		iRectanglei _vscrollButton;

		/*! rectangle of up button
		*/
		iRectanglei _upButton;

		/*! rectangle of down button
		*/
		iRectanglei _downButton;

		/*! rectangle of left button
		*/
		iRectanglei _leftButton;

		/*! rectangle of right button
		*/
		iRectanglei _rightButton;

		/*! rectangle of child frame
		*/
		iRectanglei _childFrame;

		/*! handles incomming mouse wheel event

		\param d mouse wheel delta
		\returns true: if event was consumed and therefore ignored by the parent
		*/
		bool handleMouseWheel(int32 d);

		/*! handles incomming mouse move events

		\param pos mouse position
		*/
		void handleMouseMove(const iaVector2i& pos);

		/*! updates size based on it's content
		*/
		void calcMinSize();

		/*! draws the button
		*/
		void draw();

		/*! calculate position and size of scroll buttons
		*/
		void calcScrollButtons();

		/*! calc child frame

		it's similar to the client area but it can vary if scroll bars are visible
		*/
		void calcChildFrame();

		/*! updates widget alignment

		\param clientWidth maximum width this widget can align to
		\param clientHeight maximum height this widget can align to
		*/
		void updateAlignment(int32 clientWidth, int32 clientHeight);

		/*! calculates childrens ofsets relative to thair parent

		\param offsets vector to be filled with childrens offsets
		*/
		void calcChildOffsets(std::vector<iRectanglei>& offsets);

	};
}

#endif
