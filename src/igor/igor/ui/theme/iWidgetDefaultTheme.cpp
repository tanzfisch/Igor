// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/theme/iWidgetDefaultTheme.h>
#include <igor/resources/iResourceManager.h>

#include <igor/resources/shader/iShader.h>
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

#define DRAW_DEBUG_OUTPUT_OLD(rect, state)                                \
    switch (state)                                                        \
    {                                                                     \
    case iWidgetState::Highlighted:                                       \
        iRenderer::getInstance().drawRectangle(rect, MOUSEOVER);          \
        break;                                                            \
                                                                          \
    case iWidgetState::Pressed:                                           \
        iRenderer::getInstance().drawRectangle(rect, MOUSEPRESSED);       \
        break;                                                            \
                                                                          \
    case iWidgetState::Clicked:                                           \
        iRenderer::getInstance().drawRectangle(rect, MOUSECLICKED);       \
        break;                                                            \
                                                                          \
    case iWidgetState::DoubleClicked:                                     \
        iRenderer::getInstance().drawRectangle(rect, MOUSEDOUBLECLICKED); \
        break;                                                            \
                                                                          \
    case iWidgetState::Standby:                                           \
        iRenderer::getInstance().drawRectangle(rect, STANDBY);            \
        break;                                                            \
    }

#define DRAW_DEBUG_OUTPUT(rect, id, state)                                \
    switch (state)                                                        \
    {                                                                     \
    case iWidgetState::Highlighted:                                       \
        iRenderer::getInstance().drawRectangle(rect, MOUSEOVER);          \
        drawText(rect, iaString::toString(id), 0);                        \
        break;                                                            \
                                                                          \
    case iWidgetState::Pressed:                                           \
        iRenderer::getInstance().drawRectangle(rect, MOUSEPRESSED);       \
        break;                                                            \
                                                                          \
    case iWidgetState::Clicked:                                           \
        iRenderer::getInstance().drawRectangle(rect, MOUSECLICKED);       \
        break;                                                            \
                                                                          \
    case iWidgetState::DoubleClicked:                                     \
        iRenderer::getInstance().drawRectangle(rect, MOUSEDOUBLECLICKED); \
        break;                                                            \
                                                                          \
    case iWidgetState::Standby:                                           \
        iRenderer::getInstance().drawRectangle(rect, STANDBY);            \
        break;                                                            \
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

static const iaColor4f COLOR_CHECKED_BORDER = {0.3f, 0.9f, 0.3f, 0.9f};
static const iaColor4f COLOR_CHECKED_FILL = {0.3f, 0.9f, 0.3f, 0.2f};

static const iaColor4f COLOR_SELECTED_BORDER = {0.3f, 0.3f, 0.9f, 0.9f};
static const iaColor4f COLOR_SELECTED_FILL = {0.3f, 0.3f, 0.9f, 0.2f};

static const iaColor4f COLOR_BUTTON_DEFAULT = {0.42f, 0.42f, 0.42f, 1.0f};

namespace igor
{
    IGOR_DISABLE_WARNING(4100)

    iWidgetDefaultTheme::iWidgetDefaultTheme(iTexturePtr fontTexture)
    {
        _font = iTextureFont::create(fontTexture);

        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_TEXTURE},
                           {IGOR_RESOURCE_PARAM_CACHE_MODE, iResourceCacheMode::Cache},
                           {IGOR_RESOURCE_PARAM_GENERATE, true},
                           {IGOR_RESOURCE_PARAM_TEXTURE_PATTERN, iTexturePattern::CheckerBoard},
                           {IGOR_RESOURCE_PARAM_PRIMARY_COLOR, iaColor4f::gray},
                           {IGOR_RESOURCE_PARAM_SECONDARY_COLOR, iaColor4f::lightGray},
                           {IGOR_RESOURCE_PARAM_TEXTURE_WIDTH, 128},
                           {IGOR_RESOURCE_PARAM_TEXTURE_HEIGHT, 128}});

        _checkerBoardTexture = iResourceManager::getInstance().loadResource<iTexture>(param);
    }

    void iWidgetDefaultTheme::draw(iWidgetPicturePtr widget)
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

    void iWidgetDefaultTheme::draw(iWidgetSpacerPtr widget)
    {
        iRenderer::getInstance().drawFilledRectangle(widget->getActualRect(), widget->isEnabled() ? widget->getBackground() : COLOR_SPECULAR);
    }

    void iWidgetDefaultTheme::draw(iWidgetLabelPtr widget)
    {
        const auto rect = widget->getActualRect();
        const auto &text = widget->getText();
        const auto maxTextWidth = widget->getMaxTextWidth();
        const auto state = widget->getState();

        drawText(rect, text, maxTextWidth);

        DRAW_DEBUG_OUTPUT_OLD(rect, state);
    }

    void iWidgetDefaultTheme::draw(iWidgetButtonPtr widget)
    {
        const auto scale = iWidget::getScale();
        const auto rect = widget->getActualRect();
        const auto state = widget->getState();
        const auto enabled = widget->isEnabled();
        const auto &background = widget->getBackground();
        const auto &foreground = widget->getForeground();
        const bool checked = widget->isCheckable() && widget->isChecked();
        const float32 offset = state == iWidgetState::Pressed ? scale : 0;
        iTexturePtr texture = widget->getBackgroundTexture();
        iTexturePtr icon = widget->getIcon();
        const auto &text = widget->getText();
        const auto halign = widget->getHorizontalTextAlignment();
        const auto valign = widget->getVerticalTextAlignment();
        const bool selected = widget->isSelectable() && widget->isSelected();
        const auto borderStyle = widget->getBorderStyle();

        if (texture == nullptr)
        {
            drawButtonFrame(rect, state, enabled, {COLOR_DIFFUSE_LIGHT});
        }
        else
        {
            if (borderStyle == iWidgetButtonBorderStyle::Line &&
                (state == iWidgetState::Highlighted ||
                 state == iWidgetState::Pressed))
            {
                iRenderer::getInstance().drawFilledRectangle(rect, COLOR_DIFFUSE_LIGHT);
            }

            const iaRectanglef picRect(rect._x + offset, rect._y + offset, rect._width, rect._height);
            const iaColor4f &color = enabled ? background : COLOR_AMBIENT;
            iRenderer::getInstance().drawTexturedRectangle(picRect, texture, color, texture->hasTransparency());
        }

        if (icon != nullptr)
        {
            float32 maxIconSize = std::min(rect._width, rect._height);
            iaRectanglef picRect;

            switch (halign)
            {
            case iHorizontalAlignment::Left:
                picRect.set(rect.getRight() - offset - rect._height, rect._y + offset, rect._height, rect._height);
                break;

            case iHorizontalAlignment::Center:
                picRect.set(rect._x + offset + rect._width * 0.5 - maxIconSize * 0.5, rect._y + offset, rect._height, rect._height);
                break;

            case iHorizontalAlignment::Right:
                picRect.set(rect._x + offset, rect._y + offset, rect._height, rect._height);
                break;
            };

            const iaColor4f &color = enabled ? foreground : COLOR_AMBIENT;
            iRenderer::getInstance().drawTexturedRectangle(picRect, icon, color, icon->hasTransparency());
        }

        if (!text.isEmpty())
        {
            float32 textwidth = _font->measureWidth(text, getFontSize());
            int32 textX = rect._x;
            int32 textY = rect._y;
            switch (halign)
            {
            case iHorizontalAlignment::Left:
                textX += 2 * scale;
                break;

            case iHorizontalAlignment::Center:
                textX += (rect._width - textwidth) * 0.5f;
                break;

            case iHorizontalAlignment::Right:
                textX += rect._width - 2 * scale - textwidth;
                break;
            };

            switch (valign)
            {
            case iVerticalAlignment::Top:
                textY += 2 * scale;
                break;

            case iVerticalAlignment::Center:
                textY += static_cast<int32>((static_cast<float32>(rect._height) - getFontSize()) * 0.5f);
                break;

            case iVerticalAlignment::Bottom:
                textY += rect._height - 2 * scale - static_cast<int32>(getFontSize());
                break;
            };

            const iaRectanglef textRect(textX + offset, textY + offset, 0, 0);
            drawButtonText(textRect, text);
        }

        // comes last since it is semi transparent
        if (checked)
        {
            iRenderer::getInstance().drawFilledRectangle(rect, COLOR_CHECKED_FILL);
            iRenderer::getInstance().drawRectangle(rect, COLOR_CHECKED_BORDER);
        }
        else if (selected)
        {
            iRenderer::getInstance().drawFilledRectangle(rect, COLOR_SELECTED_FILL);
            iRenderer::getInstance().drawRectangle(rect, COLOR_SELECTED_BORDER);
        }

        DRAW_DEBUG_OUTPUT(rect, widget->getID(), state);
    }

    void iWidgetDefaultTheme::drawSelection(const iaRectanglef &rect)
    {
        iRenderer::getInstance().drawFilledRectangle(rect, COLOR_SELECTED_FILL);
        iRenderer::getInstance().drawRectangle(rect, COLOR_SELECTED_BORDER);
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

        const float32 scale = iWidget::getScale();

        float32 newHeight = 64 * scale;
        float32 newWidth = 64 * scale;

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

        iRenderer::getInstance().setLineWidth(4 * scale);

        const iaColor4f green(0.0,0.75,0.0,0.75);
        const iaColor4f red(0.75,0.0,0.0,0.75);

        switch (drag.getDragState())
        {
        case iDragState::Accepted:
            iRenderer::getInstance().drawLine(rect.getBottomRight() + iaVector2f(-6, -6) * scale,
                                              rect.getBottomRight() + iaVector2f(2, 6) * scale, green);
            iRenderer::getInstance().drawLine(rect.getBottomRight() + iaVector2f(2, 6) * scale,
                                              rect.getBottomRight() + iaVector2f(6, -2) * scale, green);                                            
            break;
        case iDragState::Rejected:
            iRenderer::getInstance().drawLine(rect.getBottomRight() + iaVector2f(-6, -6) * scale,
                                              rect.getBottomRight() + iaVector2f(6, 6) * scale, red);
            iRenderer::getInstance().drawLine(rect.getBottomRight() + iaVector2f(-6, 6) * scale,
                                              rect.getBottomRight() + iaVector2f(6, -6) * scale, red);
            break;
        case iDragState::Neutral:
        default:
            break;
        }

        iRenderer::getInstance().setLineWidth(1);
    }

    void iWidgetDefaultTheme::draw(iWidgetCheckBoxPtr widget)
    {
        const auto rect = widget->getActualRect();
        const auto state = widget->getState();
        const auto enabled = widget->isEnabled();
        const bool checked = widget->isChecked();

        if (state != iWidgetState::Standby)
        {
            iRenderer::getInstance().drawFilledRectangle(rect, COLOR_SPECULAR);
        }

        iRenderer::getInstance().drawFilledRectangle(rect, enabled ? COLOR_WHITE : COLOR_DIFFUSE);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);
        iRenderer::getInstance().drawLine(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);

        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x + rect._width, rect._y, COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._y + rect._height, COLOR_AMBIENT);

        if (checked)
        {
            const iaColor4f &color = enabled ? COLOR_BLACK : COLOR_DIFFUSE_DARK;

            iRenderer::getInstance().drawLine(rect._x + 4, rect._y + 8, rect._x + rect._width / 2, rect._y + rect._height - 5, color);
            iRenderer::getInstance().drawLine(rect._x + 5, rect._y + 8, rect._x + rect._width / 2 + 1, rect._y + rect._height - 5, color);

            iRenderer::getInstance().drawLine(rect._x + rect._width - 5, rect._y + 4, rect._x + rect._width / 2, rect._y + rect._height - 5, color);
            iRenderer::getInstance().drawLine(rect._x + rect._width - 4, rect._y + 4, rect._x + rect._width / 2 + 1, rect._y + rect._height - 5, color);
        }

        DRAW_DEBUG_OUTPUT(rect, widget->getID(), state);
    }

    void iWidgetDefaultTheme::draw(iWidgetTextEditPtr widget)
    {
        const auto rect = widget->getActualRect();
        const auto state = widget->getState();
        const auto enabled = widget->isEnabled();
        const auto &text = widget->getText();

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
        iRenderer::getInstance().setFontSize(getFontSize());
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);

        iRenderer::getInstance().drawString(rect._x, rect._y, text, enabled ? COLOR_TEXT_DARK : COLOR_AMBIENT);

        // force draw call before changing the stencil settings
        iRenderer::getInstance().flush();

        iRenderer::getInstance().setStencilTestActive(false);

        DRAW_DEBUG_OUTPUT(rect, widget->getID(), state);
    }

    void iWidgetDefaultTheme::draw(iDialogPtr dialog)
    {
        const auto rect = dialog->getActualRect();
        const auto clientRect = dialog->getActualClientRect();
        const auto &title = dialog->getTitle();
        const auto state = dialog->getState();

        if (!dialog->isDocked())
        {
            drawShadowRect(rect);
        }

        iRenderer::getInstance().drawFilledRectangle(rect, COLOR_DIFFUSE_LIGHT);

        if (dialog->hasHeader())
        {
            iRenderer::getInstance().drawFilledRectangle(rect._x, rect._y, rect._width, getDialogTitleHeight(), COLOR_DIFFUSE_DARK);

            if (!title.isEmpty())
            {
                iaRectanglef titleRect = rect;
                titleRect.adjust(5 * iWidget::getScale(), (getDialogTitleHeight() - getFontSize()) * 0.5, 0, 0);
                drawText(titleRect, title, 0.0f);
            }
        }

        iRenderer::getInstance().drawFilledRectangle(clientRect, COLOR_DIFFUSE);

        DRAW_DEBUG_OUTPUT(rect, dialog->getID(), state);
    }

    void iWidgetDefaultTheme::draw(iWidgetColorPtr widget)
    {
        const auto rect = widget->getActualRect();
        const auto state = widget->getState();
        const auto &color = widget->getColor();

        const iaVector2f tiling(rect._width / _checkerBoardTexture->getWidth() * 4,
                                rect._height / _checkerBoardTexture->getHeight() * 4);
        iRenderer::getInstance().drawTexturedRectangle(rect._x, rect._y, rect._width, rect._height, _checkerBoardTexture, iaColor4f::white, false, tiling);

        iaRectanglef rectLeft = rect;
        rectLeft._width = rect._width * 0.5;
        iRenderer::getInstance().drawFilledRectangle(rectLeft, color);

        iaColor4f colorNoAlpha = color;
        colorNoAlpha._a = 1.0f;
        rectLeft._x += rectLeft._width;
        iRenderer::getInstance().drawFilledRectangle(rectLeft, colorNoAlpha);

        iRenderer::getInstance().drawRectangle(rect, COLOR_AMBIENT);

        DRAW_DEBUG_OUTPUT(rect, widget->getID(), state);
    }

    void iWidgetDefaultTheme::draw(iWidgetGroupBoxPtr widget)
    {
        const auto rect = widget->getActualRect();
        const auto state = widget->getState();
        const auto &text = widget->getText();

        float32 x = static_cast<float32>(rect._x);
        float32 y = static_cast<float32>(rect._y);
        float32 w = static_cast<float32>(rect._width);
        float32 h = static_cast<float32>(rect._height);

        if (!text.isEmpty())
        {
            y += getFontSize() * 0.5f;
            h -= getFontSize() * 0.5f;
        }

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);

        if (text == "")
        {
            iRenderer::getInstance().drawLine(x, y, x + w, y, COLOR_AMBIENT);
        }
        else
        {
            iRenderer::getInstance().drawLine(x, y, x + getFontSize(), y, COLOR_AMBIENT);
            iRenderer::getInstance().drawLine(x + _font->measureWidth(text, getFontSize()) + getFontSize() * 2.0f, y, x + w, y, COLOR_AMBIENT);
        }

        if (!widget->isHeaderOnly())
        {
            iRenderer::getInstance().drawLine(x, y, x, y + h, COLOR_AMBIENT);
            iRenderer::getInstance().drawLine(x, y + h, x + w, y + h, COLOR_AMBIENT);
            iRenderer::getInstance().drawLine(x + w, y, x + w, y + h, COLOR_AMBIENT);
        }

        drawText(iaRectanglef(static_cast<int32>(rect._x + getFontSize() * 1.5f), static_cast<int32>(rect._y + getFontSize() * 0.1f)), text, 0);

        DRAW_DEBUG_OUTPUT(rect, widget->getID(), state);
    }

    void iWidgetDefaultTheme::draw(iWidgetLineTextEditPtr widget)
    {
        const auto rect = widget->getActualRect();
        const auto state = widget->getState();
        const auto &text = widget->getText();
        const auto textwidth = _font->measureWidth(text, getFontSize());
        const auto enabled = widget->isEnabled();
        const auto hTextAlign = widget->getHorizontalTextAlignment();
        const auto vTextAlign = widget->getVerticalTextAlignment();
        const auto cursorPos = widget->getCursorPosPix();
        const auto keyboardFocus = widget->hasKeyboardFocus();

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

        switch (hTextAlign)
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

        switch (vTextAlign)
        {
        case iVerticalAlignment::Top:
            relatoveTextPosY += rect._height - 2 - static_cast<int32>(getFontSize());
            break;

        case iVerticalAlignment::Bottom:
            relatoveTextPosY += 2;
            break;

        case iVerticalAlignment::Center:
            relatoveTextPosY += static_cast<int32>((static_cast<float32>(rect._height) - getFontSize()) * 0.5f);
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

            iRenderer::getInstance().drawFilledRectangle(rect._x + relativeTextPosX + cursorPos, rect._y + relatoveTextPosY, 2.0f, getFontSize(), COLOR_TEXT_DARK);
        }

        // render text
        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(getFontSize());
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);

        iRenderer::getInstance().drawString(rect._x + relativeTextPosX, rect._y + relatoveTextPosY, text, keyboardFocus ? COLOR_TEXT_DARK : COLOR_AMBIENT);

        // force draw call before changing the stencil settings
        iRenderer::getInstance().flush();

        iRenderer::getInstance().setStencilTestActive(false);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);
        iRenderer::getInstance().drawLine(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);

        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x + rect._width, rect._y, COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._y + rect._height, COLOR_AMBIENT);

        DRAW_DEBUG_OUTPUT(rect, widget->getID(), state);
    }

    void iWidgetDefaultTheme::draw(iWidgetNumberChooserPtr widget)
    {
        const auto rect = widget->getActualRect();
        const auto state = widget->getState();
        const auto enabled = widget->isEnabled();
        const auto buttonUpState = widget->getButtonUpState();
        const auto buttonDownState = widget->getButtonDownState();

        iRenderer::getInstance().drawFilledRectangle(rect, enabled ? COLOR_SPECULAR : COLOR_DIFFUSE);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);
        iRenderer::getInstance().drawLine(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);

        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x + rect._width, rect._y, COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._y + rect._height, COLOR_AMBIENT);

        drawButton(iaRectanglef(rect._x + rect._width - rect._height - 1, rect._y + 1, rect._height, rect._height / 2 - 1),
                   "+", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, nullptr, buttonUpState, enabled, false);
        drawButton(iaRectanglef(rect._x + rect._width - rect._height - 1, rect._y + rect._height / 2, rect._height, rect._height / 2 - 1),
                   "-", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, nullptr, buttonDownState, enabled, false);

        const auto text = iaString::toString(widget->getValue(), widget->getAfterPoint()) + widget->getPostFix();
        iaRectanglef textRect(rect._x + static_cast<int32>((static_cast<float32>(rect._height) - getFontSize()) * 0.5f),
                              rect._y + static_cast<int32>((static_cast<float32>(rect._height) - getFontSize()) * 0.5f), 0, 0);
        drawText(textRect, text, 0);

        DRAW_DEBUG_OUTPUT(rect, widget->getID(), state);
    }

    void iWidgetDefaultTheme::draw(iWidgetSelectBoxPtr widget)
    {
        const auto rect = widget->getActualRect();
        const auto state = widget->getState();
        const auto enabled = widget->isEnabled();

        iRenderer::getInstance().drawFilledRectangle(rect, COLOR_DIFFUSE_LIGHT);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);
        iRenderer::getInstance().drawLine(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height, COLOR_SPECULAR);

        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x + rect._width, rect._y, COLOR_AMBIENT);
        iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._y + rect._height, COLOR_AMBIENT);

        const iaRectanglef buttonRect(rect._x + rect._width - rect._height, rect._y + 1, rect._height - 1, rect._height - 2);
        drawButton(buttonRect, "V", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, nullptr, widget->getButtonState(), enabled, false);

        iaString text = widget->getSelectedValue();
        const iaRectanglef textRect(rect._x + static_cast<int32>((static_cast<float32>(rect._height) - getFontSize()) * 0.5f),
                                    rect._y + static_cast<int32>((static_cast<float32>(rect._height) - getFontSize()) * 0.5f) + 1, 0, 0);
        drawText(textRect, text, 0);

        DRAW_DEBUG_OUTPUT(rect, widget->getID(), state);
    }

    void iWidgetDefaultTheme::draw(iWidgetSliderPtr widget)
    {
        const auto rect = widget->getActualRect();
        const auto state = widget->getState();
        const auto enabled = widget->isEnabled();
        const auto max = widget->getMaxValue();
        const auto min = widget->getMinValue();
        const auto value = widget->getValue();
        const auto backgroundTexture = widget->getBackgroundTexture();
        const auto texture = widget->getTexture();

        if (texture != nullptr)
        {
            drawPicture(iaRectanglef(rect._x, rect._y + rect._height / 4, rect._width, rect._height / 2), texture, state, enabled);
        }

        if (backgroundTexture == nullptr &&
            texture == nullptr)
        {
            iRenderer::getInstance().drawFilledRectangle(iaRectanglef(rect._x, rect._y + rect._height / 2 - 2, rect._width, 4));
            iRenderer::getInstance().drawRectangle(iaRectanglef(rect._x, rect._y + rect._height / 2 - 2, rect._width, 4));
        }

        const float32 factor = value / (max - min);
        const float32 offset = (rect._width - 9) * factor;

        iaRectanglef buttonRect;

        buttonRect.setHeight(rect._height);
        buttonRect.setWidth(9);
        buttonRect.setY(rect._y);
        buttonRect.setX(rect._x + static_cast<int32>(offset));

        drawButton(buttonRect, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, nullptr, iWidgetState::Highlighted, enabled, false);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO create new interfaces like the one above
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void iWidgetDefaultTheme::drawTooltip(const iaVector2f &pos, const iaString &text)
    {
        float32 textWidth = std::min(300.0f, _font->measureWidth(text, getFontSize()));
        float32 textHeight = _font->measureHeight(text, getFontSize(), textWidth);

        iaRectanglef rect(pos._x, pos._y, textWidth + 10, textHeight + 10);
        iRenderer::getInstance().drawFilledRectangle(rect, COLOR_WHITE);
        iRenderer::getInstance().drawRectangle(rect, COLOR_BLACK);

        rect.adjust(5, 5, 0, 0);
        drawText(rect, text, textWidth);
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
        drawButtonFrame(rect, state, enabled, {color});

        if (checked)
        {
            drawChecked(rect);
        }
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
            drawButtonFrame(rect, state, enabled, {COLOR_DIFFUSE_LIGHT});
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
            float32 textwidth = _font->measureWidth(text, getFontSize());
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
                textY += static_cast<int32>((static_cast<float32>(rect._height) - getFontSize()) * 0.5f);
                break;

            case iVerticalAlignment::Bottom:
                textY += rect._height - 2 - static_cast<int32>(getFontSize());
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

    float32 iWidgetDefaultTheme::getDialogTitleHeight() const
    {
        return _fontSize * 1.5 * iWidget::getScale();
    }

    float32 iWidgetDefaultTheme::getDialogFrameWidth() const
    {
        return _frameWidth * iWidget::getScale();
    }

    float32 iWidgetDefaultTheme::getFontSize() const
    {
        return _fontSize * iWidget::getScale();
    }

    iTextureFontPtr iWidgetDefaultTheme::getFont() const
    {
        return _font;
    }

    void iWidgetDefaultTheme::drawText(const iaRectanglef &rect, const iaString &text, int32 textwidth)
    {
        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(getFontSize());
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);
        iRenderer::getInstance().drawString(rect._x, rect._y, text, COLOR_AMBIENT, textwidth);
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
        const float32 fontSize = getFontSize() * 0.75f;

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

    void iWidgetDefaultTheme::drawPicture(const iaRectanglef &rect, iTexturePtr texture, iWidgetState state, bool enabled)
    {
        const iaColor4f &color = enabled ? COLOR_WHITE : COLOR_AMBIENT;
        iRenderer::getInstance().drawTexturedRectangle(static_cast<float32>(rect._x), static_cast<float32>(rect._y), static_cast<float32>(rect._width), static_cast<float32>(rect._height), texture, color, texture->hasTransparency());

        DRAW_DEBUG_OUTPUT_OLD(rect, state);
    }

    void iWidgetDefaultTheme::drawButtonText(const iaRectanglef &rect, const iaString &text)
    {
        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(getFontSize());
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);
        iRenderer::getInstance().drawString(rect._x, rect._y, text, COLOR_TEXT);
    }

    void iWidgetDefaultTheme::drawButtonFrame(const iaRectanglef &rect, iWidgetState state, bool enabled, const std::vector<iaColor4f> &colors)
    {
        con_assert(!colors.empty(), "need one color at least");

        iaColor4f diffuse = colors[0];
        if (!enabled)
        {
            diffuse._r *= 0.7f;
            diffuse._g *= 0.7f;
            diffuse._b *= 0.7f;
        }

        switch (state)
        {
        case iWidgetState::Pressed:
            iRenderer::getInstance().drawFilledRectangle(rect, diffuse);

            if (colors.size() == 3)
            {
                iRenderer::getInstance().setLineWidth(_defaultLineWidth);
                iRenderer::getInstance().drawLine(rect._x, rect._y, rect._width + rect._x, rect._y, colors[1]);
                iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._height + rect._y, colors[1]);

                iRenderer::getInstance().drawLine(rect._width + rect._x, rect._y, rect._width + rect._x, rect._height + rect._y, colors[2]);
                iRenderer::getInstance().drawLine(rect._x, rect._height + rect._y, rect._width + rect._x, rect._height + rect._y, colors[2]);
            }
            else if (colors.size() == 2)
            {
                iRenderer::getInstance().drawRectangle(rect, colors[1]);
            }
            break;

        case iWidgetState::Highlighted:
        case iWidgetState::Clicked:
        case iWidgetState::DoubleClicked:
        case iWidgetState::Standby:
        default:
            iRenderer::getInstance().drawFilledRectangle(rect, diffuse);

            if (colors.size() == 3)
            {
                iRenderer::getInstance().setLineWidth(_defaultLineWidth);
                iRenderer::getInstance().drawLine(rect._x, rect._y, rect._width + rect._x, rect._y, colors[2]);
                iRenderer::getInstance().drawLine(rect._x, rect._y, rect._x, rect._height + rect._y, colors[2]);

                iRenderer::getInstance().drawLine(rect._x, rect._height + rect._y, rect._width + rect._x, rect._height + rect._y, colors[1]);
                iRenderer::getInstance().drawLine(rect._width + rect._x, rect._y, rect._width + rect._x, rect._height + rect._y, colors[1]);
            }
            else if (colors.size() == 2)
            {
                iRenderer::getInstance().drawRectangle(rect, colors[1]);
            }
        };
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

    IGOR_ENABLE_WARNING(4100)
} // namespace igor