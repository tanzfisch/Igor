// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetLabel.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iTextureFont.h>

namespace Igor
{

	iWidgetLabel::iWidgetLabel()
		: iWidget(iWidgetType::Label)
	{
		setWidth(0);
		setHeight(20);
		setHorrizontalAlignment(iHorrizontalAlignment::Center);
		setVerticalAlignment(iVerticalAlignment::Center);
	}

	void iWidgetLabel::update()
	{
		int32 width = _configuredWidth;
		int32 height = _configuredHeight;

		if (isGrowingByContent())
		{
			float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();

			if (_maxTextWidth == 0)
			{
				int32 textWidth = (int32)iWidgetManager::getInstance().getTheme()->getFont()->measureWidth(_text, fontSize);
				if (textWidth > width)
				{
					width = textWidth;
				}

				if (fontSize > height)
				{
					height = fontSize;
				}
			}
			else
			{
				int32 textHeight = (int32)iWidgetManager::getInstance().getTheme()->getFont()->measureHeight(_text, fontSize, (float32)_maxTextWidth);
				if (textHeight > height)
				{
					height = textHeight;
				}

				if (_maxTextWidth > width)
				{
					width = _maxTextWidth;
				}
			}
		}

		iWidget::update(width, height);
	}

	void iWidgetLabel::draw(int32 parentPosX, int32 parentPosY)
	{
		updatePosition(parentPosX, parentPosY);

		if (isVisible())
		{
			iWidgetManager::getInstance().getTheme()->drawText(getActualPosX(), getActualPosY(), _text, _maxTextWidth);
		}
	}

	const iaString& iWidgetLabel::getText() const
	{
		return _text;
	}

	void iWidgetLabel::setMaxTextWidth(int32 width)
	{
		_maxTextWidth = width;
		update();
	}

	int32 iWidgetLabel::getMaxTextWidth()
	{
		return _maxTextWidth;
	}

	void iWidgetLabel::setText(const iaString& text)
	{
		_text = text;
		update();
	}

}
