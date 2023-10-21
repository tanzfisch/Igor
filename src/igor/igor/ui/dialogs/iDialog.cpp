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

    static float32 s_frameBorder = 5; // TODO put this in theme
    static float32 s_headerSize = 25;  // TODO put this in theme

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
            setClientArea(s_frameBorder, s_frameBorder, s_frameBorder + s_headerSize, s_frameBorder);
        }
        else
        {
            setClientArea(s_frameBorder, s_frameBorder, s_frameBorder, s_frameBorder);
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

    void iDialog::draw()
    {
        if (isVisible())
        {
            iaRectanglef clientRect = getActualRect();
            clientRect.adjust(_clientAreaLeft, _clientAreaTop, -_clientAreaRight - _clientAreaLeft, -_clientAreaBottom - _clientAreaTop);

            iWidgetManager::getInstance().getTheme()->drawDialog(getActualRect(), clientRect, _headerEnabled, _resizeEnabled, getState(), isEnabled());

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

            if (_motionState == iDialogMotionState::Static && _widgetState == iWidgetState::Pressed)
            {
                _motionState = calcMotionState(_posLast);
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

        if (_motionState != iDialogMotionState::Static && key == iKeyCode::MouseLeft)
        {
            _motionState = iDialogMotionState::Static;
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

    iDialogMotionState iDialog::calcMotionState(const iaVector2f &pos)
    {
        iaRectanglef header(_absoluteX + s_frameBorder,
                            _absoluteY,
                            _actualWidth - s_frameBorder,
                            s_headerSize);
        if (iIntersection::intersects(pos, header))
        {
            return iDialogMotionState::Moving;
        }

        iaRectanglef leftEdge(_absoluteX, _absoluteY, s_frameBorder, _actualHeight);
        bool left = iIntersection::intersects(pos, leftEdge);

        iaRectanglef topEdge(_absoluteX, _absoluteY, _actualWidth, s_frameBorder);
        bool top = iIntersection::intersects(pos, topEdge);

        if (left && top)
        {
            return iDialogMotionState::ResizeLeftTop;
        }

        iaRectanglef rightEdge(_absoluteX + _actualWidth - s_frameBorder, _absoluteY, s_frameBorder, _actualHeight);
        bool right = iIntersection::intersects(pos, rightEdge);

        if (right && top)
        {
            return iDialogMotionState::ResizeRightTop;
        }

        if (top)
        {
            return iDialogMotionState::ResizeTop;
        }

        iaRectanglef bottomEdge(_absoluteX, _absoluteY + _actualHeight - s_frameBorder, _actualWidth, s_frameBorder);
        bool bottom = iIntersection::intersects(pos, bottomEdge);

        if (left && bottom)
        {
            return iDialogMotionState::ResizeLeftBottom;
        }

        if (right && bottom)
        {
            return iDialogMotionState::ResizeRightBottom;
        }

        if (bottom)
        {
            return iDialogMotionState::ResizeBottom;
        }

        return iDialogMotionState::Static;
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

        if (_motionState != iDialogMotionState::Static)
        {
            const iaVector2f diff = pos - _posLast;
            if (_motionState == iDialogMotionState::Moving)
            {
                setPos(getPos() + diff);
            }
            else if (_motionState == iDialogMotionState::ResizeRight)
            {
                setWidth(getActualWidth() + diff._x);
            }
        }

        _posLast = pos;
    }

    void iDialog::enableResizeable(bool enable)
    {
        _resizeEnabled = enable;
    }

    bool iDialog::isResizeable() const
    {
        return _resizeEnabled;
    }

} // namespace igor