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
// (c) Copyright 2012-2022 by Martin Loga
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

#ifndef __IGOR_WIDGETTHEME_H__
#define __IGOR_WIDGETTHEME_H__

#include <igor/ui/widgets/iWidget.h>
#include <igor/resources/texture/iTexture.h>
#include <igor/resources/texture/iTextureFont.h>

#include <iaux/data/iaString.h>
#include <iaux/data/iaGradient.h>
using namespace iaux;

#include <memory>

namespace igor
{

    /*! base widget theme
	*/
    class IGOR_API iWidgetTheme
    {

    public:
        virtual void drawRectangle(const iaRectanglef &rect, const iaColor4f &color) = 0;
        virtual void drawFilledRectangle(const iaRectanglef &rect, const iaColor4f &color) = 0;

        virtual void drawRectangle(const iaRectanglef &rect) = 0;
        virtual void drawFilledRectangle(const iaRectanglef &rect) = 0;
        virtual void drawGradient(const iaRectanglef &rect, const iaGradientColor4f &gradient) = 0;

        virtual void drawTiledRectangle(const iaRectanglef &rect, iTexturePtr texture) = 0;

        virtual void drawGridCell(const iaRectanglef &rect, iWidgetState state) = 0;
        virtual void drawGridHighlight(const iaRectanglef &rect) = 0;
        virtual void drawGridSelection(const iaRectanglef &rect) = 0;
        virtual void drawBackgroundFrame(const iaRectanglef &rect, iWidgetState state, bool active) = 0;
        virtual void drawFrame(const iaRectanglef &rect, iWidgetState state, bool active) = 0;
        virtual void drawPicture(const iaRectanglef &rect, iTexturePtr texture, iWidgetState state, bool active) = 0;
        virtual void drawDialog(const iaRectanglef &rect, iWidgetState state, bool active) = 0;
        virtual void drawSpacer(const iaRectanglef &rect, iWidgetState state, bool active) = 0;
        virtual void drawButton(const iaRectanglef &rect, const iaString &text, iHorizontalAlignment align, iVerticalAlignment valign, iTexturePtr texture, iWidgetState state, bool active) = 0;
        virtual void drawButton(const iaRectanglef &rect, const iaColor4f &color, iWidgetState state, bool active) = 0;
        virtual void drawGroupBox(const iaRectanglef &rect, bool headerOnly, const iaString &text, iWidgetState state, bool active) = 0;
        virtual void drawCheckBox(const iaRectanglef &rect, const iaString &text, bool checked, iWidgetState state, bool active) = 0;
        virtual void drawLabel(const iaRectanglef &rect, const iaString &text, int32 textWidth, iWidgetState state, bool active) = 0;
        virtual void drawNumberChooser(const iaRectanglef &rect, const iaString &text, iWidgetState buttonUp, iWidgetState buttonDown, bool active) = 0;
        virtual void drawSelectBox(const iaRectanglef &rect, const iaString &text, iWidgetState buttonAppearance, bool active) = 0;
        virtual void drawSelectBoxDropDown(const iaRectanglef &rect, std::vector<iaString> &text, int highlightIndex, bool active) = 0;
        virtual void drawTextEdit(const iaRectanglef &rect, const iaString &text, const float32 cursorPos, iHorizontalAlignment align, iVerticalAlignment valign, bool keyboardFocus, iWidgetState state, bool active) = 0;
        virtual void drawText(const iaRectanglef &rect, const iaString &text, int32 textwidth) = 0;
        virtual void drawGraph(const iaRectanglef &rect, const iaColor4f &lineColor, const iaColor4f &pointColor, float32 lineWidth, float32 pointSize, const std::vector<iaVector2f> &points) = 0;
        virtual void drawGraphGridlines(const iaRectanglef &rect, float32 lineWidth, const std::vector<iaVector2f> &verticalLines, const std::vector<iaVector2f> &horizontalLines, bool active) = 0;
        virtual void drawGraphLabels(const iaRectanglef &rect, const std::vector<iaVector2f> &verticalLines, const std::vector<iaVector2f> &horizontalLines, bool active) = 0;
        virtual void drawGraphFrame(const iaRectanglef &rect, iWidgetState state, bool active) = 0;

        virtual void drawTooltip(const iaVector2f &pos, const iaString &text) = 0;

        virtual void setFontMetrics(const float32 fontSize, const float32 lineHeight) = 0;
        virtual float32 getFontSize() = 0;
        virtual iTextureFontPtr getFont() = 0;

        iWidgetTheme() = default;
        virtual ~iWidgetTheme() = default;
    };

} // namespace igor

#endif // __IGOR_WIDGETTHEME_H__
