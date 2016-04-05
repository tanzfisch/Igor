// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetNumberChooser.h>

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iTextureFont.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <sstream>
using namespace std;

namespace Igor
{

	iWidgetNumberChooser::iWidgetNumberChooser()
		: iWidget(iWidgetType::NumberChooser)
	{
		_height = 20;
		_width = 60;
	}

	void iWidgetNumberChooser::setWidth(int32 width)
	{
		_width = width;
		update();
	}

	void iWidgetNumberChooser::setHeight(int32 height)
	{
		_height = height;
		update();
	}

	void iWidgetNumberChooser::update()
	{
		float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();

		stringstream str;
		str << _max;

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
		}

        _buttonUpRctangle.setX(_width - _height - 1);
        _buttonUpRctangle.setY(1);
        _buttonUpRctangle.setWidth(_height);
        _buttonUpRctangle.setHeight(_height / 2 - 1);

        _buttonDownRctangle.setX(_width - _height - 1);
        _buttonDownRctangle.setY(_height / 2);
        _buttonDownRctangle.setWidth(_height);
        _buttonDownRctangle.setHeight(_height / 2 - 1);
		
        updateParent();
	}

    void iWidgetNumberChooser::cullBoundings()
    {
        if (_value > _max)
        {
            _value = _max;
        }

        if (_value < _min)
        {
            _value = _min;
        }
    }

	void iWidgetNumberChooser::increaseNumber(float32 value)
	{
		_value += value;
        cullBoundings();
		_change(this);
	}

	void iWidgetNumberChooser::decreaseNumber(float32 value)
	{
		_value -= value;
        cullBoundings();
		_change(this);
	}

	bool iWidgetNumberChooser::handleMouseKeyDown(iKeyCode key)
	{
		if (!isActive())
		{
			return false;
		}

		if (_mouseOverButtonUp)
		{
            _buttonUpAppearanceState = iWidgetAppearanceState::Pressed;
		}

		if (_mouseOverButtonDown)
		{
            _buttonDownAppearanceState = iWidgetAppearanceState::Pressed;
		}

		return iWidget::handleMouseKeyDown(key);
	}

	void iWidgetNumberChooser::handleMouseMove(int32 x, int32 y)
	{
		if (!isActive())
		{
			return;
		}

        iWidget::handleMouseMove(x, y);

		int32 mx = x - _posx;
		int32 my = y - _posy;

		if (mx >= _buttonUpRctangle.getX() && 
            mx < _buttonUpRctangle.getX() + _buttonUpRctangle.getWidth() &&
			my >= _buttonUpRctangle.getY() && 
            my < _buttonUpRctangle.getY() + _buttonUpRctangle.getHeight())
		{
			
            _mouseOverButtonUp = true;
            _buttonUpAppearanceState = iWidgetAppearanceState::Highlighted;
		}
		else
		{
            _mouseOverButtonUp = false;
            _buttonUpAppearanceState = iWidgetAppearanceState::Standby;
		}

        if (mx >= _buttonDownRctangle.getX() &&
            mx < _buttonDownRctangle.getX() + _buttonDownRctangle.getWidth() &&
            my >= _buttonDownRctangle.getY() &&
            my < _buttonDownRctangle.getY() + _buttonDownRctangle.getHeight())
		{
            _mouseOverButtonDown = true;
            _buttonDownAppearanceState = iWidgetAppearanceState::Highlighted;
		}
		else
		{
            _mouseOverButtonDown = false;
            _buttonDownAppearanceState = iWidgetAppearanceState::Standby;
		}
	}

	bool iWidgetNumberChooser::handleMouseKeyUp(iKeyCode key)
	{
		if (!isActive())
		{
			return false;
		}

		if (_mouseOverButtonUp)
		{
            if (key == iKeyCode::MouseLeft)
            {
                _buttonUpAppearanceState = iWidgetAppearanceState::Standby;
                increaseNumber(_stepUp);
            }

            setKeyboardFocus();
            return true;
		}

		if (_mouseOverButtonDown)
		{
            if (key == iKeyCode::MouseLeft)
            {
                _buttonDownAppearanceState = iWidgetAppearanceState::Standby;
                decreaseNumber(_stepDown);
            }

			setKeyboardFocus();
            return true;
		}

        return iWidget::handleMouseKeyUp(key);
	}

	bool iWidgetNumberChooser::handleMouseWheel(int32 d)
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
				decreaseNumber(_stepDownWheel);
			}
			else
			{
				increaseNumber(_stepUpWheel);
			}

            return true;
		}

        return false;
	}

	void iWidgetNumberChooser::setPostFix(const iaString& text)
	{
		_postFix = text;
		update();
	}

    const iaString& iWidgetNumberChooser::getPostFix() const
    {
        return _postFix;
    }

	float32 iWidgetNumberChooser::getValue()
	{
		return _value;
	}

	void iWidgetNumberChooser::setValue(float32 value)
	{
        con_assert(value <= _max, "value out of range");
        con_assert(value >= _min, "value out of range");

        _value = value;
	}

	void iWidgetNumberChooser::setMinMaxNumber(float32 min, float32 max)
	{
		_min = min;
		_max = max;
        cullBoundings();
		update();
	}

    void iWidgetNumberChooser::setAfterPoint(int32 afterPoint)
    {
        _afterPoint = afterPoint;
    }

    int32 iWidgetNumberChooser::getAfterPoint() const
    {
        return _afterPoint;
    }

	void iWidgetNumberChooser::setStepping(float32 up, float32 down)
	{
		_stepUp = up;
		_stepDown = down;
	}

	void iWidgetNumberChooser::setSteppingWheel(float32 up, float32 down)
	{
		_stepUpWheel = up;
		_stepDownWheel = down;
	}

	void iWidgetNumberChooser::draw()
	{
		if (isVisible())
		{
            iaString displayString = iaString::ftoa(_value, _afterPoint);
			displayString += _postFix;

			iWidgetManager::getInstance().getTheme()->drawNumberChooser(_posx, _posy, _width, _height, displayString, _buttonUpAppearanceState, _buttonDownAppearanceState, isActive());
		}
	}

}
