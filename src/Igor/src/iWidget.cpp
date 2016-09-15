// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidget.h>

#include <iWidgetButton.h>
#include <iWidgetManager.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

	uint64 iWidget::_nextID = iWidget::INVALID_WIDGET_ID + 1;
	iWidget* iWidget::_keyboardFocus = nullptr;

	iWidget::iWidget(iWidgetType widgetType)
		: _type(widgetType)
	{
		_id = _nextID++;
	}

	iWidget::~iWidget()
	{
		if (hasKeyboardFocus())
		{
			_keyboardFocus = nullptr;
		}
    }
    
	void iWidget::addWidget(iWidget* widget)
	{
		con_assert(widget != nullptr, "zero pointer");
		con_assert(widget != this, "incest is not supported");

		if (widget != nullptr &&
			widget != this)
		{
			auto iter = find(_children.begin(), _children.end(), widget);

			if (iter == _children.end())
			{
				_children.push_back(widget);
				widget->_parent = this;
				//widget->update();
			}
			else
			{
				con_err("widget " << widget->getID() << " already added");
			}
		}
	}

	void iWidget::removeWidget(iWidget* widget)
	{
		con_assert(widget != nullptr, "zero pointer");

		if (widget != nullptr)
		{
			auto iter = find(_children.begin(), _children.end(), widget);

			if (iter != _children.end())
			{
				widget->setParent(nullptr);
				widget->setActive(false);
				widget->setVisible(false);
				_children.erase(iter);
				//update();
			}
			else
			{
				con_err("widget " << widget->getID() << " not in list");
			}
		}
	}

	void iWidget::addWidget(uint64 id)
	{
		addWidget(iWidgetManager::getInstance().getWidget(id));
	}

	void iWidget::removeWidget(uint64 id)
	{
		removeWidget(iWidgetManager::getInstance().getWidget(id));
	}

	void iWidget::setAcceptDrop(bool acceptDrop)
	{
		_acceptDrop = acceptDrop;
	}

	bool iWidget::isAcceptingDrop()
	{
		return _acceptDrop;
	}

	void iWidget::setParent(iWidget* parent)
	{
		_parent = parent;
	}

	void iWidget::unregisterOnClickEvent(iClickDelegate iClickDelegate)
	{
		_click.remove(iClickDelegate);
	}

    void iWidget::unregisterOnMouseOffClickEvent(iMouseOffClickDelegate clickDelegate)
    {
        _mouseOffClick.remove(clickDelegate);
    }

	void iWidget::unregisterOnMouseOverEvent(iMouseOverDelegate iMouseOverDelegate)
	{
		_mouseOver.remove(iMouseOverDelegate);
	}

	void iWidget::unregisterOnMouseOffEvent(iMouseOffDelegate iMouseOffDelegate)
	{
		_mouseOff.remove(iMouseOffDelegate);
	}

	void iWidget::unregisterOnChangeEvent(iChangeDelegate iChangeDelegate)
	{
		_change.remove(iChangeDelegate);
	}

	void iWidget::unregisterOnFocusEvent(iFocusDelegate iFocusDelegate)
	{
		_focus.remove(iFocusDelegate);
	}

	void iWidget::registerOnClickEvent(iClickDelegate iClickDelegate)
	{
		_click.append(iClickDelegate);
	}

    void iWidget::registerOnMouseOffClickEvent(iMouseOffClickDelegate clickDelegate)
    {
        _mouseOffClick.append(clickDelegate);
    }

	void iWidget::registerOnMouseOverEvent(iMouseOverDelegate iMouseOverDelegate)
	{
		_mouseOver.append(iMouseOverDelegate);
	}

	void iWidget::registerOnMouseOffEvent(iMouseOffDelegate iMouseOffDelegate)
	{
		_mouseOff.append(iMouseOffDelegate);
	}

	void iWidget::registerOnChangeEvent(iChangeDelegate iChangeDelegate)
	{
		_change.append(iChangeDelegate);
	}

	void iWidget::registerOnFocusEvent(iFocusDelegate iFocusDelegate)
	{
		_focus.append(iFocusDelegate);
	}

	void iWidget::registerOnDoubleClickEvent(iDoubleClickDelegate doubleClickDelegate)
	{
		_doubleClick.append(doubleClickDelegate);
	}

	void iWidget::unregisterOnDoubleClickEvent(iDoubleClickDelegate doubleClickDelegate)
	{
		_doubleClick.remove(doubleClickDelegate);
	}

    void iWidget::registerOnContextMenuEvent(iContextMenuDelegate contextMenuDelegate)
    {
        _contextMenu.append(contextMenuDelegate);
    }
    
    void iWidget::unregisterOnContextMenuEvent(iContextMenuDelegate contextMenuDelegate)
    {
        _contextMenu.remove(contextMenuDelegate);
    }

	void iWidget::setKeyboardFocus()
	{
		if (_keyboardFocus != this)
		{
			if (_keyboardFocus != nullptr)
			{
				_keyboardFocus->handleLostKeyboardFocus();
			}

			_keyboardFocus = this;
			_keyboardFocus->handleGainedKeyboardFocus();
		}
	}

	void iWidget::resetKeyboardFocus()
	{
		if (_keyboardFocus != nullptr)
		{
			_keyboardFocus->handleLostKeyboardFocus();
		}

		_keyboardFocus = nullptr;
	}

	void iWidget::draw(int32 parentPosX, int32 parentPosY)
	{
		if (isVisible())
		{
			for (auto widget : _children)
			{
				widget->draw(getActualPosX(), getActualPosY());
			}
		}
	}

	bool iWidget::isMouseOver()
	{
		return _isMouseOver;
	}

	void iWidget::registerOnWheelUpEvent(iWheelUpDelegate wheelUpDelegate)
	{
		_wheelUp.append(wheelUpDelegate);
	}

	void iWidget::unregisterOnWheelUpEvent(iWheelUpDelegate wheelUpDelegate)
	{
		_wheelUp.remove(wheelUpDelegate);
	}

	void iWidget::registerOnWheelDownEvent(iWheelDownDelegate wheelDownDelegate)
	{
		_wheelDown.append(wheelDownDelegate);
	}

	void iWidget::unregisterOnWheelDownEvent(iWheelDownDelegate wheelDownDelegate)
	{
		_wheelDown.remove(wheelDownDelegate);
	}

	void iWidget::handleLostKeyboardFocus()
	{
		// implement in derived class if needed
	}

	void iWidget::handleGainedKeyboardFocus()
	{
		// implement in derived class if needed
	}

	bool iWidget::handleMouseWheel(int32 d)
	{
		if (isActive() && _reactOnMouseWheel)
		{
			if (_isMouseOver)
			{
				vector<iWidget*> widgets = _children;
				bool result = false;

				for (auto widget : widgets)
				{
					if (widget->handleMouseWheel(d))
					{
						result = true;
					}
				}

				if (result)
				{
					return true;
				}
				else
				{
                    bool handeled = false;
					if (d > 0)
					{
                        _wheelUp(this);
					}
					else
					{
                        _wheelDown(this);
					}
                    
					return handeled;
				}
			}
		}
		return false;
	}

	bool iWidget::handleMouseDoubleClick(iKeyCode key)
	{
		if (isActive())
		{
			if (_isMouseOver)
			{
				vector<iWidget*> widgets = _children;
				bool result = false;

				for (auto widget : widgets)
				{
					if (widget->handleMouseDoubleClick(key))
					{
						result = true;
					}
				}

				if (result)
				{
					return true;
				}
				else
				{
					if (key == iKeyCode::MouseLeft)
					{
						_widgetAppearanceState = iWidgetAppearanceState::DoubleClicked;
						setKeyboardFocus();
						_doubleClick(this);
						return true;
					}
				}
			}
		}
		return false;
	}

	bool iWidget::handleMouseKeyUp(iKeyCode key)
	{
		if (isActive())
		{
			if (_isMouseOver)
			{
				vector<iWidget*> widgets = _children;
				bool result = false;

				for (auto widget : widgets)
				{
					if (widget->handleMouseKeyUp(key))
					{
						result = true;
					}
				}

				if (result)
				{
					return true;
				}
				else
				{
					if (key == iKeyCode::MouseLeft ||
                        key == iKeyCode::MouseRight)
					{
						if (_widgetAppearanceState == iWidgetAppearanceState::Pressed)
						{
							_widgetAppearanceState = iWidgetAppearanceState::Clicked;
							setKeyboardFocus();

							_click(this);

                            if (key == iKeyCode::MouseRight)
                            {
                                _contextMenu(this);
                            }

							return true;
						}
					}
				}
			}
            else if (_acceptOutOfBoundsClicks)
            {
                _mouseOffClick(this);
            }
		}
		return false;
	}

    void iWidget::setAcceptOutOfBoundsClicks(bool acceptOutOfBoundsClick)
    {
        _acceptOutOfBoundsClicks = acceptOutOfBoundsClick;
    }

    bool iWidget::getAcceptOutOfBoundsClicks() const
    {
        return _acceptOutOfBoundsClicks;
    }

	bool iWidget::handleMouseKeyDown(iKeyCode key)
	{
		if (isActive())
		{
			if (_isMouseOver)
			{
				vector<iWidget*> widgets = _children;
				bool result = false;

				for (auto widget : widgets)
				{
					if (widget->handleMouseKeyDown(key))
					{
						result = true;
					}
				}

				if (result)
				{
					return true;
				}
				else
				{
					if (key == iKeyCode::MouseLeft ||
                        key == iKeyCode::MouseRight)
					{
						_widgetAppearanceState = iWidgetAppearanceState::Pressed;
						return true;
					}
				}
			}
		}
		return false;
	}

	bool iWidget::handleASCII(uint8 c)
	{
		if (isActive())
		{
			vector<iWidget*> widgets = _children;

			for (auto widget : widgets)
			{
				if (widget->handleASCII(c))
				{
					return true;
				}
			}
		}
		return false;
	}

	void iWidget::handleMouseMove(int32 x, int32 y)
	{
		if (isActive())
		{
			vector<iWidget*> widgets = _children;

			for (auto widget : widgets)
			{
				widget->handleMouseMove(x, y);
			}

			if (x >= _absoluteX &&
				x < _absoluteX + _actualWidth &&
				y >= _absoluteY &&
				y < _absoluteY + _actualHeight)
			{
				if (!_isMouseOver)
				{
					_widgetAppearanceState = iWidgetAppearanceState::Highlighted;
					_mouseOver(this);
				}

				_isMouseOver = true;
			}
			else
			{
				if (_isMouseOver)
				{
					_widgetAppearanceState = iWidgetAppearanceState::Standby;
					_mouseOff(this);
				}

				_isMouseOver = false;
			}
		}
	}

	iHorrizontalAlignment iWidget::getHorrizontalAlignment()
	{
		return _horrizontalAlignment;
	}

	void iWidget::setHorrizontalAlignment(iHorrizontalAlignment horrizontalAlignment)
	{
		_horrizontalAlignment = horrizontalAlignment;
		//update();
	}

	iVerticalAlignment iWidget::getVerticalAlignment()
	{
		return _verticalAlignment;
	}

	void iWidget::setVerticalAlignment(iVerticalAlignment verticalAlignment)
	{
		_verticalAlignment = verticalAlignment;
		//update();
	}

	void iWidget::setVisible(bool visible)
	{
		_visible = visible;

		if (!isVisible())
		{
			_widgetAppearanceState = iWidgetAppearanceState::Standby;
		}
	}

	void iWidget::setActive(bool active)
	{
		_active = active;

		if (!isActive())
		{
			_widgetAppearanceState = iWidgetAppearanceState::Standby;
            _isMouseOver = false;
		}
	}

	void iWidget::setWidth(int32 width)
	{
		_configuredWidth = width;
		//update();
	}

	void iWidget::setHeight(int32 height)
	{
		_configuredHeight = height;
		//update();
	}

    void iWidget::setMargin(int32 marginLeft, int32 marginRight, int32 marginTop, int32 marginBottom)
    {
        _marginLeft = marginLeft;
        _marginRight = marginRight;
        _marginTop = marginTop;
        _marginBottom = marginBottom;
    }

	void iWidget::setGrowingByContent(bool grow)
	{
		_growsByContent = grow;
		//update();
	}

	bool iWidget::isGrowingByContent() const
	{
		return _growsByContent;
	}

	void iWidget::updateAlignment()
	{
		if (hasParent())
		{
            int32 width = _minWidth;
            int32 height = _minHeight;

			switch (iWidget::getHorrizontalAlignment())
			{
			case iHorrizontalAlignment::Left:
				_relativeX = 0;
				break;

			case iHorrizontalAlignment::Strech:
				_relativeX = 0;
				if (_parent->getActualWidth() > width)
				{
                    width = _parent->getActualWidth();
				}
				break;

			case iHorrizontalAlignment::Center:
				_relativeX = (_parent->getActualWidth() - width) / 2;
				break;

			case iHorrizontalAlignment::Right:
				_relativeX = _parent->getActualWidth() - width;
				break;

			case iHorrizontalAlignment::Absolut:
				con_err("absolut positioning only supported for dialogs");
				break;

			default:;
			};

			switch (iWidget::getVerticalAlignment())
			{
			case iVerticalAlignment::Top:
				_relativeY = 0;
				break;

			case iVerticalAlignment::Strech:
				_relativeY = 0;
				if (_parent->getActualHeight() > height)
				{
                    height = _parent->getActualHeight();
				}
				break;

			case iVerticalAlignment::Center:
				_relativeY = (_parent->getActualHeight() - height) / 2;
				break;

			case iVerticalAlignment::Bottom:
				_relativeY = _parent->getActualHeight() - height;
				break;

			case iVerticalAlignment::Absolut:
				con_err("absolut positioning only supported for dialogs");
				break;

			default:;
			}

            _actualWidth = width - _parent->_marginLeft - _parent->_marginRight;
            _actualHeight = height - _parent->_marginTop - _parent->_marginBottom;
		}
	}

	void iWidget::updatePosition(int32 offsetX, int32 offsetY)
	{
		_absoluteX = _relativeX + offsetX;
		_absoluteY = _relativeY + offsetY;
	}

	void iWidget::setMinSize(int32 width, int32 height)
	{
        int32 minWidth = width + _marginLeft + _marginRight;
        int32 minHeight = height + _marginTop + _marginBottom;

        if (minWidth < _configuredWidth)
        {
            minWidth = _configuredWidth;
        }

        if (minHeight < _configuredHeight)
        {
            minHeight = _configuredHeight;
        }

		_minWidth = minWidth;
		_minHeight = minHeight;
	}

	bool iWidget::hasParent() const
	{
		return _parent != nullptr ? true : false;
	}
}