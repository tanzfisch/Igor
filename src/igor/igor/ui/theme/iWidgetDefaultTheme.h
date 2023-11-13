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

#ifndef __IGOR_WIDGETDEFAULTTHEME__
#define __IGOR_WIDGETDEFAULTTHEME__

#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/resources/texture/iTexture.h>

#include <iaux/data/iaColor4.h>
using namespace iaux;

#include <memory>

namespace igor
{

    /*! default widget theme
     */
    class IGOR_API iWidgetDefaultTheme : public iWidgetTheme
    {

    public:
        void drawRectangle(const iaRectanglef &rect, const iaColor4f &color) override;
        void drawFilledRectangle(const iaRectanglef &rect, const iaColor4f &color) override;

        void drawRectangle(const iaRectanglef &rect) override;
        void drawFilledRectangle(const iaRectanglef &rect) override;
        void drawGradient(const iaRectanglef &rect, const iaKeyFrameGraphColor4f &gradient) override;

        void drawTiledRectangle(const iaRectanglef &rect, iTexturePtr texture) override;

        void drawGridCell(const iaRectanglef &rect, iWidgetState state) override;
        void drawGridHighlight(const iaRectanglef &rect) override;
        void drawGridSelection(const iaRectanglef &rect) override;
        void drawFrame(const iaRectanglef &rect, iWidgetState state, bool active) override;
        void drawBackgroundFrame(const iaRectanglef &rect, iWidgetState state, bool active) override;
        void drawPicture(const iaRectanglef &rect, iTexturePtr texture, iWidgetState state, bool active) override;
        void drawDialog(const iaRectanglef &rect, const iaRectanglef &clientRect, bool headerEnabled, const iaString &title, bool resizeEnabled, iWidgetState state, bool active) override;
        void drawSpacer(const iaRectanglef &rect, iWidgetState state, bool active) override;
        void drawButton(const iaRectanglef &rect, const iaString &text, iHorizontalAlignment align, iVerticalAlignment valign, iTexturePtr texture, iTexturePtr icon, iWidgetState state, bool active) override;
        void drawButton(const iaRectanglef &rect, const iaColor4f &color, iWidgetState state, bool active) override;
        void drawGroupBox(const iaRectanglef &rect, bool headerOnly, const iaString &text, iWidgetState state, bool active) override;
        void drawCheckBox(const iaRectanglef &rect, const iaString &text, bool checked, iWidgetState state, bool active) override;
        void drawLabel(const iaRectanglef &rect, const iaString &text, int32 textWidth, iWidgetState state, bool active) override;
        void drawNumberChooser(const iaRectanglef &rect, const iaString &text, iWidgetState button_up_state, iWidgetState button_down_state, bool active) override;
        void drawSelectBox(const iaRectanglef &rect, const iaString &text, iWidgetState buttonAppearance, bool active) override;
        void drawSelectBoxDropDown(const iaRectanglef &rect, std::vector<iaString> &text, int highlightIndex, bool active) override;
        void drawLineTextEdit(const iaRectanglef &rect, const iaString &text, const float32 cursorPos, iHorizontalAlignment align, iVerticalAlignment valign, bool keyboardFocus, iWidgetState state, bool active) override;
        void drawTextEdit(const iaRectanglef &rect, const iaString &text, float32 maxwidth, iWidgetState state, bool active) override;
        void drawText(const iaRectanglef &rect, const iaString &text, int32 textwidth) override;
        void drawGraph(const iaRectanglef &rect, const iaColor4f &lineColor, const iaColor4f &pointColor, float32 lineWidth, float32 pointSize, const std::vector<iaVector2f> &points) override;
        void drawGraphGridlines(const iaRectanglef &rect, float32 lineWidth, const std::vector<iaVector2f> &verticalLines, const std::vector<iaVector2f> &horizontalLines, bool active) override;
        void drawGraphLabels(const iaRectanglef &rect, const std::vector<iaVector2f> &verticalLines, const std::vector<iaVector2f> &horizontalLines, bool active) override;
        void drawGraphFrame(const iaRectanglef &rect, iWidgetState state, bool active) override;

        void drawTooltip(const iaVector2f &pos, const iaString &text) override;

        float32 getFontSize() const override;
        iTextureFontPtr getFont() const override;

        float32 getDialogTitleWidth() const override;
        float32 getDialogFrameWidth() const override;

        iWidgetDefaultTheme(iTexturePtr fontTexture, iTexturePtr backgroundTexture);
        ~iWidgetDefaultTheme() = default;

    private:
        /*! default line width definition
         */
        const float32 _defaultLineWidth = 1.0;

        /*! font size
         */
        float32 _fontSize = 14;

        /*! font line height
         */
        float32 _fontLineHeight = 1.1f;

        /*! title width
         */
        float32 _titleWidth = 28;

        /*! frame width
        */
        float32 _frameWidth = 10;

        /*! background texture
        */
        iTexturePtr _backgroundTexture = nullptr;

        /*! texture based font
         */
        iTextureFontPtr _font;

        /*! draws a rectangle

        integer version to wrap the float version of the renderer

        \param x horizontal position
        \parma y vertical position
        \param width width of rectangle
        \param height height of rectangle
        */
        void drawRectangleInt(const iaRectanglef &rect);

        void drawButtonFrame(const iaRectanglef &rect, iWidgetState state, bool active);
        void drawButtonFrame(const iaRectanglef &rect, const iaColor4f &color, iWidgetState state, bool active);
        void drawButtonText(const iaRectanglef &rect, const iaString &text);
        void drawCheckBoxFrame(const iaRectanglef &rect, iWidgetState state, bool active);
        void drawCheckBox(const iaRectanglef &rect, iWidgetState state, bool active, bool checked);
        void drawNumberChooserFrame(const iaRectanglef &rect, iWidgetState state_button_up, iWidgetState state_button_down, bool active);
        void drawSelectBoxFrame(const iaRectanglef &rect, iWidgetState buttonState, bool active);
    };
} // namespace igor

#endif
