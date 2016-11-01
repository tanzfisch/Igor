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

#ifndef __iWIDGETBASETHEME__
#define __iWIDGETBASETHEME__

#include <iWidget.h>

#include <iaString.h>
#include <iaColor4.h>
using namespace IgorAux;

namespace Igor
{

	class iTextureFont;
	class iTexture;

    /*! base widget theme

    \todo need to make a better concept for this interface
    \todo replace int32 posx, int32 posy, int32 width, int32 height with iRectanglei
    \todo maybe we should go from int32 to float32

    Example:
    \ref Widgets/src/WidgetsExample.cpp "Widgets usage example"

    */
	class Igor_API iWidgetBaseTheme
	{

	public:

        virtual void drawRectangle(int32 posx, int32 posy, int32 width, int32 height, const iaColor4f& color) = 0;
        virtual void drawFilledRectangle(int32 posx, int32 posy, int32 width, int32 height, const iaColor4f& color) = 0;

        virtual void drawRectangle(int32 posx, int32 posy, int32 width, int32 height) = 0;
        virtual void drawFilledRectangle(int32 posx, int32 posy, int32 width, int32 height) = 0;

        virtual void drawTiledRectangle(int32 posx, int32 posy, int32 width, int32 height, shared_ptr<iTexture> texture) = 0;
        
		virtual void drawGridField(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state) = 0;
        virtual void drawGridHighlight(int32 posx, int32 posy, int32 width, int32 height) = 0;
        virtual void drawGridSelection(int32 posx, int32 posy, int32 width, int32 height) = 0;
        virtual void drawBackgroundFrame(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state, bool active) = 0;
        virtual void drawFrame(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state, bool active) = 0;
        virtual void drawPicture(int32 posx, int32 posy, int32 width, int32 height, shared_ptr<iTexture> texture, iWidgetAppearanceState state, bool active) = 0;
		virtual void drawDialog(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state, bool active) = 0;
        virtual void drawSpacer(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state, bool active) = 0;
		virtual void drawButton(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, iHorizontalAlignment align, iVerticalAlignment valign, shared_ptr<iTexture> texture, iWidgetAppearanceState state, bool active) = 0;
		virtual void drawGroupBox(int32 posx, int32 posy, int32 width, int32 height, bool headerOnly, const iaString& text, iWidgetAppearanceState state, bool active) = 0;
		virtual void drawCheckBox(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, bool checked, iWidgetAppearanceState state, bool active) = 0;
		virtual void drawLabel(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, long textWidth, iWidgetAppearanceState state, bool active) = 0;
		virtual void drawNumberChooser(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, iWidgetAppearanceState buttonUp, iWidgetAppearanceState buttonDown, bool active) = 0;
        virtual void drawSelectBox(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, iWidgetAppearanceState buttonAppearance, bool active) = 0;
        virtual void drawSelectBoxDropDown(int32 posx, int32 posy, int32 width, int32 height, vector<iaString>& text, int highlightIndex, bool active) = 0;
		virtual void drawTextEdit(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, iHorizontalAlignment align, iVerticalAlignment valign, bool keyboardFocus, iWidgetAppearanceState state, bool active) = 0;
		virtual void drawText(int32 posx, int32 posy, const iaString& text, long textwidth) = 0;
        virtual void drawGraph(int32 posx, int32 posy, const iaColor4f& lineColor, const iaColor4f& pointColor, float32 lineWidth, float32 pointSize, const vector<iaVector2f>& points) = 0;
        virtual void drawGridlines(int32 posx, int32 posy, int32 width, int32 height, float32 lineWidth, const vector<float32>& verticalLines, const vector<float32>& horizontalLines, bool active) = 0;

		/*
				virtual void drawVSlider() = 0;
				virtual void drawHSlider() = 0;
		*/

        virtual void setFontMetrics(const float32 fontSize, const float32 lineHeight) = 0;
		virtual float32 getFontSize() = 0;
		virtual iTextureFont* getFont() = 0;

		iWidgetBaseTheme() = default;
		virtual ~iWidgetBaseTheme() = default;
	};

}

#endif


