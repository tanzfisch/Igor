// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetCheckBox.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/resources/texture/iTextureFont.h>

namespace igor
{

	bool iWidgetCheckBox::_generatingRadioButtonGroup = false;
	std::vector<iWidgetCheckBox *> iWidgetCheckBox::_currentRadioButtons;

	iWidgetCheckBox::iWidgetCheckBox(const iWidgetPtr parent)
		: iWidget(iWidgetType::iWidgetCheckBox, iWidgetKind::Widget, parent)
	{
		_configuredHeight = 20;
		_configuredWidth = 60;
		_reactOnMouseWheel = false;

		if (_generatingRadioButtonGroup)
		{
			for (int i = 0; i < _currentRadioButtons.size(); ++i)
			{
				_currentRadioButtons[i]->_radioButtons.push_back(this);
				_radioButtons.push_back(_currentRadioButtons[i]);
			}

			_currentRadioButtons.push_back(this);
		}
	}

	void iWidgetCheckBox::beginRadioButtonGroup()
	{
		_generatingRadioButtonGroup = true;
	}

	void iWidgetCheckBox::endRadioButtonGroup()
	{
		_generatingRadioButtonGroup = false;
		_currentRadioButtons.clear();
	}

	bool iWidgetCheckBox::handleMouseKeyUp(iKeyCode key)
	{
		if (!isEnabled())
		{
			return false;
		}

		iWidget::handleMouseKeyUp(key);

		if (_widgetState == iWidgetState::Clicked)
		{
			if (_radioButtons.size())
			{
				if (!isChecked())
				{
					_checked = true;
					_change(this);

					for (int i = 0; i < _radioButtons.size(); ++i)
					{
						_radioButtons[i]->setChecked(false);
					}
				}
			}
			else
			{
				_checked = !_checked;
				_change(this);
			}

			return true;
		}

		return false;
	}

	void iWidgetCheckBox::setChecked(bool check)
	{
		_checked = check;
		_change(this);
	}

	bool iWidgetCheckBox::isChecked()
	{
		return _checked;
	}

	void iWidgetCheckBox::setText(const iaString &text)
	{
		_text = text;
	}

	void iWidgetCheckBox::calcMinSize()
	{
		int32 minWidth = 0;
		int32 minHeight = 0;

		if (_growsByContent &&
			!_text.isEmpty())
		{
			float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();
			minHeight = static_cast<int32>(fontSize * 1.5f);

			int32 textWidth = static_cast<int32>(iWidgetManager::getInstance().getTheme()->getFont()->measureWidth(_text, fontSize) + fontSize * 3.0f);
			minWidth = textWidth;
		}

		setMinSize(minWidth, minHeight);
	}

	const iaString &iWidgetCheckBox::getText() const
	{
		return _text;
	}

	void iWidgetCheckBox::draw()
	{
		if (isVisible())
		{
			iWidgetManager::getInstance().getTheme()->drawCheckBox(getActualRect(), _text, _checked, getState(), isEnabled());
		}
	}

} // namespace igor