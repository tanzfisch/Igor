// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/iWidgetManager.h>

#include <igor/ui/dialogs/iDialog.h>

namespace igor
{

    iWidgetManager::iWidgetManager()
    {
    }

    iWidgetManager::~iWidgetManager()
    {
        if (!_widgets.empty())
        {
            con_warn("possible memory leak! did not release all widgets. " << _widgets.size() << " left");

#ifdef IGOR_DEBUG
            for (auto pair : _widgets)
            {
                auto widget = pair.second;
                con_debug(widget->getInfo());

                // to get a better idea which widget this is we also print it's children
                for (const auto &child : widget->getChildren())
                {
                    con_debug(" +-- " << widget->getInfo());
                }
            }
#endif
        }

        // we can not delete widgets here anymore because
        // they might call iWidgetManager::getInstance in the process
        _widgets.clear();
    }

    void iWidgetManager::registerWidget(iWidgetPtr widget)
    {
        _widgets[widget->getID()] = widget;
    }

    void iWidgetManager::unregisterWidget(iWidgetPtr widget)
    {
        auto iter = _widgets.find(widget->getID());
        if (iter != _widgets.end())
        {
            _widgets.erase(iter);
        }
    }

    void iWidgetManager::putDialogInFront(iDialogPtr dialog)
    {
        for (auto pair : _dialogs)
        {
            if (dialog == pair.second)
            {
                continue;
            }

            pair.second->setZValue(pair.second->getZValue() + 1);
        }

        dialog->setZValue(0);
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

        // refresh mouse cursor for the other dialogs
        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs);

        for (auto dialog : dialogs)
        {
            dialog->handleMouseMove(pos);
        }
    }

    void iWidgetManager::getActiveDialogs(std::vector<iDialogPtr> &dialogs, bool sortedAscending)
    {
        for (auto dialog : _dialogs)
        {
            if (dialog.second->isEnabled())
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

    bool iWidgetManager::handleMouseMove(const iaux::iaVector2f &pos)
    {
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->handleMouseMove(pos);
            return true;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, false);

        bool consumed = false;

        for (auto dialog : dialogs)
        {
            dialog->handleMouseMove(pos);

            if (dialog->_isMouseOver)
            {
                consumed = true;
            }
        }

        return consumed;
    }

    void iWidgetManager::onUpdate()
    {
        bool refreshMousePos = false;

        for (auto id : _dialogsToClose)
        {
            auto dialog = getDialog(id);
            if (dialog != nullptr)
            {
                dialog->_dialogCloseDelegate(dialog);
                refreshMousePos = true;
            }
        }

        _dialogsToClose.clear();

        // refresh mouse pos on other dialogs because they have been blocked so far
        if (refreshMousePos)
        {
            const iaVector2i &mousePos = iMouse::getInstance().getPos();
            handleMouseMove(iaVector2f(mousePos._x, mousePos._y));
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, false);

        for (auto dialog : dialogs)
        {
            if (dialog->isEnabled())
            {
                traverseContentSize(dialog);
                traverseAlignment(dialog, 0, 0, getDesktopWidth(), getDesktopHeight());
            }
        }

        if (dialogs.back()->_motionState == iDialogMotionState::Moving &&
            dialogs.back()->isDockable())
        {
            iaRectanglei rect(0, 0, _desktopWidth, _desktopHeight);
            _docker.update(rect, iMouse::getInstance().getPos());
        }
    }

    void iWidgetManager::undockDialog(iWidgetID dialogID)
    {
        con_assert(getDialog(dialogID) != nullptr, "invalid id");

        _docker.undock(dialogID);
    }

    void iWidgetManager::dockDialog(iWidgetID dialogID)
    {
        con_assert(getDialog(dialogID) != nullptr, "invalid id");

        _docker.dock(dialogID);
    }

    void iWidgetManager::traverseContentSize(iWidgetPtr widget)
    {
        if (widget != nullptr)
        {
            iWidgetManager &wm = iWidgetManager::getInstance();

            for (const auto child : widget->_children)
            {
                traverseContentSize(child);
            }

            widget->onHandle();
            widget->calcMinSize();
        }
    }

    void iWidgetManager::traverseAlignment(iWidgetPtr widget, int32 offsetX, int32 offsetY, int32 clientRectWidth, int32 clientRectHeight)
    {
        if (widget != nullptr)
        {
            widget->updateAlignment(clientRectWidth, clientRectHeight);
            widget->updatePosition(offsetX, offsetY);

            std::vector<iaRectanglef> offsets;
            widget->calcChildOffsets(offsets);

            con_assert(offsets.size() == widget->_children.size(), "inconsistant data");

            iWidgetManager &wm = iWidgetManager::getInstance();
            uint32 index = 0;

            for (const auto child : widget->_children)
            {
                traverseAlignment(child, widget->getActualPosX() + offsets[index].getX(), widget->getActualPosY() + offsets[index].getY(), offsets[index].getWidth(), offsets[index].getHeight());
                index++;
            }
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

        if (dialogs.back()->_motionState == iDialogMotionState::Moving &&
            dialogs.back()->isDockable())
        {
            _docker.draw();
        }

        // _docker.drawDebug();
    }

    iDialogPtr iWidgetManager::getDialog(iWidgetID id)
    {
        auto iter = _dialogs.find(id);

        if (iter != _dialogs.end())
        {
            return (*iter).second;
        }
        return nullptr;
    }

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
    }

    bool iWidgetManager::onWindowResize(iEventWindowResize &event)
    {
        // update the widget managers desktop dimensions
        setDesktopDimensions(event.getWindow()->getClientWidth(), event.getWindow()->getClientHeight());

        iaRectanglei rect(0, 0, _desktopWidth, _desktopHeight);
        _docker.update(rect, iMouse::getInstance().getPos());

        return false;
    }

    bool iWidgetManager::onKeyDown(iEventKeyDown &event)
    {
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->handleKeyDown(event.getKey());
            return true;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, true);

        for (auto dialog : dialogs)
        {
            if (dialog->handleKeyDown(event.getKey()))
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
            getModal()->handleKeyUp(event.getKey());
            return true;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, true);

        for (auto dialog : dialogs)
        {
            if (dialog->handleKeyUp(event.getKey()))
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
            getModal()->handleASCII(event.getChar());
            return true;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, true);

        for (auto dialog : dialogs)
        {
            if (dialog->handleASCII(event.getChar()))
            {
                return true;
            }
        }

        return false;
    }

    bool iWidgetManager::onMouseKeyDownEvent(iEventMouseKeyDown &event)
    {
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->handleMouseKeyDown(event.getKey());
            return true;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, true);

        // let the dialogs handle the event
        for (auto dialog : dialogs)
        {
            if (dialog->handleMouseKeyDown(event.getKey()))
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
            getModal()->handleMouseKeyUp(event.getKey());
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

            if (dialog->handleMouseKeyUp(event.getKey()))
            {
                consumed = true;
            }
        }

        return consumed;
    }

    bool iWidgetManager::onMouseKeyDoubleClickEvent(iEventMouseKeyDoubleClick &event)
    {
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->handleMouseDoubleClick(event.getKey());
            return true;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, true);

        // let the dialogs handle the event
        for (auto dialog : dialogs)
        {
            if (dialog->handleMouseDoubleClick(event.getKey()))
            {
                return true;
            }
        }

        return false;
    }

    bool iWidgetManager::onMouseMoveEvent(iEventMouseMove &event)
    {
        const iaVector2i &pos = event.getPosition();

        if (handleMouseMove(iaVector2f(pos._x, pos._y)))
        {
            return true;
        }

        return false;
    }

    bool iWidgetManager::onMouseWheelEvent(iEventMouseWheel &event)
    {
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->handleMouseWheel(event.getWheelDelta());
            return true;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, true);

        for (auto dialog : dialogs)
        {
            if (dialog->handleMouseWheel(event.getWheelDelta()))
            {
                return true;
            }
        }

        return false;
    }

} // namespace igor
