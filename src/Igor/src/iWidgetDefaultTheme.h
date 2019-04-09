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

#ifndef __iWIDGETDEFAULTTHEME__
#define __iWIDGETDEFAULTTHEME__

#include <iWidgetBaseTheme.h>

#include <iaColor4.h>
using namespace IgorAux;

#include <memory>
using namespace std;

namespace Igor
{

    class iTexture;

    /*! default widget theme

    Example:
    \ref Widgets/src/WidgetsExample.cpp "Widgets usage example"
    */
	class Igor_API iWidgetDefaultTheme : public iWidgetBaseTheme
	{

    public:

        void drawRectangle(const iRectanglei& rect, const iaColor4f& color);
        void drawFilledRectangle(const iRectanglei& rect, const iaColor4f& color);

        void drawRectangle(const iRectanglei& rect);
        void drawFilledRectangle(const iRectanglei& rect);
        void drawGradient(const iRectanglei& rect, const iaGradientColor4f& gradient);

        void drawTiledRectangle(const iRectanglei& rect, shared_ptr<iTexture> texture);
        
		void drawGridField(const iRectanglei& rect, iWidgetAppearanceState state);
        void drawGridHighlight(const iRectanglei& rect);
        void drawGridSelection(const iRectanglei& rect);
        void drawFrame(const iRectanglei& rect, iWidgetAppearanceState state, bool active);
        void drawBackgroundFrame(const iRectanglei& rect, iWidgetAppearanceState state, bool active);
        void drawPicture(const iRectanglei& rect, shared_ptr<iTexture> texture, iWidgetAppearanceState state, bool active);
        void drawDialog(const iRectanglei& rect, iWidgetAppearanceState state, bool active);
        void drawSpacer(const iRectanglei& rect, iWidgetAppearanceState state, bool active);
        void drawButton(const iRectanglei& rect, const iaString& text, iHorizontalAlignment align, iVerticalAlignment valign, shared_ptr<iTexture> texture, iWidgetAppearanceState state, bool active);
        void drawButton(const iRectanglei& rect, const iaColor4f& color, iWidgetAppearanceState state, bool active);
        void drawGroupBox(const iRectanglei& rect, bool headerOnly, const iaString& text, iWidgetAppearanceState state, bool active);
        void drawCheckBox(const iRectanglei& rect, const iaString& text, bool checked, iWidgetAppearanceState state, bool active);
        void drawLabel(const iRectanglei& rect, const iaString& text, int32 textWidth, iWidgetAppearanceState state, bool active);
        void drawNumberChooser(const iRectanglei& rect, const iaString& text, iWidgetAppearanceState button_up_state, iWidgetAppearanceState button_down_state, bool active);
        void drawSelectBox(const iRectanglei& rect, const iaString& text, iWidgetAppearanceState buttonAppearance, bool active);
        void drawSelectBoxDropDown(const iRectanglei& rect, vector<iaString>& text, int highlightIndex, bool active);
        void drawTextEdit(const iRectanglei& rect, const iaString& text, const float32 cursorPos, const float32 scrollOffset, iHorizontalAlignment align, iVerticalAlignment valign, bool keyboardFocus, iWidgetAppearanceState state, bool active);
        void drawText(const iRectanglei& rect, const iaString& text, int32 textwidth);
        void drawGraph(const iRectanglei& rect, const iaColor4f& lineColor, const iaColor4f& pointColor, float32 lineWidth, float32 pointSize, const vector<iaVector2f>& points);
        void drawGraphGridlines(const iRectanglei& rect, float32 lineWidth, const vector<iaVector2f>& verticalLines, const vector<iaVector2f>& horizontalLines, bool active);
        void drawGraphLabels(const iRectanglei& rect, const vector<iaVector2f>& verticalLines, const vector<iaVector2f>& horizontalLines, bool active);
        void drawGraphFrame(const iRectanglei& rect, iWidgetAppearanceState state, bool active);


        void setFontMetrics(const float32 font_size, const float32 line_height);
        float32 getFontSize();
        iTextureFont* getFont();

        iWidgetDefaultTheme(const iaString& fontTexture, const iaString& backgroundTexture);
        virtual ~iWidgetDefaultTheme();

	private:

		/*! default line width definition
		*/
		const float32 _defaultLineWidth = 1.0;

		/*! font size
		*/
		float32 _fontSize = 12;

		/*! font line height
		*/
		float32 _fontLineHeight = 1.1f;

        shared_ptr<iTexture> _backgroundTexture = nullptr;

        /*! texture based font
        */
		iTextureFont* _font = nullptr;      

		/*! default material id
		*/
		uint64 _defaultMaterial = 0;

		/*! texture material id
		*/
		uint64 _texturedMaterial = 0;

        /*! drawing a line

		integer version to wrap the float version of the renderer

		\param x1 first horizontal position 
		\param y1 first vertical position
		\param x2 second horizontal position
		\param y2 second vertical position
		*/
		void drawLineInt(int32 x1, int32 y1, int32 x2, int32 y2);

		void drawLineFloat(float32 x1, float32 y1, float32 x2, float32 y2);

		/*! draws a rectangle

		integer version to wrap the float version of the renderer

		\param x horizontal position
		\parma y vertical position
		\param width width of rectangle
		\param height height of rectangle
		*/
		void drawRectangleInt(const iRectanglei& rect);

		/*! draws a text

		integer version to wrap the float version of the renderer
		*/
		void drawStringInt(int32 x, int32 y, iaString text, float32 angle = 0.0f, int32 maxWidth = 0);

        void drawButtonFrame(const iRectanglei& rect, iWidgetAppearanceState state, bool active);
		void drawButtonFrame(const iRectanglei& rect, const iaColor4f& color, iWidgetAppearanceState state, bool active);
		void drawButtonText(const iRectanglei& rect, const iaString& text);
		void drawCheckBoxFrame(const iRectanglei& rect, iWidgetAppearanceState state, bool active);
		void drawCheckBox(const iRectanglei& rect, iWidgetAppearanceState state, bool active, bool checked);
		void drawNumberChooserFrame(const iRectanglei& rect, iWidgetAppearanceState state_button_up, iWidgetAppearanceState state_button_down, bool active);
        void drawSelectBoxFrame(const iRectanglei& rect, iWidgetAppearanceState buttonState, bool active);

	};
}

#endif



