// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/theme/iWidgetDefaultTheme.h>
#include <igor/resources/iResourceManager.h>

#include <igor/resources/shader/iShader.h>
#include <igor/renderer/iRenderer.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#define DEBUG_OUTPUT 1

#if DEBUG_OUTPUT == 1
iaColor4f MOUSEOVER(1, 1, 1, 0.5f);
iaColor4f MOUSEPRESSED(1, 0, 0, 1);
iaColor4f MOUSECLICKED(1, 1, 0, 1);
iaColor4f MOUSEDOUBLECLICKED(1, 1, 1, 1);
iaColor4f MOUSECONTEXT(0, 1, 0, 1);
iaColor4f STANDBY(0, 0, 1, 0.5f);

#define DRAW_DEBUG_OUTPUT_OLD(rect, state)       \
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

#define DRAW_DEBUG_OUTPUT(rect, id, state)         \
    switch (state)                                 \
    {                                              \
    case iWidgetState::Highlighted:                \
        drawRectangle(rect, MOUSEOVER);            \
        drawText(rect, iaString::toString(id), 0); \
        break;                                     \
                                                   \
    case iWidgetState::Pressed:                    \
        drawRectangle(rect, MOUSEPRESSED);         \
        break;                                     \
                                                   \
    case iWidgetState::Clicked:                    \
        drawRectangle(rect, MOUSECLICKED);         \
        break;                                     \
                                                   \
    case iWidgetState::DoubleClicked:              \
        drawRectangle(rect, MOUSEDOUBLECLICKED);   \
        break;                                     \
                                                   \
    case iWidgetState::Standby:                    \
        drawRectangle(rect, STANDBY);              \
        break;                                     \
    }

#else
#define DRAW_DEBUG_OUTPUT_OLD
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

static const iaColor4f COLOR_CHECKED_BORDER = {0.3f, 0.3f, 0.3f, 0.7f};
static const iaColor4f COLOR_CHECKED_FILL = {0.2f, 0.2f, 0.2f, 0.2f};

static const iaColor4f COLOR_BUTTON_DEFAULT = {0.42f, 0.42f, 0.42f, 1.0f};

static const iaColor4f COLOR_SELECTION_FRAME(0.0, 0.0, 0.0, 0.8);
static const iaColor4f COLOR_SELECTION_FILL(0.0, 0.0, 0.0, 0.2);

namespace igor
{
    IGOR_DISABLE_WARNING(4100)

    iWidgetDefaultTheme::iWidgetDefaultTheme(iTexturePtr fontTexture, iTexturePtr backgroundTexture)
    {
        _font = iTextureFont::create(fontTexture);
        _backgroundTexture = backgroundTexture;

        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_TEXTURE},
                           {IGOR_RESOURCE_PARAM_CACHE_MODE, iResourceCacheMode::Cache},
                           {IGOR_RESOURCE_PARAM_GENERATE, true},
                           {"pattern", iTexturePattern::CheckerBoard},
                           {"primary", iaColor4f::gray},
                           {"secondary", iaColor4f::lightGray},
                           {"width", 128},
                           {"height", 128}});

        _checkerBoardTexture = iResourceManager::getInstance().loadResource<iTexture>(param);
    }

    void iWidgetDefaultTheme::drawWidgetPicture(iWidgetPicturePtr widget)
    {
        const auto &background = widget->getBackground();
        const auto &foreground = widget->getForeground();
        const auto rect = widget->getActualRect();
        const bool enabled = widget->isEnabled();
        const auto state = widget->getState();
        const auto texture = widget->getTexture();
        const auto checkerBoard = widget->isCheckerBoardEnabled();

        if (!checkerBoard && background._a != 0.0f)
        {
            iRenderer::getInstance().drawFilledRectangle(rect, background);
        }

        const iaColor4f &color = enabled ? foreground : COLOR_AMBIENT;
        if (texture != nullptr)
        {
            if (texture->hasTransparency() &&
                checkerBoard)
            {
                const float32 scale = static_cast<float32>(texture->getWidth()) / 128.0f;
                const float32 aspect = static_cast<float32>(texture->getHeight()) / static_cast<float32>(texture->getWidth());
                iRenderer::getInstance().drawTexturedRectangle(rect, _checkerBoardTexture, iaColor4f::white, false, iaVector2f(scale, aspect * scale));
            }

            iRenderer::getInstance().drawTexturedRectangle(rect, texture, color, texture->hasTransparency());
        }
        else
        {
            iRenderer::getInstance().drawFilledRectangle(rect, COLOR_DIFFUSE_LIGHT);
            iRenderer::getInstance().drawRectangle(rect, COLOR_DIFFUSE_DARK);
            iRenderer::getInstance().drawLine(rect.getTopLeft(), rect.getBottomRight(), COLOR_DIFFUSE_DARK);
            iRenderer::getInstance().drawLine(rect.getTopRight(), rect.getBottomLeft(), COLOR_DIFFUSE_DARK);
        }

        DRAW_DEBUG_OUTPUT(rect, widget->getID(), state);
    }

    void iWidgetDefaultTheme::drawWidgetSpacer(iWidgetSpacerPtr widget)
    {
        iRenderer::getInstance().drawFilledRectangle(widget->getActualRect(), widget->isEnabled() ? widget->getBackground() : COLOR_DIFFUSE_LIGHT);
    }

    void iWidgetDefaultTheme::drawSelection(const iaRectanglef &rect)
    {
        iRenderer::getInstance().drawFilledRectangle(rect, COLOR_SELECTION_FILL);
        iRenderer::getInstance().drawRectangle(rect, COLOR_SELECTION_FRAME);
    }

    void iWidgetDefaultTheme::drawDrag(const iaVector2f &pos, const iDrag &drag)
    {
        iTexturePtr texture = drag.getTexture();
        if (texture == nullptr)
        {
            return;
        }

        const float32 srcHeight = texture->getHeight();
        const float32 srcWidth = texture->getWidth();
        const float32 aspect = srcHeight / srcWidth;

        float32 newHeight = 64;
        float32 newWidth = 64;

        if (srcWidth > srcHeight)
        {
            newHeight = newWidth * aspect;
        }
        else
        {
            newWidth = newHeight * (1.0 / aspect);
        }

        const iaRectanglef rect(pos._x, pos._y, newWidth, newHeight);
        iRenderer::getInstance().drawTexturedRectangle(rect, texture, iaColor4f::white, true);

        iRenderer::getInstance().setLineWidth(3);

        switch (drag.getDragState())
        {
        case iDragState::Accepted:
            iRenderer::getInstance().drawLine(rect.getBottomRight() + iaVector2f(5, 5),
                                              rect.getBottomRight() + iaVector2f(15, 5), iaColor4f::white);
            iRenderer::getInstance().drawLine(rect.getBottomRight() + iaVector2f(10, 0),
                                              rect.getBottomRight() + iaVector2f(10, 10), iaColor4f::white);
            break;
        case iDragState::Rejected:
            iRenderer::getInstance().drawLine(rect.getBottomRight() + iaVector2f(6, 1),
                                              rect.getBottomRight() + iaVector2f(14, 9), iaColor4f::white);
            iRenderer::getInstance().drawLine(rect.getBottomRight() + iaVector2f(6, 9),
                                              rect.getBottomRight() + iaVector2f(14, 1), iaColor4f::white);
            break;
        case iDragState::Neutral:
        default:
            break;
        }

        iRenderer::getInstance().setLineWidth(1);
    }

    // TODO create new interfaces like the one above

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

    void iWidgetDefaultTheme::drawGradient(const iaRectanglef &rect, const iaKeyFrameGraphColor4f &gradient)
    {
        iRenderer::getInstance().setLineWidth(1);

        iaColor4f color;

        for (int i = 0; i < rect._width; ++i)
        {
            float32 indexPosX = static_cast<float32>(rect._x + i) + 0.5f;
            float32 halfHeight = static_cast<float32>(rect._height) * 0.5f;

            color = gradient.getValue(static_cast<float32>(i) / static_cast<float32>(rect._width));
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
        DRAW_DEBUG_OUTPUT_OLD(rect, state);
    }

    void iWidgetDefaultTheme::drawGraphFrame(const iaRectanglef &rect, iWidgetState state, bool enabled)
    {
        iRenderer::getInstance().drawFilledRectangle(rect, COLOR_DIFFUSE_TRANSPARENT);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);

        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x + rect._width, rect._y, COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._y + rect._height, COLOR_AMBIENT);

        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);
        iRenderer::getInstance().drawLine(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);
    }

    void iWidgetDefaultTheme::drawBackgroundFrame(const iaRectanglef &rect, iWidgetState state, bool enabled)
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

    static void drawChecked(const iaRectanglef &rect)
    {
        iRenderer::getInstance().drawFilledRectangle(rect, COLOR_CHECKED_FILL);
        iRenderer::getInstance().drawRectangle(rect, COLOR_CHECKED_BORDER);
    }

    void iWidgetDefaultTheme::drawButton(const iaRectanglef &rect, const iaColor4f &color, iWidgetState state, bool enabled, bool checked)
    {
        drawButtonFrame(rect, color, state, enabled);

        if (checked)
        {
            drawChecked(rect);
        }

        DRAW_DEBUG_OUTPUT_OLD(rect, state);
    }

    void iWidgetDefaultTheme::drawButton(const iaRectanglef &rect, const iaString &text, iHorizontalAlignment align, iVerticalAlignment valign, iTexturePtr texture, iTexturePtr icon, iWidgetState state, bool enabled, bool checked)
    {
        int32 offset = 0;
        if (state == iWidgetState::Pressed)
        {
            offset = +1;
        }

        if (texture == nullptr)
        {
            drawButtonFrame(rect, state, enabled);
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
            iaRectanglef picRect(rect._x + offset, rect._y + offset, rect._width, rect._height);
            drawPicture(picRect, texture, state, enabled);
        }

        if (!text.isEmpty())
        {
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
        }

        if (icon != nullptr)
        {
            float32 maxIconSize = std::min(rect._width, rect._height);

            switch (align)
            {
            case iHorizontalAlignment::Left:
            {
                iaRectanglef picRect(rect.getRight() - offset - rect._height, rect._y + offset, rect._height, rect._height);
                drawPicture(picRect, icon, state, enabled);
            }
            break;

            case iHorizontalAlignment::Center:
                if (text.isEmpty())
                {
                    iaRectanglef picRect(rect._x + offset + rect._width * 0.5 - maxIconSize * 0.5, rect._y + offset, rect._height, rect._height);
                    drawPicture(picRect, icon, state, enabled);
                }
                break;

            case iHorizontalAlignment::Right:
            {
                iaRectanglef picRect(rect._x + offset, rect._y + offset, rect._height, rect._height);
                drawPicture(picRect, icon, state, enabled);
            }
            break;
            };
        }

        if (checked)
        {
            drawChecked(rect);
        }

        DRAW_DEBUG_OUTPUT_OLD(rect, state);
    }

    void iWidgetDefaultTheme::drawTextEdit(const iaRectanglef &rect, const iaString &text, float32 maxwidth, iWidgetState state, bool enabled)
    {
        // force draw call before changing the stencil settings
        iRenderer::getInstance().flush();

        // draw stencil pattern
        iRenderer::getInstance().setStencilTestActive(true);
        iRenderer::getInstance().setStencilFunction(iStencilFunction::Always, 1, 0xff);
        iRenderer::getInstance().setStencilOperation(iStencilOperation::Keep, iStencilOperation::Keep, iStencilOperation::Replace);
        iRenderer::getInstance().setStencilMask(0xff);

        iRenderer::getInstance().drawFilledRectangle(rect, enabled ? COLOR_SPECULAR : COLOR_DIFFUSE);

        // force draw call before changing the stencil settings
        iRenderer::getInstance().flush();

        iRenderer::getInstance().setStencilMask(0xff);
        iRenderer::getInstance().setStencilFunction(iStencilFunction::Equal, 1, 0xff);

        // render text
        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(_fontSize);
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);

        iRenderer::getInstance().drawString(rect._x, rect._y, text, enabled ? COLOR_TEXT_DARK : COLOR_AMBIENT, maxwidth);

        // force draw call before changing the stencil settings
        iRenderer::getInstance().flush();

        iRenderer::getInstance().setStencilTestActive(false);
    }

    void iWidgetDefaultTheme::drawLineTextEdit(const iaRectanglef &rect, const iaString &text, const float32 cursorPos, iHorizontalAlignment align, iVerticalAlignment valign, bool keyboardFocus, iWidgetState state, bool enabled)
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

        iRenderer::getInstance().drawFilledRectangle(rect, enabled ? COLOR_SPECULAR : COLOR_DIFFUSE);

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

        DRAW_DEBUG_OUTPUT_OLD(rect, state);
    }

    void iWidgetDefaultTheme::drawNumberChooser(const iaRectanglef &rect, const iaString &text, iWidgetState button_up_state, iWidgetState button_down_state, bool enabled)
    {
        drawNumberChooserFrame(rect, button_up_state, button_down_state, enabled);
        iaRectanglef textRect(rect._x + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f),
                              rect._y + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f), 0, 0);
        drawText(textRect, text, 0);

        DRAW_DEBUG_OUTPUT_OLD(rect, button_up_state);
    }

    void iWidgetDefaultTheme::drawNumberChooserFrame(const iaRectanglef &rect, iWidgetState state_button_up, iWidgetState state_button_down, bool enabled)
    {
        iRenderer::getInstance().drawFilledRectangle(rect, COLOR_DIFFUSE_LIGHT);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);
        iRenderer::getInstance().drawLine(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);

        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x + rect._width, rect._y, COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._y + rect._height, COLOR_AMBIENT);

        drawButton(iaRectanglef(rect._x + rect._width - rect._height - 1, rect._y + 1, rect._height, rect._height / 2 - 1), "+", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, nullptr, state_button_up, enabled, false);
        drawButton(iaRectanglef(rect._x + rect._width - rect._height - 1, rect._y + rect._height / 2, rect._height, rect._height / 2 - 1), "-", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, nullptr, state_button_down, enabled, false);
    }

    void iWidgetDefaultTheme::drawSelectBox(const iaRectanglef &rect, const iaString &text, iWidgetState buttonAppearance, bool enabled)
    {
        drawSelectBoxFrame(rect, buttonAppearance, enabled);
        iaRectanglef textRect(rect._x + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f),
                              rect._y + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f) + 1, 0, 0);
        drawText(textRect, text, 0);

        DRAW_DEBUG_OUTPUT_OLD(rect, buttonAppearance);
    }

    void iWidgetDefaultTheme::drawSelectBoxFrame(const iaRectanglef &rect, iWidgetState buttonState, bool enabled)
    {
        iRenderer::getInstance().drawFilledRectangle(rect, COLOR_DIFFUSE_LIGHT);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);
        iRenderer::getInstance().drawLine(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);

        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x + rect._width, rect._y, COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._y + rect._height, COLOR_AMBIENT);

        iaRectanglef buttonRect(rect._x + rect._width - rect._height, rect._y + 1, rect._height - 1, rect._height - 2);
        drawButton(buttonRect, "V", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, nullptr, buttonState, enabled, false);
    }

    void iWidgetDefaultTheme::drawSelectBoxDropDown(const iaRectanglef &rect, std::vector<iaString> &text, int highlightIndex, bool enabled)
    {
        iRenderer::getInstance().drawFilledRectangle(iaRectanglef(rect._x, rect._y + rect._height - 1, rect._width - rect._height, rect._height * static_cast<int32>(text.size())), COLOR_DIFFUSE_LIGHT);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height - 1, rect._x, rect._y + rect._height * (static_cast<int32>(text.size()) + 1), COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x + rect._width - rect._height, rect._y + rect._height - 1, rect._x + rect._width - rect._height, rect._y + rect._height * (static_cast<int32>(text.size()) + 1), COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height * (static_cast<int32>(text.size()) + 1), rect._x + rect._width - rect._height, rect._y + rect._height * (static_cast<int32>(text.size()) + 1), COLOR_AMBIENT);
    }

    float32 iWidgetDefaultTheme::getDialogTitleWidth() const
    {
        return _titleWidth;
    }

    float32 iWidgetDefaultTheme::getDialogFrameWidth() const
    {
        return _frameWidth;
    }

    float32 iWidgetDefaultTheme::getFontSize() const
    {
        return _fontSize;
    }

    iTextureFontPtr iWidgetDefaultTheme::getFont() const
    {
        return _font;
    }

    void iWidgetDefaultTheme::drawCheckBoxFrame(const iaRectanglef &rect, iWidgetState state, bool enabled)
    {
        if (state != iWidgetState::Standby)
        {
            iRenderer::getInstance().drawFilledRectangle(rect, COLOR_SPECULAR);
        }
    }

    void iWidgetDefaultTheme::drawCheckBox(const iaRectanglef &rect, iWidgetState state, bool enabled, bool checked)
    {
        iRenderer::getInstance().drawFilledRectangle(rect, enabled ? COLOR_WHITE : COLOR_DIFFUSE);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);
        iRenderer::getInstance().drawLine(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);

        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x + rect._width, rect._y, COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._y + rect._height, COLOR_AMBIENT);

        if (checked)
        {
            const iaColor4f &color = enabled ? COLOR_BLACK : COLOR_DIFFUSE_DARK;

            iRenderer::getInstance().drawLine(rect._x + 2, rect._y + 4, rect._x + rect._width / 2, rect._y + rect._height - 3, color);
            iRenderer::getInstance().drawLine(rect._x + 3, rect._y + 4, rect._x + rect._width / 2 + 1, rect._y + rect._height - 3, color);

            iRenderer::getInstance().drawLine(rect._x + rect._width - 3, rect._y + 2, rect._x + rect._width / 2, rect._y + rect._height - 3, color);
            iRenderer::getInstance().drawLine(rect._x + rect._width - 2, rect._y + 2, rect._x + rect._width / 2 + 1, rect._y + rect._height - 3, color);
        }
    }

    void iWidgetDefaultTheme::drawCheckBox(const iaRectanglef &rect, const iaString &text, bool checked, iWidgetState state, bool enabled)
    {
        drawCheckBoxFrame(rect, state, enabled);
        drawCheckBox(iaRectanglef(rect._x + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f),
                                  rect._y + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f), static_cast<int32>(_fontSize), static_cast<int32>(_fontSize)),
                     state, enabled, checked);
        drawText(iaRectanglef(rect._x + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f) + static_cast<int32>(_fontSize) * 2,
                              rect._y + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f), 0, 0),
                 text, 0);

        DRAW_DEBUG_OUTPUT_OLD(rect, state);
    }

    void iWidgetDefaultTheme::drawLabel(const iaRectanglef &rect, const iaString &text, int32 textwidth, iWidgetState state, bool enabled)
    {
        drawText(rect, text, textwidth);

        DRAW_DEBUG_OUTPUT_OLD(rect, state);
    }

    void iWidgetDefaultTheme::drawText(const iaRectanglef &rect, const iaString &text, int32 textwidth)
    {
        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(_fontSize);
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);
        iRenderer::getInstance().drawString(rect._x, rect._y, text, COLOR_AMBIENT, textwidth);

        DRAW_DEBUG_OUTPUT_OLD(rect, iWidgetState::Pressed);
    }

    void iWidgetDefaultTheme::drawGraphGridlines(const iaRectanglef &rect, float32 lineWidth, const std::vector<iaVector2f> &verticalLines, const std::vector<iaVector2f> &horizontalLines, bool enabled)
    {
        if (lineWidth > 0.0)
        {
            iRenderer::getInstance().setLineWidth(lineWidth);

            const iaColor4f &color = enabled ? COLOR_AMBIENT : COLOR_DIFFUSE_DARK;

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

    void iWidgetDefaultTheme::drawGraphLabels(const iaRectanglef &rect, const std::vector<iaVector2f> &verticalLines, const std::vector<iaVector2f> &horizontalLines, bool enabled)
    {

        const iaColor4f &color = enabled ? COLOR_AMBIENT : COLOR_DIFFUSE_DARK;
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

        DRAW_DEBUG_OUTPUT_OLD(rect, iWidgetState::Pressed);
    }

    void iWidgetDefaultTheme::drawGroupBox(const iaRectanglef &rect, bool headerOnly, const iaString &text, iWidgetState state, bool enabled)
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

        DRAW_DEBUG_OUTPUT_OLD(rect, state);
    }

    void iWidgetDefaultTheme::drawTiledRectangle(const iaRectanglef &rect, iTexturePtr texture)
    {
        const iaVector2f tiling(rect._width / texture->getWidth(),
                                rect._height / texture->getHeight());
        iRenderer::getInstance().drawTexturedRectangle(rect._x, rect._y, rect._width, rect._height, texture, iaColor4f::white, false, tiling);
    }

    void iWidgetDefaultTheme::drawPicture(const iaRectanglef &rect, iTexturePtr texture, iWidgetState state, bool enabled)
    {
        const iaColor4f &color = enabled ? COLOR_WHITE : COLOR_AMBIENT;
        iRenderer::getInstance().drawTexturedRectangle(static_cast<float32>(rect._x), static_cast<float32>(rect._y), static_cast<float32>(rect._width), static_cast<float32>(rect._height), texture, color, texture->hasTransparency());

        DRAW_DEBUG_OUTPUT_OLD(rect, state);
    }

    void iWidgetDefaultTheme::drawButtonText(const iaRectanglef &rect, const iaString &text)
    {
        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(_fontSize);
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);
        iRenderer::getInstance().drawString(rect._x, rect._y, text, COLOR_TEXT);
    }

    void iWidgetDefaultTheme::drawButtonFrame(const iaRectanglef &rect, iWidgetState state, bool enabled)
    {
        const iaColor4f &diffuse = enabled ? COLOR_BUTTON_DEFAULT : COLOR_DIFFUSE_DARK;

        switch (state)
        {
        case iWidgetState::Pressed:
            iRenderer::getInstance().drawFilledRectangle(rect, COLOR_DIFFUSE_LIGHT);

            /*iRenderer::getInstance().setLineWidth(_defaultLineWidth);
            iRenderer::getInstance().drawLine(rect._x, rect._y, rect._width + rect._x, rect._y, COLOR_AMBIENT);
            iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._height + rect._y, COLOR_AMBIENT);

            iRenderer::getInstance().drawLine(rect._width + rect._x, rect._y, rect._width + rect._x, rect._height + rect._y, COLOR_SPECULAR);
            iRenderer::getInstance().drawLine(rect._x, rect._height + rect._y, rect._width + rect._x, rect._height + rect._y, COLOR_SPECULAR);*/
            break;

        case iWidgetState::Highlighted:
        case iWidgetState::Clicked:
        case iWidgetState::DoubleClicked:
        case iWidgetState::Standby:
        default:
            iRenderer::getInstance().drawFilledRectangle(rect, diffuse);

            /*iRenderer::getInstance().setLineWidth(_defaultLineWidth);
            iRenderer::getInstance().drawLine(rect._x, rect._y, rect._width + rect._x, rect._y, COLOR_SPECULAR);
            iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._height + rect._y, COLOR_SPECULAR);

            iRenderer::getInstance().drawLine(rect._x, rect._height + rect._y, rect._width + rect._x, rect._height + rect._y, COLOR_AMBIENT);
            iRenderer::getInstance().drawLine(rect._width + rect._x, rect._y, rect._width + rect._x, rect._height + rect._y, COLOR_AMBIENT);*/
            break;
        };
    }

    void iWidgetDefaultTheme::drawButtonFrame(const iaRectanglef &rect, const iaColor4f &color, iWidgetState state, bool enabled)
    {
        iaColor4f diffuse = color;
        if (!enabled)
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

    void iWidgetDefaultTheme::drawFrame(const iaRectanglef &rect, iWidgetState state, bool enabled)
    {
        const iaColor4f &color = enabled ? COLOR_AMBIENT : COLOR_DIFFUSE_DARK;

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._width + rect._x, rect._y, color);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._height + rect._y, color);
        iRenderer::getInstance().drawLine(rect._x, rect._height + rect._y, rect._width + rect._x, rect._height + rect._y, color);
        iRenderer::getInstance().drawLine(rect._width + rect._x, rect._y, rect._width + rect._x, rect._height + rect._y, color);
    }

    void iWidgetDefaultTheme::drawShadowRect(const iaRectanglef &rect)
    {
        // draw shadow
        iaRectanglef shadowRect = rect;
        iaColor4f color(0.0, 0.0, 0.0, 0.09);

        for (int i = 0; i < 10; ++i)
        {
            shadowRect.adjust(-1, -1, 2, 2);
            iRenderer::getInstance().drawRectangle(shadowRect, color);
            color._a -= 0.009;
        }
    }

    void iWidgetDefaultTheme::drawDialog(const iaRectanglef &rect, const iaRectanglef &clientRect, bool headerEnabled, const iaString &title, bool resizeEnabled, iWidgetState state, bool enabled)
    {
        iRenderer::getInstance().drawFilledRectangle(rect, COLOR_DIFFUSE_LIGHT);

        if (headerEnabled)
        {
            iRenderer::getInstance().drawFilledRectangle(rect._x, rect._y, rect._width, _titleWidth, COLOR_DIFFUSE_DARK);

            if (!title.isEmpty())
            {
                iaRectanglef titleRect = rect;
                titleRect.adjust(5, 8, 0, 0);
                drawText(titleRect, title, 0.0f);
            }
        }

        iRenderer::getInstance().drawFilledRectangle(clientRect, COLOR_DIFFUSE);

        DRAW_DEBUG_OUTPUT_OLD(rect, state);
    }

    IGOR_ENABLE_WARNING(4100)
} // namespace igor