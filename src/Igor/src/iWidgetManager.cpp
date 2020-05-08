// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetManager.h>

#include <iDialog.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetGroupBox.h>
#include <iWidgetGrid.h>
#include <iWidgetCheckBox.h>
#include <iWidgetNumberChooser.h>
#include <iWidgetTextEdit.h>
#include <iWidgetPicture.h>
#include <iWidgetScroll.h>
#include <iWidgetSpacer.h>
#include <iWidgetColor.h>
#include <iWidgetColorGradient.h>
#include <iWidgetSlider.h>
#include <iWidgetSelectBox.h>
#include <iKeyboard.h>
#include <iApplication.h>
#include <iWidgetGraph.h>
#include <iWidgetBaseTheme.h>
#include <iTextureFont.h>

#include <iDialogColorChooser.h>
#include <iDialogDecisionBox.h>
#include <iDialogIndexMenu.h>
#include <iDialogMessageBox.h>
#include <iDialogColorGradient.h>
#include <iDialogGraph.h>

#include <iUserControlColorChooser.h>
#include <iUserControlFileChooser.h>

#include <iMaterialResourceFactory.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <algorithm>

namespace Igor
{

    iWidgetManager::iWidgetManager()
    {
        registerHandles();
    }

    iWidgetManager::~iWidgetManager()
    {
        unregisterHandles();

        if (!_widgets.empty())
        {
            con_err("possible mem leak! did not release all widgets. " << _widgets.size() << " left");

#ifdef __IGOR_DEBUG__
            for (auto pair : _widgets)
            {
                auto widget = pair.second;
                con_debug_endl(widget->getInfo());

                // to get a better idea which widget this is we also print it's children
                std::vector<iWidgetPtr> children;
                widget->getChildren(children);
                for (auto child : children)
                {
                    con_debug_endl(" +-- " << widget->getInfo());
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

    void iWidgetManager::registerDialog(iDialogPtr dialog)
    {
        _dialogs[dialog->getID()] = dialog;
    }

    void iWidgetManager::unregisterDialog(iDialogPtr dialog)
    {
        auto iter = _dialogs.find(dialog->getID());
        if (iter != _dialogs.end())
        {
            _dialogs.erase(iter);
        }
    }

    void iWidgetManager::showTooltip(const iaVector2i& pos, const iaString& text)
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
        con_assert(_modal == nullptr, "an other dialog is alsready modal");

        if (_modal == nullptr)
        {
            _modal = dialog;
        }
        else
        {
            con_err("an other dialog is already modal");
        }
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

    void iWidgetManager::registerHandles()
    {
        iMouse::getInstance().registerMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &iWidgetManager::onMouseKeyDown));
        iMouse::getInstance().registerMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &iWidgetManager::onMouseKeyUp));
        iMouse::getInstance().registerMouseDoubleClickDelegate(iMouseKeyDoubleClickDelegate(this, &iWidgetManager::onMouseDoubleClick));
        iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &iWidgetManager::onMouseMove));
        iMouse::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &iWidgetManager::onMouseWheel));
        iKeyboard::getInstance().registerKeyASCIIDelegate(iKeyASCIIDelegate(this, &iWidgetManager::onASCII));
        iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownDelegate(this, &iWidgetManager::onKeyDown));
        iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &iWidgetManager::onKeyUp));

        iApplication::getInstance().registerApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &iWidgetManager::onPreDraw));
        iApplication::getInstance().registerApplicationPostDrawHandleDelegate(iApplicationPostDrawHandleDelegate(this, &iWidgetManager::onPostDraw));
    }

    void iWidgetManager::unregisterHandles()
    {
        iMouse::getInstance().unregisterMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &iWidgetManager::onMouseKeyDown));
        iMouse::getInstance().unregisterMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &iWidgetManager::onMouseKeyUp));
        iMouse::getInstance().unregisterMouseDoubleClickDelegate(iMouseKeyDoubleClickDelegate(this, &iWidgetManager::onMouseDoubleClick));
        iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &iWidgetManager::onMouseMove));
        iMouse::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &iWidgetManager::onMouseWheel));
        iKeyboard::getInstance().unregisterKeyASCIIDelegate(iKeyASCIIDelegate(this, &iWidgetManager::onASCII));
        iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownDelegate(this, &iWidgetManager::onKeyDown));
        iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &iWidgetManager::onKeyUp));

        iApplication::getInstance().unregisterApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &iWidgetManager::onPreDraw));
        iApplication::getInstance().unregisterApplicationPostDrawHandleDelegate(iApplicationPostDrawHandleDelegate(this, &iWidgetManager::onPostDraw));
    }

    void iWidgetManager::getActiveDialogs(std::vector<iDialogPtr>& dialogs, bool sortedAccending)
    {
        dialogs.clear();

        for (auto dialog : _dialogs)
        {
            if (dialog.second->isActive())
            {
                dialogs.push_back(dialog.second);
            }
        }

        if (sortedAccending)
        {
            std::sort(dialogs.begin(), dialogs.end(), [](iDialogPtr const a, iDialogPtr const b) { return a->getZValue() < b->getZValue(); });
        }
        else
        {
            std::sort(dialogs.begin(), dialogs.end(), [](iDialogPtr const a, iDialogPtr const b) { return a->getZValue() > b->getZValue(); });
        }
    }

    void iWidgetManager::onKeyDown(iKeyCode key)
    {
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->handleKeyDown(key);
            return;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, false);

        for (auto dialog : dialogs)
        {
            if (dialog->handleKeyDown(key))
            {
                return;
            }
        }

        _keyDownEvent(key);
    }

    void iWidgetManager::onKeyUp(iKeyCode key)
    {
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->handleKeyUp(key);
            return;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, false);

        for (auto dialog : dialogs)
        {
            if (dialog->handleKeyUp(key))
            {
                return;
            }
        }

        _keyUpEvent(key);
    }

    void iWidgetManager::onMouseKeyDown(iKeyCode key)
    {
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->handleMouseKeyDown(key);
            return;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, false);

        // let the dialogs handle the event
        for (auto dialog : dialogs)
        {
            if (dialog->handleMouseKeyDown(key))
            {
                return;
            }
        }

        _mouseKeyDownEvent(key);
    }

    void iWidgetManager::onMouseKeyUp(iKeyCode key)
    {
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->handleMouseKeyUp(key);
            return;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, false);

        // let the dialogs handle the event
        for (auto dialog : dialogs)
        {
            if (dialog->handleMouseKeyUp(key))
            {
                return;
            }
        }

        _mouseKeyUpEvent(key);
    }

    void iWidgetManager::onMouseDoubleClick(iKeyCode key)
    {
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->handleMouseDoubleClick(key);
            return;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, false);

        // let the dialogs handle the event
        for (auto dialog : dialogs)
        {
            if (dialog->handleMouseDoubleClick(key))
            {
                return;
            }
        }

        _doubleClickEvent(key);
    }

    void iWidgetManager::onMouseMove(const iaVector2i& from, const iaVector2i& to, iWindow* window)
    {
        if (handleMouseMove(to))
        {
            return;
        }

        _moveFullEvent(from, to, window);
        _moveEvent(to);
    }

    bool iWidgetManager::handleMouseMove(const IgorAux::iaVector2i& to)
    {
        
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->handleMouseMove(to);
            return true;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, false);

        if (!dialogs.empty())
        {
            int32 zValue = dialogs.front()->getZValue();
            // only have the top level dialogs with the 
            // same z value get the mouse move event
            for (auto dialog : dialogs)
            {
                if (dialog->getZValue() < zValue)
                {
                    return true;
                }

                dialog->handleMouseMove(to);
            }
        }

        return false;
    }

    void iWidgetManager::onMouseWheel(int32 d)
    {
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->handleMouseWheel(d);
            return;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, false);

        for (auto dialog : dialogs)
        {
            if (dialog->handleMouseWheel(d))
            {
                return;
            }
        }

        _wheelEvent(d);
    }

    void iWidgetManager::onASCII(const char c)
    {
        // if there is a modal dialog handle only that one
        if (getModal() != nullptr)
        {
            getModal()->handleASCII(c);
            return;
        }

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs, false);

        for (auto dialog : dialogs)
        {
            dialog->handleASCII(c);
        }
    }

    void iWidgetManager::onPostDraw()
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
            auto pos = iMouse::getInstance().getPos();
            handleMouseMove(pos);
        }
    }

    void iWidgetManager::onPreDraw()
    {
        for (auto dialog : _dialogs)
        {
            if (dialog.second->isActive())
            {
                traverseContentSize(dialog.second);
                traverseAlignment(dialog.second, 0, 0, getDesktopWidth(), getDesktopHeight());
            }
        }
    }

    void iWidgetManager::traverseContentSize(iWidgetPtr widget)
    {
        if (widget != nullptr)
        {
            iWidgetManager& wm = iWidgetManager::getInstance();

            for (const auto child : widget->_children)
            {
                traverseContentSize(child);
            }

            widget->calcMinSize();
        }
    }

    void iWidgetManager::traverseAlignment(iWidgetPtr widget, int32 offsetX, int32 offsetY, int32 clientRectWidth, int32 clientRectHeight)
    {
        if (widget != nullptr)
        {
            widget->updateAlignment(clientRectWidth, clientRectHeight);
            widget->updatePosition(offsetX, offsetY);

            std::vector<iRectanglei> offsets;
            widget->calcChildOffsets(offsets);

            con_assert(offsets.size() == widget->_children.size(), "inconsistant data");

            iWidgetManager& wm = iWidgetManager::getInstance();
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

    iWidgetBaseTheme* iWidgetManager::getTheme()
    {
        return _currentTheme;
    }

    void iWidgetManager::setTheme(iWidgetBaseTheme* theme)
    {
        _currentTheme = theme;
    }

    void iWidgetManager::draw()
    {
        con_assert(_currentTheme != nullptr, "no theme defined");

        std::vector<iDialogPtr> dialogs;
        getActiveDialogs(dialogs);

        for (const auto dialog : dialogs)
        {
            if (!isModal(dialog))
            {
                dialog->draw();
            }
        }

        if (_modal != nullptr)
        {
            _modal->draw();
        }

        if (!_tooltipText.isEmpty())
        {
            _currentTheme->drawTooltip(_tooltipPos, _tooltipText);
        }
    }

    iDialogPtr iWidgetManager::getDialog(uint64 id)
    {
        auto iter = _dialogs.find(id);

        if (iter != _dialogs.end())
        {
            return (*iter).second;
        }
        return nullptr;
    }

    void iWidgetManager::registerMouseDoubleClickDelegate(iMouseKeyDoubleClickDelegate doubleClickDelegate)
    {
        _doubleClickEvent.append(doubleClickDelegate);
    }

    void iWidgetManager::unregisterMouseDoubleClickDelegate(iMouseKeyDoubleClickDelegate doubleClickDelegate)
    {
        _doubleClickEvent.remove(doubleClickDelegate);
    }

    void iWidgetManager::registerMouseKeyDownDelegate(iMouseKeyDownDelegate keydown_delegate)
    {
        _mouseKeyDownEvent.append(keydown_delegate);
    }

    void iWidgetManager::registerMouseKeyUpDelegate(iMouseKeyUpDelegate keyup_delegate)
    {
        _mouseKeyUpEvent.append(keyup_delegate);
    }

    void iWidgetManager::registerMouseMoveDelegate(iMouseMoveDelegate move_delegate)
    {
        _moveEvent.append(move_delegate);
    }

    void iWidgetManager::unregisterMouseMoveDelegate(iMouseMoveDelegate move_delegate)
    {
        _moveEvent.remove(move_delegate);
    }

    void iWidgetManager::registerMouseMoveFullDelegate(iMouseMoveFullDelegate move_delegate)
    {
        _moveFullEvent.append(move_delegate);
    }

    void iWidgetManager::registerMouseWheelDelegate(iMouseWheelDelegate wheel_delegate)
    {
        _wheelEvent.append(wheel_delegate);
    }

    void iWidgetManager::unregisterMouseKeyDownDelegate(iMouseKeyDownDelegate keydown_delegate)
    {
        _mouseKeyDownEvent.remove(keydown_delegate);
    }

    void iWidgetManager::unregisterMouseKeyUpDelegate(iMouseKeyUpDelegate keyup_delegate)
    {
        _mouseKeyUpEvent.remove(keyup_delegate);
    }

    void iWidgetManager::unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate move_delegate)
    {
        _moveFullEvent.remove(move_delegate);
    }

    void iWidgetManager::unregisterMouseWheelDelegate(iMouseWheelDelegate wheel_delegate)
    {
        _wheelEvent.remove(wheel_delegate);
    }

    void iWidgetManager::registerKeyDownDelegate(iKeyDownDelegate delegate)
    {
        _keyDownEvent.append(delegate);
    }

    void iWidgetManager::unregisterKeyDownDelegate(iKeyDownDelegate delegate)
    {
        _keyDownEvent.remove(delegate);
    }

    void iWidgetManager::registerKeyUpDelegate(iKeyUpDelegate delegate)
    {
        _keyUpEvent.append(delegate);
    }

    void iWidgetManager::unregisterKeyUpDelegate(iKeyUpDelegate delegate)
    {
        _keyUpEvent.remove(delegate);
    }

}

