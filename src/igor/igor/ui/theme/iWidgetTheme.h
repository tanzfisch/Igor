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

#ifndef IGOR_WIDGETTHEME_H
#define IGOR_WIDGETTHEME_H

#include <igor/ui/widgets/iWidgetPicture.h>
#include <igor/ui/widgets/iWidgetSpacer.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetCheckBox.h>
#include <igor/ui/widgets/iWidgetTextEdit.h>
#include <igor/ui/widgets/iWidgetColor.h>
#include <igor/ui/widgets/iWidgetGroupBox.h>
#include <igor/ui/widgets/iWidgetLineTextEdit.h>
#include <igor/ui/widgets/iWidgetNumberChooser.h>
#include <igor/ui/widgets/iWidgetSelectBox.h>
#include <igor/ui/widgets/iWidgetSlider.h>
#include <igor/ui/dialogs/iDialog.h>

#include <igor/resources/texture/iTextureFont.h>

#include <iaux/data/iaString.h>
#include <iaux/data/iaKeyFrameGraph.h>
using namespace iaux;

#include <memory>

namespace igor
{

    /*! base widget theme
     */
    class IGOR_API iWidgetTheme
    {

    public:
        /*! does nothing
         */
        iWidgetTheme() = default;

        /*! does nothing
         */
        virtual ~iWidgetTheme() = default;

        /*! draw widget picture

        \param widget the widget to draw
        */
        virtual void draw(iWidgetPicturePtr widget) = 0;

        /*! draw widget spacer

        \param widget the widget to draw
        */
        virtual void draw(iWidgetSpacerPtr widget) = 0;

        /*! draw widget button

        \param widget the widget to draw
        */
        virtual void draw(iWidgetButtonPtr widget) = 0;

        /*! draw widget label

        \param widget the widget to draw
        */
        virtual void draw(iWidgetLabelPtr widget) = 0;

        /*! draw widget check box

        \param widget the widget to draw
        */
        virtual void draw(iWidgetCheckBoxPtr widget) = 0;

        /*! draw widget text edit

        \param widget the widget to draw
        */
        virtual void draw(iWidgetTextEditPtr widget) = 0;

        /*! draw dialog

        \param widget the widget to draw
        */
        virtual void draw(iDialogPtr dialog) = 0;

        /*! draw color widget

        \param widget the widget to draw
        */
        virtual void draw(iWidgetColorPtr widget) = 0;

        /*! draw widget group box

        \param widget the widget to draw
        */
        virtual void draw(iWidgetGroupBoxPtr widget) = 0;

        /*! draw widget line text edit

        \param widget the widget to draw
        */
        virtual void draw(iWidgetLineTextEditPtr widget) = 0;

        /*! draw widget number chooser

        \param widget the widget to draw
        */
        virtual void draw(iWidgetNumberChooserPtr widget) = 0;

        /*! draw widget select box

        \param widget the widget to draw
        */
        virtual void draw(iWidgetSelectBoxPtr widget) = 0;

        /*! draw widget slider

        \param widget the widget to draw
        */
        virtual void draw(iWidgetSliderPtr widget) = 0;

        /*! draw selection box

        \param rect the selection rectangle
        */
        virtual void drawSelection(const iaRectanglef &rect) = 0;

        /*! draw drag icon

        \param pos the position of the drag icon
        \param drag the drag information to draw
        */
        virtual void drawDrag(const iaVector2f &pos, const iDrag &drag) = 0;

        // TODO

        /*! \returns dialog title height
         */
        virtual float32 getDialogTitleHeight() const = 0;

        /*! \returns dialog frame width
         */
        virtual float32 getDialogFrameWidth() const = 0;

        /*! \returns scaled font size
         */
        virtual float32 getFontSize() const = 0;

        /*! \returns primary font used in this theme
         */
        virtual iTextureFontPtr getFont() const = 0;

        /*! draw tooltip at given position

        \param pos the given position
        \param text the given tool tip
        */
        virtual void drawTooltip(const iaVector2f &pos, const iaString &text) = 0;

        virtual void drawGridCell(const iaRectanglef &rect, iWidgetState state) = 0;
        virtual void drawGridHighlight(const iaRectanglef &rect) = 0;
        virtual void drawGridSelection(const iaRectanglef &rect) = 0;

        virtual void drawGraphFrame(const iaRectanglef &rect, iWidgetState state, bool enabled) = 0;
        virtual void drawGraphGridlines(const iaRectanglef &rect, float32 lineWidth, const std::vector<iaVector2f> &verticalLines, const std::vector<iaVector2f> &horizontalLines, bool enabled) = 0;
        virtual void drawGraphLabels(const iaRectanglef &rect, const std::vector<iaVector2f> &verticalLines, const std::vector<iaVector2f> &horizontalLines, bool enabled) = 0;
        virtual void drawGraph(const iaRectanglef &rect, const iaColor4f &lineColor, const iaColor4f &pointColor, float32 lineWidth, float32 pointSize, const std::vector<iaVector2f> &points) = 0;

        virtual void drawButton(const iaRectanglef &rect, const iaString &text, iHorizontalAlignment align, iVerticalAlignment valign, iTexturePtr texture, iTexturePtr icon, iWidgetState state, bool enabled, bool checked) = 0;
        virtual void drawButton(const iaRectanglef &rect, const iaColor4f &color, iWidgetState state, bool enabled, bool checked) = 0;

        virtual void drawGradient(const iaRectanglef &rect, const iaKeyFrameGraphColor4f &gradient) = 0;

        virtual void drawBackgroundFrame(const iaRectanglef &rect, iWidgetState state, bool enabled) = 0;

    private: // TODO
        virtual void drawPicture(const iaRectanglef &rect, iTexturePtr texture, iWidgetState state, bool active) = 0;

        virtual void drawText(const iaRectanglef &rect, const iaString &text, int32 textwidth) = 0;

        virtual void drawShadowRect(const iaRectanglef &rect) = 0;
    };

    /*! widget theme pointer definition
     */
    typedef std::shared_ptr<iWidgetTheme> iWidgetThemePtr;

} // namespace igor

#endif // IGOR_WIDGETTHEME_H
