// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
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

#include <iDialogColorChooser.h>
#include <iDialogDecisionBox.h>
#include <iDialogFileSelect.h>
#include <iDialogMenu.h>
#include <iDialogMessageBox.h>
#include <iDialogColorGradient.h>
#include <iDialogGraph.h>

#include <iUserControlColorChooser.h>
#include <iUserControlFileChooser.h>

#include <iMaterialResourceFactory.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iDialog* iWidgetManager::_modal = nullptr;

    iWidgetManager::iWidgetManager()
    {
        registerWidgetType("Button", iInstanciateWidgetDelegate(iWidgetButton::createInstance));
        registerWidgetType("CheckBox", iInstanciateWidgetDelegate(iWidgetCheckBox::createInstance));
        registerWidgetType("Color", iInstanciateWidgetDelegate(iWidgetColor::createInstance));
        registerWidgetType("ColorGradient", iInstanciateWidgetDelegate(iWidgetColorGradient::createInstance));
        registerWidgetType("Graph", iInstanciateWidgetDelegate(iWidgetGraph::createInstance));
        registerWidgetType("Grid", iInstanciateWidgetDelegate(iWidgetGrid::createInstance));
        registerWidgetType("GroupBox", iInstanciateWidgetDelegate(iWidgetGroupBox::createInstance));
        registerWidgetType("Label", iInstanciateWidgetDelegate(iWidgetLabel::createInstance));
        registerWidgetType("NumberChooser", iInstanciateWidgetDelegate(iWidgetNumberChooser::createInstance));
        registerWidgetType("Picture", iInstanciateWidgetDelegate(iWidgetPicture::createInstance));
        registerWidgetType("Scroll", iInstanciateWidgetDelegate(iWidgetScroll::createInstance));
        registerWidgetType("SelectBox", iInstanciateWidgetDelegate(iWidgetSelectBox::createInstance));
        registerWidgetType("Slider", iInstanciateWidgetDelegate(iWidgetSlider::createInstance));
        registerWidgetType("Spacer", iInstanciateWidgetDelegate(iWidgetSpacer::createInstance));
        registerWidgetType("TextEdit", iInstanciateWidgetDelegate(iWidgetTextEdit::createInstance));

        registerWidgetType("UserControlColorChooser", iInstanciateWidgetDelegate(iUserControlColorChooser::createInstance));
        registerWidgetType("UserControlFileChooser", iInstanciateWidgetDelegate(iUserControlFileChooser::createInstance));

        registerDialogType("Dialog", iInstanciateDialogDelegate(iDialog::createInstance));
        registerDialogType("DialogColorChooser", iInstanciateDialogDelegate(iDialogColorChooser::createInstance));
        registerDialogType("DialogDecisionBox", iInstanciateDialogDelegate(iDialogDecisionBox::createInstance));
        registerDialogType("DialogFileSelect", iInstanciateDialogDelegate(iDialogFileSelect::createInstance));
        registerDialogType("DialogMenu", iInstanciateDialogDelegate(iDialogMenu::createInstance));
        registerDialogType("DialogMessageBox", iInstanciateDialogDelegate(iDialogMessageBox::createInstance));
        registerDialogType("DialogColorGradient", iInstanciateDialogDelegate(iDialogColorGradient::createInstance));
        registerDialogType("DialogGraph", iInstanciateDialogDelegate(iDialogGraph::createInstance));

        registerHandles();
    }

    iWidgetManager::~iWidgetManager()
    {
        unregisterHandles();

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

    bool iWidgetManager::isModal(iDialog* dialog)
    {
        return (_modal == dialog) ? true : false;
    }

    void iWidgetManager::setModal(iDialog* dialog)
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

    iDialog* iWidgetManager::getModal()
    {
        return _modal;
    }

    void iWidgetManager::resetModal()
    {
        _modal = nullptr;
    }

    void iWidgetManager::registerHandles()
    {
        iMouse::getInstance().registerMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &iWidgetManager::onMouseKeyDown));
        iMouse::getInstance().registerMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &iWidgetManager::onMouseKeyUp));
        iMouse::getInstance().registerMouseDoubleClickDelegate(iMouseKeyDoubleClickDelegate(this, &iWidgetManager::onMouseDoubleClick));
        iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &iWidgetManager::onMouseMove));
        iMouse::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &iWidgetManager::onMouseWheel));
        iKeyboard::getInstance().registerKeyASCIIDelegate(iKeyASCIIDelegate(this, &iWidgetManager::onASCII));

        iApplication::getInstance().registerApplicationHandleDelegate(iApplicationHandleDelegate(this, &iWidgetManager::onHandle));
    }

    void iWidgetManager::unregisterHandles()
    {
        iMouse::getInstance().unregisterMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &iWidgetManager::onMouseKeyDown));
        iMouse::getInstance().unregisterMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &iWidgetManager::onMouseKeyUp));
        iMouse::getInstance().unregisterMouseDoubleClickDelegate(iMouseKeyDoubleClickDelegate(this, &iWidgetManager::onMouseDoubleClick));
        iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &iWidgetManager::onMouseMove));
        iMouse::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &iWidgetManager::onMouseWheel));
        iKeyboard::getInstance().unregisterKeyASCIIDelegate(iKeyASCIIDelegate(this, &iWidgetManager::onASCII));

        iApplication::getInstance().unregisterApplicationHandleDelegate(iApplicationHandleDelegate(this, &iWidgetManager::onHandle));
    }

    void iWidgetManager::onMouseKeyDown(iKeyCode key)
    {
        bool consumed = false;

        // this copy is not because of a race condition but because the original list might be changed while handling the event
        map<uint64, iDialog*> dialogs = _dialogs;

        for (auto dialog : dialogs)
        {
            if (isModal(dialog.second))
            {
                dialog.second->handleMouseKeyDown(key);
                consumed = true;
                break;
            }
        }

        if (!consumed)
        {
            for (auto dialog : dialogs)
            {
                if (dialog.second->handleMouseKeyDown(key))
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
        map<uint64, iDialog*> dialogs = _dialogs;

        for (auto dialog : dialogs)
        {
            if (isModal(dialog.second))
            {
                dialog.second->handleMouseKeyUp(key);
                consumed = true;
                break;
            }
        }

        if (!consumed)
        {
            for (auto dialog : dialogs)
            {
                if (dialog.second->handleMouseKeyUp(key))
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
        map<uint64, iDialog*> dialogs = _dialogs;

        for (auto dialog : dialogs)
        {
            if (isModal(dialog.second))
            {
                dialog.second->handleMouseDoubleClick(key);
                consumed = true;
                break;
            }
        }

        if (!consumed)
        {
            for (auto dialog : dialogs)
            {
                if (dialog.second->handleMouseDoubleClick(key))
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
        map<uint64, iDialog*> dialogs = _dialogs;

        for (auto dialog : dialogs)
        {
            if (isModal(dialog.second))
            {
                dialog.second->handleMouseMove(x2, y2);
                foundModal = true;
                break;
            }
        }

        if (!foundModal)
        {
            for (auto dialog : dialogs)
            {
                dialog.second->handleMouseMove(x2, y2);
            }

            _moveFullEvent(x1, y1, x2, y2, window);
            _moveEvent(x2, y2);
        }
    }

    void iWidgetManager::onMouseWheel(int32 d)
    {
        bool consumed = false;

        // this copy is not because of a race condition but because the original list might be changed while handling the event
        map<uint64, iDialog*> dialogs = _dialogs;

        for (auto dialog : dialogs)
        {
            if (isModal(dialog.second))
            {
                dialog.second->handleMouseWheel(d);
                consumed = true;
                break;
            }
        }

        if (!consumed)
        {
            for (auto dialog : dialogs)
            {
                if (dialog.second->handleMouseWheel(d))
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
        map<uint64, iDialog*> dialogs = _dialogs;

        for (auto dialog : dialogs)
        {
            if (isModal(dialog.second))
            {
                dialog.second->handleASCII(c);
                foundModal = true;
                break;
            }
        }

        if (!foundModal)
        {
            for (auto dialog : dialogs)
            {
                dialog.second->handleASCII(c);
            }
        }
    }

    void iWidgetManager::onHandle()
    {
        for (auto dialog : _dialogs)
        {
            traverseContentSize(dialog.second);
            traverseAlignment(dialog.second, 0, 0, getDesktopWidth(), getDesktopHeight());
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

            for (int i = 0; i < offsets.size(); ++i)
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
            if (!isModal(dialog.second))
            {
                dialog.second->draw();
            }
        }

        if (_modal != nullptr)
        {
            _modal->draw();
        }
    }

    void iWidgetManager::registerDialogType(const iaString& dialogType, iInstanciateDialogDelegate instanciateDialogDelegate)
    {
        uint64 key = dialogType.getHashValue();
        auto iter = _dialogTypes.find(key);
        if (iter == _dialogTypes.end())
        {
            _dialogTypes[key] = instanciateDialogDelegate;
        }
        else
        {
            con_err("dialog type " << dialogType << " already registered");
        }
    }

    void iWidgetManager::unregisterDialogType(const iaString& dialogType)
    {
        uint64 key = dialogType.getHashValue();
        auto iter = _dialogTypes.find(key);
        if (iter != _dialogTypes.end())
        {
            _dialogTypes.erase(iter);
        }
        else
        {
            con_err("dialog type " << dialogType << " not found");
        }
    }

    void iWidgetManager::registerWidgetType(const iaString& widgetType, iInstanciateWidgetDelegate instanciateWidgetDelegate)
    {
        uint64 key = widgetType.getHashValue();
        auto iter = _widgetTypes.find(key);
        if (iter == _widgetTypes.end())
        {
            _widgetTypes[key] = instanciateWidgetDelegate;
        }
        else
        {
            con_err("widget type " << widgetType << " already registered");
        }
    }

    void iWidgetManager::unregisterWidgetType(const iaString& widgetType)
    {
        uint64 key = widgetType.getHashValue();
        auto iter = _widgetTypes.find(key);
        if (iter != _widgetTypes.end())
        {
            _widgetTypes.erase(iter);
        }
        else
        {
            con_err("widget type " << widgetType << " not found");
        }
    }

    iDialog* iWidgetManager::createDialog(const iaString& type)
    {
        iDialog* result = nullptr;

        uint64 key = type.getHashValue();
        auto iter = _dialogTypes.find(key);
        con_assert(iter != _dialogTypes.end(), "dialog type not found");
        if (iter != _dialogTypes.end())
        {
            result = (*iter).second();
            _dialogs[result->getID()] = result;
        }
        else
        {
            con_err("unknown widget type " << type);
        }

        return result;
    }

    void iWidgetManager::destroyDialog(iDialog* dialog)
    {
        con_assert(dialog != nullptr, "zero pointer");

        if (dialog == nullptr)
        {
            return;
        }

        _toDeleteDialogs.push_back(dialog);
    }

    void iWidgetManager::destroyDialog(uint64 id)
    {
        auto iter = _dialogs.find(id);

        if (iter != _dialogs.end())
        {
            destroyDialog((*iter).second);
        }
        else
        {
            con_err("dialog with id " << id << " does not exist");
        }
    }

    iWidget* iWidgetManager::createWidget(const iaString& widgetType)
    {
        iWidget* result = nullptr;

        uint64 key = widgetType.getHashValue();
        auto iter = _widgetTypes.find(key);
        con_assert(iter != _widgetTypes.end(), "widget type not found");
        if (iter != _widgetTypes.end())
        {
            result = (*iter).second();
            _widgets[result->getID()] = result;
        }
        else
        {
            con_err("unknown widget type " << widgetType);
        }

        return result;
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

        _toDeleteWidgets.push_back(widget);
    }

    iDialog* iWidgetManager::getDialog(uint64 id)
    {
        auto iter = _dialogs.find(id);

        if (iter != _dialogs.end())
        {
            return (*iter).second;
        }
        return nullptr;
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

    void iWidgetManager::destroyWidgets()
    {
        for (auto widget : _toDeleteWidgets)
        {
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

        _toDeleteWidgets.clear();

        for (auto dialog : _toDeleteDialogs)
        {
            auto iter = _dialogs.find(dialog->getID());
            con_assert(iter != _dialogs.end(), "possible mem leak. dialog does not exist");
            if (iter != _dialogs.end())
            {
                delete (*iter).second;
                _dialogs.erase(iter);
            }
            else
            {
                con_err("dialog does not exist");
            }
        }

        _toDeleteDialogs.clear();
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
