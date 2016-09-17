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
// (c) Copyright 2014-2016 by Martin Loga
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

    class iWidgetUserControl;

    /*! scroll widget

    basically allows to have a child that is bigger an ads some srcoll bars then
    */
	class Igor_API iWidgetScroll : public iWidget
	{

        /*! needs to be friend because it's the factory that creates this widget
        */
		friend class iWidgetManager;

	public:

        /*! sets the width or thikness of the scrollbars
        */
		void setScollbarWidth(int32 width);

        /*! \returns scrollbar width
        */
		int32 getScollbarWidth();

        /*! sets horrizontal scroll value

        \param value new value for horrizontal scroll
        */
        void setHorrizontalScroll(float32 value);

        /*! sets vertical scroll value

        \param value new value for vertical scroll
        */
        void setVerticalScroll(float32 value);

	private:

        /*! translation matrix
        */
        iaMatrixf _translate;

        /*! scroll bar width
        */
		int32 _scrollbarWidth = 15;

        /*! vertical scroll factor 0.0 - 1.0
        */
		float32 _vscroll = 0;

        /*! horrizontal scroll factor 0.0 - 1.0
        */
        float32 _hscroll = 0;

        /*! vertical scrollbar active aka visible
        */
		bool _vscrollActive = false;

        /*! horrizontal scrollbar active aka visible
        */
        bool _hscrollActive = false;

        /*! rectangle of horrizontal scroll button
        */
		iRectanglei _hscrollButton;
		iRectanglei _vscrollButton;
		iRectanglei _upButton;
		iRectanglei _downButton;
		iRectanglei _leftButton;
		iRectanglei _rightButton;
		iRectanglei _childFrame;

        /*! handles incomming mouse wheel event

        \param d mouse wheel delta
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool handleMouseWheel(int32 d);

        /*! handles incomming mouse move events

        \param x horrizontal mouse position
        \param y horrizontal mouse position
        */
        void handleMouseMove(int32 x, int32 y);

        /*! updates size based on it's content
        */
        void calcMinSize();

		/*! draws the button

		\param parentPosX parent absolute horrizontal position
		\param parentPosY parent absolute vertical position
		*/
		void draw(int32 parentPosX, int32 parentPosY);

        /*! calculates the translation of the child widget
        */
        void calcTranslation();

        /*! 
        */
		void calcScrollButtons();
		void calcChildFrame();


		/*! ctor initializes membervariables
		*/
		iWidgetScroll();

		/*! does nothing
		*/
		~iWidgetScroll() = default;

	};
}

#endif
