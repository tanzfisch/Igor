#include <iWidgetDefaultTheme.h>

#include <iTextureFont.h>
#include <iMaterialResourceFactory.h>
#include <iTextureResourceFactory.h>
#include <iMaterial.h>
#include <iRenderer.h>

#include <iaConsole.h>
using namespace IgorAux;

// TODO remove later again
#include <GLee.h>
#include <GL\glu.h>


#define DEBUG_OUTPUT 0

#if DEBUG_OUTPUT == 1
iaColor4f MOUSEOVER(1, 1, 1, 0.5f);
iaColor4f MOUSEPRESSED(1, 0, 0, 1);
iaColor4f MOUSECLICKED(1, 1, 0, 1);
iaColor4f MOUSEDOUBLECLICKED(1, 1, 1, 1);
iaColor4f MOUSECONTEXT(0, 1, 0, 1);
iaColor4f STANDBY(0, 0, 1, 0.5f);

#define DRAW_DEBUG_OUTPUT(rect, state) \
    switch(state) \
    { \
        case iWidgetAppearanceState::Highlighted: \
            drawRectangle(rect, MOUSEOVER); \
            break; \
 \
        case iWidgetAppearanceState::Pressed: \
            drawRectangle(rect, MOUSEPRESSED); \
            break; \
 \
        case iWidgetAppearanceState::Clicked: \
            drawRectangle(rect, MOUSECLICKED); \
            break; \
 \
        case iWidgetAppearanceState::DoubleClicked: \
            drawRectangle(rect, MOUSEDOUBLECLICKED); \
            break; \
 \
        case iWidgetAppearanceState::Context: \
            drawRectangle(rect, MOUSECONTEXT); \
            break; \
 \
        case iWidgetAppearanceState::Standby: \
            drawRectangle(rect, STANDBY); \
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

	void iWidgetDefaultTheme::drawRectangle(const iRectanglei& rect)
	{
		drawRectangle(rect, COLOR_AMBIENT);
	}

	void iWidgetDefaultTheme::drawFilledRectangle(const iRectanglei& rect)
	{
		drawFilledRectangle(rect, COLOR_DIFFUSE);
	}

	void iWidgetDefaultTheme::drawGradient(const iRectanglei& rect, const iaGradientColor4f& gradient)
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

	void iWidgetDefaultTheme::drawRectangleInt(const iRectanglei & rect)
	{
		iRenderer::getInstance().drawRectangle(static_cast<float32>(rect._x), static_cast<float32>(rect._y),
			static_cast<float32>(rect._width), static_cast<float32>(rect._height));
	}

	void iWidgetDefaultTheme::drawStringInt(int32 x, int32 y, iaString text, float32 angle, int32 maxWidth)
	{
		iRenderer::getInstance().drawString(static_cast<float32>(x), static_cast<float32>(y), text, angle, static_cast<float32>(maxWidth));
	}

	void iWidgetDefaultTheme::drawRectangle(const iRectanglei & rect, const iaColor4f & color)
	{
		iRenderer::getInstance().setMaterial(_defaultMaterial);
		iRenderer::getInstance().setLineWidth(_defaultLineWidth);

		iRenderer::getInstance().setColor(color);
		drawLineInt(rect._x, rect._y, rect._x + rect._width, rect._y);
		drawLineInt(rect._x, rect._y, rect._x, rect._y + rect._height);
		drawLineInt(rect._x, rect._y + rect._height, rect._x + rect._width, rect._y + rect._height);
		drawLineInt(rect._x + rect._width, rect._y, rect._x + rect._width, rect._y + rect._height);
	}

	void iWidgetDefaultTheme::drawFilledRectangle(const iRectanglei & rect, const iaColor4f & color)
	{
		iRenderer::getInstance().setMaterial(_defaultMaterial);

		iRenderer::getInstance().setColor(color);
		drawRectangleInt(rect);
	}

	void iWidgetDefaultTheme::drawGridHighlight(const iRectanglei & rect)
	{
		iRenderer::getInstance().setMaterial(_defaultMaterial);
		iRenderer::getInstance().setColor(COLOR_DIFFUSE_LIGHT);
		drawRectangleInt(rect);
	}

	void iWidgetDefaultTheme::drawGridSelection(const iRectanglei & rect)
	{
		iRenderer::getInstance().setMaterial(_defaultMaterial);
		iRenderer::getInstance().setColor(COLOR_SPECULAR);
		drawRectangleInt(rect);
	}

	void iWidgetDefaultTheme::drawGridField(const iRectanglei & rect, iWidgetAppearanceState state)
	{
		DRAW_DEBUG_OUTPUT(rect, state);
	}

	void iWidgetDefaultTheme::drawGraphFrame(const iRectanglei & rect, iWidgetAppearanceState state, bool active)
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

	void iWidgetDefaultTheme::drawBackgroundFrame(const iRectanglei & rect, iWidgetAppearanceState state, bool active)
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

	void iWidgetDefaultTheme::drawButton(const iRectanglei & rect, const iaColor4f & color, iWidgetAppearanceState state, bool active)
	{
		iRenderer::getInstance().setMaterial(_defaultMaterial);
		drawButtonFrame(rect, color, state, active);

		DRAW_DEBUG_OUTPUT(rect, state);
	}

	void iWidgetDefaultTheme::drawButton(const iRectanglei & rect, const iaString & text, iHorizontalAlignment align, iVerticalAlignment valign, shared_ptr<iTexture> texture, iWidgetAppearanceState state, bool active)
	{
		const int32 reduction = 2;
		int32 offset = 0;
		if (state == iWidgetAppearanceState::Pressed)
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
			if (state == iWidgetAppearanceState::Highlighted ||
				state == iWidgetAppearanceState::Pressed)
			{
				iRenderer::getInstance().setColor(COLOR_DIFFUSE_LIGHT);
				drawRectangleInt(rect);
			}
		}

		if (texture != nullptr)
		{
			iRenderer::getInstance().setMaterial(_texturedMaterial);
			iRectanglei picRect(rect._x + offset + reduction / 2, rect._y + offset + reduction / 2, rect._width - reduction, rect._height - reduction);
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

		iRectanglei textRect(textX + offset, textY + offset, 0, 0);
		drawButtonText(textRect, text);

		DRAW_DEBUG_OUTPUT(rect, state);
	}

	void iWidgetDefaultTheme::drawTextEdit(const iRectanglei & rect, const iaString & text, const uint32 cursorPos, iHorizontalAlignment align, iVerticalAlignment valign, bool keyboardFocus, iWidgetAppearanceState state, bool active)
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
		glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE);  // draw 1s on test fail (always)

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

		if (keyboardFocus)
		{
			iRenderer::getInstance().setColor(COLOR_TEXT_DARK);
			float32 textwidth = _font->measureWidth(modText.getSubString(0, cursorPos), _fontSize);
			iRenderer::getInstance().drawRectangle(rect._x + textwidth + 2, rect._y + 3, 2, _fontSize);
		}

		iRenderer::getInstance().setStencilMask(0xff);
		iRenderer::getInstance().setStencilFunction(iRenderStateValue::Equal, 1, 0xff);

		// render text
		iRenderer::getInstance().setMaterial(_texturedMaterial);
		iRenderer::getInstance().setFont(_font);
		iRenderer::getInstance().setFontSize(_fontSize);
		iRenderer::getInstance().setFontLineHeight(_fontLineHeight);

		int32 textPosX = rect._x;
		int32 textPosY = rect._y;

		switch (align)
		{
		case iHorizontalAlignment::Left:
			textPosX += 2;
			break;

		case iHorizontalAlignment::Right:
			textPosX += rect._width - 2 - static_cast<int32>(textwidth);
			break;

		case iHorizontalAlignment::Center:
			textPosX += static_cast<int32>((static_cast<float32>(rect._width) - textwidth) * 0.5f);
			break;
		};

		switch (valign)
		{
		case iVerticalAlignment::Top:
			textPosY += rect._height - 2 - static_cast<int32>(_fontSize);
			break;

		case iVerticalAlignment::Bottom:
			textPosY += 2;
			break;

		case iVerticalAlignment::Center:
			textPosY += static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f);
			break;
		};

		drawStringInt(textPosX, textPosY, modText);

		iRenderer::getInstance().enableStencilTest(false);
			
		DRAW_DEBUG_OUTPUT(rect, state);
	}

	void iWidgetDefaultTheme::drawNumberChooser(const iRectanglei & rect, const iaString & text, iWidgetAppearanceState button_up_state, iWidgetAppearanceState button_down_state, bool active)
	{
		drawNumberChooserFrame(rect, button_up_state, button_down_state, active);
		iRectanglei textRect(rect._x + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f),
			rect._y + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f), 0, 0);
		drawText(textRect, text, 0);

		DRAW_DEBUG_OUTPUT(rect, button_up_state);
	}

	void iWidgetDefaultTheme::drawNumberChooserFrame(const iRectanglei & rect, iWidgetAppearanceState state_button_up, iWidgetAppearanceState state_button_down, bool active)
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

		drawButton(iRectanglei(rect._x + rect._width - rect._height - 1, rect._y + 1, rect._height, rect._height / 2 - 1), "+", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, state_button_up, active);
		drawButton(iRectanglei(rect._x + rect._width - rect._height - 1, rect._y + rect._height / 2, rect._height, rect._height / 2 - 1), "-", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, state_button_down, active);
	}

	void iWidgetDefaultTheme::drawSelectBox(const iRectanglei & rect, const iaString & text, iWidgetAppearanceState buttonAppearance, bool active)
	{
		drawSelectBoxFrame(rect, buttonAppearance, active);
		iRectanglei textRect(rect._x + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f),
			rect._y + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f) + 1, 0, 0);
		drawText(textRect, text, 0);

		DRAW_DEBUG_OUTPUT(rect, buttonAppearance);
	}

	void iWidgetDefaultTheme::drawSelectBoxFrame(const iRectanglei & rect, iWidgetAppearanceState buttonState, bool active)
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

		iRectanglei buttonRect(rect._x + rect._width - rect._height, rect._y + 1, rect._height - 1, rect._height - 2);
		drawButton(buttonRect, "V", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, buttonState, active);
	}

	void iWidgetDefaultTheme::drawSelectBoxDropDown(const iRectanglei & rect, vector<iaString> & text, int highlightIndex, bool active)
	{
		iRenderer::getInstance().setMaterial(_defaultMaterial);

		iRenderer::getInstance().setColor(COLOR_DIFFUSE_LIGHT);


		drawRectangleInt(iRectanglei(rect._x, rect._y + rect._height - 1, rect._width - rect._height, rect._height * static_cast<int32>(text.size())));

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

	iTextureFont* iWidgetDefaultTheme::getFont()
	{
		return _font;
	}

	void iWidgetDefaultTheme::setFontMetrics(const float32 fontSize, const float32 lineHeight)
	{
		_fontSize = fontSize;
		_fontLineHeight = lineHeight;
	}

	void iWidgetDefaultTheme::drawCheckBoxFrame(const iRectanglei & rect, iWidgetAppearanceState state, bool active)
	{
		iRenderer::getInstance().setMaterial(_defaultMaterial);

		if (state != iWidgetAppearanceState::Standby)
		{
			iRenderer::getInstance().setColor(COLOR_SPECULAR);
			drawRectangleInt(rect);
		}
	}

	void iWidgetDefaultTheme::drawCheckBox(const iRectanglei & rect, iWidgetAppearanceState state, bool active, bool checked)
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

	void iWidgetDefaultTheme::drawCheckBox(const iRectanglei & rect, const iaString & text, bool checked, iWidgetAppearanceState state, bool active)
	{
		drawCheckBoxFrame(rect, state, active);
		drawCheckBox(iRectanglei(rect._x + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f),
			rect._y + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f), static_cast<int32>(_fontSize), static_cast<int32>(_fontSize)), state, active, checked);
		drawText(iRectanglei(rect._x + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f) + static_cast<int32>(_fontSize) * 2,
			rect._y + static_cast<int32>((static_cast<float32>(rect._height) - _fontSize) * 0.5f), 0, 0), text, 0);

		DRAW_DEBUG_OUTPUT(rect, state);
	}

	void iWidgetDefaultTheme::drawLabel(const iRectanglei & rect, const iaString & text, int32 textwidth, iWidgetAppearanceState state, bool active)
	{
		drawText(rect, text, textwidth);

		DRAW_DEBUG_OUTPUT(rect, state);
	}

	void iWidgetDefaultTheme::drawText(const iRectanglei & rect, const iaString & text, int32 textwidth)
	{
		iRenderer::getInstance().setMaterial(_texturedMaterial);

		iRenderer::getInstance().setColor(COLOR_AMBIENT);
		iRenderer::getInstance().setFont(_font);
		iRenderer::getInstance().setFontSize(_fontSize);
		iRenderer::getInstance().setFontLineHeight(_fontLineHeight);
		drawStringInt(rect._x, rect._y, text, 0.0f, textwidth);

		DRAW_DEBUG_OUTPUT(rect, iWidgetAppearanceState::Pressed);
	}

	void iWidgetDefaultTheme::drawGraphGridlines(const iRectanglei & rect, float32 lineWidth, const vector<iaVector2f> & verticalLines, const vector<iaVector2f> & horizontalLines, bool active)
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

	void iWidgetDefaultTheme::drawGraphLabels(const iRectanglei & rect, const vector<iaVector2f> & verticalLines, const vector<iaVector2f> & horizontalLines, bool active)
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
			value = iaString::ftoa(verticalLines[i]._y, 2);

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
			value = iaString::ftoa(horizontalLine._y, 2);

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

	void iWidgetDefaultTheme::drawGraph(const iRectanglei & rect, const iaColor4f & lineColor, const iaColor4f & pointColor, float32 lineWidth, float32 pointSize, const vector<iaVector2f> & points)
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


		DRAW_DEBUG_OUTPUT(rect, iWidgetAppearanceState::Pressed);
	}

	void iWidgetDefaultTheme::drawGroupBox(const iRectanglei & rect, bool headerOnly, const iaString & text, iWidgetAppearanceState state, bool active)
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

		drawText(iRectanglei(static_cast<int32>(rect._x + _fontSize * 1.5f), static_cast<int32>(rect._y - _fontSize * 0.5f)), text, 0);

		DRAW_DEBUG_OUTPUT(rect, state);
	}

	void iWidgetDefaultTheme::drawTiledRectangle(const iRectanglei & rect, shared_ptr<iTexture> texture)
	{
		iRenderer::getInstance().setMaterial(_texturedMaterial);
		iRenderer::getInstance().setColor(COLOR_WHITE);
		iRenderer::getInstance().drawTextureTiled(static_cast<float32>(rect._x), static_cast<float32>(rect._y), static_cast<float32>(rect._width), static_cast<float32>(rect._height), texture);
	}

	void iWidgetDefaultTheme::drawPicture(const iRectanglei & rect, shared_ptr<iTexture> texture, iWidgetAppearanceState state, bool active)
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

	void iWidgetDefaultTheme::drawButtonText(const iRectanglei & rect, const iaString & text)
	{
		iRenderer::getInstance().setMaterial(_texturedMaterial);

		iRenderer::getInstance().setColor(COLOR_TEXT);
		iRenderer::getInstance().setFont(_font);
		iRenderer::getInstance().setFontSize(_fontSize);
		iRenderer::getInstance().setFontLineHeight(_fontLineHeight);
		drawStringInt(rect._x, rect._y, text);
	}

	void iWidgetDefaultTheme::drawButtonFrame(const iRectanglei & rect, iWidgetAppearanceState state, bool active)
	{
		iRenderer::getInstance().setMaterial(_defaultMaterial);

		iaColor4f diffuse = COLOR_DIFFUSE;
		if (!active)
		{
			diffuse = COLOR_DIFFUSE_DARK;
		}

		switch (state)
		{
		case iWidgetAppearanceState::Pressed:
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

		case iWidgetAppearanceState::Highlighted:
		case iWidgetAppearanceState::Clicked:
		case iWidgetAppearanceState::DoubleClicked:
			diffuse = COLOR_DIFFUSE_LIGHT;

		case iWidgetAppearanceState::Standby:
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

	void iWidgetDefaultTheme::drawButtonFrame(const iRectanglei & rect, const iaColor4f & color, iWidgetAppearanceState state, bool active)
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
		case iWidgetAppearanceState::Pressed:
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

		case iWidgetAppearanceState::Highlighted:
		case iWidgetAppearanceState::Clicked:
		case iWidgetAppearanceState::DoubleClicked:
		case iWidgetAppearanceState::Standby:
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

	void iWidgetDefaultTheme::drawFrame(const iRectanglei & rect, iWidgetAppearanceState state, bool active)
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

	void iWidgetDefaultTheme::drawSpacer(const iRectanglei & rect, iWidgetAppearanceState state, bool active)
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

	void iWidgetDefaultTheme::drawDialog(const iRectanglei & rect, iWidgetAppearanceState state, bool active)
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
}