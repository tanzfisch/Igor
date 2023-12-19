// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/iWidgetManager.h>

#include <igor/ui/dialogs/iDialog.h>
#include <igor/ui/layouts/iWidgetDockingLayout.h>

#include <cstdlib>

namespace igor
{

    iWidgetManager::iWidgetManager()
    {
    }

    iWidgetManager::~iWidgetManager()
    {
        while (!_forDeletion.empty())
        {
            flushDeleteQueue();
        }

        if (!_widgets.empty())
        {
            con_warn("possible memory leak! did not release all widgets. " << _widgets.size() << " left");

            for (auto pair : _widgets)
            {
                auto widget = pair.second;
                con_debug(widget->getInfo());
            }
        }
    }

    void iWidgetManager::registerWidget(iWidgetPtr widget)
    {
        _widgets[widget->getID()] = widget;

        if (widget->getWidgetKind() == iWidgetKind::Dialog)
        {
            registerDialog(static_cast<iDialogPtr>(widget));
        }
    }

    void iWidgetManager::unregisterWidget(iWidgetPtr widget)
    {
        if (widget->getWidgetKind() == iWidgetKind::Dialog)
        {
            unregisterDialog(static_cast<iDialogPtr>(widget));
        }

        auto iter = _widgets.find(widget->getID());
        if (iter != _widgets.end())
        {
            _widgets.erase(iter);
        }
    }

    iWidgetPtr iWidgetManager::getWidget(iWidgetID id) const
    {
        auto iter = _widgets.find(id);

        if (iter != _widgets.end())
        {
            return (*iter).second;
        }
        return nullptr;
    }

    iDialogPtr iWidgetManager::getModal() const
    {
        return _modal;
    }

    void iWidgetManager::putDialogInFront(iDialogPtr dialog)
    {
        if (!dialog->isMoveable())
        {
            return;
        }

        for (auto pair : _dialogs)
        {
            if (dialog == pair.second)
            {
                pair.second->setZValue(0);
                continue;
            }

            pair.second->setZValue(pair.second->getZValue() + 1);
        }
    }

    void iWidgetManager::registerDialog(iDialogPtr dialog)
    {
        _dialogs[dialog->getID()] = dialog;
        putDialogInFront(dialog);
    }

    void iWidgetManager::unregisterDialog(iDialogPtr dialog)
    {
        auto iter = _dialogs.find(dialog->getID());
        if (iter != _dialogs.end())
        {
            _dialogs.erase(iter);
        }
    }

    void iWidgetManager::registerOverlayWidget(iWidgetPtr overlayWidget)
    {
        con_assert(std::find(_overlayWidgets.begin(), _overlayWidgets.end(), overlayWidget->getID()) == _overlayWidgets.end(), "can't add a second time");
        _overlayWidgets.push_back(overlayWidget->getID());
    }

    void iWidgetManager::unregisterOverlayWidget(iWidgetPtr overlayWidget)
    {
        auto iter = std::find(_overlayWidgets.begin(), _overlayWidgets.end(), overlayWidget->getID());
        if (iter != _overlayWidgets.end())
        {
            _overlayWidgets.erase(iter);
        }
    }

    void iWidgetManager::showTooltip(const iaVector2f &pos, const iaString &text)
    {
        _tooltipPos = pos;
        _tooltipText = text;

        float32 textWidth = std::min(300.0f, getTheme()->getFont()->measureWidth(text, getTheme()->getFontSize()));
        float32 textHeight = getTheme()->getFont()->measureHeight(text, getTheme()->getFontSize(), textWidth);

        if (_tooltipPos._x + textWidth > _desktopWidth)
        {
            _tooltipPos._x -= textWidth;
            _tooltipPos._x -= 30;
        }

        if (_tooltipPos._y + textHeight > _desktopHeight)
        {
            _tooltipPos._y -= textHeight;
            _tooltipPos._y -= 30;
        }
    }

    void iWidgetManager::hideTooltip()
    {
        _tooltipText = "";
    }

    bool iWidgetManager::isModal(iDialogPtr dialog)
    {
        return (_modal == dialog) ? true : false;
    }

    void iWidgetManager::setModal(iDialogPtr dialog)
    {
        con_assert_sticky(_modal == nullptr, "an other dialog is alsready modal");
        _modal = dialog;
    }

    void iWidgetManager::closeDialog(iDialogPtr dialog)
    {
        con_assert(dialog != nullptr, "zero pointer");

        if (dialog == nullptr)
        {
            con_err("zero pointer");
            return;
        }

        _dialogsToClose.insert(dialog->getID());
    }

    void iWidgetManager::resetModal()
    {
        if (_modal == nullptr)
        {
            return;
        }

        auto pos = _modal->getLastMousePos();
        _modal = nullptr;
    }

    void iWidgetManager::getActiveDialogs(std::vector<iDialogPtr> &dialogs, bool sortedAscending)
    {
        for (auto dialog : _dialogs)
        {
            if (dialog.second->isEnabled() &&
                !dialog.second->isDocked())
            {
                dialogs.push_back(dialog.second);
            }
        }

        if (sortedAscending)
        {
            std::sort(dialogs.begin(), dialogs.end(), [](iDialogPtr const a, iDialogPtr const b)
                      { return a->getZValue() < b->getZValue(); });
        }
        else
        {
            std::sort(dialogs.begin(), dialogs.end(), [](iDialogPtr const a, iDialogPtr const b)
                      { return a->getZValue() > b->getZValue(); });
        }
    }

    void iWidgetManager::onUpdate()
    {
        bool refreshMousePos = false;

        for (auto id : _dialogsToClose)
        {
            auto dialog = getDialog(id);
            if (dialog != nullptr &&
                dialog->_dialogCloseDelegate.isValid()) // TODO this is ugly!
            {
                dialog->_dialogCloseDelegate(dialog);
                refreshMousePos = true;
            }
        }

        _dialogsToClose.clear();

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, false);

        for (auto dialog : dialogs)
        {
            // figure out all sizes of things bottom up
            traverseContentSize(dialog);

            // align children with their parents top down
            traverseAlignment(dialog, 0, 0, getDesktopWidth(), getDesktopHeight());
        }

        for (auto pair : _widgets)
        {
            pair.second->onUpdate();
        }

        applyCursor();

        flushDeleteQueue();
    }

    void iWidgetManager::flushDeleteQueue()
    {
        std::set<iWidgetPtr> forDeletion = std::move(_forDeletion);

        for (auto widget : forDeletion)
        {
            if (_widgets.find(widget->getID()) == _widgets.end())
            {
                continue;
            }

            delete widget;
        }
    }

    void iWidgetManager::deleteWidget(iWidgetPtr widget)
    {
        con_assert(!widget->hasParent(), "can't have parent");

        _forDeletion.insert(widget);
    }

    void iWidgetManager::traverseContentSize(iWidgetPtr widget)
    {
        if (widget == nullptr)
        {
            return;
        }

        for (const auto child : widget->_children)
        {
            traverseContentSize(child);
        }

        widget->calcMinSize();
    }

    void iWidgetManager::traverseAlignment(iWidgetPtr widget, int32 offsetX, int32 offsetY, int32 clientRectWidth, int32 clientRectHeight)
    {
        if (widget == nullptr)
        {
            return;
        }

        widget->updateAlignment(clientRectWidth, clientRectHeight);
        widget->updatePosition(offsetX, offsetY);

        std::vector<iaRectanglef> offsets;
        widget->calcChildOffsets(offsets);
        con_assert(offsets.size() == widget->_children.size(), "inconsistant data");

        uint32 index = 0;

        for (const auto child : widget->_children)
        {
            traverseAlignment(child, widget->getActualPosX() + offsets[index].getX(), widget->getActualPosY() + offsets[index].getY(), offsets[index].getWidth(), offsets[index].getHeight());
            index++;
        }
    }

    void iWidgetManager::setDesktopDimensions(uint32 width, uint32 height)
    {
        _desktopWidth = width;
        _desktopHeight = height;
    }

    uint32 iWidgetManager::getDesktopWidth() const
    {
        return _desktopWidth;
    }

    uint32 iWidgetManager::getDesktopHeight() const
    {
        return _desktopHeight;
    }

    iWidgetThemePtr iWidgetManager::getTheme() const
    {
        return _currentTheme;
    }

    void iWidgetManager::setTheme(const iWidgetThemePtr &theme)
    {
        _currentTheme = theme;
    }

    void iWidgetManager::endDrag()
    {
        _drag.reset();
    }

    void iWidgetManager::beginDrag(const iDrag &drag)
    {
        if (_drag != nullptr)
        {
            con_warn("last drag wasn't clean");
        }

        _drag = std::make_unique<iDrag>(drag);
    }

    const iDrag &iWidgetManager::getDrag() const
    {
        con_assert_sticky(_drag != nullptr, "test with inDrag before using getDrag");

        return *_drag.get();
    }

    bool iWidgetManager::inDrag() const
    {
        return _drag != nullptr;
    }

    void iWidgetManager::draw()
    {
        con_assert(_currentTheme != nullptr, "no theme defined");

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, false);

        for (const auto dialog : dialogs)
        {
            dialog->draw();
        }

        if (!_tooltipText.isEmpty())
        {
            _currentTheme->drawTooltip(_tooltipPos, _tooltipText);
        }

        for (const auto widgetID : _overlayWidgets)
        {
            iWidgetPtr widget = getWidget(widgetID);
            if (widget == nullptr)
            {
                continue;
            }

            widget->drawOverlay();
        }
    }

    iDialogPtr iWidgetManager::getDialog(iWidgetID id)
    {
        auto iter = _dialogs.find(id);

        if (iter != _dialogs.end())
        {
            return (*iter).second;
        }
        return nullptr;
    } // see copyright notice in corresponding header file

    void iWidgetManager::onEvent(iEvent &event)
    {
        event.dispatch<iEventMouseKeyDown>(IGOR_BIND_EVENT_FUNCTION(iWidgetManager::onMouseKeyDownEvent));
        event.dispatch<iEventMouseKeyUp>(IGOR_BIND_EVENT_FUNCTION(iWidgetManager::onMouseKeyUpEvent));
        event.dispatch<iEventMouseKeyDoubleClick>(IGOR_BIND_EVENT_FUNCTION(iWidgetManager::onMouseKeyDoubleClickEvent));
        event.dispatch<iEventMouseMove>(IGOR_BIND_EVENT_FUNCTION(iWidgetManager::onMouseMoveEvent));
        event.dispatch<iEventMouseWheel>(IGOR_BIND_EVENT_FUNCTION(iWidgetManager::onMouseWheelEvent));
        event.dispatch<iEventKeyASCII>(IGOR_BIND_EVENT_FUNCTION(iWidgetManager::onKeyASCIIInput));
        event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(iWidgetManager::onKeyDown));
        event.dispatch<iEventKeyUp>(IGOR_BIND_EVENT_FUNCTION(iWidgetManager::onKeyUp));
        event.dispatch<iEventWindowResize>(IGOR_BIND_EVENT_FUNCTION(iWidgetManager::onWindowResize));

        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->onEvent(event);
            return;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, true);

        for (auto dialog : dialogs)
        {
            dialog->onEvent(event);
        }
    }

    bool iWidgetManager::onWindowResize(iEventWindowResize &event)
    {
        // update the widget managers desktop dimensions
        setDesktopDimensions(event.getWindow()->getClientWidth(), event.getWindow()->getClientHeight());

        return false;
    }

    bool iWidgetManager::onKeyDown(iEventKeyDown &event)
    {
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->onKeyDown(event);
            return true;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, true);

        for (auto dialog : dialogs)
        {
            if (dialog->onKeyDown(event))
            {
                return true;
            }
        }

        return false;
    }

    bool iWidgetManager::onKeyUp(iEventKeyUp &event)
    {
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->onKeyUp(event);
            return true;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, true);

        for (auto dialog : dialogs)
        {
            if (dialog->onKeyUp(event))
            {
                return true;
            }
        }

        return false;
    }

    bool iWidgetManager::onKeyASCIIInput(iEventKeyASCII &event)
    {
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->onASCII(event);
            return true;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, true);

        for (auto dialog : dialogs)
        {
            if (dialog->onASCII(event))
            {
                return true;
            }
        }

        return false;
    }

    bool iWidgetManager::onMouseKeyDownEvent(iEventMouseKeyDown &event)
    {
        hideTooltip();

        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->onMouseKeyDown(event);
            return true;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, true);

        // let the dialogs handle the event
        for (auto dialog : dialogs)
        {
            if (dialog->onMouseKeyDown(event))
            {
                return true;
            }
        }

        return false;
    }

    bool iWidgetManager::onMouseKeyUpEvent(iEventMouseKeyUp &event)
    {
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->onMouseKeyUp(event);
            endDrag();
            return true;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, true);

        bool consumed = false;

        // let the dialogs handle the event
        for (auto dialog : dialogs)
        {
            if (!dialog->getAcceptOutOfBoundsClicks() &&
                consumed)
            {
                continue;
            }

            if (dialog->onMouseKeyUp(event))
            {
                consumed = true;
            }
        }

        endDrag();
        return consumed;
    }

    bool iWidgetManager::onMouseKeyDoubleClickEvent(iEventMouseKeyDoubleClick &event)
    {
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->onMouseDoubleClick(event);
            return true;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, true);

        // let the dialogs handle the event
        for (auto dialog : dialogs)
        {
            if (dialog->onMouseDoubleClick(event))
            {
                return true;
            }
        }

        return false;
    }

    bool iWidgetManager::onMouseMoveEvent(iEventMouseMove &event)
    {
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->onMouseMove(event);
            return true;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, true);

        for (auto dialog : dialogs)
        {
            dialog->onMouseMove(event);

            if (dialog->_isMouseOver)
            {
                event.consume();
            }
        }

        return event.isConsumed();
    }

    bool iWidgetManager::onMouseWheelEvent(iEventMouseWheel &event)
    {
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->onMouseWheel(event);
            return true;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, true);

        for (auto dialog : dialogs)
        {
            if (dialog->onMouseWheel(event))
            {
                return true;
            }
        }

        return false;
    }

    void iWidgetManager::setCursor(iMouseCursorType cursorType)
    {
        if (!_firstCursor)
        {
            return;
        }

        _firstCursor = false;
        _cursorType = cursorType;
    }

    void iWidgetManager::applyCursor()
    {
        _firstCursor = true;

        if (_lastCursorType == _cursorType)
        {
            return;
        }

        _lastCursorType = _cursorType;
        iMouse::getInstance().setCursor(_lastCursorType);
    }

} // namespace igor
