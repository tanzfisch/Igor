// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/dialogs/iDialog.h>

#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/ui/iWidgetManager.h>
#include <igor/ui/user_controls/iUserControl.h>
#include <igor/data/iIntersection.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iDialog::iDialog(iWidgetType type, const iWidgetPtr parent)
        : iWidget(type, iWidgetKind::Dialog, nullptr)
    {
        iWidgetManager::getInstance().registerDialog(this);

        setEnabled(false);
        setVisible(false);
        setWidth(100);
        setHeight(100);
        setHorizontalAlignment(iHorizontalAlignment::Center);
        setVerticalAlignment(iVerticalAlignment::Center);

        setAcceptOutOfBoundsClicks(true);
        setIgnoreChildEventHandling(true);

        if (parent != nullptr)
        {
            iWidgetPtr parentDialog = nullptr;
            iWidgetPtr iterator = parent;

            while (iterator->_parent != nullptr)
            {
                iterator = iterator->_parent;
                if (iterator->getWidgetKind() == iWidgetKind::Dialog)
                {
                    parentDialog = iterator;
                    break;
                }
            }

            if (parentDialog != nullptr)
            {
                setZValue(parentDialog->getZValue() + 1);
            }
        }
    }

    iDialog::~iDialog()
    {
        if (iWidgetManager::getInstance().isModal(this))
        {
            iWidgetManager::getInstance().resetModal();
        }

        iWidgetManager::getInstance().unregisterDialog(this);
    }

    void iDialog::enableHeader(bool enable)
    {
        _headerEnabled = enable;
    }

    bool iDialog::isHeaderEnabled() const
    {
        return _headerEnabled;
    }

    void iDialog::setReturnState(iDialogReturnState returnState)
    {
        _returnState = returnState;
    }

    iDialogReturnState iDialog::getReturnState() const
    {
        return _returnState;
    }

    void iDialog::open(iDialogCloseDelegate dialogCloseDelegate)
    {
        _dialogCloseDelegate = dialogCloseDelegate;
        setEnabled();
        setVisible();

        _isOpen = true;
    }

    void iDialog::close()
    {
        setEnabled(false);
        setVisible(false);
        iWidgetManager::getInstance().resetModal();
        iWidgetManager::getInstance().closeDialog(this);

        _isOpen = false;
    }

    bool iDialog::isOpen() const
    {
        return _isOpen;
    }

    void iDialog::calcMinSize()
    {
        int32 minWidth = 0;
        int32 minHeight = 0;

        if (isGrowingByContent() &&
            !_children.empty())
        {
            iWidgetPtr widget = _children.front();
            minWidth = widget->getMinWidth();
            minHeight = widget->getMinHeight();
        }

        if (_headerEnabled)
        {
            minHeight += 20; // TODO comes from theme
            setClientArea(_border, _border, _border + 20, _border);
        }
        else
        {
            setClientArea(_border, _border, _border, _border);
        }

        setMinSize(minWidth, minHeight);
    }

    void iDialog::updateAlignment(int32 clientWidth, int32 clientHeight)
    {
        int32 width = getMinWidth();
        int32 height = getMinHeight();

        switch (iWidget::getHorizontalAlignment())
        {
        case iHorizontalAlignment::Left:
            _relativeX = 0;
            break;

        case iHorizontalAlignment::Stretch:
            _relativeX = 0;
            width = clientWidth;
            break;

        case iHorizontalAlignment::Center:
            _relativeX = (clientWidth - width) / 2;
            break;

        case iHorizontalAlignment::Right:
            _relativeX = clientWidth - width;
            break;

        case iHorizontalAlignment::Absolute:
            _relativeX = _offset._x;
            break;

        default:;
        };

        switch (iWidget::getVerticalAlignment())
        {
        case iVerticalAlignment::Top:
            _relativeY = 0;
            break;

        case iVerticalAlignment::Stretch:
            _relativeY = 0;
            height = clientHeight;
            break;

        case iVerticalAlignment::Center:
            _relativeY = (clientHeight - height) / 2;
            break;

        case iVerticalAlignment::Bottom:
            _relativeY = clientHeight - height;
            break;

        case iVerticalAlignment::Absolute:
            _relativeY = _offset._y;
            break;

        default:;
        };

        _actualWidth = width;
        _actualHeight = height;
    }

    void iDialog::setBorder(int32 border)
    {
        _border = border;
    }

    int32 iDialog::getBorder()
    {
        return _border;
    }

    void iDialog::draw()
    {
        if (isVisible())
        {
            iWidgetManager::getInstance().getTheme()->drawDialog(getActualRect(), _headerEnabled, getState(), isEnabled());

            for (const auto child : _children)
            {
                child->draw();
            }
        }
    }

    void iDialog::setX(int32 x)
    {
        _offset._x = x;
        setHorizontalAlignment(iHorizontalAlignment::Absolute);
    }

    void iDialog::setY(int32 y)
    {
        _offset._y = y;
        setVerticalAlignment(iVerticalAlignment::Absolute);
    }

    const iaVector2f &iDialog::getPos() const
    {
        return _offset;
    }

    void iDialog::setPos(const iaVector2f &pos)
    {
        _offset = pos;
        setVerticalAlignment(iVerticalAlignment::Absolute);
        setHorizontalAlignment(iHorizontalAlignment::Absolute);
    }

    bool iDialog::handleMouseKeyDown(iKeyCode key)
    {
        if (!isEnabled())
        {
            return false;
        }

        if (_isMouseOver)
        {
            // get copy of children
            std::vector<iWidgetPtr> widgets = getChildren();
            bool result = false;

            for (auto widget : widgets)
            {
                if (widget->handleMouseKeyDown(key))
                {
                    result = true;
                }
            }

            if (!_ignoreChildEventHandling && result)
            {
                return true;
            }

            if (key == iKeyCode::MouseLeft ||
                key == iKeyCode::MouseRight)
            {
                _widgetState = iWidgetState::Pressed;
            }

            if (_isMouseOverHeader && _widgetState == iWidgetState::Pressed)
            {
                _isInMotion = true;
            }
        }

        return true;
    }

    bool iDialog::handleMouseKeyUp(iKeyCode key)
    {
        if (!isEnabled())
        {
            return false;
        }

        if(_isInMotion && key == iKeyCode::MouseLeft)
        {
            _isInMotion = false;
        }

        if (_isMouseOver)
        {
            // get copy of children
            std::vector<iWidgetPtr> children = getChildren();
            bool result = false;

            for (auto child : children)
            {
                if (child->handleMouseKeyUp(key))
                {
                    result = true;
                }
            }

            if (!_ignoreChildEventHandling && result)
            {
                return true;
            }

            if (key == iKeyCode::MouseLeft ||
                key == iKeyCode::MouseRight)
            {
                if (_widgetState == iWidgetState::Pressed)
                {
                    _widgetState = iWidgetState::Clicked;
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

        if (_acceptOutOfBoundsClicks)
        {
            _mouseOffClick(this);
            return true;
        }

        return true;
    }

    void iDialog::handleMouseMove(const iaVector2f &pos)
    {
        if (!isEnabled())
        {
            return;
        }

        // get copy of children
        std::vector<iWidgetPtr> widgets = getChildren();

        for (auto widget : widgets)
        {
            widget->handleMouseMove(pos);
        }

        auto rect = getActualRect();
        if (iIntersection::intersects(pos, rect))
        {
            if (!_isMouseOver)
            {
                _widgetState = iWidgetState::Highlighted;
                _mouseOver(this);
            }

            _isMouseOver = true;
        }
        else
        {
            if (_isMouseOver)
            {
                _widgetState = iWidgetState::Standby;
                _mouseOff(this);
            }

            _isMouseOver = false;
        }

        iaRectanglef header(_absoluteX + _border,
                            _absoluteY + _border,
                            _actualWidth - _border,
                            20);
        _isMouseOverHeader = iIntersection::intersects(pos, header);

        if (_isInMotion)
        {
            const iaVector2f diff = pos - _posLast;
            setPos(getPos() + diff);
        }

        _posLast = pos;
    }

} // namespace igor