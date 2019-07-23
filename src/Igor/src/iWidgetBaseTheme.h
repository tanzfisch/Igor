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

#ifndef __iWIDGETBASETHEME__
#define __iWIDGETBASETHEME__

#include <iWidget.h>
#include <iTexture.h>

#include <iaString.h>
#include <iaGradient.h>
using namespace IgorAux;

#include <memory>

namespace Igor
{

	class iTextureFont;

	/*! base widget theme

	\todo need to make a better concept for this interface
	\todo replace const iRectanglei& rect with iRectanglei

	Example:
	\ref Widgets/src/WidgetsExample.cpp "Widgets usage example"

	*/
	class Igor_API iWidgetBaseTheme
	{

	public:

		virtual void drawRectangle(const iRectanglei& rect, const iaColor4f& color) = 0;
		virtual void drawFilledRectangle(const iRectanglei& rect, const iaColor4f& color) = 0;

		virtual void drawRectangle(const iRectanglei& rect) = 0;
		virtual void drawFilledRectangle(const iRectanglei& rect) = 0;
		virtual void drawGradient(const iRectanglei& rect, const iaGradientColor4f& gradient) = 0;

		virtual void drawTiledRectangle(const iRectanglei& rect, iTexturePtr texture) = 0;

		virtual void drawGridField(const iRectanglei& rect, iWidgetAppearanceState state) = 0;
		virtual void drawGridHighlight(const iRectanglei& rect) = 0;
		virtual void drawGridSelection(const iRectanglei& rect) = 0;
		virtual void drawBackgroundFrame(const iRectanglei& rect, iWidgetAppearanceState state, bool active) = 0;
		virtual void drawFrame(const iRectanglei& rect, iWidgetAppearanceState state, bool active) = 0;
		virtual void drawPicture(const iRectanglei& rect, iTexturePtr texture, iWidgetAppearanceState state, bool active) = 0;
		virtual void drawDialog(const iRectanglei& rect, iWidgetAppearanceState state, bool active) = 0;
		virtual void drawSpacer(const iRectanglei& rect, iWidgetAppearanceState state, bool active) = 0;
		virtual void drawButton(const iRectanglei& rect, const iaString& text, iHorizontalAlignment align, iVerticalAlignment valign, iTexturePtr texture, iWidgetAppearanceState state, bool active) = 0;
		virtual void drawButton(const iRectanglei& rect, const iaColor4f& color, iWidgetAppearanceState state, bool active) = 0;
		virtual void drawGroupBox(const iRectanglei& rect, bool headerOnly, const iaString& text, iWidgetAppearanceState state, bool active) = 0;
		virtual void drawCheckBox(const iRectanglei& rect, const iaString& text, bool checked, iWidgetAppearanceState state, bool active) = 0;
		virtual void drawLabel(const iRectanglei& rect, const iaString& text, int32 textWidth, iWidgetAppearanceState state, bool active) = 0;
		virtual void drawNumberChooser(const iRectanglei& rect, const iaString& text, iWidgetAppearanceState buttonUp, iWidgetAppearanceState buttonDown, bool active) = 0;
		virtual void drawSelectBox(const iRectanglei& rect, const iaString& text, iWidgetAppearanceState buttonAppearance, bool active) = 0;
		virtual void drawSelectBoxDropDown(const iRectanglei& rect, std::vector<iaString>& text, int highlightIndex, bool active) = 0;
		virtual void drawTextEdit(const iRectanglei& rect, const iaString& text, const float32 cursorPos, iHorizontalAlignment align, iVerticalAlignment valign, bool keyboardFocus, iWidgetAppearanceState state, bool active) = 0;
		virtual void drawText(const iRectanglei& rect, const iaString& text, int32 textwidth) = 0;
		virtual void drawGraph(const iRectanglei& rect, const iaColor4f& lineColor, const iaColor4f& pointColor, float32 lineWidth, float32 pointSize, const std::vector<iaVector2f>& points) = 0;
		virtual void drawGraphGridlines(const iRectanglei& rect, float32 lineWidth, const std::vector<iaVector2f>& verticalLines, const std::vector<iaVector2f>& horizontalLines, bool active) = 0;
		virtual void drawGraphLabels(const iRectanglei& rect, const std::vector<iaVector2f>& verticalLines, const std::vector<iaVector2f>& horizontalLines, bool active) = 0;
		virtual void drawGraphFrame(const iRectanglei& rect, iWidgetAppearanceState state, bool active) = 0;

		virtual void drawTooltip(const iaVector2i& pos, const iaString& text) = 0;

		virtual void setFontMetrics(const float32 fontSize, const float32 lineHeight) = 0;
		virtual float32 getFontSize() = 0;
		virtual iTextureFont* getFont() = 0;

		iWidgetBaseTheme() = default;
		virtual ~iWidgetBaseTheme() = default;
	};

}

#endif


