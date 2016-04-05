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
		setHorrizontalAlignment(iHorrizontalAlignment::Center);
		setVerticalAlignment(iVerticalAlignment::Center);
	}

	void iWidgetLabel::update()
	{
		calcDimensions();
		updateParent();
	}

	void iWidgetLabel::calcDimensions()
	{
		float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();

		if (_maxTextWidth == 0)
		{
			_width = (int32)iWidgetManager::getInstance().getTheme()->getFont()->measureWidth(_text, fontSize);
			_height = fontSize;
		}
		else
		{
			_height = (int32)iWidgetManager::getInstance().getTheme()->getFont()->measureHeight(_text, fontSize, (float32)_maxTextWidth);
			_width = _maxTextWidth;
		}
	}

	void iWidgetLabel::draw()
	{
		if (isVisible())
		{
			iWidgetManager::getInstance().getTheme()->drawText(_posx, _posy, _text, _maxTextWidth);
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

	void iWidgetLabel::setText(const iaString& _text)
	{
		this->_text = _text;
		update();
	}

}
