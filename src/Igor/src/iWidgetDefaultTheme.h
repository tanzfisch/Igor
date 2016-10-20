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

        void drawRectangle(int32 posx, int32 posy, int32 width, int32 height, const iaColor4f& color);
        void drawFilledRectangle(int32 posx, int32 posy, int32 width, int32 height, const iaColor4f& color);

        void drawRectangle(int32 posx, int32 posy, int32 width, int32 height);
        void drawFilledRectangle(int32 posx, int32 posy, int32 width, int32 height);

        void drawTiledRectangle(int32 posx, int32 posy, int32 width, int32 height, shared_ptr<iTexture> texture);
        
		void drawGridField(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state);
        void drawGridHighlight(int32 posx, int32 posy, int32 width, int32 height);
        void drawGridSelection(int32 posx, int32 posy, int32 width, int32 height);
        void drawBackgroundFrame(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state, bool active);
        void drawPicture(int32 posx, int32 posy, int32 width, int32 height, shared_ptr<iTexture> texture, iWidgetAppearanceState state, bool active);
        void drawDialog(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state, bool active);
        void drawSpacer(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state, bool active);
        void drawButton(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, iHorizontalAlignment align, iVerticalAlignment valign, shared_ptr<iTexture> texture, iWidgetAppearanceState state, bool active);
        void drawGroupBox(int32 posx, int32 posy, int32 width, int32 height, bool headerOnly, const iaString& text, iWidgetAppearanceState state, bool active);
        void drawCheckBox(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, bool checked, iWidgetAppearanceState state, bool active);
        void drawLabel(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, iWidgetAppearanceState state, bool active);
        void drawNumberChooser(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, iWidgetAppearanceState button_up_state, iWidgetAppearanceState button_down_state, bool active);
        void drawSelectBox(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, iWidgetAppearanceState buttonAppearance, bool active);
        void drawSelectBoxDropDown(int32 posx, int32 posy, int32 width, int32 height, vector<iaString>& text, int highlightIndex, bool active);
        void drawTextEdit(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, iHorizontalAlignment align, iVerticalAlignment valign, bool keyboardFocus, iWidgetAppearanceState state, bool active);
        void drawText(int32 posx, int32 posy, const iaString& text, long textwidth);

        void setFontMetrics(const float32 font_size, const float32 line_height);
        float32 getFontSize();
        iTextureFont* getFont();

        iWidgetDefaultTheme(const iaString& fontTexture, const iaString& backgroundTexture);
        virtual ~iWidgetDefaultTheme();

	private:

        shared_ptr<iTexture> _backgroundTexture = nullptr;

        /*! texture based font
        */
		iTextureFont* _font = nullptr;

        /*! font size
        */
		float32 _fontSize = 12;

        /*! font line height
        */
		float32 _fontLineHeight = 1.1f;

        iaColor4f _ambient;
		iaColor4f _darkDiffuse;
		iaColor4f _diffuse;
        iaColor4f _diffuseTransparent;
		iaColor4f _lightDiffuse;
		iaColor4f _specular;
		iaColor4f _white;
		iaColor4f _black;

		iaColor4f _textColor;
		iaColor4f _textColorDark;

		int32 _defaultMaterial = 0;
		int32 _texturedMaterial = 0;

		void drawButtonFrame(int32 x, int32 y, int32 width, int32 height, iWidgetAppearanceState state, bool active);
		void drawButtonText(int32 posx, int32 posy, const iaString& text);
		void drawCheckBoxFrame(int32 x, int32 y, int32 width, int32 height, iWidgetAppearanceState state, bool active);
		void drawCheckBox(int32 x, int32 y, int32 width, int32 height, iWidgetAppearanceState state, bool active, bool checked);
		void drawNumberChooserFrame(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state_button_up, iWidgetAppearanceState state_button_down, bool active);
        void drawSelectBoxFrame(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState buttonState, bool active);

	};
}

#endif

