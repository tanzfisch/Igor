//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2026 by Martin A. Loga
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

#ifndef IGOR_WIDGET_DEFAULT_THEME_H
#define IGOR_WIDGET_DEFAULT_THEME_H

#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/resources/texture/iTexture.h>
#include <igor/ui/iDrag.h>

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
        /*! init theme

        \param fontTexture texture used for font
         */
        iWidgetDefaultTheme(iTexturePtr fontTexture);

        /*! does nothing
         */
        virtual ~iWidgetDefaultTheme() = default;

        /*! draw widget picture

        \param widget the widget to draw
        */
        void draw(iWidgetPicturePtr widget) override;

        /*! draw widget spacer

        \param widget the widget to draw
        */
        void draw(iWidgetSpacerPtr widget) override;

        /*! draw widget button

        \param widget the widget to draw
        */
        void draw(iWidgetButtonPtr widget) override;

        /*! draw widget label

        \param widget the widget to draw
        */
        void draw(iWidgetLabelPtr widget) override;

        /*! draw widget check box

        \param widget the widget to draw
        */
        void draw(iWidgetCheckBoxPtr widget) override;

        /*! draw widget text edit

        \param widget the widget to draw
        */
        void draw(iWidgetTextEditPtr widget) override;

        /*! draw dialog

        \param widget the widget to draw
        */
        void draw(iDialogPtr dialog) override;

        /*! draw color widget

        \param widget the widget to draw
        */
        void draw(iWidgetColorPtr widget) override;

        /*! draw widget group box

        \param widget the widget to draw
        */
        void draw(iWidgetGroupBoxPtr widget) override;

        /*! draw widget line text edit

        \param widget the widget to draw
        */
        void draw(iWidgetLineTextEditPtr widget) override;

        /*! draw widget number chooser

        \param widget the widget to draw
        */
        void draw(iWidgetNumberChooserPtr widget) override;

        /*! draw widget select box

        \param widget the widget to draw
        */
        void draw(iWidgetSelectBoxPtr widget) override;

        /*! draw widget slider

        \param widget the widget to draw
        */
        void draw(iWidgetSliderPtr widget) override;

        /*! draw selection box

        \param rect the selection rectangle
        */
        void drawSelection(const iaRectanglef &rect) override;

        /*! draw drag icon

        \param pos the position of the drag icon
        \param drag the drag information to draw
        */
        void drawDrag(const iaVector2f &pos, const iDrag &drag) override;

        // TODO

        /*! \returns dialog title height
         */
        float32 getDialogTitleHeight() const override;

        /*! \returns dialog frame width
         */
        float32 getDialogFrameWidth() const override;

        /*! \returns scaled font size
         */
        float32 getFontSize() const override;

        /*! \returns primary font used in this theme
         */
        iTextureFontPtr getFont() const override;

        /*! draw tooltip at given position

        \param pos the given position
        \param text the given tool tip
        */
        void drawTooltip(const iaVector2f &pos, const iaString &text) override;

        void drawGridCell(const iaRectanglef &rect, iWidgetState state) override;
        void drawGridHighlight(const iaRectanglef &rect) override;
        void drawGridSelection(const iaRectanglef &rect) override;

        void drawGraphFrame(const iaRectanglef &rect, iWidgetState state, bool enabled) override;
        void drawGraphGridlines(const iaRectanglef &rect, float32 lineWidth, const std::vector<iaVector2f> &verticalLines, const std::vector<iaVector2f> &horizontalLines, bool enabled) override;
        void drawGraphLabels(const iaRectanglef &rect, const std::vector<iaVector2f> &verticalLines, const std::vector<iaVector2f> &horizontalLines, bool enabled) override;
        void drawGraph(const iaRectanglef &rect, const iaColor4f &lineColor, const iaColor4f &pointColor, float32 lineWidth, float32 pointSize, const std::vector<iaVector2f> &points) override;

        void drawButton(const iaRectanglef &rect, const iaString &text, iHorizontalAlignment align, iVerticalAlignment valign, iTexturePtr texture, iTexturePtr icon, iWidgetState state, bool enabled, bool checked) override;
        void drawButton(const iaRectanglef &rect, const iaColor4f &color, iWidgetState state, bool enabled, bool checked) override;

        void drawGradient(const iaRectanglef &rect, const iaKeyFrameGraphColor4f &gradient) override;

        void drawBackgroundFrame(const iaRectanglef &rect, iWidgetState state, bool enabled) override;

    private:

        void drawPicture(const iaRectanglef &rect, iTexturePtr texture, iWidgetState state, bool enabled) override;

        void drawText(const iaRectanglef &rect, const iaString &text, int32 textwidth) override;

        void drawShadowRect(const iaRectanglef &rect) override;

    private:
        /*! background texture
         */
        iTexturePtr _checkerBoardTexture;

        /*! default line width definition
         */
        const float32 _defaultLineWidth = 1.0;

        /*! font size
         */
        float32 _fontSize = 14;

        /*! font line height
         */
        float32 _fontLineHeight = 1.1f;

        /*! frame width
         */
        float32 _frameWidth = 5;

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

        void drawButtonFrame(const iaRectanglef &rect, iWidgetState state, bool enabled, const std::vector<iaColor4f> &colors);
        void drawButtonText(const iaRectanglef &rect, const iaString &text);
    };
} // namespace igor

#endif // IGOR_WIDGET_DEFAULT_THEME_H