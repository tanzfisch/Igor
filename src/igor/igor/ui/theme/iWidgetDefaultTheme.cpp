// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/theme/iWidgetDefaultTheme.h>

#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/resources/texture/iTextureResourceFactory.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/renderer/iRenderer.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#define DEBUG_OUTPUT 0

#if DEBUG_OUTPUT == 1
iaColor4f MOUSEOVER(1, 1, 1, 0.5f);
iaColor4f MOUSEPRESSED(1, 0, 0, 1);
iaColor4f MOUSECLICKED(1, 1, 0, 1);
iaColor4f MOUSEDOUBLECLICKED(1, 1, 1, 1);
iaColor4f MOUSECONTEXT(0, 1, 0, 1);
iaColor4f STANDBY(0, 0, 1, 0.5f);

#define DRAW_DEBUG_OUTPUT(rect, state)           \
    switch (state)                               \
    {                                            \
    case iWidgetState::Highlighted:              \
        drawRectangle(rect, MOUSEOVER);          \
        break;                                   \
                                                 \
    case iWidgetState::Pressed:                  \
        drawRectangle(rect, MOUSEPRESSED);       \
        break;                                   \
                                                 \
    case iWidgetState::Clicked:                  \
        drawRectangle(rect, MOUSECLICKED);       \
        break;                                   \
                                                 \
    case iWidgetState::DoubleClicked:            \
        drawRectangle(rect, MOUSEDOUBLECLICKED); \
        break;                                   \
                                                 \
    case iWidgetState::Standby:                  \
        drawRectangle(rect, STANDBY);            \
        break;                                   \
    }

#else
#define DRAW_DEBUG_OUTPUT
#endif

static const iaColor4f COLOR_AMBIENT = {0.2f, 0.2f, 0.2f, 1.0f};
static const iaColor4f COLOR_DIFFUSE_DARK = {0.35f, 0.35f, 0.35f, 1.0f};
static const iaColor4f COLOR_DIFFUSE = {0.5f, 0.5f, 0.5f, 1.0f};
static const iaColor4f COLOR_DIFFUSE_TRANSPARENT = {0.5f, 0.5f, 0.5f, 0.75f};
static const iaColor4f COLOR_DIFFUSE_LIGHT = {0.6f, 0.6f, 0.6f, 1.0f};
static const iaColor4f COLOR_SPECULAR = {0.8f, 0.8f, 0.8f, 1.0f};
static const iaColor4f COLOR_WHITE = {1.0f, 1.0f, 1.0f, 1.0f};
static const iaColor4f COLOR_BLACK = {0.0f, 0.0f, 0.0f, 1.0f};
static const iaColor4f COLOR_TEXT = {0.2f, 0.2f, 0.2f, 1.0f};
static const iaColor4f COLOR_TEXT_DARK = {0.0f, 0.0f, 0.0f, 1.0f};

namespace igor
{
    __IGOR_DISABLE_WARNING__(4100)

    iWidgetDefaultTheme::iWidgetDefaultTheme(const iaString &fontTexture, const iaString &backgroundTexture)
    {
        _font = iTextureFont::create(fontTexture);
        _backgroundTexture = iTextureResourceFactory::getInstance().loadFile(backgroundTexture);
    }

    iWidgetDefaultTheme::~iWidgetDefaultTheme()
    {
        // release resources
        _font = nullptr;
    }

    void iWidgetDefaultTheme::drawTooltip(const iaVector2f &pos, const iaString &text)
    {
        float32 textWidth = std::min(300.0f, _font->measureWidth(text, _fontSize));
        float32 textHeight = _font->measureHeight(text, _fontSize, textWidth);

        iaRectanglef rect(pos._x, pos._y, textWidth + 10, textHeight + 10);
        drawFilledRectangle(rect, COLOR_WHITE);
        drawRectangle(rect, COLOR_BLACK);

        rect.adjust(5, 5, 0, 0);
        drawText(rect, text, textWidth);
    }

    void iWidgetDefaultTheme::drawRectangle(const iaRectanglef &rect)
    {
        drawRectangle(rect, COLOR_AMBIENT);
    }

    void iWidgetDefaultTheme::drawFilledRectangle(const iaRectanglef &rect)
    {
        drawFilledRectangle(rect, COLOR_DIFFUSE);
    }

    void iWidgetDefaultTheme::drawGradient(const iaRectanglef &rect, const iaGradientColor4f &gradient)
    {
        iRenderer::getInstance().setLineWidth(1);

        iaColor4f color;

        for (int i = 0; i < rect._width; ++i)
        {
            float32 indexPosX = static_cast<float32>(rect._x + i) + 0.5f;
            float32 halfHeight = static_cast<float32>(rect._height) * 0.5f;

            gradient.getValue(static_cast<float32>(i) / static_cast<float32>(rect._width), color);
            // TODO iRenderer::getInstance().setMaterial TODO(true);
            iRenderer::getInstance().drawLine(indexPosX, static_cast<float32>(rect._y) + halfHeight + 0.5f, indexPosX, static_cast<float32>(rect._y + rect._height) + 0.5f, color);
            // TODO iRenderer::getInstance().setMaterial TODO(false);
            color._a = 1.0;
            iRenderer::getInstance().drawLine(indexPosX, static_cast<float32>(rect._y) + 0.5f, indexPosX, static_cast<float32>(rect._y) + halfHeight + 0.5f, color);
        }
    }

    void iWidgetDefaultTheme::drawRectangle(const iaRectanglef &rect, const iaColor4f &color)
    {
        iRenderer::getInstance().setLineWidth(_defaultLineWidth);

        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x + rect._width, rect._y, color);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._y + rect._height, color);
        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height, color);
        iRenderer::getInstance().drawLine(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height, color);
    }

    void iWidgetDefaultTheme::drawFilledRectangle(const iaRectanglef &rect, const iaColor4f &color)
    {
        if (color._a != 1.0f)
        {
            // TODO iRenderer::getInstance().setMaterial TODO(true);
        }

        iRenderer::getInstance().drawFilledRectangle(rect, color);
        // TODO iRenderer::getInstance().setMaterial TODO(false);
    }

    void iWidgetDefaultTheme::drawGridHighlight(const iaRectanglef &rect)
    {
        iRenderer::getInstance().drawFilledRectangle(rect, COLOR_DIFFUSE_LIGHT);
    }

    void iWidgetDefaultTheme::drawGridSelection(const iaRectanglef &rect)
    {
        iRenderer::getInstance().drawFilledRectangle(rect, COLOR_SPECULAR);
    }

    void iWidgetDefaultTheme::drawGridCell(const iaRectanglef &rect, iWidgetState state)
    {
        DRAW_DEBUG_OUTPUT(rect, state);
    }

    void iWidgetDefaultTheme::drawGraphFrame(const iaRectanglef &rect, iWidgetState state, bool active)
    {
        iRenderer::getInstance().drawFilledRectangle(rect, COLOR_DIFFUSE_TRANSPARENT);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);

        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x + rect._width, rect._y, COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._y + rect._height, COLOR_AMBIENT);

        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);
        iRenderer::getInstance().drawLine(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);
    }

    void iWidgetDefaultTheme::drawBackgroundFrame(const iaRectanglef &rect, iWidgetState state, bool active)
    {
        iRenderer::getInstance().drawFilledRectangle(rect, COLOR_DIFFUSE_TRANSPARENT);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);

        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x + rect._width, rect._y, COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._y + rect._height, COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x + 1, rect._y + 1, rect._x + rect._width - 1, rect._y + 1, COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x + 1, rect._y + 1, rect._x + 1, rect._y + rect._height - 1, COLOR_AMBIENT);

        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);
        iRenderer::getInstance().drawLine(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);
        iRenderer::getInstance().drawLine(rect._x + 1, rect._y + rect._height - 1, rect._x + rect._width - 1, rect._y + rect._height - 1, COLOR_SPECULAR);
        iRenderer::getInstance().drawLine(rect._x + rect._width - 1, rect._y + 1, rect._x + rect._width - 1, rect._y + rect._height - 1, COLOR_SPECULAR);
    }

    void iWidgetDefaultTheme::drawButton(const iaRectanglef &rect, const iaColor4f &color, iWidgetState state, bool active)
    {
        drawButtonFrame(rect, color, state, active);

        DRAW_DEBUG_OUTPUT(rect, state);
    }

    void iWidgetDefaultTheme::drawButton(const iaRectanglef &rect, const iaString &text, iHorizontalAlignment align, iVerticalAlignment valign, iTexturePtr texture, iWidgetState state, bool active)
    {
        const int32 reduction = 2;
        int32 offset = 0;
        if (state == iWidgetState::Pressed)
        {
            offset = +1;
        }

        if (texture == nullptr)
        {
            drawButtonFrame(rect, state, active);
        }
        else
        {
            if (state == iWidgetState::Highlighted ||
                state == iWidgetState::Pressed)
            {
                iRenderer::getInstance().drawFilledRectangle(rect, COLOR_DIFFUSE_LIGHT);
            }
        }

        if (texture != nullptr)
        {
            iaRectanglef picRect(rect._x + offset + reduction / 2, rect._y + offset + reduction / 2, rect._width - reduction, rect._height - reduction);
            drawPicture(picRect, texture, state, active);
        }

        float32 textwidth = _font->measureWidth(text, _fontSize);
        int32 textX = rect._x;
        int32 textY = rect._y;
        switch (align)
        {
        case iHorizontalAlignment::Left:
            textX += 2;
            break;

        case iHorizontalAlignment::Center:
            textX += (rect._width - textwidth) * 0.5f;
            break;

        case iHorizontalAlignment::Right:
            textX += rect._width - 2 - textwidth;
            break;
        };

        switch (valign)
        {
        case iVerticalAlignment::Top:
            textY += 2;
            break;

        case iVerticalAlignment::Center:
            textY += static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f);
            break;

        case iVerticalAlignment::Bottom:
            textY += rect._height - 2 - static_cast<int32>(_fontSize);
            break;
        };

        iaRectanglef textRect(textX + offset, textY + offset, 0, 0);
        drawButtonText(textRect, text);

        DRAW_DEBUG_OUTPUT(rect, state);
    }

    void iWidgetDefaultTheme::drawTextEdit(const iaRectanglef &rect, const iaString &text, float32 maxwidth, iWidgetState state, bool active)
    {
        // force draw call before changing the stencil settings
        iRenderer::getInstance().flush();

        // draw stencil pattern
        iRenderer::getInstance().setStencilTestActive(true);
        iRenderer::getInstance().setStencilFunction(iStencilFunction::Always, 1, 0xff);
        iRenderer::getInstance().setStencilOperation(iStencilOperation::Keep, iStencilOperation::Keep, iStencilOperation::Replace);
        iRenderer::getInstance().setStencilMask(0xff);

        iRenderer::getInstance().drawFilledRectangle(rect, active ? COLOR_SPECULAR : COLOR_DIFFUSE);

        // force draw call before changing the stencil settings
        iRenderer::getInstance().flush();

        iRenderer::getInstance().setStencilMask(0xff);
        iRenderer::getInstance().setStencilFunction(iStencilFunction::Equal, 1, 0xff);

        // render text
        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(_fontSize);
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);

        iRenderer::getInstance().drawString(rect._x, rect._y, text, active ? COLOR_TEXT_DARK : COLOR_AMBIENT, maxwidth);

        // force draw call before changing the stencil settings
        iRenderer::getInstance().flush();

        iRenderer::getInstance().setStencilTestActive(false);
    }

    void iWidgetDefaultTheme::drawLineTextEdit(const iaRectanglef &rect, const iaString &text, const float32 cursorPos, iHorizontalAlignment align, iVerticalAlignment valign, bool keyboardFocus, iWidgetState state, bool active)
    {
        iaString modText = text;

        float32 textwidth = _font->measureWidth(modText, _fontSize);

        // force draw call before changing the stencil settings
        iRenderer::getInstance().flush();

        // draw stencil pattern
        iRenderer::getInstance().setStencilTestActive(true);
        iRenderer::getInstance().setStencilFunction(iStencilFunction::Always, 1, 0xff);
        iRenderer::getInstance().setStencilOperation(iStencilOperation::Keep, iStencilOperation::Keep, iStencilOperation::Replace);
        iRenderer::getInstance().setStencilMask(0xff);

        iRenderer::getInstance().drawFilledRectangle(rect, active ? COLOR_SPECULAR : COLOR_DIFFUSE);

        int32 relativeTextPosX = 0;
        int32 relatoveTextPosY = 0;

        switch (align)
        {
        case iHorizontalAlignment::Left:
            relativeTextPosX += 2;
            break;

        case iHorizontalAlignment::Right:
            relativeTextPosX += rect._width - 2 - static_cast<int32>(textwidth);
            break;

        case iHorizontalAlignment::Center:
            relativeTextPosX += static_cast<int32>((static_cast<float32>(rect._width) - textwidth) * 0.5f);
            break;
        };

        switch (valign)
        {
        case iVerticalAlignment::Top:
            relatoveTextPosY += rect._height - 2 - static_cast<int32>(_fontSize);
            break;

        case iVerticalAlignment::Bottom:
            relatoveTextPosY += 2;
            break;

        case iVerticalAlignment::Center:
            relatoveTextPosY += static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f);
            break;
        };

        // force draw call before changing the stencil settings
        iRenderer::getInstance().flush();

        iRenderer::getInstance().setStencilMask(0xff);
        iRenderer::getInstance().setStencilFunction(iStencilFunction::Equal, 1, 0xff);

        if (keyboardFocus)
        {
            int scrollOffset = 0;

            if (relativeTextPosX + cursorPos > rect._width)
            {
                scrollOffset = rect._width - (relativeTextPosX + cursorPos) - 5;
            }
            else if (relativeTextPosX + cursorPos < 0)
            {
                scrollOffset = 5 - relativeTextPosX;
            }

            relativeTextPosX += scrollOffset;

            iRenderer::getInstance().drawFilledRectangle(rect._x + relativeTextPosX + cursorPos, rect._y + relatoveTextPosY, 2.0f, _fontSize, COLOR_TEXT_DARK);
        }

        // render text
        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(_fontSize);
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);

        iRenderer::getInstance().drawString(rect._x + relativeTextPosX, rect._y + relatoveTextPosY, modText, keyboardFocus ? COLOR_TEXT_DARK : COLOR_AMBIENT);

        // force draw call before changing the stencil settings
        iRenderer::getInstance().flush();

        iRenderer::getInstance().setStencilTestActive(false);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);
        iRenderer::getInstance().drawLine(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);

        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x + rect._width, rect._y, COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._y + rect._height, COLOR_AMBIENT);        

        DRAW_DEBUG_OUTPUT(rect, state);
    }

    void iWidgetDefaultTheme::drawNumberChooser(const iaRectanglef &rect, const iaString &text, iWidgetState button_up_state, iWidgetState button_down_state, bool active)
    {
        drawNumberChooserFrame(rect, button_up_state, button_down_state, active);
        iaRectanglef textRect(rect._x + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f),
                              rect._y + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f), 0, 0);
        drawText(textRect, text, 0);

        DRAW_DEBUG_OUTPUT(rect, button_up_state);
    }

    void iWidgetDefaultTheme::drawNumberChooserFrame(const iaRectanglef &rect, iWidgetState state_button_up, iWidgetState state_button_down, bool active)
    {
        iRenderer::getInstance().drawFilledRectangle(rect, COLOR_DIFFUSE_LIGHT);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);
        iRenderer::getInstance().drawLine(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);

        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x + rect._width, rect._y, COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._y + rect._height, COLOR_AMBIENT);

        drawButton(iaRectanglef(rect._x + rect._width - rect._height - 1, rect._y + 1, rect._height, rect._height / 2 - 1), "+", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, state_button_up, active);
        drawButton(iaRectanglef(rect._x + rect._width - rect._height - 1, rect._y + rect._height / 2, rect._height, rect._height / 2 - 1), "-", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, state_button_down, active);
    }

    void iWidgetDefaultTheme::drawSelectBox(const iaRectanglef &rect, const iaString &text, iWidgetState buttonAppearance, bool active)
    {
        drawSelectBoxFrame(rect, buttonAppearance, active);
        iaRectanglef textRect(rect._x + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f),
                              rect._y + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f) + 1, 0, 0);
        drawText(textRect, text, 0);

        DRAW_DEBUG_OUTPUT(rect, buttonAppearance);
    }

    void iWidgetDefaultTheme::drawSelectBoxFrame(const iaRectanglef &rect, iWidgetState buttonState, bool active)
    {
        iRenderer::getInstance().drawFilledRectangle(rect, COLOR_DIFFUSE_LIGHT);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);
        iRenderer::getInstance().drawLine(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);

        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x + rect._width, rect._y, COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._y + rect._height, COLOR_AMBIENT);

        iaRectanglef buttonRect(rect._x + rect._width - rect._height, rect._y + 1, rect._height - 1, rect._height - 2);
        drawButton(buttonRect, "V", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, buttonState, active);
    }

    void iWidgetDefaultTheme::drawSelectBoxDropDown(const iaRectanglef &rect, std::vector<iaString> &text, int highlightIndex, bool active)
    {
        iRenderer::getInstance().drawFilledRectangle(iaRectanglef(rect._x, rect._y + rect._height - 1, rect._width - rect._height, rect._height * static_cast<int32>(text.size())), COLOR_DIFFUSE_LIGHT);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height - 1, rect._x, rect._y + rect._height * (static_cast<int32>(text.size()) + 1), COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x + rect._width - rect._height, rect._y + rect._height - 1, rect._x + rect._width - rect._height, rect._y + rect._height * (static_cast<int32>(text.size()) + 1), COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height * (static_cast<int32>(text.size()) + 1), rect._x + rect._width - rect._height, rect._y + rect._height * (static_cast<int32>(text.size()) + 1), COLOR_AMBIENT);
    }

    float32 iWidgetDefaultTheme::getFontSize()
    {
        return _fontSize;
    }

    iTextureFontPtr iWidgetDefaultTheme::getFont()
    {
        return _font;
    }

    void iWidgetDefaultTheme::setFontMetrics(const float32 fontSize, const float32 lineHeight)
    {
        _fontSize = fontSize;
        _fontLineHeight = lineHeight;
    }

    void iWidgetDefaultTheme::drawCheckBoxFrame(const iaRectanglef &rect, iWidgetState state, bool active)
    {
        if (state != iWidgetState::Standby)
        {
            iRenderer::getInstance().drawFilledRectangle(rect, COLOR_SPECULAR);
        }
    }

    void iWidgetDefaultTheme::drawCheckBox(const iaRectanglef &rect, iWidgetState state, bool active, bool checked)
    {
        iRenderer::getInstance().drawFilledRectangle(rect, active ? COLOR_WHITE : COLOR_DIFFUSE);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);
        iRenderer::getInstance().drawLine(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);

        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x + rect._width, rect._y, COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._y + rect._height, COLOR_AMBIENT);

        if (checked)
        {
            const iaColor4f &color = active ? COLOR_BLACK : COLOR_DIFFUSE_DARK;

            iRenderer::getInstance().drawLine(rect._x + 2, rect._y + 4, rect._x + rect._width / 2, rect._y + rect._height - 3, color);
            iRenderer::getInstance().drawLine(rect._x + 3, rect._y + 4, rect._x + rect._width / 2 + 1, rect._y + rect._height - 3, color);

            iRenderer::getInstance().drawLine(rect._x + rect._width - 3, rect._y + 2, rect._x + rect._width / 2, rect._y + rect._height - 3, color);
            iRenderer::getInstance().drawLine(rect._x + rect._width - 2, rect._y + 2, rect._x + rect._width / 2 + 1, rect._y + rect._height - 3, color);
        }
    }

    void iWidgetDefaultTheme::drawCheckBox(const iaRectanglef &rect, const iaString &text, bool checked, iWidgetState state, bool active)
    {
        drawCheckBoxFrame(rect, state, active);
        drawCheckBox(iaRectanglef(rect._x + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f),
                                  rect._y + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f), static_cast<int32>(_fontSize), static_cast<int32>(_fontSize)),
                     state, active, checked);
        drawText(iaRectanglef(rect._x + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f) + static_cast<int32>(_fontSize) * 2,
                              rect._y + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f), 0, 0),
                 text, 0);

        DRAW_DEBUG_OUTPUT(rect, state);
    }

    void iWidgetDefaultTheme::drawLabel(const iaRectanglef &rect, const iaString &text, int32 textwidth, iWidgetState state, bool active)
    {
        drawText(rect, text, textwidth);

        DRAW_DEBUG_OUTPUT(rect, state);
    }

    void iWidgetDefaultTheme::drawText(const iaRectanglef &rect, const iaString &text, int32 textwidth)
    {
        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(_fontSize);
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);
        iRenderer::getInstance().drawString(rect._x, rect._y, text, COLOR_AMBIENT, textwidth);

        DRAW_DEBUG_OUTPUT(rect, iWidgetState::Pressed);
    }

    void iWidgetDefaultTheme::drawGraphGridlines(const iaRectanglef &rect, float32 lineWidth, const std::vector<iaVector2f> &verticalLines, const std::vector<iaVector2f> &horizontalLines, bool active)
    {
        if (lineWidth > 0.0)
        {
            iRenderer::getInstance().setLineWidth(lineWidth);

            const iaColor4f &color = active ? COLOR_AMBIENT : COLOR_DIFFUSE_DARK;

            for (auto verticalLine : verticalLines)
            {
                iRenderer::getInstance().drawLine(rect._x + static_cast<int32>(verticalLine._x), rect._y, rect._x + static_cast<int32>(verticalLine._x), rect._y + rect._height, color);
            }

            for (auto horizontalLine : horizontalLines)
            {
                iRenderer::getInstance().drawLine(rect._x, rect._y + static_cast<int32>(horizontalLine._x), rect._x + rect._width, rect._y + static_cast<int32>(horizontalLine._x), color);
            }
        }
    }

    void iWidgetDefaultTheme::drawGraphLabels(const iaRectanglef &rect, const std::vector<iaVector2f> &verticalLines, const std::vector<iaVector2f> &horizontalLines, bool active)
    {

        const iaColor4f &color = active ? COLOR_AMBIENT : COLOR_DIFFUSE_DARK;
        const float32 fontSize = _fontSize * 0.75f;

        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(fontSize);
        iaString value;

        for (int i = 0; i < verticalLines.size(); ++i)
        {
            value = iaString::toString(verticalLines[i]._y, 2);

            if (i < verticalLines.size() - 1)
            {
                iRenderer::getInstance().drawString(rect._x + verticalLines[i]._x + 2, rect._y + rect._height - 2 - fontSize, value, color);
            }
            else
            {
                iRenderer::getInstance().drawString(rect._x + verticalLines[i]._x - fontSize - 2, rect._y + rect._height - 2 - fontSize, value, color);
            }
        }

        bool first = true;
        for (auto horizontalLine : horizontalLines)
        {
            value = iaString::toString(horizontalLine._y, 2);

            if (first)
            {
                iRenderer::getInstance().drawString(rect._x + 2 + fontSize, rect._y + horizontalLine._x + 2, value, color);
                first = false;
            }
            else
            {
                iRenderer::getInstance().drawString(rect._x + 2 + fontSize, rect._y + horizontalLine._x - fontSize, value, color);
            }
        }
    }

    void iWidgetDefaultTheme::drawGraph(const iaRectanglef &rect, const iaColor4f &lineColor, const iaColor4f &pointColor, float32 lineWidth, float32 pointSize, const std::vector<iaVector2f> &points)
    {
        iaVector2f currentPoint;
        iaVector2f lastPoint;

        if (lineWidth > 0.0)
        {
            iRenderer::getInstance().setLineWidth(lineWidth);

            for (int i = 0; i < points.size(); ++i)
            {
                currentPoint._x = points[i]._x + rect._x;
                currentPoint._y = points[i]._y + rect._y;

                if (i > 0)
                {
                    iRenderer::getInstance().drawLine(lastPoint, currentPoint, lineColor);
                }

                lastPoint = currentPoint;
            }
        }

        if (pointSize > 0.0)
        {
            iRenderer::getInstance().setPointSize(pointSize);

            for (int i = 0; i < points.size(); ++i)
            {
                currentPoint._x = points[i]._x + rect._x;
                currentPoint._y = points[i]._y + rect._y;

                iRenderer::getInstance().drawPoint(currentPoint, pointColor);
            }
        }

        DRAW_DEBUG_OUTPUT(rect, iWidgetState::Pressed);
    }

    void iWidgetDefaultTheme::drawGroupBox(const iaRectanglef &rect, bool headerOnly, const iaString &text, iWidgetState state, bool active)
    {
        float32 x = static_cast<float32>(rect._x);
        float32 y = static_cast<float32>(rect._y);
        float32 w = static_cast<float32>(rect._width);
        float32 h = static_cast<float32>(rect._height);

        if (text != "")
        {
            y += _fontSize * 0.5f;
            h -= _fontSize * 0.5f;
        }

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);

        if (text == "")
        {
            iRenderer::getInstance().drawLine(x, y, x + w, y, COLOR_AMBIENT);
        }
        else
        {
            iRenderer::getInstance().drawLine(x, y, x + _fontSize, y, COLOR_AMBIENT);
            iRenderer::getInstance().drawLine(x + _font->measureWidth(text, _fontSize) + _fontSize * 2.0f, y, x + w, y, COLOR_AMBIENT);
        }

        if (!headerOnly)
        {
            iRenderer::getInstance().drawLine(x, y, x, y + h, COLOR_AMBIENT);
            iRenderer::getInstance().drawLine(x, y + h, x + w, y + h, COLOR_AMBIENT);
            iRenderer::getInstance().drawLine(x + w, y, x + w, y + h, COLOR_AMBIENT);
        }

        drawText(iaRectanglef(static_cast<int32>(rect._x + _fontSize * 1.5f), static_cast<int32>(rect._y + _fontSize * 0.1f)), text, 0);

        DRAW_DEBUG_OUTPUT(rect, state);
    }

    void iWidgetDefaultTheme::drawTiledRectangle(const iaRectanglef &rect, iTexturePtr texture)
    {
        const iaVector2f tiling(rect._width / texture->getWidth(),
                                rect._height / texture->getHeight());
        iRenderer::getInstance().drawTexturedRectangle(rect._x, rect._y, rect._width, rect._height, texture, iaColor4f::white, false, tiling);
    }

    void iWidgetDefaultTheme::drawPicture(const iaRectanglef &rect, iTexturePtr texture, iWidgetState state, bool active)
    {
        const iaColor4f &color = active ? COLOR_WHITE : COLOR_AMBIENT;
        iRenderer::getInstance().drawTexturedRectangle(static_cast<float32>(rect._x), static_cast<float32>(rect._y), static_cast<float32>(rect._width), static_cast<float32>(rect._height), texture, color, texture->hasTransparency());

        DRAW_DEBUG_OUTPUT(rect, state);
    }

    void iWidgetDefaultTheme::drawButtonText(const iaRectanglef &rect, const iaString &text)
    {
        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(_fontSize);
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);
        iRenderer::getInstance().drawString(rect._x, rect._y, text, COLOR_TEXT);
    }

    void iWidgetDefaultTheme::drawButtonFrame(const iaRectanglef &rect, iWidgetState state, bool active)
    {
        const iaColor4f &diffuse = active ? COLOR_DIFFUSE : COLOR_DIFFUSE_DARK;

        switch (state)
        {
        case iWidgetState::Pressed:
            iRenderer::getInstance().drawFilledRectangle(rect, COLOR_DIFFUSE_LIGHT);

            iRenderer::getInstance().setLineWidth(_defaultLineWidth);
            iRenderer::getInstance().drawLine(rect._x, rect._y, rect._width + rect._x, rect._y, COLOR_AMBIENT);
            iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._height + rect._y, COLOR_AMBIENT);

            iRenderer::getInstance().drawLine(rect._width + rect._x, rect._y, rect._width + rect._x, rect._height + rect._y, COLOR_SPECULAR);
            iRenderer::getInstance().drawLine(rect._x, rect._height + rect._y, rect._width + rect._x, rect._height + rect._y, COLOR_SPECULAR);
            break;

        case iWidgetState::Highlighted:
        case iWidgetState::Clicked:
        case iWidgetState::DoubleClicked:
        case iWidgetState::Standby:
        default:
            iRenderer::getInstance().drawFilledRectangle(rect, diffuse);

            iRenderer::getInstance().setLineWidth(_defaultLineWidth);
            iRenderer::getInstance().drawLine(rect._x, rect._y, rect._width + rect._x, rect._y, COLOR_SPECULAR);
            iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._height + rect._y, COLOR_SPECULAR);

            iRenderer::getInstance().drawLine(rect._x, rect._height + rect._y, rect._width + rect._x, rect._height + rect._y, COLOR_AMBIENT);
            iRenderer::getInstance().drawLine(rect._width + rect._x, rect._y, rect._width + rect._x, rect._height + rect._y, COLOR_AMBIENT);
            break;
        };
    }

    void iWidgetDefaultTheme::drawButtonFrame(const iaRectanglef &rect, const iaColor4f &color, iWidgetState state, bool active)
    {
        iaColor4f diffuse = color;
        if (!active)
        {
            diffuse = color;
            diffuse._r *= 0.7f;
            diffuse._g *= 0.7f;
            diffuse._b *= 0.7f;
        }

        switch (state)
        {
        case iWidgetState::Pressed:
            iRenderer::getInstance().drawFilledRectangle(rect, diffuse);

            iRenderer::getInstance().setLineWidth(_defaultLineWidth);
            iRenderer::getInstance().drawLine(rect._x, rect._y, rect._width + rect._x, rect._y, COLOR_AMBIENT);
            iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._height + rect._y, COLOR_AMBIENT);

            iRenderer::getInstance().drawLine(rect._width + rect._x, rect._y, rect._width + rect._x, rect._height + rect._y, COLOR_SPECULAR);
            iRenderer::getInstance().drawLine(rect._x, rect._height + rect._y, rect._width + rect._x, rect._height + rect._y, COLOR_SPECULAR);
            break;

        case iWidgetState::Highlighted:
        case iWidgetState::Clicked:
        case iWidgetState::DoubleClicked:
        case iWidgetState::Standby:
        default:
            iRenderer::getInstance().drawFilledRectangle(rect, diffuse);

            iRenderer::getInstance().setLineWidth(_defaultLineWidth);
            iRenderer::getInstance().drawLine(rect._x, rect._y, rect._width + rect._x, rect._y, COLOR_SPECULAR);
            iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._height + rect._y, COLOR_SPECULAR);

            iRenderer::getInstance().drawLine(rect._x, rect._height + rect._y, rect._width + rect._x, rect._height + rect._y, COLOR_AMBIENT);
            iRenderer::getInstance().drawLine(rect._width + rect._x, rect._y, rect._width + rect._x, rect._height + rect._y, COLOR_AMBIENT);
        };
    }

    void iWidgetDefaultTheme::drawFrame(const iaRectanglef &rect, iWidgetState state, bool active)
    {
        const iaColor4f &color = active ? COLOR_AMBIENT : COLOR_DIFFUSE_DARK;

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._width + rect._x, rect._y, color);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._height + rect._y, color);
        iRenderer::getInstance().drawLine(rect._x, rect._height + rect._y, rect._width + rect._x, rect._height + rect._y, color);
        iRenderer::getInstance().drawLine(rect._width + rect._x, rect._y, rect._width + rect._x, rect._height + rect._y, color);
    }

    void iWidgetDefaultTheme::drawSpacer(const iaRectanglef &rect, iWidgetState state, bool active)
    {
        iRenderer::getInstance().drawFilledRectangle(rect, active ? COLOR_DIFFUSE_DARK : COLOR_DIFFUSE_LIGHT);
    }

    void iWidgetDefaultTheme::drawDialog(const iaRectanglef &rect, iWidgetState state, bool active)
    {
        iRenderer::getInstance().drawFilledRectangle(rect, COLOR_DIFFUSE);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x + rect._width, rect._y, COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._y + rect._height, COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height, COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height, COLOR_AMBIENT);

        DRAW_DEBUG_OUTPUT(rect, state);
    }

    __IGOR_ENABLE_WARNING__(4100)
} // namespace igor