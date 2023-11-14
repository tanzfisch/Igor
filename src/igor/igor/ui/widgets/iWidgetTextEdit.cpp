// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
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
	}

	void iWidgetTextEdit::calcMinSize()
	{
		updateMinSize(0, 0);
	}

	void iWidgetTextEdit::draw()
	{
		if (isVisible())
		{
	 		iWidgetManager::getInstance().getTheme()->drawTextEdit(getActualRect(), _text, 0.0, _widgetState, isEnabled());
		}
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
