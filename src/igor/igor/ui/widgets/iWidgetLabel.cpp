// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetLabel.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/resources/texture/iTextureFont.h>

namespace igor
{

	iWidgetLabel::iWidgetLabel(const iWidgetPtr parent)
		: iWidget(iWidgetType::iWidgetLabel, iWidgetKind::Widget, parent)
	{
		_configuredMinHeight = 0;
		_configuredMinWidth = 0;
		_reactOnMouseWheel = false;

		setHorizontalAlignment(iHorizontalAlignment::Center);
		setVerticalAlignment(iVerticalAlignment::Center);
	}

	void iWidgetLabel::calcMinSize()
	{
		int32 minWidth = 0;
		int32 minHeight = 0;

		if (isGrowingByContent())
		{
			float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();

			minWidth = (int32)iWidgetManager::getInstance().getTheme()->getFont()->measureWidth(_text, fontSize);
			minHeight = (int32)iWidgetManager::getInstance().getTheme()->getFont()->measureHeight(_text, fontSize, (float32)_maxTextWidth);
		}

		updateMinSize(minWidth, minHeight);
	}

	void iWidgetLabel::draw()
	{
		if (!isVisible())
		{
			return;
		}

		iWidgetManager::getInstance().getTheme()->drawLabel(getActualRect(), _text, _maxTextWidth, getState(), isEnabled());
	}

	const iaString &iWidgetLabel::getText() const
	{
		return _text;
	}

	void iWidgetLabel::setMaxTextWidth(int32 width)
	{
		_maxTextWidth = width;
	}

	int32 iWidgetLabel::getMaxTextWidth()
	{
		return _maxTextWidth;
	}

	void iWidgetLabel::setText(const iaString &text)
	{
		_text = text;
	}

} // namespace igor
