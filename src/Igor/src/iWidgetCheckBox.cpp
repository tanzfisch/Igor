// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetCheckBox.h>

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iTextureFont.h>

namespace Igor
{

	bool iWidgetCheckBox::_generatingRadioButtonGroup = false;
	vector<iWidgetCheckBox*> iWidgetCheckBox::_currentRadioButtons;

	iWidgetCheckBox::iWidgetCheckBox()
		: iWidget(iWidgetType::CheckBox)
	{
		_height = 20;
		_width = 60;

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
        if (!isActive())
        {
            return false;
        }

        iWidget::handleMouseKeyUp(key);

		if (_widgetAppearanceState == iWidgetAppearanceState::Clicked)
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

	void iWidgetCheckBox::setText(const iaString& text)
	{
		_text = text;
		update();
	}

	void iWidgetCheckBox::update()
	{
		float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();
		_height = fontSize*1.5f;
		_width = iWidgetManager::getInstance().getTheme()->getFont()->measureWidth(_text, fontSize) + (_height - fontSize) / 2 + fontSize * 3;

		updateParent();
	}

	const iaString& iWidgetCheckBox::getText() const
	{
		return _text;
	}

	void iWidgetCheckBox::draw()
	{
		if (isVisible())
		{
			iWidgetManager::getInstance().getTheme()->drawCheckBox(_posx, _posy, _width, _height, _text, _checked, _widgetAppearanceState, isActive());
		}
	}

}