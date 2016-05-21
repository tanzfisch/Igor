// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetSelectBox.h>

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iTextureFont.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <sstream>
using namespace std;

namespace Igor
{

	iWidgetSelectBox::iWidgetSelectBox()
		: iWidget(iWidgetType::NumberChooser)
	{
		_height = 20;
		_width = 80;
	}

	void iWidgetSelectBox::setWidth(int32 width)
	{
		_width = width;
		update();
	}

	void iWidgetSelectBox::setHeight(int32 height)
	{
		_height = height;
		update();
	}

	void iWidgetSelectBox::update()
	{
/*		float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();

		iaString displayString = str.str().c_str();
		displayString += _postFix;

		int32 textWidth = static_cast<int32>(iWidgetManager::getInstance().getTheme()->getFont()->measureWidth(displayString, fontSize));

		if (_height < fontSize * 1.5f)
		{
			_height = static_cast<int32>(fontSize * 1.5f);
		}

		if (_width < textWidth + _height + fontSize)
		{
			_width = textWidth + _height + static_cast<int32>(fontSize);
		}*/

		_buttonRectangle.setX(_width - _height - 1);
		_buttonRectangle.setY(_height);
		_buttonRectangle.setWidth(_height);
		_buttonRectangle.setHeight(_height - 1);
		
        updateParent();
	}

	bool iWidgetSelectBox::handleMouseKeyDown(iKeyCode key)
	{
		if (!isActive())
		{
			return false;
		}

		if (_mouseOverButton)
		{
            _appearanceState = iWidgetAppearanceState::Pressed;
		}

		return iWidget::handleMouseKeyDown(key);
	}

	void iWidgetSelectBox::handleMouseMove(int32 x, int32 y)
	{
		if (!isActive())
		{
			return;
		}

        iWidget::handleMouseMove(x, y);

		int32 mx = x - _posx;
		int32 my = y - _posy;

		if (mx >= _buttonRectangle.getX() &&
            mx < _buttonRectangle.getX() + _buttonRectangle.getWidth() &&
			my >= _buttonRectangle.getY() &&
            my < _buttonRectangle.getY() + _buttonRectangle.getHeight())
		{
			_mouseOverButton = true;
			_appearanceState = iWidgetAppearanceState::Highlighted;
		}
		else
		{
			_mouseOverButton = false;
			_appearanceState = iWidgetAppearanceState::Standby;
		}
	}

	bool iWidgetSelectBox::handleMouseKeyUp(iKeyCode key)
	{
		if (!isActive())
		{
			return false;
		}

		if (_mouseOverButton)
		{
            if (key == iKeyCode::MouseLeft)
            {
				_appearanceState = iWidgetAppearanceState::Standby;

				// todo open drop down
            }

            setKeyboardFocus();
            return true;
		}

        return iWidget::handleMouseKeyUp(key);
	}

	bool iWidgetSelectBox::handleMouseWheel(int32 d)
	{
		if (!isActive())
		{
			return false;
		}

        iWidget::handleMouseWheel(d);

		if (isMouseOver())
		{
			if (d < 0)
			{
				// TODO go to next lower entry
			}
			else
			{
				// TODO go to next higher entry
			}

            return true;
		}

        return false;
	}

	void iWidgetSelectBox::setSelectedKey(uint32 key)
	{

	}

	void iWidgetSelectBox::setSelectedValue(const iaString& value)
	{

	}

	uint32 iWidgetSelectBox::getSelectedKey() const
	{

	}

	iaString iWidgetSelectBox::getSelectedValue() const
	{

	}

	void iWidgetSelectBox::draw()
	{
		if (isVisible())
		{
            iaString displayString = iaString::ftoa(_value, _afterPoint);
			displayString += _postFix;

			iWidgetManager::getInstance().getTheme()->drawNumberChooser(_posx, _posy, _width, _height, displayString, _buttonUpAppearanceState, _buttonDownAppearanceState, isActive());
		}
	}

}
