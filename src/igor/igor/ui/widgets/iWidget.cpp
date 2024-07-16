// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidget.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/data/iIntersection.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <algorithm>

namespace igor
{
    iaIDGenerator64 iWidget::_idGenerator;
    iWidgetPtr iWidget::_keyboardFocus = nullptr;

    iWidget::iWidget(iWidgetType type, iWidgetKind kind, const iWidgetPtr parent)
        : _type(type), _kind(kind)
    {
        _id = _idGenerator.getNextID();

        iWidgetManager::getInstance().registerWidget(this);

        if (parent != nullptr)
        {
            parent->addWidget(this);
        }
    }

    iWidget::~iWidget()
    {
        if (hasKeyboardFocus())
        {
            resetKeyboardFocus();
        }

        clearChildren();

        iWidgetManager::getInstance().unregisterWidget(this);
    }

    void iWidget::clear()
    {
        clearChildren();
    }

    void iWidget::clearChildren()
    {
        for (const auto child : _children)
        {
            child->_parent = nullptr;
            iWidgetManager::getInstance().deleteWidget(child);
        }

        _children.clear();
    }

    void iWidget::onParentChanged()
    {
        // implement in deriving classes
    }

    iWidgetType iWidget::getWidgetType() const
    {
        return _type;
    }

    iWidgetKind iWidget::getWidgetKind() const
    {
        return _kind;
    }

    void iWidget::setBackground(const iaColor4f &color)
    {
        _background = color;
    }

    const iaColor4f &iWidget::getBackground() const
    {
        return _background;
    }

    void iWidget::setForeground(const iaColor4f &color)
    {
        _foreground = color;
    }

    const iaColor4f &iWidget::getForeground() const
    {
        return _foreground;
    }

    void iWidget::unblockEvents()
    {
        _blockedEvents = false;

        _click.block(_blockedEvents);
        _mouseOffClick.block(_blockedEvents);
        _contextMenu.block(_blockedEvents);
        _doubleClick.block(_blockedEvents);
        _mouseOver.block(_blockedEvents);
        _mouseOff.block(_blockedEvents);
        _change.block(_blockedEvents);
        _focus.block(_blockedEvents);
        _wheelUp.block(_blockedEvents);
        _wheelDown.block(_blockedEvents);
        _selectionChanged.block(_blockedEvents);
    }

    void iWidget::blockEvents()
    {
        _blockedEvents = true;

        _click.block(_blockedEvents);
        _mouseOffClick.block(_blockedEvents);
        _contextMenu.block(_blockedEvents);
        _doubleClick.block(_blockedEvents);
        _mouseOver.block(_blockedEvents);
        _mouseOff.block(_blockedEvents);
        _change.block(_blockedEvents);
        _focus.block(_blockedEvents);
        _wheelUp.block(_blockedEvents);
        _wheelDown.block(_blockedEvents);
        _selectionChanged.block(_blockedEvents);
    }

    const std::vector<iWidgetPtr> &iWidget::getChildren() const
    {
        return _children;
    }

    void iWidget::setTooltip(const iaString &text)
    {
        _tooltip = text;
    }

    iaString iWidget::getTooltip() const
    {
        return _tooltip;
    }

    iWidgetPtr iWidget::getKeyboardFocusWidget()
    {
        return _keyboardFocus;
    }

    iaString iWidget::getInfo() const
    {
        std::wstringstream stream;

        stream << getWidgetType() << " [" << _id << "] (" << _absoluteX << ", " << _absoluteY << ", " << _actualWidth << ", " << _actualHeight << ") ";

        if (!hasParent())
        {
            stream << ", no parent";
        }

        return iaString(stream.str().c_str());
    }

    iWidgetPtr iWidget::getRoot()
    {
        iWidgetPtr current = this;

        while (current->_parent != nullptr)
        {
            current = current->_parent;
        }

        return current;
    }

    void iWidget::addWidget(iWidgetPtr widget)
    {
        con_assert(widget != nullptr, "zero pointer");
        con_assert(widget != this, "widget can't be added to it self");

        if (widget == nullptr ||
            widget == this)
        {
            return;
        }

        if (std::find(_children.begin(), _children.end(), widget) != _children.end())
        {
            con_err("widget \"" << widget->getID() << "\" was added already");
            return;
        }

        _children.push_back(widget);

        widget->setParent(this);
    }

    void iWidget::removeWidget(iWidgetPtr widget)
    {
        con_assert(widget != nullptr, "zero pointer");

        if (widget == nullptr)
        {
            return;
        }

        auto iter = std::find(_children.begin(), _children.end(), widget);

        if (iter != _children.end())
        {
            widget->setParent(nullptr);
            _children.erase(iter);
        }
    }

    void iWidget::setUserData(const std::any &userData)
    {
        _userData = userData;
    }

    std::any iWidget::getUserData() const
    {
        return _userData;
    }

    void iWidget::setAcceptDrop(bool acceptDrop)
    {
        _acceptDrop = acceptDrop;
    }

    bool iWidget::isAcceptingDrop()
    {
        return _acceptDrop;
    }

    void iWidget::setAcceptDrag(bool acceptDrag)
    {
        _acceptDrag = acceptDrag;
    }

    bool iWidget::isAcceptingDrag()
    {
        return _acceptDrag;
    }

    void iWidget::setParent(iWidgetPtr parent)
    {
        _parent = parent;
        onParentChanged();
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
        _click.add(iClickDelegate);
    }

    void iWidget::registerOnMouseOffClickEvent(iMouseOffClickDelegate clickDelegate)
    {
        _mouseOffClick.add(clickDelegate);
    }

    void iWidget::registerOnMouseOverEvent(iMouseOverDelegate iMouseOverDelegate)
    {
        _mouseOver.add(iMouseOverDelegate);
    }

    void iWidget::registerOnMouseOffEvent(iMouseOffDelegate iMouseOffDelegate)
    {
        _mouseOff.add(iMouseOffDelegate);
    }

    void iWidget::registerOnChangeEvent(iChangeDelegate iChangeDelegate)
    {
        _change.add(iChangeDelegate);
    }

    void iWidget::registerOnFocusEvent(iFocusDelegate iFocusDelegate)
    {
        _focus.add(iFocusDelegate);
    }

    void iWidget::registerOnDoubleClickEvent(iDoubleClickDelegate doubleClickDelegate)
    {
        _doubleClick.add(doubleClickDelegate);
    }

    void iWidget::unregisterOnDoubleClickEvent(iDoubleClickDelegate doubleClickDelegate)
    {
        _doubleClick.remove(doubleClickDelegate);
    }

    void iWidget::registerOnContextMenuEvent(iContextMenuDelegate contextMenuDelegate)
    {
        _contextMenu.add(contextMenuDelegate);
    }

    void iWidget::unregisterOnContextMenuEvent(iContextMenuDelegate contextMenuDelegate)
    {
        _contextMenu.remove(contextMenuDelegate);
    }

    void iWidget::setKeyboardFocus()
    {
        if (_doNotTakeKeyboard)
        {
            return;
        }

        if (_keyboardFocus != this)
        {
            if (_keyboardFocus != nullptr)
            {
                _keyboardFocus->onLostKeyboardFocus();
            }

            _keyboardFocus = this;
            _keyboardFocus->onGainedKeyboardFocus();
        }
    }

    void iWidget::resetKeyboardFocus()
    {
        if (_keyboardFocus != nullptr)
        {
            _keyboardFocus->onLostKeyboardFocus();
        }

        _keyboardFocus = nullptr;
    }

    void iWidget::drawOverlay()
    {
        // implement in overlay capable widgets
    }

    void iWidget::draw()
    {
        if (!isVisible())
        {
            return;
        }

        for (const auto child : _children)
        {
            child->draw();
        }
    }

    bool iWidget::isMouseOver()
    {
        return _isMouseOver;
    }

    void iWidget::registerOnWheelUpEvent(iWheelUpDelegate wheelUpDelegate)
    {
        _wheelUp.add(wheelUpDelegate);
    }

    void iWidget::unregisterOnWheelUpEvent(iWheelUpDelegate wheelUpDelegate)
    {
        _wheelUp.remove(wheelUpDelegate);
    }

    void iWidget::registerOnWheelDownEvent(iWheelDownDelegate wheelDownDelegate)
    {
        _wheelDown.add(wheelDownDelegate);
    }

    void iWidget::unregisterOnWheelDownEvent(iWheelDownDelegate wheelDownDelegate)
    {
        _wheelDown.remove(wheelDownDelegate);
    }

    void iWidget::onLostKeyboardFocus()
    {
        // implement in derived class if needed
    }

    void iWidget::onGainedKeyboardFocus()
    {
        // implement in derived class if needed
    }

    bool iWidget::onMouseWheel(iEventMouseWheel &event)
    {
        if (!isEnabled() ||
            !_reactOnMouseWheel ||
            !isMouseOver())
        {
            return false;
        }

        // get copy of children
        std::vector<iWidgetPtr> widgets = getChildren();
        bool result = false;

        for (auto widget : widgets)
        {
            if (widget->onMouseWheel(event))
            {
                result = true;
            }
        }

        if (!_ignoreChildEventConsumption && result)
        {
            return true;
        }

        if (event.getWheelDelta() > 0)
        {
            _wheelUp(this);
        }
        else
        {
            _wheelDown(this);
        }

        return false;
    }

    bool iWidget::onMouseDoubleClick(iEventMouseKeyDoubleClick &event)
    {
        if (!isEnabled() ||
            !isMouseOver())
        {
            return false;
        }

        // get copy of children
        std::vector<iWidgetPtr> widgets = getChildren();
        bool result = false;

        for (auto widget : widgets)
        {
            if (widget->onMouseDoubleClick(event))
            {
                result = true;
            }
        }

        if (!_ignoreChildEventConsumption && result)
        {
            return true;
        }
        else
        {
            if (event.getKey() == iKeyCode::MouseLeft)
            {
                _widgetState = iWidgetState::DoubleClicked;
                setKeyboardFocus();
                _doubleClick(this);
                return true;
            }
        }

        return false;
    }

    void iWidget::setZValue(uint32 zValue)
    {
        _zValue = zValue;
    }

    uint32 iWidget::getZValue() const
    {
        return _zValue;
    }

    void iWidget::setIgnoreChildEventConsumption(bool value)
    {
        _ignoreChildEventConsumption = value;
    }

    bool iWidget::isIgnoringChildEventConsumption() const
    {
        return _ignoreChildEventConsumption;
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
        _selectionChanged.add(delegate);
    }

    void iWidget::unregisterOnSelectionChangedEvent(iSelectionChangedDelegate delegate)
    {
        _selectionChanged.remove(delegate);
    }

    bool iWidget::onMouseKeyDown(iEventMouseKeyDown &event)
    {
        if (!isEnabled() ||
            !isMouseOver())
        {
            return false;
        }

        // get copy of children
        std::vector<iWidgetPtr> widgets = getChildren();
        bool result = false;

        for (auto widget : widgets)
        {
            if (widget->onMouseKeyDown(event))
            {
                result = true;
            }
        }

        if (!_ignoreChildEventConsumption && result)
        {
            return true;
        }

        if (event.getKey() == iKeyCode::MouseLeft ||
            event.getKey() == iKeyCode::MouseRight)
        {
            _widgetState = iWidgetState::Pressed;
            _lastMousePressPos.set(event.getPosition()._x, event.getPosition()._y);
            return true;
        }

        return false;
    }

    bool iWidget::onMouseKeyUp(iEventMouseKeyUp &event)
    {
        if (!isEnabled())
        {
            return false;
        }

        if (_isMouseOver)
        {
            // get copy of children
            std::vector<iWidgetPtr> children = getChildren();
            bool result = false;

            for (auto child : children)
            {
                if (child->onMouseKeyUp(event))
                {
                    result = true;
                }
            }

            if (!_ignoreChildEventConsumption && result)
            {
                return true;
            }
            else
            {
                if (isAcceptingDrop() &&
                    iWidgetManager::getInstance().inDrag())
                {
                    onDrop(iWidgetManager::getInstance().getDrag(), event.getPosition());
                    return true;
                }

                if (event.getKey() == iKeyCode::MouseLeft ||
                    event.getKey() == iKeyCode::MouseRight)
                {
                    if (_widgetState == iWidgetState::Pressed)
                    {
                        _widgetState = iWidgetState::Clicked;
                        setKeyboardFocus();

                        if (event.getKey() == iKeyCode::MouseLeft)
                        {
                            _click(this);
                        }

                        select();

                        if (event.getKey() == iKeyCode::MouseRight)
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
            return true;
        }

        return false;
    }

    bool iWidget::onASCII(iEventKeyASCII &event)
    {
        if (!isEnabled())
        {
            return false;
        }

        // get copy of children
        std::vector<iWidgetPtr> widgets = getChildren();

        for (auto widget : widgets)
        {
            if (widget->onASCII(event))
            {
                return true;
            }
        }

        return false;
    }

    bool iWidget::onEvent(iEvent &event)
    {
        if (!isEnabled())
        {
            return false;
        }

        // get copy of children
        std::vector<iWidgetPtr> widgets = getChildren();

        for (auto widget : widgets)
        {
            if (widget->onEvent(event))
            {
                return true;
            }
        }

        return false;
    }

    bool iWidget::onKeyDown(iEventKeyDown &event)
    {
        if (!isEnabled())
        {
            return false;
        }

        // get copy of children
        std::vector<iWidgetPtr> widgets = getChildren();

        for (auto widget : widgets)
        {
            if (widget->onKeyDown(event))
            {
                return true;
            }
        }

        return false;
    }

    bool iWidget::onKeyUp(iEventKeyUp &event)
    {
        if (!isEnabled())
        {
            return false;
        }

        // get copy of children
        std::vector<iWidgetPtr> widgets = getChildren();
        bool result = false;

        for (auto widget : widgets)
        {
            if (widget->onKeyUp(event))
            {
                result = true;
                break;
            }
        }

        if (!_ignoreChildEventConsumption && result)
        {
            return true;
        }

        return false;
    }

    void iWidget::onMouseMove(iEventMouseMove &event)
    {
        if (!isEnabled())
        {
            return;
        }

        // get copy of children
        std::vector<iWidgetPtr> widgets = getChildren();

        for (auto widget : widgets)
        {
            widget->onMouseMove(event);
        }

        auto rect = getActualRect();
        if (iIntersection::intersects(event.getPosition(), rect) &&
            !event.isConsumed())
        {
            if (!_isMouseOver)
            {
                _widgetState = iWidgetState::Highlighted;
                _mouseOver(this);

                if (isAcceptingDrop() &&
                    iWidgetManager::getInstance().inDrag())
                {
                    onDragEnter(iWidgetManager::getInstance().getDrag());
                }
                else
                {
                    if (!_tooltip.isEmpty())
                    {
                        if (!_initTooltip)
                        {
                            _tooltipTime = iaTime::getNow() + iaTime::fromMilliseconds(1000);
                            _initTooltip = true;
                            _tooltipPos._x = event.getPosition()._x + 15.0f;
                            _tooltipPos._y = event.getPosition()._y + 15.0f;
                        }
                    }
                }
            }
            else
            {
                if (isAcceptingDrop() &&
                    iWidgetManager::getInstance().inDrag())
                {
                    onDragMove(iWidgetManager::getInstance().getDrag(), event.getPosition());
                }

                if (isAcceptingDrag() &&
                    iMouse::getInstance().getLeftButton() &&
                    !iWidgetManager::getInstance().inDrag() &&
                    _lastMousePressPos.distance(event.getPosition()) > 3.0)
                {
                    onDrag();
                }
            }

            _isMouseOver = true;
        }
        else
        {
            if (_isMouseOver)
            {
                _widgetState = iWidgetState::Standby;
                _mouseOff(this);

                if (_acceptDrop &&
                    iWidgetManager::getInstance().inDrag())
                {
                    onDragLeave(iWidgetManager::getInstance().getDrag());
                }
            }

            _initTooltip = false;
            iWidgetManager::getInstance().hideTooltip();

            _isMouseOver = false;
        }

        _lastMousePos = event.getPosition();
    }

    const iaVector2f &iWidget::getLastMousePos() const
    {
        return _lastMousePos;
    }

    iHorizontalAlignment iWidget::getHorizontalAlignment() const
    {
        return _horizontalAlignment;
    }

    void iWidget::setHorizontalAlignment(iHorizontalAlignment horizontalAlignment)
    {
        _horizontalAlignment = horizontalAlignment;
    }

    iVerticalAlignment iWidget::getVerticalAlignment() const
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
            _widgetState = iWidgetState::Standby;
        }
    }

    void iWidget::setEnabled(bool enabled)
    {
        _enabled = enabled;

        if (!_enabled)
        {
            _widgetState = iWidgetState::Standby;
            _isMouseOver = false;
        }
    }

    void iWidget::setMinSize(int32 width, int32 height)
    {
        _configuredMinWidth = width;
        _configuredMinHeight = height;
    }

    void iWidget::setMinWidth(int32 width)
    {
        _configuredMinWidth = width;
    }

    void iWidget::setMinHeight(int32 height)
    {
        _configuredMinHeight = height;
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
            con_err("absolute positioning only supported for dialogs");
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
            con_err("absolute positioning only supported for dialogs");
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

    void iWidget::calcChildOffsets(std::vector<iaRectanglef> &offsets)
    {
        offsets.clear();

        for (int i = 0; i < _children.size(); ++i)
        {
            int32 clientWidth = _actualWidth - _clientAreaLeft - _clientAreaRight;
            int32 clientHeight = _actualHeight - _clientAreaTop - _clientAreaBottom;

            offsets.push_back(iaRectanglef(_clientAreaLeft, _clientAreaTop, clientWidth, clientHeight));
        }
    }

    void iWidget::onUpdate()
    {
        if (isMouseOver() &&
            _initTooltip &&
            _tooltipTime <= iaTime::getNow())
        {
            _tooltipTime = iaTime(0);
            _initTooltip = false;
            iWidgetManager::getInstance().showTooltip(_tooltipPos, _tooltip);
        }
    }

    void iWidget::calcMinSize()
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

        updateMinSize(minWidth, minHeight);
    }

    void iWidget::updateMinSize(int32 width, int32 height)
    {
        int32 minWidth = width + _clientAreaLeft + _clientAreaRight;
        int32 minHeight = height + _clientAreaTop + _clientAreaBottom;

        if (minWidth < _configuredMinWidth)
        {
            minWidth = _configuredMinWidth;
        }

        if (minHeight < _configuredMinHeight)
        {
            minHeight = _configuredMinHeight;
        }

        _minWidth = minWidth;
        _minHeight = minHeight;
    }

    std::wostream &operator<<(std::wostream &stream, const iWidgetType &widgetType)
    {
        const static iaString text[] = {
            "iWidgetButton",
            "iWidgetCheckBox",
            "iWidgetColor",
            "iWidgetColorGradient",
            "iWidgetGraph",
            "iWidgetGroupBox",
            "iWidgetLabel",
            "iWidgetMenu",
            "iWidgetMenuBar",
            "iWidgetNumberChooser",
            "iWidgetPicture",
            "WidgetScroll",
            "iWidgetSelectBox",
            "iWidgetSlider",
            "iWidgetSpacer",
            "iWidgetLineTextEdit",
            "iWidgetTextEdit",
            "iWidgetSplitter",
            "iWidgetViewport",

            "iWidgetGridLayout",
            "iWidgetFixedGridLayout",
            "iWidgetBoxLayout",
            "iWidgetDockingLayout",

            "iUserControl",
            "iUserControlColorChooser",
            "iUserControlFileChooser",
            "iUserControlTextureChooser",
            "iUserControlMaterialChooser",
            "iUserControlShaderMaterialChooser",
            "iUserControlTreeView",

            "iDialog",
            "iDialogColorChooser",
            "iDialogColorGradient",
            "iDialogDecisionBox",
            "iDialogFileSelect",
            "iDialogGraph",
            "iDialogIndexMenu",
            "iDialogMenu",
            "iDialogMessageBox"};

        stream << text[static_cast<int>(widgetType)].getData();
        return stream;
    }

    void iWidget::onDragEnter(iDrag &drag)
    {
        // nothing to do
    }

    void iWidget::onDragMove(iDrag &drag, const iaVector2f &mousePos)
    {
        // nothing to do
    }

    void iWidget::onDragLeave(iDrag &drag)
    {
        drag.clear();
    }

    void iWidget::onDrop(const iDrag &drag, const iaVector2f &mousePos)
    {
        // nothing to do
    }

    void iWidget::onDrag()
    {
        // noting to do
    }

    void iWidget::setOverlayEnabled(bool overlay)
    {
        if (_overlay == overlay)
        {
            return;
        }

        _overlay = overlay;

        if (_overlay)
        {
            iWidgetManager::getInstance().registerOverlayWidget(this);
        }
        else
        {
            iWidgetManager::getInstance().unregisterOverlayWidget(this);
        }
    }

    bool iWidget::isOverlayEnabled() const
    {
        return _overlay;
    }

    void iWidget::setCursor(iMouseCursorType cursorType)
    {
        iWidgetManager::getInstance().setCursor(cursorType);
    }

    bool iWidget::isVisible() const
    {
        // parent overrides local visibility if invisible
        if (hasParent() &&
            !getParent()->isVisible())
        {
            return false;
        }

        return _visible;
    }

    uint64 iWidget::getID() const
    {
        return _id;
    }

    bool iWidget::isBlocked() const
    {
        return _blockedEvents;
    }

    iWidgetPtr iWidget::getParent() const
    {
        return _parent;
    }

    iWidgetID iWidget::getParentID() const
    {
        if (_parent != nullptr)
        {
            return _parent->_id;
        }
        else
        {
            return iWidget::INVALID_WIDGET_ID;
        }
    }

    bool iWidget::hasKeyboardFocus() const
    {
        return (_keyboardFocus == this) ? true : false;
    }

    iWidgetState iWidget::getState() const
    {
        return _widgetState;
    }

    bool iWidget::hasParent() const
    {
        return (_parent != nullptr) ? true : false;
    }

    int32 iWidget::getConfiguredMinWidth() const
    {
        return _configuredMinWidth;
    }

    int32 iWidget::getConfiguredMinHeight() const
    {
        return _configuredMinHeight;
    }

    int32 iWidget::getActualWidth() const
    {
        return _actualWidth;
    }

    int32 iWidget::getActualHeight() const
    {
        return _actualHeight;
    }

    int32 iWidget::getMinWidth() const
    {
        return _minWidth;
    }

    int32 iWidget::getMinHeight() const
    {
        return _minHeight;
    }

    iaVector2f iWidget::getActualPos() const
    {
        return iaVector2f(_absoluteX, _absoluteY);
    }

    int32 iWidget::getActualPosX() const
    {
        return _absoluteX;
    }

    int32 iWidget::getActualPosY() const
    {
        return _absoluteY;
    }

    iaRectanglef iWidget::getActualRect() const
    {
        return iaRectanglef(_absoluteX, _absoluteY, _actualWidth, _actualHeight);
    }

    int32 iWidget::getRelativePosX() const
    {
        return _relativeX;
    }

    int32 iWidget::getRelativePosY() const
    {
        return _relativeY;
    }

    bool iWidget::isEnabled() const
    {
        if (_parent != nullptr)
        {
            if (!_parent->isEnabled())
            {
                return false;
            }
        }

        return _enabled;
    }

    void iWidget::setMultiSelection(bool enabled)
    {
        _isMultiSelectionEnabled = enabled;
    }

    bool iWidget::isMultiSelectionEnabled() const
    {
        return _isMultiSelectionEnabled;
    }

    void iWidget::select()
    {
        if (!isSelectable())
        {
            return;
        }

        auto parent = getParent();

        if (parent != nullptr &&
            !parent->isMultiSelectionEnabled())
        {
            parent->clearSelection();
        }

        _selected = true;

        if (parent != nullptr)
        {
            parent->_selectionChanged(parent);
        }
    }

    void iWidget::unselect()
    {
        if (!isSelectable())
        {
            return;
        }

        _selected = false;

        auto parent = getParent();
        if (parent != nullptr)
        {
            parent->_selectionChanged(parent);
        }
    }

    bool iWidget::isSelected() const
    {
        return _selected;
    }

    void iWidget::clearSelection()
    {
        for (auto child : getChildren())
        {
            child->_selected = false;
        }

        _selectionChanged(this);
    }

    const std::vector<iWidgetPtr> iWidget::getSelection() const
    {
        std::vector<iWidgetPtr> selection;

        for (auto child : getChildren())
        {
            if (child->isSelected())
            {
                selection.push_back(child);
            }
        }

        return selection;
    }

    void iWidget::setSelection(const std::vector<iWidgetPtr> &selection)
    {
        for (auto child : getChildren())
        {
            for (auto other : selection)
            {
                if (child == other)
                {
                    child->_selected = true;
                }
            }
        }

        _selectionChanged(this);
    }

    void iWidget::setSelectable(bool selectable)
    {
        _isSelectable = selectable;
    }

    bool iWidget::isSelectable() const
    {
        return _isSelectable;
    }

    void iWidget::refresh()
    {
        _needRefresh = true;
    }

    void iWidget::onRefresh()
    {
        // does nothing
    }

} // namespace igor