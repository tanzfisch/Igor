// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetGroupBox.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/ui/user_controls/iUserControl.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

	iWidgetGroupBox::iWidgetGroupBox(const iWidgetPtr parent)
		: iWidget(iWidgetType::iWidgetGroupBox, iWidgetKind::Widget, parent)
	{
		_configuredMinWidth = 60;
		_configuredMinHeight = 20;
	}

	void iWidgetGroupBox::setBorder(int32 border)
	{
		_border = border;
	}

	int32 iWidgetGroupBox::getBorder()
	{
		return _border;
	}

	void iWidgetGroupBox::setHeaderOnly(bool headerOnly)
	{
		_headerOnly = headerOnly;
	}

	bool iWidgetGroupBox::getHeaderOnly() const
	{
		return _headerOnly;
	}

	void iWidgetGroupBox::calcMinSize()
	{
		con_assert(iWidgetManager::getInstance().getTheme() != nullptr, "no theme");

		int32 minWidth = 0;
		int32 minHeight = 0;

		if (isGrowingByContent() &&
			!_children.empty())
		{
			iWidgetPtr widget = _children.front();
			minHeight = std::max(minHeight, widget->getMinHeight());
			minWidth = std::max(minWidth, widget->getMinWidth());

			if (!_text.isEmpty())
			{
				const float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();
				iTextureFontPtr font = iWidgetManager::getInstance().getTheme()->getFont();
				minWidth = std::max(minWidth, static_cast<int32>(font->measureWidth(_text, fontSize) + _border * 2.0f + fontSize * 2.0f));

				if (_headerOnly)
				{
					setClientArea(0, 0, static_cast<int32>(static_cast<float32>(_border) + fontSize * 0.75f), 0);
				}
				else
				{
					setClientArea(_border, _border, static_cast<int32>(static_cast<float32>(_border) + fontSize * 0.75), _border);
				}
			}
			else
			{
				if (_headerOnly)
				{
					setClientArea(0, 0, _border, 0);
				}
				else
				{
					setClientArea(_border, _border, _border, _border);
				}
			}
		}
		else
		{
			// TODO copy paste code
			if (_headerOnly)
			{
				setClientArea(0, 0, _border, 0);
			}
			else
			{
				setClientArea(_border, _border, _border, _border);
			}
		}

		updateMinSize(minWidth, minHeight);
	}

	void iWidgetGroupBox::setText(const iaString &text)
	{
		_text = text;
	}

	const iaString &iWidgetGroupBox::getText() const
	{
		return _text;
	}

	void iWidgetGroupBox::draw()
	{
		if (!isVisible())
		{
			return;
		}

		iWidgetManager::getInstance().getTheme()->drawGroupBox(getActualRect(), _headerOnly, _text, getState(), isEnabled());

		for (const auto child : _children)
		{
			child->draw();
		}
	}

} // namespace igor