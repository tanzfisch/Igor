// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetCheckBox.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>

namespace igor
{

	bool iWidgetCheckBox::_generatingRadioButtonGroup = false;
	std::vector<iWidgetCheckBox *> iWidgetCheckBox::_currentRadioButtons;

	iWidgetCheckBox::iWidgetCheckBox(const iWidgetPtr parent)
		: iWidget(iWidgetType::iWidgetCheckBox, iWidgetKind::Widget, parent)
	{
		_configuredMinHeight = 20;
		_configuredMinWidth = 20;
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

	bool iWidgetCheckBox::onMouseKeyUp(const iEventMouseKeyUp &event)
	{
		if (!isEnabled())
		{
			return false;
		}

		iWidget::onMouseKeyUp(event);

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

	void iWidgetCheckBox::calcMinSize()
	{
		updateMinSize(0, 0);
	}

	void iWidgetCheckBox::draw()
	{
		if (!isVisible())
		{
			return;
		}

		iWidgetManager::getInstance().getTheme()->draw(this);
	}

} // namespace igor