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
    iWidget* iWidget::_modal = nullptr;

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

        if (isModal())
        {
            _modal = nullptr;
        }
    }

    void iWidget::setModal()
    {
        con_assert(_modal == nullptr, "an other dialog is alsready modal");

        if (_modal == nullptr)
        {
            con_assert(getType() == iWidgetType::Dialog, "only dialogs can be modal");

            if (getType() == iWidgetType::Dialog)
            {
                _modal = this;
            }
        }
        else
        {
            con_err("an other dialog is alsready modal");
        }
    }

    void iWidget::resetModal()
    {
        con_assert(_modal == this, "can't unset modal because it was not modal before");

        if (_modal == this)
        {
            _modal = nullptr;
        }
        else
        {
            con_err("can't unset modal because it was not modal before");
        }
    }

    void iWidget::addWidget(iWidget* widget)
    {
        con_assert(widget != nullptr, "zero pointer");

        if (widget != nullptr)
        {
            auto iter = find(_widgets.begin(), _widgets.end(), widget);

            if (iter == _widgets.end())
            {
                _widgets.push_back(widget);
                widget->_parent = this;
                update();
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
            auto iter = find(_widgets.begin(), _widgets.end(), widget);

            if (iter != _widgets.end())
            {
                widget->setParent(nullptr);
                widget->setActive(false);
                widget->setVisible(false);
                _widgets.erase(iter);
                update();
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
            for (auto widget : _widgets)
            {
                widget->calcPosition(_posx, _posy, _width, _height);
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
        if (isActive())
        {
            if (_isMouseOver)
            {
                vector<iWidget*> widgets = _widgets;
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
                    if (d > 0)
                    {
                        _wheelUp(this);
                    }
                    else
                    {
                        _wheelDown(this);
                    }

                    return true;
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
                vector<iWidget*> widgets = _widgets;
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
                vector<iWidget*> widgets = _widgets;
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
                    if (key == iKeyCode::MouseLeft)
                    {
                        if (_widgetAppearanceState == iWidgetAppearanceState::Pressed)
                        {
                            _widgetAppearanceState = iWidgetAppearanceState::Clicked;
                            setKeyboardFocus();
                            _click(this);
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    bool iWidget::handleMouseKeyDown(iKeyCode key)
    {
        if (isActive())
        {
            if (_isMouseOver)
            {
                vector<iWidget*> widgets = _widgets;
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
                    if (key == iKeyCode::MouseLeft)
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
            vector<iWidget*> widgets = _widgets;

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
            vector<iWidget*> widgets = _widgets;

            for (auto widget : widgets)
            {
                widget->handleMouseMove(x, y);
            }

            if (x >= _posx &&
                x < _posx + _width &&
                y >= _posy &&
                y < _posy + _height)
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
        con_assert(horrizontalAlignment != iHorrizontalAlignment::Strech, "Strech alignment currently only supported for dialogs");

        _horrizontalAlignment = horrizontalAlignment;
        update();
    }

    iVerticalAlignment iWidget::getVerticalAlignment()
    {
        return _verticalAlignment;
    }

    void iWidget::setVerticalAlignment(iVerticalAlignment verticalAlignment)
    {
        con_assert(verticalAlignment != iVerticalAlignment::Strech, "Strech alignment currently only supported for dialogs");

        _verticalAlignment = verticalAlignment;
        update();
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
        }
    }

    void iWidget::updateParent()
    {
        if (_parent != nullptr)
        {
            _parent->update();
        }
    }

    void iWidget::calcPosition(int32 parentPosx, int32 parentPosy, int32 parentWidth, int32 parentHeight)
    {
        switch (_horrizontalAlignment)
        {
        case iHorrizontalAlignment::Left:
        case iHorrizontalAlignment::Strech:
            _posx = 0;
            break;

        case iHorrizontalAlignment::Center:
            _posx = (parentWidth - _width) / 2;
            break;

        case iHorrizontalAlignment::Right:
            _posx = parentWidth - _width;
            break;

        case iHorrizontalAlignment::Undefined:
        default:
            con_err("unexpected horrizontal alignment");
        };

        switch (_verticalAlignment)
        {
        case iVerticalAlignment::Top:
        case iVerticalAlignment::Strech:
            _posy = 0;
            break;

        case iVerticalAlignment::Center:
            _posy = (parentHeight - _height) / 2;
            break;

        case iVerticalAlignment::Bottom:
            _posy = parentHeight - _height;
            break;

        case iVerticalAlignment::Undefined:
        default:
            con_err("unexpected vertical alignment");
        };

        _posx += parentPosx;
        _posy += parentPosy;
    }

}