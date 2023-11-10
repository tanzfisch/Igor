// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/dialogs/iDialog.h>

#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/ui/iWidgetManager.h>
#include <igor/ui/user_controls/iUserControl.h>
#include <igor/data/iIntersection.h>
#include <igor/renderer/iRenderer.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iDialog::iDialog(iWidgetType type, const iWidgetPtr parent)
        : iWidget(type, iWidgetKind::Dialog, nullptr)
    {
        setEnabled(false);
        setVisible(false);
        setWidth(100);
        setHeight(100);
        setHorizontalAlignment(iHorizontalAlignment::Absolute);
        setVerticalAlignment(iVerticalAlignment::Absolute);

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
    }

    void iDialog::setHeaderEnabled(bool enable)
    {
        _headerEnabled = enable;
    }

    bool iDialog::hasHeader() const
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
            const int32 titleWidth = iWidgetManager::getInstance().getTheme()->getDialogTitleWidth();
            setClientArea(0, 0, titleWidth, 0);

            const float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();
            const int32 titleTextWidth = static_cast<int32>(iWidgetManager::getInstance().getTheme()->getFont()->measureWidth(_title, fontSize));
            minWidth = std::max(minWidth, titleTextWidth + (int32)fontSize * 2);
        }
        else
        {
            setClientArea(0, 0, 0, 0);
        }

        minWidth = std::max(getConfiguredWidth(), minWidth);
        minHeight = std::max(getConfiguredHeight(), minHeight);

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
        if (!isVisible())
        {
            return;
        }

        iaRectanglef clientRect = getActualRect();
        clientRect.adjust(_clientAreaLeft, _clientAreaTop, -_clientAreaRight - _clientAreaLeft, -_clientAreaBottom - _clientAreaTop);

        iWidgetManager::getInstance().getTheme()->drawDialog(getActualRect(), clientRect, _headerEnabled, _title + " " + iaString::toString(getZValue()), isResizeable(), getState(), isEnabled());

        // store current render states
        const iaRectanglei viewport = iRenderer::getInstance().getViewport();
        const iaMatrixd projectionMatrix = iRenderer::getInstance().getProjectionMatrix();
        const iaMatrixd modelMatrix = iRenderer::getInstance().getModelMatrix();

        iRenderer::getInstance().setViewport(clientRect._x, iWidgetManager::getInstance().getDesktopHeight() - clientRect._y - clientRect._height, clientRect._width, clientRect._height);
        iRenderer::getInstance().setOrtho(clientRect._x, clientRect._x + clientRect._width, clientRect._y + clientRect._height, clientRect._y, 0.1f, 10.0f);

        for (const auto child : _children)
        {
            child->draw();
        }

        // restore everything
        iRenderer::getInstance().setModelMatrix(modelMatrix);
        iRenderer::getInstance().setProjectionMatrix(projectionMatrix);
        iRenderer::getInstance().setViewport(viewport);
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

    void iDialog::putInFront()
    {
        iWidgetManager::getInstance().putDialogInFront(this);
    }

    bool iDialog::handleASCII(uint8 c)
    {
        if (iWidget::handleASCII(c))
        {
            return true;
        }

        return true;
    }

    bool iDialog::handleMouseKeyDown(iKeyCode key)
    {
        if (!isEnabled())
        {
            return false;
        }

        if (!_isMouseOver)
        {
            return false;
        }

        if (_motionState == iDialogMotionState::Static &&
            (key == iKeyCode::MouseLeft ||
             key == iKeyCode::MouseRight))
        {
            _widgetState = iWidgetState::Pressed;
            _motionState = calcMotionState(_posLast);
            _lastMousePos.set(iMouse::getInstance().getPos()._x, iMouse::getInstance().getPos()._y);

            putInFront();

            if (_motionState != iDialogMotionState::Static)
            {
                return true;
            }
        }

        // get copy of children
        std::vector<iWidgetPtr> widgets = getChildren();
        bool childResult = false;

        for (auto widget : widgets)
        {
            if (widget->handleMouseKeyDown(key))
            {
                childResult = true;
            }
        }

        if (!_ignoreChildEventHandling && childResult)
        {
            return true;
        }

        return false;
    }

    iWidgetID iDialog::getDockingParent() const
    {
        return _dockingParentID;
    }

    bool iDialog::isDocked() const
    {
        return _dockingParentID != iWidget::INVALID_WIDGET_ID;
    }

    bool iDialog::handleMouseKeyUp(iKeyCode key)
    {
        _moving = false;

        if (!isEnabled())
        {
            return false;
        }

        if (_motionState != iDialogMotionState::Static && key == iKeyCode::MouseLeft)
        {
            if (_motionState == iDialogMotionState::Moving &&
                isDockable())
            {
                _dockingParentID = iWidgetManager::getInstance().dockDialog(getID());
            }

            _motionState = iDialogMotionState::Static;
        }

        if (!_isMouseOver)
        {
            return false;
        }

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

        if (_acceptOutOfBoundsClicks)
        {
            _mouseOffClick(this);
            return true;
        }

        return false;
    }

    iDialogMotionState iDialog::calcMotionState(const iaVector2f &pos)
    {
        const float32 titleWidth = iWidgetManager::getInstance().getTheme()->getDialogTitleWidth();
        const float32 frameWidth = iWidgetManager::getInstance().getTheme()->getDialogFrameWidth();

        if (!isMoveable())
        {
            return iDialogMotionState::Static;
        }

        if (_headerEnabled)
        {
            iaRectanglef header(_absoluteX, _absoluteY, _actualWidth, titleWidth);
            if (iIntersection::intersects(pos, header))
            {
                return iDialogMotionState::Moving;
            }
        }

        if (!isResizeable() ||
            isDocked())
        {
            return iDialogMotionState::Static;
        }

        iaRectanglef leftEdge(_absoluteX - frameWidth, _absoluteY - frameWidth, frameWidth, _actualHeight + frameWidth * 2.0f);
        bool left = iIntersection::intersects(pos, leftEdge);

        iaRectanglef topEdge(_absoluteX - frameWidth, _absoluteY - frameWidth, _actualWidth + frameWidth * 2.0f, frameWidth);
        bool top = iIntersection::intersects(pos, topEdge);

        if (left && top)
        {
            return iDialogMotionState::ResizeLeftTop;
        }

        iaRectanglef rightEdge(_absoluteX + _actualWidth, _absoluteY - frameWidth, frameWidth, _actualHeight + frameWidth * 2.0);
        bool right = iIntersection::intersects(pos, rightEdge);

        if (right && top)
        {
            return iDialogMotionState::ResizeRightTop;
        }

        if (top)
        {
            return iDialogMotionState::ResizeTop;
        }

        iaRectanglef bottomEdge(_absoluteX - frameWidth, _absoluteY + _actualHeight, _actualWidth + frameWidth * 2.0f, frameWidth);
        bool bottom = iIntersection::intersects(pos, bottomEdge);

        if (left && bottom)
        {
            return iDialogMotionState::ResizeLeftBottom;
        }

        if (right && bottom)
        {
            return iDialogMotionState::ResizeRightBottom;
        }

        if (left)
        {
            return iDialogMotionState::ResizeLeft;
        }

        if (right)
        {
            return iDialogMotionState::ResizeRight;
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

        const float32 frameWidth = iWidgetManager::getInstance().getTheme()->getDialogFrameWidth();
        auto rect = getActualRect();
        rect.adjust(-frameWidth, -frameWidth, frameWidth * 2.0f, frameWidth * 2.0f);
        if (iIntersection::intersects(pos, rect))
        {
            if (!_isMouseOver)
            {
                _widgetState = iWidgetState::Highlighted;
                _mouseOver(this);
            }

            _isMouseOver = true;

            iDialogMotionState motionState = calcMotionState(pos);
            iMouseCursorType cursorType;

            switch (motionState)
            {
            case iDialogMotionState::Moving:
            case iDialogMotionState::Static:
                cursorType = iMouseCursorType::Arrow;
                break;
            case iDialogMotionState::ResizeLeft:
                cursorType = iMouseCursorType::ArrowLeftEdge;
                break;
            case iDialogMotionState::ResizeRight:
                cursorType = iMouseCursorType::ArrowRightEdge;
                break;
            case iDialogMotionState::ResizeTop:
                cursorType = iMouseCursorType::ArrowTopEdge;
                break;
            case iDialogMotionState::ResizeBottom:
                cursorType = iMouseCursorType::ArrowBottomEdge;
                break;
            case iDialogMotionState::ResizeLeftTop:
                cursorType = iMouseCursorType::ArrowTopLeftCorner;
                break;
            case iDialogMotionState::ResizeRightTop:
                cursorType = iMouseCursorType::ArrowTopRightCorner;
                break;
            case iDialogMotionState::ResizeLeftBottom:
                cursorType = iMouseCursorType::ArrowBottomLeftCorner;
                break;
            case iDialogMotionState::ResizeRightBottom:
                cursorType = iMouseCursorType::ArrowBottomRightCorner;
                break;
            }

            iMouse::getInstance().setCursorType(cursorType);

            if (_motionState == iDialogMotionState::Moving &&
                _lastMousePos.distance(pos) > 3.0)
            {
                _moving = true;
            }
        }
        else
        {
            if (_isMouseOver)
            {
                _widgetState = iWidgetState::Standby;
                _mouseOff(this);

                iMouse::getInstance().resetCursorType();
            }

            _isMouseOver = false;
        }

        if (isDocked() && _moving)
        {
            iWidgetManager::getInstance().undockDialog(getID());
            _dockingParentID = iWidget::INVALID_WIDGET_ID;
        }

        if (_motionState != iDialogMotionState::Static)
        {
            // convert to absolute positioning to prevent a pop during first move
            if (getVerticalAlignment() != iVerticalAlignment::Absolute ||
                getHorizontalAlignment() != iHorizontalAlignment::Absolute)
            {
                setPos(getActualPos());
                setWidth(getActualWidth());
                setHeight(getActualHeight());
            }

            const iaVector2f diff = pos - _posLast;
            if (_motionState == iDialogMotionState::Moving &&
                _moving)
            {
                setPos(getPos() + diff);
            }

            if (_motionState == iDialogMotionState::ResizeRight ||
                _motionState == iDialogMotionState::ResizeRightBottom ||
                _motionState == iDialogMotionState::ResizeRightTop)
            {
                setWidth(getConfiguredWidth() + diff._x);
            }

            if (_motionState == iDialogMotionState::ResizeLeft ||
                _motionState == iDialogMotionState::ResizeLeftBottom ||
                _motionState == iDialogMotionState::ResizeLeftTop)
            {
                iaVector2f dialogPos = getPos();
                dialogPos._x += diff._x;
                setPos(dialogPos);
                setWidth(getConfiguredWidth() - diff._x);
            }

            if (_motionState == iDialogMotionState::ResizeBottom ||
                _motionState == iDialogMotionState::ResizeRightBottom ||
                _motionState == iDialogMotionState::ResizeLeftBottom)
            {
                setHeight(getConfiguredHeight() + diff._y);
            }

            if (_motionState == iDialogMotionState::ResizeTop ||
                _motionState == iDialogMotionState::ResizeRightTop ||
                _motionState == iDialogMotionState::ResizeLeftTop)
            {
                iaVector2f dialogPos = getPos();
                dialogPos._y += diff._y;
                setPos(dialogPos);
                setHeight(getConfiguredHeight() - diff._y);
            }
        }

        _posLast = pos;
    }

    void iDialog::setResizeable(bool enable)
    {
        _isResizeable = enable;
    }

    bool iDialog::isResizeable() const
    {
        return _isResizeable;
    }

    void iDialog::setMoveable(bool enable)
    {
        _isMoveable = enable;
    }

    bool iDialog::isMoveable() const
    {
        return _isMoveable;
    }

    void iDialog::setTitle(const iaString &title)
    {
        _title = title;
    }

    const iaString &iDialog::getTitle() const
    {
        return _title;
    }

    void iDialog::setDockable(bool enable)
    {
        _isDockable = enable;
    }

    bool iDialog::isDockable() const
    {
        return _isDockable;
    }

} // namespace igor