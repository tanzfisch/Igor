// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetManager.h>

#include <iWidgetDialog.h>
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

#include <iMaterialResourceFactory.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iWidgetDialog* iWidgetManager::_modal = nullptr;

    iWidgetManager::iWidgetManager()
    {
        iApplication::getInstance().registerApplicationHandleDelegate(iApplicationHandleDelegate(this, &iWidgetManager::onHandle));
    }

    iWidgetManager::~iWidgetManager()
    {
        iApplication::getInstance().unregisterApplicationHandleDelegate(iApplicationHandleDelegate(this, &iWidgetManager::onHandle));

        destroyWidgets();

        if (_widgets.size() != 0)
        {
            con_err("possible mem leak! did not release all widgets. " << _widgets.size() << " left");
        }

        for (auto iter : _widgets)
        {
            delete iter.second;
        }

        _widgets.clear();
    }

    bool iWidgetManager::isModal(iWidgetDialog* dialog)
    {
        return (_modal == dialog) ? true : false;
    }

    void iWidgetManager::setModal(iWidgetDialog* dialog)
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

    iWidgetDialog* iWidgetManager::getModal()
    {
        return _modal;
    }

    void iWidgetManager::resetModal()
    {
        _modal = nullptr;
    }

    void iWidgetManager::registerIOEvents()
    {
        iMouse::getInstance().registerMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &iWidgetManager::onMouseKeyDown));
        iMouse::getInstance().registerMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &iWidgetManager::onMouseKeyUp));
        iMouse::getInstance().registerMouseDoubleClickDelegate(iMouseKeyDoubleClickDelegate(this, &iWidgetManager::onMouseDoubleClick));
        iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &iWidgetManager::onMouseMove));
        iMouse::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &iWidgetManager::onMouseWheel));
        iKeyboard::getInstance().registerKeyASCIIDelegate(iKeyASCIIDelegate(this, &iWidgetManager::onASCII));
    }

    void iWidgetManager::unregisterIOEvents()
    {
        iMouse::getInstance().unregisterMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &iWidgetManager::onMouseKeyDown));
        iMouse::getInstance().unregisterMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &iWidgetManager::onMouseKeyUp));
        iMouse::getInstance().unregisterMouseDoubleClickDelegate(iMouseKeyDoubleClickDelegate(this, &iWidgetManager::onMouseDoubleClick));
        iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &iWidgetManager::onMouseMove));
        iMouse::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &iWidgetManager::onMouseWheel));
        iKeyboard::getInstance().unregisterKeyASCIIDelegate(iKeyASCIIDelegate(this, &iWidgetManager::onASCII));
    }

    void iWidgetManager::onMouseKeyDown(iKeyCode key)
    {
        bool consumed = false;

        // this copy is not because of a race condition but because the original list might be changed while handling the event
        vector<iWidgetDialog*> dialogs = _dialogs;

        for (auto dialog : dialogs)
        {
            if (isModal(dialog))
            {
                dialog->handleMouseKeyDown(key);
                consumed = true;
                break;
            }
        }

        if (!consumed)
        {
            for (auto dialog : dialogs)
            {
                if (dialog->handleMouseKeyDown(key))
                {
                    consumed = true;
                    break;
                }
            }
        }

        if (!consumed)
        {
            _keyDownEvent(key);
        }
    }

    void iWidgetManager::onMouseKeyUp(iKeyCode key)
    {
        bool consumed = false;

        // this copy is not because of a race condition but because the original list might be changed while handling the event
        vector<iWidgetDialog*> dialogs = _dialogs;

        for (auto dialog : dialogs)
        {
            if (isModal(dialog))
            {
                dialog->handleMouseKeyUp(key);
                consumed = true;
                break;
            }
        }

        if (!consumed)
        {
            for (auto dialog : dialogs)
            {
                if (dialog->handleMouseKeyUp(key))
                {
                    consumed = true;
                    break;
                }
            }
        }

        if (!consumed)
        {
            _keyUpEvent(key);
        }
    }

    void iWidgetManager::onMouseDoubleClick(iKeyCode key)
    {
        bool consumed = false;        

        // this copy is not because of a race condition but because the original list might be changed while handling the event
        vector<iWidgetDialog*> dialogs = _dialogs;

        for (auto dialog : dialogs)
        {
            if (isModal(dialog))
            {
                dialog->handleMouseDoubleClick(key);
                consumed = true;
                break;
            }
        }

        if (!consumed)
        {
            for (auto dialog : dialogs)
            {
                if (dialog->handleMouseDoubleClick(key))
                {
                    consumed = true;
                    break;
                }
            }
        }

        if (!consumed)
        {
            _doubleClickEvent(key);
        }
    }

    void iWidgetManager::onMouseMove(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window)
    {
        bool foundModal = false;        

        // this copy is not because of a race condition but because the original list might be changed while handling the event
        vector<iWidgetDialog*> dialogs = _dialogs; 

        for (auto dialog : dialogs)
        {
            if (isModal(dialog))
            {
                dialog->handleMouseMove(x2, y2);
                foundModal = true;
                break;
            }
        }

        if (!foundModal)
        {
            for (auto dialog : dialogs)
            {
                dialog->handleMouseMove(x2, y2);
            }

            _moveFullEvent(x1, y1, x2, y2, window);
            _moveEvent(x2, y2);
        }
    }

    void iWidgetManager::onMouseWheel(int32 d)
    {
        bool consumed = false;

        // this copy is not because of a race condition but because the original list might be changed while handling the event
        vector<iWidgetDialog*> dialogs = _dialogs;

        for (auto dialog : dialogs)
        {
            if (isModal(dialog))
            {
                dialog->handleMouseWheel(d);
                consumed = true;
                break;
            }
        }

        if (!consumed)
        {
            for (auto dialog : dialogs)
            {
                if (dialog->handleMouseWheel(d))
                {
                    consumed = true;
                }
            }
        }

        if (!consumed)
        {
            _wheelEvent(d);
        }
    }

    void iWidgetManager::onASCII(char c)
    {
        bool foundModal = false;

        // this copy is not because of a race condition but because the original list might be changed while handling the event
        vector<iWidgetDialog*> dialogs = _dialogs;

        for (auto dialog : dialogs)
        {
            if (isModal(dialog))
            {
                dialog->handleASCII(c);
                foundModal = true;
                break;
            }
        }

        if (!foundModal)
        {
            for (auto dialog : dialogs)
            {
                dialog->handleASCII(c);
            }
        }
    }

    void iWidgetManager::onHandle()
    {
        for (auto dialog : _dialogs)
        {
            traverseContentSize(dialog);
            traverseAlignment(dialog, 0, 0, getDesktopWidth(), getDesktopHeight());
        }
    }

    void iWidgetManager::traverseContentSize(iWidget* widget)
    {
        if (widget != nullptr)
        {
            for (auto child : widget->_children)
            {
                traverseContentSize(child);
            }

            widget->calcMinSize();
        }
    }

    void iWidgetManager::traverseAlignment(iWidget* widget, int32 offsetX, int32 offsetY, int32 clientRectWidth, int32 clientRectHeight)
    {
        if (widget != nullptr)
        {
            widget->updateAlignment(clientRectWidth, clientRectHeight);
            widget->updatePosition(offsetX, offsetY);

            vector<iRectanglei> offsets;
            widget->calcChildOffsets(offsets);

            con_assert(offsets.size() == widget->_children.size(), "inconsistant data");

            for (int i = 0; i < offsets.size();++i)
            {
                traverseAlignment(widget->_children[i], widget->getActualPosX() + offsets[i].getX(), widget->getActualPosY() + offsets[i].getY(), offsets[i].getWidth(), offsets[i].getHeight());
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

    void iWidgetManager::setTheme(iWidgetBaseTheme *theme)
    {
        _currentTheme = theme;
    }

    void iWidgetManager::draw()
    {
        con_assert(_currentTheme != nullptr, "no theme defined");

        for (auto dialog : _dialogs)
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
    }

    iWidget* iWidgetManager::createWidget(iWidgetType widgetType)
    {
        iWidget* result = nullptr;

        switch (widgetType)
        {
        case iWidgetType::Dialog:
            if (_dialogs.empty())
            {
                registerIOEvents();
            }

            result = new iWidgetDialog();
            _dialogs.push_back(static_cast<iWidgetDialog*>(result));
            break;

        case iWidgetType::Label:
            result = new iWidgetLabel();
            break;

        case iWidgetType::Button:
            result = new iWidgetButton();
            break;

        case iWidgetType::GroupBox:
            result = new iWidgetGroupBox();
            break;

        case iWidgetType::Grid:
            result = new iWidgetGrid();
            break;

        case iWidgetType::CheckBox:
            result = new iWidgetCheckBox();
            break;

        case iWidgetType::NumberChooser:
            result = new iWidgetNumberChooser();
            break;

        case iWidgetType::TextEdit:
            result = new iWidgetTextEdit();
            break;

        case iWidgetType::Picture:
            result = new iWidgetPicture();
            break;

        case iWidgetType::Scroll:
            result = new iWidgetScroll();
            break;

        case iWidgetType::Spacer:
            result = new iWidgetSpacer();
            break;

        case iWidgetType::Color:
            result = new iWidgetColor();
            break;

        case iWidgetType::ColorGradient:
            result = new iWidgetColorGradient();
            break;

        case iWidgetType::Slider:
            result = new iWidgetSlider();
            break;

        case iWidgetType::SelectBox:
            result = new iWidgetSelectBox();
            break;

        case iWidgetType::Graph:
            result = new iWidgetGraph();
            break;

        case iWidgetType::Undefined:
        default:
            con_assert(false, "unexpected widget type");
        }

        _widgets[result->getID()] = result;
        return result;
    }

    iWidget* iWidgetManager::getWidget(uint64 id)
    {
        auto iter = _widgets.find(id);

        if (iter != _widgets.end())
        {
            return (*iter).second;
        }
        return nullptr;
    }

    void iWidgetManager::destroyWidget(uint64 id)
    {
        auto iter = _widgets.find(id);

        if (iter != _widgets.end())
        {
            destroyWidget((*iter).second);
        }
        else
        {
            con_err("widget with id " << id << " does not exist");
        }
    }

    void iWidgetManager::destroyWidget(iWidget* widget)
    {
        con_assert(widget != nullptr, "zero pointer");

        if (widget == nullptr)
        {
            return;
        }

        _toDelete.push_back(widget);
    }

    void iWidgetManager::destroyWidgets()
    {
        for (auto widget : _toDelete)
        {
            if (widget->getType() == iWidgetType::Dialog)
            {
                auto iter = find(_dialogs.begin(), _dialogs.end(), static_cast<iWidgetDialog*>(widget));
                if (iter != _dialogs.end())
                {
                    _dialogs.erase(iter);

                    if (_dialogs.empty())
                    {
                        unregisterIOEvents();
                    }
                }
                else
                {
                    con_err("inconsistent dialog list");
                }
            }

            auto iter = _widgets.find(widget->getID());
            con_assert(iter != _widgets.end(), "possible mem leak. widget does not exist");
            if (iter != _widgets.end())
            {
                delete (*iter).second;
                _widgets.erase(iter);
            }
            else
            {
                con_err("widget does not exist");
            }
        }

        _toDelete.clear();
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
        _keyDownEvent.append(keydown_delegate);
    }

    void iWidgetManager::registerMouseKeyUpDelegate(iMouseKeyUpDelegate keyup_delegate)
    {
        _keyUpEvent.append(keyup_delegate);
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
        _keyDownEvent.remove(keydown_delegate);
    }

    void iWidgetManager::unregisterMouseKeyUpDelegate(iMouseKeyUpDelegate keyup_delegate)
    {
        _keyUpEvent.remove(keyup_delegate);
    }

    void iWidgetManager::unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate move_delegate)
    {
        _moveFullEvent.remove(move_delegate);
    }

    void iWidgetManager::unregisterMouseWheelDelegate(iMouseWheelDelegate wheel_delegate)
    {
        _wheelEvent.remove(wheel_delegate);
    }

}
