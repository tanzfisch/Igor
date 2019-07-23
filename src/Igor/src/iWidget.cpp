// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidget.h>

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

	uint64 iWidget::_nextID = iWidget::INVALID_WIDGET_ID + 1;
	iWidget* iWidget::_keyboardFocus = nullptr;

	iWidget::iWidget()
	{
		_id = _nextID++;
	}

	iWidget::~iWidget()
	{
		if (hasKeyboardFocus())
		{
			_keyboardFocus = nullptr;
		}

		destroyTooltipTimer();
	}

	void iWidget::setTooltip(const iaString& text)
	{
		_tooltip = text;
	}

	iaString iWidget::getTooltip() const
	{
		return _tooltip;
	}

	iWidget* iWidget::getKeyboardFocusWidget()
	{
		return _keyboardFocus;
	}

	iaString iWidget::getInfo() const
	{
		iaString result;

		iaString type(typeid(*this).name());
		type = type.getSubString(type.findLastOf(':') + 1, type.getSize() - 1);

		result = type;
		result += " [";
		result += iaString::itoa(_id);
		result += "] (";
		result += iaString::itoa(_absoluteX);
		result += ", ";
		result += iaString::itoa(_absoluteY);
		result += ", ";
		result += iaString::itoa(_actualWidth);
		result += ", ";
		result += iaString::itoa(_actualHeight);
		result += ")";
		if (hasParent())
		{
			result += ", parent";
		}

		return result;
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
			}
			else
			{
				con_err("widget " << widget->getID() << " already added");
			}
		}
	}

	void iWidget::removeWidget(iWidget * widget)
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

	void iWidget::setParent(iWidget * parent)
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

	void iWidget::draw()
	{
		if (isVisible())
		{
			for (auto widget : _children)
			{
				widget->draw();
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
				std::vector<iWidget*> widgets = _children;
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
				std::vector<iWidget*> widgets = _children;
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
				std::vector<iWidget*> widgets = _children;
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

	void iWidget::registerOnSelectionChangedEvent(iSelectionChangedDelegate delegate)
	{
		_selectionChanged.append(delegate);
	}

	void iWidget::unregisterOnSelectionChangedEvent(iSelectionChangedDelegate delegate)
	{
		_selectionChanged.remove(delegate);
	}

	bool iWidget::handleMouseKeyDown(iKeyCode key)
	{
		if (isActive())
		{
			if (_isMouseOver)
			{
				std::vector<iWidget*> widgets = _children;
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
			std::vector<iWidget*> widgets = _children;

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

	bool iWidget::handleKeyDown(iKeyCode key)
	{
		if (isActive())
		{
			std::vector<iWidget*> widgets = _children;

			for (auto widget : widgets)
			{
				if (widget->handleKeyDown(key))
				{
					return true;
				}
			}
		}
		return false;
	}

	bool iWidget::handleKeyUp(iKeyCode key)
	{
		if (isActive())
		{
			std::vector<iWidget*> widgets = _children;

			for (auto widget : widgets)
			{
				if (widget->handleKeyUp(key))
				{
					return true;
				}
			}
		}
		return false;
	}

	void iWidget::handleMouseMove(const iaVector2i & pos)
	{
		if (isActive())
		{
			std::vector<iWidget*> widgets = _children;

			for (auto widget : widgets)
			{
				widget->handleMouseMove(pos);
			}

			if (pos._x >= _absoluteX &&
				pos._x < _absoluteX + _actualWidth &&
				pos._y >= _absoluteY &&
				pos._y < _absoluteY + _actualHeight)
			{
				if (!_isMouseOver)
				{
					_widgetAppearanceState = iWidgetAppearanceState::Highlighted;
					_mouseOver(this);

					if (!_tooltip.isEmpty())
					{
						if (_timerTooltip == nullptr)
						{
							_timerTooltip = new iTimerHandle(iTimerTickDelegate(this, &iWidget::onToolTipTimer), 1000, true);
						}

						_tooltipPos._x = pos._x + 15;
						_tooltipPos._y = pos._y + 15;

						_timerTooltip->restart();
					}
				}

				_isMouseOver = true;
			}
			else
			{
				if (_isMouseOver)
				{
					_widgetAppearanceState = iWidgetAppearanceState::Standby;
					_mouseOff(this);

					iWidgetManager::getInstance().hideTooltip();
				}

				destroyTooltipTimer();

				_isMouseOver = false;
			}
		}

		_posLast = pos;
	}

	void iWidget::onToolTipTimer()
	{
		iWidgetManager::getInstance().showTooltip(_tooltipPos, _tooltip);

		destroyTooltipTimer();
	}

	void iWidget::destroyTooltipTimer()
	{
		if (_timerTooltip != nullptr)
		{
			delete _timerTooltip;
			_timerTooltip = nullptr;
		}
	}

	iaVector2i iWidget::getLastMousePos() const
	{
		return _posLast;
	}

	iHorizontalAlignment iWidget::getHorizontalAlignment()
	{
		return _horizontalAlignment;
	}

	void iWidget::setHorizontalAlignment(iHorizontalAlignment horizontalAlignment)
	{
		_horizontalAlignment = horizontalAlignment;
	}

	iVerticalAlignment iWidget::getVerticalAlignment()
	{
		return _verticalAlignment;
	}

	void iWidget::setVerticalAlignment(iVerticalAlignment verticalAlignment)
	{
		_verticalAlignment = verticalAlignment;
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

	void iWidget::setSize(int32 width, int32 height)
	{
		_configuredWidth = width;
		_configuredHeight = height;
	}

	void iWidget::setWidth(int32 width)
	{
		_configuredWidth = width;
	}

	void iWidget::setHeight(int32 height)
	{
		_configuredHeight = height;
	}

	void iWidget::setClientArea(int32 left, int32 right, int32 top, int32 bottom)
	{
		_clientAreaLeft = left;
		_clientAreaRight = right;
		_clientAreaTop = top;
		_clientAreaBottom = bottom;
	}

	void iWidget::setGrowingByContent(bool grow)
	{
		_growsByContent = grow;
	}

	bool iWidget::isGrowingByContent() const
	{
		return _growsByContent;
	}

	void iWidget::updateAlignment(int32 clientWidth, int32 clientHeight)
	{
		int32 width = getMinWidth();
		int32 height = getMinHeight();

		switch (iWidget::getHorizontalAlignment())
		{
		case iHorizontalAlignment::Left:
			_relativeX = 0;
			break;

		case iHorizontalAlignment::Strech:
			_relativeX = 0;
			width = clientWidth;
			break;

		case iHorizontalAlignment::Center:
			_relativeX = (clientWidth - width) / 2;
			break;

		case iHorizontalAlignment::Right:
			_relativeX = clientWidth - width;
			break;

		case iHorizontalAlignment::Absolut:
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
			height = clientHeight;
			break;

		case iVerticalAlignment::Center:
			_relativeY = (clientHeight - height) / 2;
			break;

		case iVerticalAlignment::Bottom:
			_relativeY = clientHeight - height;
			break;

		case iVerticalAlignment::Absolut:
			con_err("absolut positioning only supported for dialogs");
			break;

		default:;
		}

		_actualWidth = width;
		_actualHeight = height;
	}

	void iWidget::updatePosition(int32 offsetX, int32 offsetY)
	{
		_absoluteX = _relativeX + offsetX;
		_absoluteY = _relativeY + offsetY;
	}

	void iWidget::calcChildOffsets(std::vector<iRectanglei> & offsets)
	{
		offsets.clear();

		for (int i = 0; i < _children.size(); ++i)
		{
			int32 clientWidth = _actualWidth - _clientAreaLeft - _clientAreaRight;
			int32 clientHeight = _actualHeight - _clientAreaTop - _clientAreaBottom;

			offsets.push_back(iRectanglei(_clientAreaLeft, _clientAreaTop, clientWidth, clientHeight));
		}
	}

	void iWidget::setMinSize(int32 width, int32 height)
	{
		int32 minWidth = width + _clientAreaLeft + _clientAreaRight;
		int32 minHeight = height + _clientAreaTop + _clientAreaBottom;

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