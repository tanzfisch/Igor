// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/theme/iWidgetDefaultTheme.h>

#include <igor/resources/texture/iTextureFont.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/resources/texture/iTextureResourceFactory.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/graphics/iRenderer.h>

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
        _font = new iTextureFont(fontTexture);
        con_assert(_font != nullptr, "can't create font");

        _backgroundTexture = iTextureResourceFactory::getInstance().loadFile(backgroundTexture);

        _defaultMaterial = iMaterialResourceFactory::getInstance().createMaterial("igor.widget.default");
        iMaterialResourceFactory::getInstance().getMaterial(_defaultMaterial)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
        iMaterialResourceFactory::getInstance().getMaterial(_defaultMaterial)->setRenderState(iRenderState::Blend, iRenderStateValue::On);

        _texturedMaterial = iMaterialResourceFactory::getInstance().createMaterial("igor.widget.textured");
        iMaterialResourceFactory::getInstance().getMaterial(_texturedMaterial)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
        iMaterialResourceFactory::getInstance().getMaterial(_texturedMaterial)->setRenderState(iRenderState::Blend, iRenderStateValue::On);
        iMaterialResourceFactory::getInstance().getMaterial(_texturedMaterial)->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    }

    iWidgetDefaultTheme::~iWidgetDefaultTheme()
    {
        if (_font)
        {
            delete _font;
            _font = nullptr;
        }
    }

    void iWidgetDefaultTheme::drawTooltip(const iaVector2i &pos, const iaString &text)
    {
        float32 textWidth = std::min(300.0f, _font->measureWidth(text, _fontSize));
        float32 textHeight = _font->measureHeight(text, _fontSize, textWidth);

        iaRectanglei rect(pos._x, pos._y, textWidth + 10, textHeight + 10);
        drawFilledRectangle(rect, COLOR_WHITE);
        drawRectangle(rect, COLOR_BLACK);

        rect.adjust(5, 5, 0, 0);
        drawText(rect, text, textWidth);
    }

    void iWidgetDefaultTheme::drawRectangle(const iaRectanglei &rect)
    {
        drawRectangle(rect, COLOR_AMBIENT);
    }

    void iWidgetDefaultTheme::drawFilledRectangle(const iaRectanglei &rect)
    {
        drawFilledRectangle(rect, COLOR_DIFFUSE);
    }

    void iWidgetDefaultTheme::drawGradient(const iaRectanglei &rect, const iaGradientColor4f &gradient)
    {
        iRenderer::getInstance().setMaterial(_defaultMaterial);
        iRenderer::getInstance().setLineWidth(1);

        iaColor4f color;

        for (int i = 0; i < rect._width; ++i)
        {
            float32 indexPosX = static_cast<float32>(rect._x + i) + 0.5f;
            float32 halfHeight = static_cast<float32>(rect._height) * 0.5f;

            gradient.getValue(static_cast<float32>(i) / static_cast<float32>(rect._width), color);
            iRenderer::getInstance().setColor(color);
            iRenderer::getInstance().drawLine(indexPosX, static_cast<float32>(rect._y) + halfHeight + 0.5f, indexPosX, static_cast<float32>(rect._y + rect._height) + 0.5f);
            color._a = 1.0;
            iRenderer::getInstance().setColor(color);
            iRenderer::getInstance().drawLine(indexPosX, static_cast<float32>(rect._y) + 0.5f, indexPosX, static_cast<float32>(rect._y) + halfHeight + 0.5f);
        }
    }

    void iWidgetDefaultTheme::drawLineInt(int32 x1, int32 y1, int32 x2, int32 y2)
    {
        // sub pixel position correction
        iRenderer::getInstance().drawLine(static_cast<float32>(x1) + 0.5f, static_cast<float32>(y1) + 0.5f, static_cast<float32>(x2) + 0.5f, static_cast<float32>(y2) + 0.5f);
    }

    void iWidgetDefaultTheme::drawLineFloat(float32 x1, float32 y1, float32 x2, float32 y2)
    {
        iRenderer::getInstance().drawLine(x1 + 0.5f, y1 + 0.5f, x2 + 0.5f, y2 + 0.5f);
    }

    void iWidgetDefaultTheme::drawRectangleInt(const iaRectanglei &rect)
    {
        iRenderer::getInstance().drawFilledRectangle(static_cast<float32>(rect._x), static_cast<float32>(rect._y),
                                               static_cast<float32>(rect._width), static_cast<float32>(rect._height));
    }

    void iWidgetDefaultTheme::drawStringInt(int32 x, int32 y, iaString text, float32 angle, int32 maxWidth)
    {
        iRenderer::getInstance().drawString(static_cast<float32>(x), static_cast<float32>(y), text, angle, static_cast<float32>(maxWidth));
    }

    void iWidgetDefaultTheme::drawRectangle(const iaRectanglei &rect, const iaColor4f &color)
    {
        iRenderer::getInstance().setMaterial(_defaultMaterial);
        iRenderer::getInstance().setLineWidth(_defaultLineWidth);

        iRenderer::getInstance().setColor(color);
        drawLineInt(rect._x, rect._y, rect._x + rect._width, rect._y);
        drawLineInt(rect._x, rect._y, rect._x, rect._y + rect._height);
        drawLineInt(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height);
        drawLineInt(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height);
    }

    void iWidgetDefaultTheme::drawFilledRectangle(const iaRectanglei &rect, const iaColor4f &color)
    {
        iRenderer::getInstance().setMaterial(_defaultMaterial);

        iRenderer::getInstance().setColor(color);
        drawRectangleInt(rect);
    }

    void iWidgetDefaultTheme::drawGridHighlight(const iaRectanglei &rect)
    {
        iRenderer::getInstance().setMaterial(_defaultMaterial);
        iRenderer::getInstance().setColor(COLOR_DIFFUSE_LIGHT);
        drawRectangleInt(rect);
    }

    void iWidgetDefaultTheme::drawGridSelection(const iaRectanglei &rect)
    {
        iRenderer::getInstance().setMaterial(_defaultMaterial);
        iRenderer::getInstance().setColor(COLOR_SPECULAR);
        drawRectangleInt(rect);
    }

    void iWidgetDefaultTheme::drawGridCell(const iaRectanglei &rect, iWidgetState state)
    {
        DRAW_DEBUG_OUTPUT(rect, state);
    }

    void iWidgetDefaultTheme::drawGraphFrame(const iaRectanglei &rect, iWidgetState state, bool active)
    {
        iRenderer::getInstance().setMaterial(_defaultMaterial);

        iRenderer::getInstance().setColor(COLOR_DIFFUSE_TRANSPARENT);
        drawRectangleInt(rect);
        iRenderer::getInstance().setLineWidth(_defaultLineWidth);

        iRenderer::getInstance().setColor(COLOR_AMBIENT);
        drawLineInt(rect._x, rect._y, rect._x + rect._width, rect._y);
        drawLineInt(rect._x, rect._y, rect._x, rect._y + rect._height);

        iRenderer::getInstance().setColor(COLOR_SPECULAR);
        drawLineInt(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height);
        drawLineInt(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height);
    }

    void iWidgetDefaultTheme::drawBackgroundFrame(const iaRectanglei &rect, iWidgetState state, bool active)
    {
        iRenderer::getInstance().setMaterial(_defaultMaterial);

        iRenderer::getInstance().setColor(COLOR_DIFFUSE_TRANSPARENT);
        drawRectangleInt(rect);
        iRenderer::getInstance().setLineWidth(_defaultLineWidth);

        iRenderer::getInstance().setColor(COLOR_AMBIENT);
        drawLineInt(rect._x, rect._y, rect._x + rect._width, rect._y);
        drawLineInt(rect._x, rect._y, rect._x, rect._y + rect._height);
        drawLineInt(rect._x + 1, rect._y + 1, rect._x + rect._width - 1, rect._y + 1);
        drawLineInt(rect._x + 1, rect._y + 1, rect._x + 1, rect._y + rect._height - 1);

        iRenderer::getInstance().setColor(COLOR_SPECULAR);
        drawLineInt(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height);
        drawLineInt(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height);
        drawLineInt(rect._x + 1, rect._y + rect._height - 1, rect._x + rect._width - 1, rect._y + rect._height - 1);
        drawLineInt(rect._x + rect._width - 1, rect._y + 1, rect._x + rect._width - 1, rect._y + rect._height - 1);
    }

    void iWidgetDefaultTheme::drawButton(const iaRectanglei &rect, const iaColor4f &color, iWidgetState state, bool active)
    {
        iRenderer::getInstance().setMaterial(_defaultMaterial);
        drawButtonFrame(rect, color, state, active);

        DRAW_DEBUG_OUTPUT(rect, state);
    }

    void iWidgetDefaultTheme::drawButton(const iaRectanglei &rect, const iaString &text, iHorizontalAlignment align, iVerticalAlignment valign, iTexturePtr texture, iWidgetState state, bool active)
    {
        const int32 reduction = 2;
        int32 offset = 0;
        if (state == iWidgetState::Pressed)
        {
            offset = +1;
        }

        iRenderer::getInstance().setMaterial(_defaultMaterial);
        if (texture == nullptr)
        {
            drawButtonFrame(rect, state, active);
        }
        else
        {
            if (state == iWidgetState::Highlighted ||
                state == iWidgetState::Pressed)
            {
                iRenderer::getInstance().setColor(COLOR_DIFFUSE_LIGHT);
                drawRectangleInt(rect);
            }
        }

        if (texture != nullptr)
        {
            iRenderer::getInstance().setMaterial(_texturedMaterial);
            iaRectanglei picRect(rect._x + offset + reduction / 2, rect._y + offset + reduction / 2, rect._width - reduction, rect._height - reduction);
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
            textX += static_cast<int32>((static_cast<float32>(rect._width) - textwidth) * 0.5f);
            break;

        case iHorizontalAlignment::Right:
            textX += rect._width - 2 - static_cast<int32>(textwidth);
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

        iaRectanglei textRect(textX + offset, textY + offset, 0, 0);
        drawButtonText(textRect, text);

        DRAW_DEBUG_OUTPUT(rect, state);
    }

    void iWidgetDefaultTheme::drawTextEdit(const iaRectanglei &rect, const iaString &text, const float32 cursorPos, iHorizontalAlignment align, iVerticalAlignment valign, bool keyboardFocus, iWidgetState state, bool active)
    {
        iaString modText = text;

        float32 textwidth = _font->measureWidth(modText, _fontSize);

        iRenderer::getInstance().setMaterial(_defaultMaterial);

        if (active)
        {
            iRenderer::getInstance().setColor(COLOR_SPECULAR);
        }
        else
        {
            iRenderer::getInstance().setColor(COLOR_DIFFUSE);
        }

        iRenderer::getInstance().enableStencilTest(true);
        iRenderer::getInstance().setStencilFunction(iRenderStateValue::Always, 1, 0xff);
        iRenderer::getInstance().setStencilOperation(iRenderStateValue::Keep, iRenderStateValue::Keep, iRenderStateValue::Replace);

        // draw stencil pattern
        iRenderer::getInstance().setStencilMask(0xff);
        drawRectangleInt(rect);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().setColor(COLOR_SPECULAR);
        drawLineInt(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height);
        drawLineInt(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height);

        iRenderer::getInstance().setColor(COLOR_AMBIENT);
        drawLineInt(rect._x, rect._y, rect._x + rect._width, rect._y);
        drawLineInt(rect._x, rect._y, rect._x, rect._y + rect._height);

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

        iRenderer::getInstance().setStencilMask(0xff);
        iRenderer::getInstance().setStencilFunction(iRenderStateValue::Equal, 1, 0xff);

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

            iRenderer::getInstance().setColor(COLOR_TEXT_DARK);
            iRenderer::getInstance().drawFilledRectangle(rect._x + relativeTextPosX + cursorPos, rect._y + relatoveTextPosY, 2, _fontSize);
        }

        // render text
        iRenderer::getInstance().setMaterial(_texturedMaterial);
        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(_fontSize);
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);

        drawStringInt(rect._x + relativeTextPosX, rect._y + relatoveTextPosY, modText);

        iRenderer::getInstance().enableStencilTest(false);

        DRAW_DEBUG_OUTPUT(rect, state);
    }

    void iWidgetDefaultTheme::drawNumberChooser(const iaRectanglei &rect, const iaString &text, iWidgetState button_up_state, iWidgetState button_down_state, bool active)
    {
        drawNumberChooserFrame(rect, button_up_state, button_down_state, active);
        iaRectanglei textRect(rect._x + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f),
                             rect._y + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f), 0, 0);
        drawText(textRect, text, 0);

        DRAW_DEBUG_OUTPUT(rect, button_up_state);
    }

    void iWidgetDefaultTheme::drawNumberChooserFrame(const iaRectanglei &rect, iWidgetState state_button_up, iWidgetState state_button_down, bool active)
    {
        iRenderer::getInstance().setMaterial(_defaultMaterial);

        iRenderer::getInstance().setColor(COLOR_DIFFUSE_LIGHT);
        drawRectangleInt(rect);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().setColor(COLOR_SPECULAR);
        drawLineInt(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height);
        drawLineInt(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height);

        iRenderer::getInstance().setColor(COLOR_AMBIENT);
        drawLineInt(rect._x, rect._y, rect._x + rect._width, rect._y);
        drawLineInt(rect._x, rect._y, rect._x, rect._y + rect._height);

        drawButton(iaRectanglei(rect._x + rect._width - rect._height - 1, rect._y + 1, rect._height, rect._height / 2 - 1), "+", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, state_button_up, active);
        drawButton(iaRectanglei(rect._x + rect._width - rect._height - 1, rect._y + rect._height / 2, rect._height, rect._height / 2 - 1), "-", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, state_button_down, active);
    }

    void iWidgetDefaultTheme::drawSelectBox(const iaRectanglei &rect, const iaString &text, iWidgetState buttonAppearance, bool active)
    {
        drawSelectBoxFrame(rect, buttonAppearance, active);
        iaRectanglei textRect(rect._x + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f),
                             rect._y + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f) + 1, 0, 0);
        drawText(textRect, text, 0);

        DRAW_DEBUG_OUTPUT(rect, buttonAppearance);
    }

    void iWidgetDefaultTheme::drawSelectBoxFrame(const iaRectanglei &rect, iWidgetState buttonState, bool active)
    {
        iRenderer::getInstance().setMaterial(_defaultMaterial);

        iRenderer::getInstance().setColor(COLOR_DIFFUSE_LIGHT);
        drawRectangleInt(rect);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().setColor(COLOR_SPECULAR);
        drawLineInt(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height);
        drawLineInt(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height);

        iRenderer::getInstance().setColor(COLOR_AMBIENT);
        drawLineInt(rect._x, rect._y, rect._x + rect._width, rect._y);
        drawLineInt(rect._x, rect._y, rect._x, rect._y + rect._height);

        iaRectanglei buttonRect(rect._x + rect._width - rect._height, rect._y + 1, rect._height - 1, rect._height - 2);
        drawButton(buttonRect, "V", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, buttonState, active);
    }

    void iWidgetDefaultTheme::drawSelectBoxDropDown(const iaRectanglei &rect, std::vector<iaString> &text, int highlightIndex, bool active)
    {
        iRenderer::getInstance().setMaterial(_defaultMaterial);

        iRenderer::getInstance().setColor(COLOR_DIFFUSE_LIGHT);

        drawRectangleInt(iaRectanglei(rect._x, rect._y + rect._height - 1, rect._width - rect._height, rect._height * static_cast<int32>(text.size())));

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().setColor(COLOR_AMBIENT);
        drawLineInt(rect._x, rect._y + rect._height - 1, rect._x, rect._y + rect._height * (static_cast<int32>(text.size()) + 1));
        drawLineInt(rect._x + rect._width - rect._height, rect._y + rect._height - 1, rect._x + rect._width - rect._height, rect._y + rect._height * (static_cast<int32>(text.size()) + 1));
        drawLineInt(rect._x, rect._y + rect._height * (static_cast<int32>(text.size()) + 1), rect._x + rect._width - rect._height, rect._y + rect._height * (static_cast<int32>(text.size()) + 1));
    }

    float32 iWidgetDefaultTheme::getFontSize()
    {
        return _fontSize;
    }

    iTextureFont *iWidgetDefaultTheme::getFont()
    {
        return _font;
    }

    void iWidgetDefaultTheme::setFontMetrics(const float32 fontSize, const float32 lineHeight)
    {
        _fontSize = fontSize;
        _fontLineHeight = lineHeight;
    }

    void iWidgetDefaultTheme::drawCheckBoxFrame(const iaRectanglei &rect, iWidgetState state, bool active)
    {
        iRenderer::getInstance().setMaterial(_defaultMaterial);

        if (state != iWidgetState::Standby)
        {
            iRenderer::getInstance().setColor(COLOR_SPECULAR);
            drawRectangleInt(rect);
        }
    }

    void iWidgetDefaultTheme::drawCheckBox(const iaRectanglei &rect, iWidgetState state, bool active, bool checked)
    {
        iRenderer::getInstance().setMaterial(_defaultMaterial);

        if (active)
        {
            iRenderer::getInstance().setColor(COLOR_WHITE);
        }
        else
        {
            iRenderer::getInstance().setColor(COLOR_DIFFUSE);
        }

        drawRectangleInt(rect);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().setColor(COLOR_SPECULAR);
        drawLineInt(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height);
        drawLineInt(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height);

        iRenderer::getInstance().setColor(COLOR_AMBIENT);
        drawLineInt(rect._x, rect._y, rect._x + rect._width, rect._y);
        drawLineInt(rect._x, rect._y, rect._x, rect._y + rect._height);

        if (checked)
        {
            if (active)
            {
                iRenderer::getInstance().setColor(COLOR_BLACK);
            }
            else
            {
                iRenderer::getInstance().setColor(COLOR_DIFFUSE_DARK);
            }

            drawLineInt(rect._x + 2, rect._y + 4, rect._x + rect._width / 2, rect._y + rect._height - 3);
            drawLineInt(rect._x + 3, rect._y + 4, rect._x + rect._width / 2 + 1, rect._y + rect._height - 3);

            drawLineInt(rect._x + rect._width - 3, rect._y + 2, rect._x + rect._width / 2, rect._y + rect._height - 3);
            drawLineInt(rect._x + rect._width - 2, rect._y + 2, rect._x + rect._width / 2 + 1, rect._y + rect._height - 3);
        }
    }

    void iWidgetDefaultTheme::drawCheckBox(const iaRectanglei &rect, const iaString &text, bool checked, iWidgetState state, bool active)
    {
        drawCheckBoxFrame(rect, state, active);
        drawCheckBox(iaRectanglei(rect._x + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f),
                                 rect._y + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f), static_cast<int32>(_fontSize), static_cast<int32>(_fontSize)),
                     state, active, checked);
        drawText(iaRectanglei(rect._x + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f) + static_cast<int32>(_fontSize) * 2,
                             rect._y + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f), 0, 0),
                 text, 0);

        DRAW_DEBUG_OUTPUT(rect, state);
    }

    void iWidgetDefaultTheme::drawLabel(const iaRectanglei &rect, const iaString &text, int32 textwidth, iWidgetState state, bool active)
    {
        drawText(rect, text, textwidth);

        DRAW_DEBUG_OUTPUT(rect, state);
    }

    void iWidgetDefaultTheme::drawText(const iaRectanglei &rect, const iaString &text, int32 textwidth)
    {
        iRenderer::getInstance().setMaterial(_texturedMaterial);

        iRenderer::getInstance().setColor(COLOR_AMBIENT);
        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(_fontSize);
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);
        drawStringInt(rect._x, rect._y, text, 0.0f, textwidth);

        DRAW_DEBUG_OUTPUT(rect, iWidgetState::Pressed);
    }

    void iWidgetDefaultTheme::drawGraphGridlines(const iaRectanglei &rect, float32 lineWidth, const std::vector<iaVector2f> &verticalLines, const std::vector<iaVector2f> &horizontalLines, bool active)
    {
        if (lineWidth > 0.0)
        {
            iRenderer::getInstance().setMaterial(_defaultMaterial);

            if (active)
            {
                iRenderer::getInstance().setColor(COLOR_AMBIENT);
            }
            else
            {
                iRenderer::getInstance().setColor(COLOR_DIFFUSE_DARK);
            }

            iRenderer::getInstance().setLineWidth(lineWidth);

            for (auto verticalLine : verticalLines)
            {
                drawLineInt(rect._x + static_cast<int32>(verticalLine._x), rect._y, rect._x + static_cast<int32>(verticalLine._x), rect._y + rect._height);
            }

            for (auto horizontalLine : horizontalLines)
            {
                drawLineInt(rect._x, rect._y + static_cast<int32>(horizontalLine._x), rect._x + rect._width, rect._y + static_cast<int32>(horizontalLine._x));
            }

            iRenderer::getInstance().setLineWidth(1.0);
        }
    }

    void iWidgetDefaultTheme::drawGraphLabels(const iaRectanglei &rect, const std::vector<iaVector2f> &verticalLines, const std::vector<iaVector2f> &horizontalLines, bool active)
    {
        iRenderer::getInstance().setMaterial(_texturedMaterial);

        if (active)
        {
            iRenderer::getInstance().setColor(COLOR_AMBIENT);
        }
        else
        {
            iRenderer::getInstance().setColor(COLOR_DIFFUSE_DARK);
        }

        iRenderer::getInstance().setFont(_font);
        float32 fontSize = _fontSize * 0.75f;
        iRenderer::getInstance().setFontSize(fontSize);
        iaString value;

        for (int i = 0; i < verticalLines.size(); ++i)
        {
            value = iaString::toString(verticalLines[i]._y, 2);

            if (i < verticalLines.size() - 1)
            {
                iRenderer::getInstance().drawString(rect._x + verticalLines[i]._x + 2, rect._y + rect._height - 2 - fontSize, value, 90);
            }
            else
            {
                iRenderer::getInstance().drawString(rect._x + verticalLines[i]._x - fontSize - 2, rect._y + rect._height - 2 - fontSize, value, 90);
            }
        }

        bool first = true;
        for (auto horizontalLine : horizontalLines)
        {
            value = iaString::toString(horizontalLine._y, 2);

            if (first)
            {
                iRenderer::getInstance().drawString(rect._x + 2 + fontSize, rect._y + horizontalLine._x + 2, value);
                first = false;
            }
            else
            {
                iRenderer::getInstance().drawString(rect._x + 2 + fontSize, rect._y + horizontalLine._x - fontSize, value);
            }
        }
    }

    void iWidgetDefaultTheme::drawGraph(const iaRectanglei &rect, const iaColor4f &lineColor, const iaColor4f &pointColor, float32 lineWidth, float32 pointSize, const std::vector<iaVector2f> &points)
    {
        iaVector2f currentPoint;
        iaVector2f lastPoint;

        iRenderer::getInstance().setMaterial(_defaultMaterial);

        if (lineWidth > 0.0)
        {
            iRenderer::getInstance().setColor(lineColor);
            iRenderer::getInstance().setLineWidth(lineWidth);

            for (int i = 0; i < points.size(); ++i)
            {
                currentPoint._x = points[i]._x + rect._x;
                currentPoint._y = points[i]._y + rect._y;

                if (i > 0)
                {
                    drawLineFloat(lastPoint._x, lastPoint._y, currentPoint._x, currentPoint._y);
                }

                lastPoint = currentPoint;
            }

            iRenderer::getInstance().setLineWidth(1.0);
        }

        if (pointSize > 0.0)
        {
            iRenderer::getInstance().setColor(pointColor);
            iRenderer::getInstance().setPointSize(pointSize);

            for (int i = 0; i < points.size(); ++i)
            {
                currentPoint._x = points[i]._x + rect._x;
                currentPoint._y = points[i]._y + rect._y;

                iRenderer::getInstance().drawPoint(currentPoint._x, currentPoint._y);
            }
        }

        DRAW_DEBUG_OUTPUT(rect, iWidgetState::Pressed);
    }

    void iWidgetDefaultTheme::drawGroupBox(const iaRectanglei &rect, bool headerOnly, const iaString &text, iWidgetState state, bool active)
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

        iRenderer::getInstance().setMaterial(_defaultMaterial);
        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().setColor(COLOR_AMBIENT);

        if (text == "")
        {
            drawLineFloat(x, y, x + w, y);
        }
        else
        {
            drawLineFloat(x, y, x + _fontSize, y);
            drawLineFloat(x + _font->measureWidth(text, _fontSize) + _fontSize * 2.0f, y, x + w, y);
        }

        if (!headerOnly)
        {
            drawLineFloat(x, y, x, y + h);
            drawLineFloat(x, y + h, x + w, y + h);
            drawLineFloat(x + w, y, x + w, y + h);
        }

        drawText(iaRectanglei(static_cast<int32>(rect._x + _fontSize * 1.5f), static_cast<int32>(rect._y + _fontSize * 0.1f)), text, 0);

        DRAW_DEBUG_OUTPUT(rect, state);
    }

    void iWidgetDefaultTheme::drawTiledRectangle(const iaRectanglei &rect, iTexturePtr texture)
    {
        iRenderer::getInstance().setMaterial(_texturedMaterial);
        iRenderer::getInstance().setColor(COLOR_WHITE);
        iRenderer::getInstance().drawTextureTiled(static_cast<float32>(rect._x), static_cast<float32>(rect._y), static_cast<float32>(rect._width), static_cast<float32>(rect._height), texture);
    }

    void iWidgetDefaultTheme::drawPicture(const iaRectanglei &rect, iTexturePtr texture, iWidgetState state, bool active)
    {
        iRenderer::getInstance().setMaterial(_texturedMaterial);

        if (active)
        {
            iRenderer::getInstance().setColor(COLOR_WHITE);
        }
        else
        {
            iRenderer::getInstance().setColor(COLOR_AMBIENT);
        }

        iRenderer::getInstance().drawTexture(static_cast<float32>(rect._x), static_cast<float32>(rect._y), static_cast<float32>(rect._width), static_cast<float32>(rect._height), texture);

        DRAW_DEBUG_OUTPUT(rect, state);
    }

    void iWidgetDefaultTheme::drawButtonText(const iaRectanglei &rect, const iaString &text)
    {
        iRenderer::getInstance().setMaterial(_texturedMaterial);

        iRenderer::getInstance().setColor(COLOR_TEXT);
        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(_fontSize);
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);
        drawStringInt(rect._x, rect._y, text);
    }

    void iWidgetDefaultTheme::drawButtonFrame(const iaRectanglei &rect, iWidgetState state, bool active)
    {
        iRenderer::getInstance().setMaterial(_defaultMaterial);

        iaColor4f diffuse = COLOR_DIFFUSE;
        if (!active)
        {
            diffuse = COLOR_DIFFUSE_DARK;
        }

        switch (state)
        {
        case iWidgetState::Pressed:
            iRenderer::getInstance().setColor(COLOR_DIFFUSE_LIGHT);
            drawRectangleInt(rect);

            iRenderer::getInstance().setLineWidth(_defaultLineWidth);
            iRenderer::getInstance().setColor(COLOR_AMBIENT);
            drawLineInt(rect._x, rect._y, rect._width + rect._x, rect._y);
            drawLineInt(rect._x, rect._y, rect._x, rect._height + rect._y);

            iRenderer::getInstance().setColor(COLOR_SPECULAR);
            drawLineInt(rect._width + rect._x, rect._y, rect._width + rect._x, rect._height + rect._y);
            drawLineInt(rect._x, rect._height + rect._y, rect._width + rect._x, rect._height + rect._y);
            break;

        case iWidgetState::Highlighted:
        case iWidgetState::Clicked:
        case iWidgetState::DoubleClicked:
            diffuse = COLOR_DIFFUSE_LIGHT;

        case iWidgetState::Standby:
        default:
            iRenderer::getInstance().setColor(diffuse);
            drawRectangleInt(rect);

            iRenderer::getInstance().setLineWidth(_defaultLineWidth);
            iRenderer::getInstance().setColor(COLOR_SPECULAR);
            drawLineInt(rect._x, rect._y, rect._width + rect._x, rect._y);
            drawLineInt(rect._x, rect._y, rect._x, rect._height + rect._y);

            iRenderer::getInstance().setColor(COLOR_AMBIENT);
            drawLineInt(rect._x, rect._height + rect._y, rect._width + rect._x, rect._height + rect._y);
            drawLineInt(rect._width + rect._x, rect._y, rect._width + rect._x, rect._height + rect._y);
        };
    }

    void iWidgetDefaultTheme::drawButtonFrame(const iaRectanglei &rect, const iaColor4f &color, iWidgetState state, bool active)
    {
        iRenderer::getInstance().setMaterial(_defaultMaterial);

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
            iRenderer::getInstance().setColor(diffuse);
            drawRectangleInt(rect);

            iRenderer::getInstance().setLineWidth(_defaultLineWidth);
            iRenderer::getInstance().setColor(COLOR_AMBIENT);
            drawLineInt(rect._x, rect._y, rect._width + rect._x, rect._y);
            drawLineInt(rect._x, rect._y, rect._x, rect._height + rect._y);

            iRenderer::getInstance().setColor(COLOR_SPECULAR);
            drawLineInt(rect._width + rect._x, rect._y, rect._width + rect._x, rect._height + rect._y);
            drawLineInt(rect._x, rect._height + rect._y, rect._width + rect._x, rect._height + rect._y);
            break;

        case iWidgetState::Highlighted:
        case iWidgetState::Clicked:
        case iWidgetState::DoubleClicked:
        case iWidgetState::Standby:
        default:
            iRenderer::getInstance().setColor(diffuse);
            drawRectangleInt(rect);

            iRenderer::getInstance().setLineWidth(_defaultLineWidth);
            iRenderer::getInstance().setColor(COLOR_SPECULAR);
            drawLineInt(rect._x, rect._y, rect._width + rect._x, rect._y);
            drawLineInt(rect._x, rect._y, rect._x, rect._height + rect._y);

            iRenderer::getInstance().setColor(COLOR_AMBIENT);
            drawLineInt(rect._x, rect._height + rect._y, rect._width + rect._x, rect._height + rect._y);
            drawLineInt(rect._width + rect._x, rect._y, rect._width + rect._x, rect._height + rect._y);
        };
    }

    void iWidgetDefaultTheme::drawFrame(const iaRectanglei &rect, iWidgetState state, bool active)
    {
        iRenderer::getInstance().setMaterial(_defaultMaterial);

        if (active)
        {
            iRenderer::getInstance().setColor(COLOR_AMBIENT);
        }
        else
        {
            iRenderer::getInstance().setColor(COLOR_DIFFUSE_DARK);
        }

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        drawLineInt(rect._x, rect._y, rect._width + rect._x, rect._y);
        drawLineInt(rect._x, rect._y, rect._x, rect._height + rect._y);
        drawLineInt(rect._x, rect._height + rect._y, rect._width + rect._x, rect._height + rect._y);
        drawLineInt(rect._width + rect._x, rect._y, rect._width + rect._x, rect._height + rect._y);
    }

    void iWidgetDefaultTheme::drawSpacer(const iaRectanglei &rect, iWidgetState state, bool active)
    {
        iRenderer::getInstance().setMaterial(_defaultMaterial);

        if (active)
        {
            iRenderer::getInstance().setColor(COLOR_DIFFUSE_DARK);
        }
        else
        {
            iRenderer::getInstance().setColor(COLOR_DIFFUSE_LIGHT);
        }

        drawRectangleInt(rect);
    }

    void iWidgetDefaultTheme::drawDialog(const iaRectanglei &rect, iWidgetState state, bool active)
    {
        iRenderer::getInstance().setMaterial(_defaultMaterial);

        drawTiledRectangle(rect, _backgroundTexture);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().setColor(COLOR_AMBIENT);
        drawLineInt(rect._x, rect._y, rect._x + rect._width, rect._y);
        drawLineInt(rect._x, rect._y, rect._x, rect._y + rect._height);
        drawLineInt(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height);
        drawLineInt(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height);

        DRAW_DEBUG_OUTPUT(rect, state);
    }

    __IGOR_ENABLE_WARNING__(4100)
} // namespace igor