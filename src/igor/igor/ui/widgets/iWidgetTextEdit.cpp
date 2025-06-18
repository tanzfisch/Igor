// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetTextEdit.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/resources/texture/iTextureFont.h>

namespace igor
{

	iWidgetTextEdit::iWidgetTextEdit(const iWidgetPtr parent)
		: iWidget(iWidgetType::iWidgetTextEdit, iWidgetKind::Widget, parent)
	{
		_reactOnMouseWheel = false;
		_configuredMinWidth = 20;
		_configuredMinHeight = 20;
	}

	void iWidgetTextEdit::calcMinSize()
	{
		updateMinSize(0, iWidgetManager::getInstance().getTheme()->getFontSize() * 1.2f);
	}

	void iWidgetTextEdit::draw()
	{
		if (!isVisible())
		{
			return;
		}

		iWidgetManager::getInstance().getTheme()->draw(this);
	}

	const iaString &iWidgetTextEdit::getText() const
	{
		return _text;
	}

	void iWidgetTextEdit::setText(const iaString &text)
	{
		_text = text;
	}

} // namespace igor
