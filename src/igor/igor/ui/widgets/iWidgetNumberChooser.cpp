// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetNumberChooser.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/resources/texture/iTextureFont.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

	iWidgetNumberChooser::iWidgetNumberChooser(const iWidgetPtr parent)
		: iWidget(iWidgetType::iWidgetNumberChooser, iWidgetKind::Widget, parent)
	{
	}

	void iWidgetNumberChooser::calcMinSize()
	{
		int32 minWidth = 0;
		int32 minHeight = 0;

		if (isGrowingByContent())
		{
			float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();

			iaString displayString = iaString::toString(_max, _afterPoint);
			displayString += _postFix;

			int32 textWidth = static_cast<int32>(iWidgetManager::getInstance().getTheme()->getFont()->measureWidth(displayString, fontSize));
			minHeight = static_cast<int32>(fontSize * 1.5f);
			minWidth = textWidth + minHeight + static_cast<int32>(fontSize);
		}

		updateMinSize(minWidth, minHeight);
	}

	void iWidgetNumberChooser::updateAlignment(int32 clientWidth, int32 clientHeight)
	{
		iWidget::updateAlignment(clientWidth, clientHeight);

		_buttonUpRectangle.setX(_actualWidth - _actualHeight - 1);
		_buttonUpRectangle.setY(1);
		_buttonUpRectangle.setWidth(_actualHeight);
		_buttonUpRectangle.setHeight(_actualHeight / 2 - 1);

		_buttonDownRectangle.setX(_actualWidth - _actualHeight - 1);
		_buttonDownRectangle.setY(_actualHeight / 2);
		_buttonDownRectangle.setWidth(_actualHeight);
		_buttonDownRectangle.setHeight(_actualHeight / 2 - 1);
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

	bool iWidgetNumberChooser::onMouseKeyDown(iEventMouseKeyDown &event)
	{
		if (!isEnabled())
		{
			return false;
		}

		if (_mouseOverButtonUp)
		{
			_buttonUpAppearanceState = iWidgetState::Pressed;
		}

		if (_mouseOverButtonDown)
		{
			_buttonDownAppearanceState = iWidgetState::Pressed;
		}

		return iWidget::onMouseKeyDown(event);
	}

	void iWidgetNumberChooser::onMouseMove(iEventMouseMove &event)
	{
		if (!isEnabled())
		{
			return;
		}

		iWidget::onMouseMove(event);

		const auto &pos = event.getPosition();
		const int32 mx = pos._x - getActualPosX();
		const int32 my = pos._y - getActualPosY();

		if (mx >= _buttonUpRectangle.getX() &&
			mx < _buttonUpRectangle.getX() + _buttonUpRectangle.getWidth() &&
			my >= _buttonUpRectangle.getY() &&
			my < _buttonUpRectangle.getY() + _buttonUpRectangle.getHeight() &&
			!event.isConsumed())
		{

			_mouseOverButtonUp = true;
			_buttonUpAppearanceState = iWidgetState::Highlighted;
		}
		else
		{
			_mouseOverButtonUp = false;
			_buttonUpAppearanceState = iWidgetState::Standby;
		}

		if (mx >= _buttonDownRectangle.getX() &&
			mx < _buttonDownRectangle.getX() + _buttonDownRectangle.getWidth() &&
			my >= _buttonDownRectangle.getY() &&
			my < _buttonDownRectangle.getY() + _buttonDownRectangle.getHeight() &&
			!event.isConsumed())
		{
			_mouseOverButtonDown = true;
			_buttonDownAppearanceState = iWidgetState::Highlighted;
		}
		else
		{
			_mouseOverButtonDown = false;
			_buttonDownAppearanceState = iWidgetState::Standby;
		}
	}

	bool iWidgetNumberChooser::onMouseKeyUp(iEventMouseKeyUp &event)
	{
		if (!isEnabled())
		{
			return false;
		}

		if (_mouseOverButtonUp)
		{
			if (event.getKey() == iKeyCode::MouseLeft)
			{
				_buttonUpAppearanceState = iWidgetState::Standby;
				increaseNumber(_stepUp);
			}

			setKeyboardFocus();
			return true;
		}

		if (_mouseOverButtonDown)
		{
			if (event.getKey() == iKeyCode::MouseLeft)
			{
				_buttonDownAppearanceState = iWidgetState::Standby;
				decreaseNumber(_stepDown);
			}

			setKeyboardFocus();
			return true;
		}

		return iWidget::onMouseKeyUp(event);
	}

	bool iWidgetNumberChooser::onMouseWheel(iEventMouseWheel &event)
	{
		if (!isEnabled() ||
			!isMouseOver())
		{
			return false;
		}

        if (iWidget::onMouseWheel(event) &&
            !_ignoreChildEventConsumption)
        {
            return true;
        }

		if (event.getWheelDelta() < 0)
		{
			decreaseNumber(_stepDownWheel);
		}
		else
		{
			increaseNumber(_stepUpWheel);
		}

		return true;
	}

	void iWidgetNumberChooser::setPostFix(const iaString &text)
	{
		_postFix = text;
	}

	const iaString &iWidgetNumberChooser::getPostFix() const
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
		if (!isVisible())
		{
			return;
		}

		iaString displayString = iaString::toString(_value, _afterPoint);
		displayString += _postFix;

		iWidgetManager::getInstance().getTheme()->drawNumberChooser(getActualRect(), displayString, _buttonUpAppearanceState, _buttonDownAppearanceState, isEnabled());
	}

} // namespace igor
