#include <iWidgetDefaultTheme.h>

#include <iTextureFont.h>
#include <iMaterialResourceFactory.h>
#include <iTextureResourceFactory.h>
#include <iMaterial.h>

#include <iaConsole.h>
using namespace IgorAux;

#define DEBUG_OUTPUT 0

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

static const iaColor4f COLOR_AMBIENT = { 0.2f, 0.2f, 0.2f, 1.0f };
static const iaColor4f COLOR_DIFFUSE_DARK = { 0.35f, 0.35f, 0.35f, 1.0f };
static const iaColor4f COLOR_DIFFUSE = { 0.5f, 0.5f, 0.5f, 1.0f };
static const iaColor4f COLOR_DIFFUSE_TRANSPARENT = { 0.5f, 0.5f, 0.5f, 0.75f };
static const iaColor4f COLOR_DIFFUSE_LIGHT = { 0.55f, 0.55f, 0.55f, 1.0f };
static const iaColor4f COLOR_SPECULAR = { 0.8f, 0.8f, 0.8f, 1.0f };
static const iaColor4f COLOR_WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
static const iaColor4f COLOR_BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };
static const iaColor4f COLOR_TEXT = { 0.2f, 0.2f, 0.2f, 1.0f };
static const iaColor4f COLOR_TEXT_DARK = { 0.0f, 0.0f, 0.0f, 1.0f };

namespace Igor
{
	__IGOR_DISABLE_WARNING__(4100)

    iWidgetDefaultTheme::iWidgetDefaultTheme(const iaString& fontTexture, const iaString& backgroundTexture)
    {
        _font = new iTextureFont(fontTexture);
        con_assert(_font != nullptr, "can't create font");

        _backgroundTexture = iTextureResourceFactory::getInstance().loadFile(backgroundTexture);

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
        drawRectangle(posx, posy, width, height, COLOR_AMBIENT);
    }

    void iWidgetDefaultTheme::drawFilledRectangle(int32 posx, int32 posy, int32 width, int32 height)
    {
        drawFilledRectangle(posx, posy, width, height, COLOR_DIFFUSE);
    }

    void iWidgetDefaultTheme::drawGradient(int32 posx, int32 posy, int32 width, int32 height, const iaGradientColor4f& gradient)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);
        iRenderer::getInstance().setLineWidth(1);

        iaColor4f color;

        for (int i = 0; i < width; ++i)
        {
            float32 indexPosX = static_cast<float32>(posx + i) + 0.5f;
            float32 halfHeight = static_cast<float32>(height) * 0.5f;

            gradient.getValue(static_cast<float32>(i) / static_cast<float32>(width), color);
            iRenderer::getInstance().setColor(color);
            iRenderer::getInstance().drawLine(indexPosX, static_cast<float32>(posy) + halfHeight + 0.5f, indexPosX, static_cast<float32>(posy+ height) + 0.5f);
            color._a = 1.0;
            iRenderer::getInstance().setColor(color);
            iRenderer::getInstance().drawLine(indexPosX, static_cast<float32>(posy) + 0.5f, indexPosX, static_cast<float32>(posy) + halfHeight + 0.5f);
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

	void iWidgetDefaultTheme::drawRectangleInt(int32 x, int32 y, int32 width, int32 height)
	{
		iRenderer::getInstance().drawRectangle(static_cast<float32>(x), static_cast<float32>(y), static_cast<float32>(width), static_cast<float32>(height));
	}
	
	void iWidgetDefaultTheme::drawStringInt(int32 x, int32 y, iaString text, float32 angle, int32 maxWidth)
	{
		iRenderer::getInstance().drawString(static_cast<float32>(x), static_cast<float32>(y), text, angle, static_cast<float32>(maxWidth));
	}

    void iWidgetDefaultTheme::drawRectangle(int32 posx, int32 posy, int32 width, int32 height, const iaColor4f& color)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);
        iRenderer::getInstance().setLineWidth(_defaultLineWidth);

        iRenderer::getInstance().setColor(color);
		drawLineInt(posx, posy, posx + width, posy);
		drawLineInt(posx, posy, posx, posy + height);
		drawLineInt(posx, posy + height, posx + width, posy + height);
		drawLineInt(posx + width, posy, posx + width, posy + height);
    }

    void iWidgetDefaultTheme::drawFilledRectangle(int32 posx, int32 posy, int32 width, int32 height, const iaColor4f& color)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        iRenderer::getInstance().setColor(color);
        drawRectangleInt(posx, posy, width, height);
    }

    void iWidgetDefaultTheme::drawGridHighlight(int32 posx, int32 posy, int32 width, int32 height)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);
        iRenderer::getInstance().setColor(COLOR_DIFFUSE_LIGHT);
        drawRectangleInt(posx, posy, width, height);
    }

    void iWidgetDefaultTheme::drawGridSelection(int32 posx, int32 posy, int32 width, int32 height)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);
        iRenderer::getInstance().setColor(COLOR_SPECULAR);
        drawRectangleInt(posx, posy, width, height);
    }

	void iWidgetDefaultTheme::drawGridField(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state)
	{
		DRAW_DEBUG_OUTPUT(posx, posy, width, height, state);
	}

    void iWidgetDefaultTheme::drawGraphFrame(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        iRenderer::getInstance().setColor(COLOR_DIFFUSE_TRANSPARENT);
        drawRectangleInt(posx, posy, width, height);
        iRenderer::getInstance().setLineWidth(_defaultLineWidth);

        iRenderer::getInstance().setColor(COLOR_AMBIENT);
		drawLineInt(posx, posy, posx + width, posy);
		drawLineInt(posx, posy, posx, posy + height);

        iRenderer::getInstance().setColor(COLOR_SPECULAR);
		drawLineInt(posx, posy + height, posx + width, posy + height);
		drawLineInt(posx + width, posy, posx + width, posy + height);
    }

    void iWidgetDefaultTheme::drawBackgroundFrame(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        iRenderer::getInstance().setColor(COLOR_DIFFUSE_TRANSPARENT);
        drawRectangleInt(posx, posy, width, height);
        iRenderer::getInstance().setLineWidth(_defaultLineWidth);

        iRenderer::getInstance().setColor(COLOR_AMBIENT);
		drawLineInt(posx, posy, posx + width, posy);
		drawLineInt(posx, posy, posx, posy + height);
		drawLineInt(posx + 1, posy + 1, posx + width - 1, posy + 1);
		drawLineInt(posx + 1, posy + 1, posx + 1, posy + height - 1);

        iRenderer::getInstance().setColor(COLOR_SPECULAR);
		drawLineInt(posx, posy + height, posx + width, posy + height);
		drawLineInt(posx + width, posy, posx + width, posy + height);
		drawLineInt(posx + 1, posy + height - 1, posx + width - 1, posy + height - 1);
		drawLineInt(posx + width - 1, posy + 1, posx + width - 1, posy + height - 1);
    }	

    void iWidgetDefaultTheme::drawButton(int32 posx, int32 posy, int32 width, int32 height, const iaColor4f& color, iWidgetAppearanceState state, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);
        drawButtonFrame(posx, posy, width, height, color, state, active);

        DRAW_DEBUG_OUTPUT(posx, posy, width, height, state);
    }

    void iWidgetDefaultTheme::drawButton(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, iHorizontalAlignment align, iVerticalAlignment valign, shared_ptr<iTexture> texture, iWidgetAppearanceState state, bool active)
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
                iRenderer::getInstance().setColor(COLOR_DIFFUSE_LIGHT);
                drawRectangleInt(posx, posy, width, height);
            }
        }

        if (texture != nullptr)
        {
            iMaterialResourceFactory::getInstance().setMaterial(_texturedMaterial);
            drawPicture(posx + offset + reduction / 2, posy + offset + reduction / 2, width - reduction, height - reduction, texture, state, active);
        }

        float32 textwidth = _font->measureWidth(text, _fontSize);
        int32 textX = posx;
        int32 textY = posy;
        switch (align)
        {
        case iHorizontalAlignment::Left:
            textX += 2;
            break;

        case iHorizontalAlignment::Center:
            textX += static_cast<int32>((static_cast<float32>(width) - textwidth) * 0.5f);
            break;

        case iHorizontalAlignment::Right:
            textX += width - 2 - static_cast<int32>(textwidth);
            break;
        };

        switch (valign)
        {
        case iVerticalAlignment::Top:
            textY += 2;
            break;

        case iVerticalAlignment::Center:
            textY += static_cast<int32>((static_cast<float32>(height) - _fontSize) * 0.5f);
            break;

        case iVerticalAlignment::Bottom:
            textY += height - 2 - static_cast<int32>(_fontSize);
            break;
        };

        drawButtonText(textX + offset, textY + offset, text);

        DRAW_DEBUG_OUTPUT(posx, posy, width, height, state);
    }

    void iWidgetDefaultTheme::drawTextEdit(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, iHorizontalAlignment align, iVerticalAlignment valign, bool keyboardFocus, iWidgetAppearanceState state, bool active)
    {
        iaString modText = text;

        float32 textwidth = _font->measureWidth(modText, _fontSize);

        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        if (active)
        {
            iRenderer::getInstance().setColor(COLOR_SPECULAR);
        }
        else
        {
            iRenderer::getInstance().setColor(COLOR_DIFFUSE);
        }
        drawRectangleInt(posx, posy, width, height);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().setColor(COLOR_SPECULAR);
		drawLineInt(posx, posy + height, posx + width, posy + height);
		drawLineInt(posx + width, posy, posx + width, posy + height);

        iRenderer::getInstance().setColor(COLOR_AMBIENT);
		drawLineInt(posx, posy, posx + width, posy);
		drawLineInt(posx, posy, posx, posy + height);

        iMaterialResourceFactory::getInstance().setMaterial(_texturedMaterial);

        if (keyboardFocus)
        {
            modText = modText + "|";
            iRenderer::getInstance().setColor(COLOR_TEXT_DARK);
        }
        else
        {
            iRenderer::getInstance().setColor(COLOR_TEXT);
        }

        // todo begrenzung des textes auf das Eingabefeld

        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(_fontSize);
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);

        int32 textPosX = posx;
        int32 textPosY = posy;

        switch (align)
        {
        case iHorizontalAlignment::Left:
            textPosX += 2;
            break;

        case iHorizontalAlignment::Right:
            textPosX += width - 2 - static_cast<int32>(textwidth);
            break;

        case iHorizontalAlignment::Center:
            textPosX += static_cast<int32>((static_cast<float32>(width) - textwidth) * 0.5f);
            break;
        };

        switch (valign)
        {
        case iVerticalAlignment::Top:
            textPosY += height - 2 - static_cast<int32>(_fontSize);
            break;

        case iVerticalAlignment::Bottom:
            textPosY += 2;
            break;

        case iVerticalAlignment::Center:
            textPosY += static_cast<int32>((static_cast<float32>(height) - _fontSize) * 0.5f);
            break;
        };

        drawStringInt(textPosX, textPosY, modText);

        DRAW_DEBUG_OUTPUT(posx, posy, width, height, state);
    }

    void iWidgetDefaultTheme::drawNumberChooser(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, iWidgetAppearanceState button_up_state, iWidgetAppearanceState button_down_state, bool active)
    {
        drawNumberChooserFrame(posx, posy, width, height, button_up_state, button_down_state, active);
        drawText(posx + static_cast<int32>((static_cast<float32>(height) - _fontSize) * 0.5f), 
			posy + static_cast<int32>((static_cast<float32>(height) - _fontSize) * 0.5f), text, 0);

        DRAW_DEBUG_OUTPUT(posx, posy, width, height, button_up_state);
    }

    void iWidgetDefaultTheme::drawNumberChooserFrame(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state_button_up, iWidgetAppearanceState state_button_down, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        iRenderer::getInstance().setColor(COLOR_DIFFUSE_LIGHT);
        drawRectangleInt(posx, posy, width, height);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().setColor(COLOR_SPECULAR);
		drawLineInt(posx, posy + height, posx + width, posy + height);
		drawLineInt(posx + width, posy, posx + width, posy + height);

        iRenderer::getInstance().setColor(COLOR_AMBIENT);
		drawLineInt(posx, posy, posx + width, posy);
		drawLineInt(posx, posy, posx, posy + height);

        drawButton(posx + width - height - 1, posy + 1, height, height / 2 - 1, "+", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, state_button_up, active);
        drawButton(posx + width - height - 1, posy + height / 2, height, height / 2 - 1, "-", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, state_button_down, active);
    }

    void iWidgetDefaultTheme::drawSelectBox(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, iWidgetAppearanceState buttonAppearance, bool active)
    {
        drawSelectBoxFrame(posx, posy, width, height, buttonAppearance, active);
        drawText(posx + static_cast<int32>((static_cast<float32>(height) - _fontSize) * 0.5f), 
			posy + static_cast<int32>((static_cast<float32>(height) - _fontSize) * 0.5f) + 1, text, 0);

        DRAW_DEBUG_OUTPUT(posx, posy, width, height, buttonAppearance);
    }

    void iWidgetDefaultTheme::drawSelectBoxFrame(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState buttonState, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        iRenderer::getInstance().setColor(COLOR_DIFFUSE_LIGHT);
        drawRectangleInt(posx, posy, width, height);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().setColor(COLOR_SPECULAR);
		drawLineInt(posx, posy + height, posx + width, posy + height);
		drawLineInt(posx + width, posy, posx + width, posy + height);

        iRenderer::getInstance().setColor(COLOR_AMBIENT);
        drawLineInt(posx, posy, posx + width, posy);
        drawLineInt(posx, posy, posx, posy + height);

        drawButton(posx + width - height, posy + 1, height - 1, height - 2, "V", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, buttonState, active);
    }

    void iWidgetDefaultTheme::drawSelectBoxDropDown(int32 posx, int32 posy, int32 width, int32 height, vector<iaString>& text, int highlightIndex, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        iRenderer::getInstance().setColor(COLOR_DIFFUSE_LIGHT);
        drawRectangleInt(posx, posy + height - 1, width - height, height * static_cast<int32>(text.size()));

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().setColor(COLOR_AMBIENT);
		drawLineInt(posx, posy + height - 1, posx, posy + height * (static_cast<int32>(text.size()) + 1));
		drawLineInt(posx + width - height, posy + height - 1, posx + width - height, posy + height * (static_cast<int32>(text.size()) + 1));
		drawLineInt(posx, posy + height * (static_cast<int32>(text.size()) + 1), posx + width - height, posy + height * (static_cast<int32>(text.size()) + 1));
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

    void iWidgetDefaultTheme::drawCheckBoxFrame(int32 x, int32 y, int32 width, int32 height, iWidgetAppearanceState state, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        if (state != iWidgetAppearanceState::Standby)
        {
            iRenderer::getInstance().setColor(COLOR_SPECULAR);
            drawRectangleInt(x, y, width, height);
        }
    }

    void iWidgetDefaultTheme::drawCheckBox(int32 x, int32 y, int32 width, int32 height, iWidgetAppearanceState state, bool active, bool checked)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        if (active)
        {
            iRenderer::getInstance().setColor(COLOR_WHITE);
        }
        else
        {
            iRenderer::getInstance().setColor(COLOR_DIFFUSE);
        }

        drawRectangleInt(x, y, width, height);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().setColor(COLOR_SPECULAR);
		drawLineInt(x, y + height, x + width, y + height);
		drawLineInt(x + width, y, x + width, y + height);

        iRenderer::getInstance().setColor(COLOR_AMBIENT);
		drawLineInt(x, y, x + width, y);
		drawLineInt(x, y, x, y + height);

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

			drawLineInt(x + 2, y + 4, x + width / 2, y + height - 3);
			drawLineInt(x + 3, y + 4, x + width / 2 + 1, y + height - 3);

			drawLineInt(x + width - 3, y + 2, x + width / 2, y + height - 3);
			drawLineInt(x + width - 2, y + 2, x + width / 2 + 1, y + height - 3);
        }
    }

    void iWidgetDefaultTheme::drawCheckBox(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, bool checked, iWidgetAppearanceState state, bool active)
    {
        drawCheckBoxFrame(posx, posy, width, height, state, active);
        drawCheckBox(posx + static_cast<int32>((static_cast<float32>(height) - _fontSize) * 0.5f), 
			posy + static_cast<int32>((static_cast<float32>(height) - _fontSize) * 0.5f), static_cast<int32>(_fontSize), static_cast<int32>(_fontSize), state, active, checked);
        drawText(posx + static_cast<int32>((static_cast<float32>(height) - _fontSize) * 0.5f) + static_cast<int32>(_fontSize) * 2, 
			posy + static_cast<int32>((static_cast<float32>(height) - _fontSize) *0.5f), text, 0);

        DRAW_DEBUG_OUTPUT(posx, posy, width, height, state);
    }

    void iWidgetDefaultTheme::drawLabel(int32 posx, int32 posy, int32 width, int32 height, const iaString& text, int32 textwidth, iWidgetAppearanceState state, bool active)
    {
        drawText(posx, posy, text, textwidth);

        DRAW_DEBUG_OUTPUT(posx, posy, width, height, state);
    }

    void iWidgetDefaultTheme::drawText(int32 posx, int32 posy, const iaString& text, int32 textwidth)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_texturedMaterial);

        iRenderer::getInstance().setColor(COLOR_AMBIENT);
        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(_fontSize);
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);
        drawStringInt(posx, posy, text, 0.0f, textwidth);

		DRAW_DEBUG_OUTPUT(posx, posy, 10, 10, iWidgetAppearanceState::Pressed);
    }

    void iWidgetDefaultTheme::drawGraphGridlines(int32 posx, int32 posy, int32 width, int32 height, float32 lineWidth, const vector<iaVector2f>& verticalLines, const vector<iaVector2f>& horizontalLines, bool active)
    {
        if (lineWidth > 0.0)
        {
            iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

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
				drawLineInt(posx + static_cast<int32>(verticalLine._x), posy, posx + static_cast<int32>(verticalLine._x), posy + height);
            }

            for (auto horizontalLine : horizontalLines)
            {
				drawLineInt(posx, posy + static_cast<int32>(horizontalLine._x), posx + width, posy + static_cast<int32>(horizontalLine._x));
            }

            iRenderer::getInstance().setLineWidth(1.0);
        }
    }

    void iWidgetDefaultTheme::drawGraphLabels(int32 posx, int32 posy, int32 width, int32 height, const vector<iaVector2f>& verticalLines, const vector<iaVector2f>& horizontalLines, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_texturedMaterial);

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

        for (int i = 0; i < verticalLines.size();++i)
        {
            value = iaString::ftoa(verticalLines[i]._y, 2);

            if (i < verticalLines.size() - 1)
            {
                iRenderer::getInstance().drawString(posx + verticalLines[i]._x + 2, posy + height - 2 - fontSize, value, 90);
            }
            else
            {
                iRenderer::getInstance().drawString(posx + verticalLines[i]._x - fontSize - 2, posy + height - 2 - fontSize, value, 90);
            }
        }

        bool first = true;
        for (auto horizontalLine : horizontalLines)
        {
            value = iaString::ftoa(horizontalLine._y, 2);

            if (first)
            {
                iRenderer::getInstance().drawString(posx + 2 + fontSize, posy + horizontalLine._x + 2, value);
                first = false;
            }
            else
            {
                iRenderer::getInstance().drawString(posx + 2 + fontSize, posy + horizontalLine._x - fontSize, value);
            }
        }
    }

    void iWidgetDefaultTheme::drawGraph(int32 posx, int32 posy, const iaColor4f& lineColor, const iaColor4f& pointColor, float32 lineWidth, float32 pointSize, const vector<iaVector2f>& points)
    {
        iaVector2f currentPoint;
        iaVector2f lastPoint;

        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        if (lineWidth > 0.0)
        {
            iRenderer::getInstance().setColor(lineColor);
            iRenderer::getInstance().setLineWidth(lineWidth);

            for (int i = 0; i < points.size(); ++i)
            {
                currentPoint._x = points[i]._x + posx;
                currentPoint._y = points[i]._y + posy;

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
                currentPoint._x = points[i]._x + posx;
                currentPoint._y = points[i]._y + posy;
                
                iRenderer::getInstance().drawPoint(currentPoint._x, currentPoint._y);
            }
        }


        DRAW_DEBUG_OUTPUT(posx, posy, 10, 10, iWidgetAppearanceState::Pressed);
    }

    void iWidgetDefaultTheme::drawGroupBox(int32 posx, int32 posy, int32 width, int32 height, bool headerOnly, const iaString& text, iWidgetAppearanceState state, bool active)
    {
        float32 x = static_cast<float32>(posx);
        float32 y = static_cast<float32>(posy);
        float32 w = static_cast<float32>(width);
        float32 h = static_cast<float32>(height);

        if (text != "")
        {
            y += _fontSize * 0.5f;
            h -= _fontSize * 0.5f;
        }

        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);
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

        drawText(static_cast<int32>(x + _fontSize * 1.5f), static_cast<int32>(y - _fontSize * 0.5f), text, 0);

        DRAW_DEBUG_OUTPUT(posx, posy, width, height, state);
    }

    void iWidgetDefaultTheme::drawTiledRectangle(int32 posx, int32 posy, int32 width, int32 height, shared_ptr<iTexture> texture)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_texturedMaterial);
        iRenderer::getInstance().setColor(COLOR_WHITE);
        iRenderer::getInstance().drawTextureTiled(static_cast<float32>(posx), static_cast<float32>(posy), static_cast<float32>(width), static_cast<float32>(height), texture);
    }

    void iWidgetDefaultTheme::drawPicture(int32 posx, int32 posy, int32 width, int32 height, shared_ptr<iTexture> texture, iWidgetAppearanceState state, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_texturedMaterial);

        if (active)
        {
            iRenderer::getInstance().setColor(COLOR_WHITE);
        }
        else
        {
            iRenderer::getInstance().setColor(COLOR_AMBIENT);
        }

        iRenderer::getInstance().drawTexture(static_cast<float32>(posx), static_cast<float32>(posy), static_cast<float32>(width), static_cast<float32>(height), texture);

        DRAW_DEBUG_OUTPUT(posx, posy, width, height, state);
    }

    void iWidgetDefaultTheme::drawButtonText(int32 posx, int32 posy, const iaString& text)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_texturedMaterial);

        iRenderer::getInstance().setColor(COLOR_TEXT);
        iRenderer::getInstance().setFont(_font);
        iRenderer::getInstance().setFontSize(_fontSize);
        iRenderer::getInstance().setFontLineHeight(_fontLineHeight);
		drawStringInt(posx, posy, text);
    }

    void iWidgetDefaultTheme::drawButtonFrame(int32 x, int32 y, int32 width, int32 height, iWidgetAppearanceState state, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        iaColor4f diffuse = COLOR_DIFFUSE;
        if (!active)
        {
            diffuse = COLOR_DIFFUSE_DARK;
        }

        switch (state)
        {
        case iWidgetAppearanceState::Pressed:
            iRenderer::getInstance().setColor(COLOR_DIFFUSE_LIGHT);
            drawRectangleInt(x, y, width, height);

            iRenderer::getInstance().setLineWidth(_defaultLineWidth);
            iRenderer::getInstance().setColor(COLOR_AMBIENT);
			drawLineInt(x, y, width + x, y);
			drawLineInt(x, y, x, height + y);

            iRenderer::getInstance().setColor(COLOR_SPECULAR);
            drawLineInt(width + x, y, width + x, height + y);
            drawLineInt(x, height + y, width + x, height + y);
            break;

        case iWidgetAppearanceState::Highlighted:
        case iWidgetAppearanceState::Clicked:
        case iWidgetAppearanceState::DoubleClicked:
            diffuse = COLOR_DIFFUSE_LIGHT;

        case iWidgetAppearanceState::Standby:
        default:
            iRenderer::getInstance().setColor(diffuse);
            drawRectangleInt(x, y, width, height);

            iRenderer::getInstance().setLineWidth(_defaultLineWidth);
            iRenderer::getInstance().setColor(COLOR_SPECULAR);
            drawLineInt(x, y, width + x, y);
            drawLineInt(x, y, x, height + y);

            iRenderer::getInstance().setColor(COLOR_AMBIENT);
            drawLineInt(x, height + y, width + x, height + y);
            drawLineInt(width + x, y, width + x, height + y);
        };
    }

    void iWidgetDefaultTheme::drawButtonFrame(int32 x, int32 y, int32 width, int32 height, const iaColor4f& color, iWidgetAppearanceState state, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

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
        case iWidgetAppearanceState::Pressed:
            iRenderer::getInstance().setColor(diffuse);
            drawRectangleInt(x, y, width, height);

            iRenderer::getInstance().setLineWidth(_defaultLineWidth);
            iRenderer::getInstance().setColor(COLOR_AMBIENT);
            drawLineInt(x, y, width + x, y);
            drawLineInt(x, y, x, height + y);

            iRenderer::getInstance().setColor(COLOR_SPECULAR);
            drawLineInt(width + x, y, width + x, height + y);
            drawLineInt(x, height + y, width + x, height + y);
            break;

        case iWidgetAppearanceState::Highlighted:
        case iWidgetAppearanceState::Clicked:
        case iWidgetAppearanceState::DoubleClicked:
        case iWidgetAppearanceState::Standby:
        default:
            iRenderer::getInstance().setColor(diffuse);
            drawRectangleInt(x, y, width, height);

            iRenderer::getInstance().setLineWidth(_defaultLineWidth);
            iRenderer::getInstance().setColor(COLOR_SPECULAR);
            drawLineInt(x, y, width + x, y);
            drawLineInt(x, y, x, height + y);

            iRenderer::getInstance().setColor(COLOR_AMBIENT);
            drawLineInt(x, height + y, width + x, height + y);
            drawLineInt(width + x, y, width + x, height + y);
        };
    }

    void iWidgetDefaultTheme::drawFrame(int32 x, int32 y, int32 width, int32 height, iWidgetAppearanceState state, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        if (active)
        {
            iRenderer::getInstance().setColor(COLOR_AMBIENT);
        }
        else
        {
            iRenderer::getInstance().setColor(COLOR_DIFFUSE_DARK);
        }

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        drawLineInt(x, y, width + x, y);
        drawLineInt(x, y, x, height + y);
        drawLineInt(x, height + y, width + x, height + y);
        drawLineInt(width + x, y, width + x, height + y);
    }

    void iWidgetDefaultTheme::drawSpacer(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        if (active)
        {
            iRenderer::getInstance().setColor(COLOR_DIFFUSE_DARK);
        }
        else
        {
            iRenderer::getInstance().setColor(COLOR_DIFFUSE_LIGHT);
        }

        drawRectangleInt(posx, posy, width, height);
    }

    void iWidgetDefaultTheme::drawDialog(int32 posx, int32 posy, int32 width, int32 height, iWidgetAppearanceState state, bool active)
    {
        iMaterialResourceFactory::getInstance().setMaterial(_defaultMaterial);

        drawTiledRectangle(posx, posy, width, height, _backgroundTexture);

        iRenderer::getInstance().setLineWidth(_defaultLineWidth);
        iRenderer::getInstance().setColor(COLOR_AMBIENT);
        drawLineInt(posx, posy, posx + width, posy);
        drawLineInt(posx, posy, posx, posy + height);
        drawLineInt(posx, posy + height, posx + width, posy + height);
        drawLineInt(posx + width, posy, posx + width, posy + height);

        DRAW_DEBUG_OUTPUT(posx, posy, width, height, state);
    }

	__IGOR_ENABLE_WARNING__(4100)
}