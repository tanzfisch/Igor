#include <iWidgetDefaultTheme.h>

#include <iTextureFont.h>
#include <iMaterialResourceFactory.h>
#include <iTextureResourceFactory.h>
#include <iMaterial.h>

#include <iaConsole.h>
using namespace IgorAux;

#define DEBUG_OUTPUT 1

#if DEBUG_OUTPUT == 1
    iaColor4f MOUSEOVER(1,1,1,0.5f);
    iaColor4f MOUSEPRESSED(1, 0, 0, 1);
    iaColor4f MOUSECLICKED(1, 1, 0, 1);
    iaColor4f MOUSEDOUBLECLICKED(1, 1, 1, 1);
    iaColor4f MOUSECONTEXT(0, 1, 0, 1);
    iaColor4f STANDBY(0, 0, 1, 0.5f);

#define DRAW_DEBUG_OUTPUT(x, y, width, height, state) \
    switch(state) \
    { \
        case iWidgetAppearanceState::Highlighted: \
            drawRectangle(x, y, width, height, MOUSEOVER); \
            break; \
 \
        case iWidgetAppearanceState::Pressed: \
            drawRectangle(x, y, width, height, MOUSEPRESSED); \
            break; \
 \
        case iWidgetAppearanceState::Clicked: \
            drawRectangle(x, y, width, height, MOUSECLICKED); \
            break; \
 \
        case iWidgetAppearanceState::DoubleClicked: \
            drawRectangle(x, y, width, height, MOUSEDOUBLECLICKED); \
            break; \
 \
        case iWidgetAppearanceState::Context: \
            drawRectangle(x, y, width, height, MOUSECONTEXT); \
            break; \
 \
        case iWidgetAppearanceState::Standby: \
            drawRectangle(x, y, width, height, STANDBY); \
            break; \
    }

#else
#define DRAW_DEBUG_OUTPUT
#endif

namespace Igor
{
    iWidgetDefaultTheme::iWidgetDefaultTheme(const iaString& fontTexture, const iaString& backgroundTexture)
    {
        _font = new iTextureFont(fontTexture);
        con_assert(_font != nullptr, "can't create font");

        _backgroundTexture = iTextureResourceFactory::getInstance().loadFile(backgroundTexture);

        _ambient.set(0.2f, 0.2f, 0.2f, 1.0f);
        _darkDiffuse.set(0.35f, 0.35f, 0.35f, 1.0f);
        _diffuse.set(0.5f, 0.5f, 0.5f, 1.0f);
        _diffuseTransparent = _diffuse;
        _diffuseTransparent._a = 0.75f;
        _lightDiffuse.set(0.55f, 0.55f, 0.55f, 1.0f);
        _specular.set(0.8f, 0.8f, 0.8f, 1.0f);
        _textColor.set(0.2f, 0.2f, 0.2f, 1.0f);
        _textColorDark.set(0.0f, 0.0f, 0.0f, 1.0f);
        _black.set(0.0f, 0.0f, 0.0f, 1.0f);
        _white.set(1.0f, 1.0f, 1.0f, 1.0f);

        _defaultMaterial = iMaterialResourceFactory::getInstance().createMaterial();
        iMaterialResourceFactory::getInstance().getMaterial(_defaultMaterial)->setName("Widget:Default");
        iMaterialResourceFactory::getInstance().getMaterial(_defaultMaterial)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
        iMaterialResourceFactory::getInstance().getMaterial(_defaultMaterial)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);

        _texturedMaterial = iMaterialResourceFactory::getInstance().createMaterial();
        iMaterialResourceFactory::getInstance().getMaterial(_texturedMaterial)->setName("Widget:Textured");
        iMaterialResourceFactory::getInstance().getMaterial(_texturedMaterial)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
        iMaterialResourceFactory::getInstance().getMaterial(_texturedMaterial)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
        iMaterialResourceFactory::getInstance().getMaterial(_texturedMaterial)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    }

    iWidgetDefaultTheme::~iWidgetDefaultTheme()
    {
        if (_font)
        {
            delete _font;
            _font = nullptr;
        }
    }

    void iWidgetDefaultTheme::drawRectangle(int32 posx, int32 posy, int32 width, int32 height)
    {
        drawRectangle(posx, posy, width, height, _ambient);
    }

    void iWidgetDefaultTheme::drawFilledRectangle(int32 posx, int32 posy, int32 width, int32 height)
    {
        drawFilledRectangle(posx, posy, width, height, _diffuse);
    }

    void iWidgetDefaultTheme::drawRectangle(int32 posx, int32 posy, int32 width, int32 height, const iaColor4f& color)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        iRenderer::getInstance().setColor(color);
        iRenderer::getInstance().drawLine(posx, posy, posx + width, posy);
        iRenderer::getInstance().drawLine(posx, posy, posx, posy + height);
        iRenderer::getInstance().drawLine(posx, posy + height, posx + width, posy + height);
        iRenderer::getInstance().drawLine(posx + width, posy, posx + width, posy + height);
    }

    void iWidgetDefaultTheme::drawFilledRectangle(int32 posx, int32 posy, int32 width, int32 height, const iaColor4f& color)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        iRenderer::getInstance().setColor(color);
        iRenderer::getInstance().drawRectangle(posx, posy, width, height);
    }

    void iWidgetDefaultTheme::drawGridHighlight(int32 posx, int32 posy, int32 width, int32 height)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);
        iRenderer::getInstance().setColor(_lightDiffuse);
        iRenderer::getInstance().drawRectangle(posx, posy, width, height);
    }

    void iWidgetDefaultTheme::drawGridSelection(int32 posx, int32 posy, int32 width, int32 height)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);
        iRenderer::getInstance().setColor(_specular);
        iRenderer::getInstance().drawRectangle(posx, posy, width, height);
    }

	void iWidgetDefaultTheme::drawGridField(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state)
	{
		DRAW_DEBUG_OUTPUT(posx, posy, width, height, state);
	}

    void iWidgetDefaultTheme::drawBackgroundFrame(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        iRenderer::getInstance().setColor(_diffuseTransparent);
        iRenderer::getInstance().drawRectangle(posx, posy, width, height);

        iRenderer::getInstance().setColor(_ambient);
        iRenderer::getInstance().drawLine(posx, posy, posx + width, posy);
        iRenderer::getInstance().drawLine(posx, posy, posx, posy + height);
        iRenderer::getInstance().drawLine(posx + 1, posy + 1, posx + width - 1, posy + 1);
        iRenderer::getInstance().drawLine(posx + 1, posy + 1, posx + 1, posy + height - 1);

        iRenderer::getInstance().setColor(_specular);
        iRenderer::getInstance().drawLine(posx, posy + height, posx + width, posy + height);
        iRenderer::getInstance().drawLine(posx + width, posy, posx + width, posy + height);
        iRenderer::getInstance().drawLine(posx + 1, posy + height - 1, posx + width - 1, posy + height - 1);
        iRenderer::getInstance().drawLine(posx + width - 1, posy + 1, posx + width - 1, posy + height - 1);
    }

    void iWidgetDefaultTheme::drawButton(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, iHorrizontalAlignment align, iVerticalAlignment valign, shared_ptr<iTexture> texture, iWidgetAppearanceState state, bool active)
    {
        const int32 reduction = 2;
        int32 offset = 0;
        if (state == iWidgetAppearanceState::Pressed)
        {
            offset = +1;
        }

        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);
        if (texture == nullptr)
        {
            drawButtonFrame(posx, posy, width, height, state, active);
        }
        else
        {
            if (state == iWidgetAppearanceState::Highlighted ||
                state == iWidgetAppearanceState::Pressed)
            {
                iRenderer::getInstance().setColor(_lightDiffuse);
                iRenderer::getInstance().drawRectangle(posx, posy, width, height);
            }
        }

        iMaterialResourceFactory::getInstance().setMaterial(_texturedMaterial);
        if (texture != nullptr)
        {
            drawPicture(posx + offset + reduction / 2, posy + offset + reduction / 2, width - reduction, height - reduction, texture, state, active);
        }

        float32 textwidth = _font->measureWidth(text, _fontSize);
        int32 textX = posx;
        int32 textY = posy;
        switch (align)
        {
        case iHorrizontalAlignment::Left:
            textX += 2;
            break;

        case iHorrizontalAlignment::Center:
            textX += (width - textwidth) / 2.0f;
            break;

        case iHorrizontalAlignment::Right:
            textX += width - 2 - textwidth;
            break;
        };

        switch (valign)
        {
        case iVerticalAlignment::Top:
            textY += 2;
            break;

        case iVerticalAlignment::Center:
            textY += (height - _fontSize) / 2.0f;
            break;

        case iVerticalAlignment::Bottom:
            textY += height - 2 - _fontSize;
            break;
        };

        drawButtonText(textX + offset, textY + offset, text);

        DRAW_DEBUG_OUTPUT(posx, posy, width, height, state);
    }

    void iWidgetDefaultTheme::drawTextEdit(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, iHorrizontalAlignment align, iVerticalAlignment valign, bool keyboardFocus, iWidgetAppearanceState state, bool active)
    {
        iaString modText = text;

        float32 textwidth = _font->measureWidth(modText, _fontSize);

        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        if (active)
        {
            iRenderer::getInstance().setColor(_specular);
        }
        else
        {
            iRenderer::getInstance().setColor(_diffuse);
        }
        iRenderer::getInstance().drawRectangle(posx, posy, width, height);

        iRenderer::getInstance().setColor(_specular);
        iRenderer::getInstance().drawLine(posx, posy + height, posx + width, posy + height);
        iRenderer::getInstance().drawLine(posx + width, posy, posx + width, posy + height);

        iRenderer::getInstance().setColor(_ambient);
        iRenderer::getInstance().drawLine(posx, posy, posx + width, posy);
        iRenderer::getInstance().drawLine(posx, posy, posx, posy + height);

        iMaterialResourceFactory::getInstance().setMaterial(_texturedMaterial);

        if (keyboardFocus)
        {
            modText = modText + "|";
            iRenderer::getInstance().setColor(_textColorDark);
        }
        else
        {
            iRenderer::getInstance().setColor(_textColor);
        }

        // todo begrenzung des textes auf das Eingabefeld

        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(_fontSize);
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);

        int32 textPosX = posx;
        int32 textPosY = posy;

        switch (align)
        {
        case iHorrizontalAlignment::Left:
            textPosX += 2;
            break;

        case iHorrizontalAlignment::Right:
            textPosX += width - 2 - textwidth;
            break;

        case iHorrizontalAlignment::Center:
            textPosX += (width - textwidth) / 2.0f;
            break;
        };

        switch (valign)
        {
        case iVerticalAlignment::Top:
            textPosY += height - 2 - _fontSize;
            break;

        case iVerticalAlignment::Bottom:
            textPosY += 2;
            break;

        case iVerticalAlignment::Center:
            textPosY += (height - _fontSize) / 2.0f;
            break;
        };

        iRenderer::getInstance().drawString(textPosX, textPosY, modText);

        DRAW_DEBUG_OUTPUT(posx, posy, width, height, state);
    }

    void iWidgetDefaultTheme::drawNumberChooser(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, iWidgetAppearanceState button_up_state, iWidgetAppearanceState button_down_state, bool active)
    {
        drawNumberChooserFrame(posx, posy, width, height, button_up_state, button_down_state, active);
        drawText(posx + (height - _fontSize) / 2, posy + (height - _fontSize) / 2, text, 0);

        DRAW_DEBUG_OUTPUT(posx, posy, width, height, button_up_state);
    }

    void iWidgetDefaultTheme::drawNumberChooserFrame(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state_button_up, iWidgetAppearanceState state_button_down, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        iRenderer::getInstance().setColor(_lightDiffuse);
        iRenderer::getInstance().drawRectangle(posx, posy, width, height);

        iRenderer::getInstance().setColor(_specular);
        iRenderer::getInstance().drawLine(posx, posy + height, posx + width, posy + height);
        iRenderer::getInstance().drawLine(posx + width, posy, posx + width, posy + height);

        iRenderer::getInstance().setColor(_ambient);
        iRenderer::getInstance().drawLine(posx, posy, posx + width, posy);
        iRenderer::getInstance().drawLine(posx, posy, posx, posy + height);

        drawButton(posx + width - height - 1, posy + 1, height, height / 2 - 1, "+", iHorrizontalAlignment::Center, iVerticalAlignment::Center, nullptr, state_button_up, active);
        drawButton(posx + width - height - 1, posy + height / 2, height, height / 2 - 1, "-", iHorrizontalAlignment::Center, iVerticalAlignment::Center, nullptr, state_button_down, active);
    }

    void iWidgetDefaultTheme::drawSelectBox(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, iWidgetAppearanceState buttonAppearance, bool active)
    {
        drawSelectBoxFrame(posx, posy, width, height, buttonAppearance, active);
        drawText(posx + (height - _fontSize) * 0.5f, posy + (height - _fontSize) * 0.5f + 1, text, 0);

        DRAW_DEBUG_OUTPUT(posx, posy, width, height, buttonAppearance);
    }

    void iWidgetDefaultTheme::drawSelectBoxFrame(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState buttonState, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        iRenderer::getInstance().setColor(_lightDiffuse);
        iRenderer::getInstance().drawRectangle(posx, posy, width, height);

        iRenderer::getInstance().setColor(_specular);
        iRenderer::getInstance().drawLine(posx, posy + height, posx + width, posy + height);
        iRenderer::getInstance().drawLine(posx + width, posy, posx + width, posy + height);

        iRenderer::getInstance().setColor(_ambient);
        iRenderer::getInstance().drawLine(posx, posy, posx + width, posy);
        iRenderer::getInstance().drawLine(posx, posy, posx, posy + height);

        drawButton(posx + width - height, posy + 1, height - 1, height - 2, "V", iHorrizontalAlignment::Center, iVerticalAlignment::Center, nullptr, buttonState, active);
    }

    void iWidgetDefaultTheme::drawSelectBoxDropDown(int32 posx, int32 posy, int32 width, int32 height, vector<iaString>& text, int highlightIndex, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        iRenderer::getInstance().setColor(_lightDiffuse);
        iRenderer::getInstance().drawRectangle(posx, posy + height - 1, width - height, height * text.size());

        iRenderer::getInstance().setColor(_ambient);
        iRenderer::getInstance().drawLine(posx, posy + height - 1, posx, posy + height * (text.size() + 1));
        iRenderer::getInstance().drawLine(posx + width - height, posy + height - 1, posx + width - height, posy + height * (text.size() + 1));
        iRenderer::getInstance().drawLine(posx, posy + height * (text.size() + 1), posx + width - height, posy + height * (text.size() + 1));
    }

    float32 iWidgetDefaultTheme::getFontSize()
    {
        return _fontSize;
    }

    iTextureFont* iWidgetDefaultTheme::getFont()
    {
        return _font;
    }

    void iWidgetDefaultTheme::setFontMetrics(const float32 fontSize, const float32 lineHeight)
    {
        _fontSize = fontSize;
        _fontLineHeight = lineHeight;
    }

    void iWidgetDefaultTheme::drawLabel(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, iWidgetAppearanceState state, bool active)
    {
        drawText(posx + (height - _fontSize) * 0.5f, posy + (height - _fontSize) * 0.5f, text, 0);

        DRAW_DEBUG_OUTPUT(posx, posy, width, height, state);
    }

    void iWidgetDefaultTheme::drawCheckBoxFrame(int32 x, int32 y, int32 width, int32 height, iWidgetAppearanceState state, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        if (state != iWidgetAppearanceState::Standby)
        {
            iRenderer::getInstance().setColor(_specular);
            iRenderer::getInstance().drawRectangle(x, y, width, height);
        }
    }

    void iWidgetDefaultTheme::drawCheckBox(int32 x, int32 y, int32 width, int32 height, iWidgetAppearanceState state, bool active, bool checked)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        if (active)
        {
            iRenderer::getInstance().setColor(_white);
        }
        else
        {
            iRenderer::getInstance().setColor(_diffuse);
        }

        iRenderer::getInstance().drawRectangle(x, y, width, height);

        iRenderer::getInstance().setColor(_specular);
        iRenderer::getInstance().drawLine(x, y + height, x + width, y + height);
        iRenderer::getInstance().drawLine(x + width, y, x + width, y + height);

        iRenderer::getInstance().setColor(_ambient);
        iRenderer::getInstance().drawLine(x, y, x + width, y);
        iRenderer::getInstance().drawLine(x, y, x, y + height);

        if (checked)
        {
            if (active)
            {
                iRenderer::getInstance().setColor(_black);
            }
            else
            {
                iRenderer::getInstance().setColor(_darkDiffuse);
            }

            iRenderer::getInstance().drawLine(x + 2, y + 4, x + width / 2, y + height - 3);
            iRenderer::getInstance().drawLine(x + 3, y + 4, x + width / 2 + 1, y + height - 3);

            iRenderer::getInstance().drawLine(x + width - 3, y + 2, x + width / 2, y + height - 3);
            iRenderer::getInstance().drawLine(x + width - 2, y + 2, x + width / 2 + 1, y + height - 3);
        }
    }

    void iWidgetDefaultTheme::drawCheckBox(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, bool checked, iWidgetAppearanceState state, bool active)
    {
        drawCheckBoxFrame(posx, posy, width, height, state, active);
        drawCheckBox(posx + (height - _fontSize) / 2, posy + (height - _fontSize) / 2, _fontSize, _fontSize, state, active, checked);
        drawText(posx + (height - _fontSize) / 2 + _fontSize * 2, posy + (height - _fontSize) / 2, text, 0);

        DRAW_DEBUG_OUTPUT(posx, posy, width, height, state);
    }

    void iWidgetDefaultTheme::drawText(int32 posx, int32 posy, const iaString& text, long textwidth)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_texturedMaterial);

        iRenderer::getInstance().setColor(_ambient);
        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(_fontSize);
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);
        iRenderer::getInstance().drawString(posx, posy, text, false, textwidth);

		DRAW_DEBUG_OUTPUT(posx, posy, 10, 10, iWidgetAppearanceState::Pressed);
    }

    void iWidgetDefaultTheme::drawGroupBox(int32 posx, int32 posy, int32 width, int32 height, int32 border, const iaString& text, iWidgetAppearanceState state, bool active)
    {
        float32 x = static_cast<float32>(posx) + static_cast<float32>(border) * 0.5f;
        float32 y = static_cast<float32>(posy) + static_cast<float32>(border) * 0.5f;
        float32 w = static_cast<float32>(width) - static_cast<float32>(border);
        float32 h = static_cast<float32>(height) - static_cast<float32>(border);

        if (text != "")
        {
            y += _fontSize * 0.5f;
            h -= _fontSize * 0.5f;
        }

        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);
        iRenderer::getInstance().setColor(_ambient);

        if (text == "")
        {
            iRenderer::getInstance().drawLine(x, y, x + w, y);
        }
        else
        {
            iRenderer::getInstance().drawLine(x, y, x + _fontSize, y);
            iRenderer::getInstance().drawLine(x + _font->measureWidth(text, _fontSize) + _fontSize * 2.0f, y, x + w, y);
        }
        
        iRenderer::getInstance().drawLine(x, y, x, y + h);
        iRenderer::getInstance().drawLine(x, y + h, x + w, y + h);
        iRenderer::getInstance().drawLine(x + w, y, x + w, y + h);

        drawText(x + _fontSize * 1.5, y - _fontSize * 0.5f, text, 0);

        DRAW_DEBUG_OUTPUT(posx, posy, width, height, state);
    }

    void iWidgetDefaultTheme::drawTiledRectangle(int32 posx, int32 posy, int32 width, int32 height, shared_ptr<iTexture> texture)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_texturedMaterial);
        iRenderer::getInstance().setColor(_white);
        iRenderer::getInstance().drawTextureTiled(posx, posy, width, height, texture);
    }

    void iWidgetDefaultTheme::drawPicture(int32 posx, int32 posy, int32 width, int32 height, shared_ptr<iTexture> texture, iWidgetAppearanceState state, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_texturedMaterial);

        if (active)
        {
            iRenderer::getInstance().setColor(_white);
        }
        else
        {
            iRenderer::getInstance().setColor(_ambient);
        }

        iRenderer::getInstance().drawTexture(posx, posy, width, height, texture);

        DRAW_DEBUG_OUTPUT(posx, posy, width, height, state);
    }

    void iWidgetDefaultTheme::drawButtonText(int32 posx, int32 posy, const iaString& text)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_texturedMaterial);

        iRenderer::getInstance().setColor(_textColor);
        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(_fontSize);
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);
        iRenderer::getInstance().drawString(posx, posy, text);
    }

    void iWidgetDefaultTheme::drawButtonFrame(int32 x, int32 y, int32 width, int32 height, iWidgetAppearanceState state, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        iaColor4f diffuse = _diffuse;
        if (!active)
        {
            diffuse = _darkDiffuse;
        }

        switch (state)
        {
        case iWidgetAppearanceState::Pressed:
            iRenderer::getInstance().setColor(_lightDiffuse);
            iRenderer::getInstance().drawRectangle(x, y, width, height);

            iRenderer::getInstance().setColor(_ambient);
            iRenderer::getInstance().drawLine(x, y, width + x, y);
            iRenderer::getInstance().drawLine(x, y, x, height + y);

            iRenderer::getInstance().setColor(_specular);
            iRenderer::getInstance().drawLine(width + x, y, width + x, height + y);
            iRenderer::getInstance().drawLine(x, height + y, width + x, height + y);
            break;

        case iWidgetAppearanceState::Highlighted:
        case iWidgetAppearanceState::Clicked:
        case iWidgetAppearanceState::DoubleClicked:
            diffuse = _lightDiffuse;

        case iWidgetAppearanceState::Standby:
        default:
            iRenderer::getInstance().setColor(diffuse);
            iRenderer::getInstance().drawRectangle(x, y, width, height);

            iRenderer::getInstance().setColor(_specular);
            iRenderer::getInstance().drawLine(x, y, width + x, y);
            iRenderer::getInstance().drawLine(x, y, x, height + y);

            iRenderer::getInstance().setColor(_ambient);
            iRenderer::getInstance().drawLine(x, height + y, width + x, height + y);
            iRenderer::getInstance().drawLine(width + x, y, width + x, height + y);
        };
    }

    void iWidgetDefaultTheme::drawSpacer(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        if (active)
        {
            iRenderer::getInstance().setColor(_darkDiffuse);
        }
        else
        {
            iRenderer::getInstance().setColor(_lightDiffuse);
        }

        iRenderer::getInstance().drawRectangle(posx, posy, width, height);
    }

    void iWidgetDefaultTheme::drawDialog(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        drawTiledRectangle(posx, posy, width, height, _backgroundTexture);

        iRenderer::getInstance().setColor(_ambient);
        iRenderer::getInstance().drawLine(posx, posy, posx + width, posy);
        iRenderer::getInstance().drawLine(posx, posy, posx, posy + height);
        iRenderer::getInstance().drawLine(posx, posy + height, posx + width, posy + height);
        iRenderer::getInstance().drawLine(posx + width, posy, posx + width, posy + height);

        DRAW_DEBUG_OUTPUT(posx, posy, width, height, state);
    }

    /*

    void iWidgetDefaultTheme::drawTextEditField(long width, long height, iWidgetAppearanceState state, bool active, const iaString& text, float32 _fontSize, float32 _fontLineHeight, WidgetAlign align, WidgetVAlign valign, bool keyboard_focus)
    {
    if(!standard_font) return;

    float32 x = 0, y = 0;

    float32 textwidth = (long)standard_font->measureWidth(text, _fontSize);

    switch(align)
    {
    case WA_left:   x = 2; break;
    case WA_right:  x = width - 2 - textwidth; break;
    case WA_center: x = (width - textwidth)/2.0f; break;
    };

    switch(valign)
    {
    case WVA_top:   y = height - 2 - _fontSize; break;
    case WVA_bottom:y = 2; break;
    case WVA_center:y = (height - _fontSize)/2.0f; break;
    };

    iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

    iRenderer::getInstance().setColor(_lightDiffuse);
    iRenderer::getInstance().drawRectangle(0,0,width,height);

    iRenderer::getInstance().setColor(_specular);
    iRenderer::getInstance().drawLine(0,height,width,height);
    iRenderer::getInstance().drawLine(width,0,width,height);

    iRenderer::getInstance().setColor(_ambient);
    iRenderer::getInstance().drawLine(0,0,width,0);
    iRenderer::getInstance().drawLine(0,0,0,height);

    iMaterialResourceFactory::getInstance().setMaterial(_texturedMaterial);

    if(keyboard_focus)
    {
    text = text + "|";
    iRenderer::getInstance().setColor(_textColorDark);
    }
    else
    {
    iRenderer::getInstance().setColor(_textColor);
    }

    // todo begrenzung des textes auf das Eingabefeld
    iRenderer::getInstance().setFont(standard_font);
    iRenderer::getInstance().setFontSize(_fontSize);
    iRenderer::getInstance().setFontLineHeight(_fontLineHeight);
    dcRenderer.printString(x, y, text);
    }

    */

}