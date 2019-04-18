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

		if (!_widgets.empty())
		{
			con_err("possible mem leak! did not release all widgets. " << _widgets.size() << " left");

			for (auto pair : _widgets)
			{
				con_debug_endl(pair.second->getInfo());
			}
		}

		// we can not delete widgets here anymore because 
		// they might call iWidgetManager::getInstance in the process
		_widgets.clear();
	}

	bool iWidgetManager::isModal(iDialog* dialog)
	{
		return (_modal == dialog) ? true : false;
	}

	void iWidgetManager::setModal(iDialog * dialog)
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
		iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownDelegate(this, &iWidgetManager::onKeyDown));
		iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &iWidgetManager::onKeyUp));

		iApplication::getInstance().registerApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &iWidgetManager::onHandle));
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

		iApplication::getInstance().unregisterApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &iWidgetManager::onHandle));
	}

	void iWidgetManager::onKeyDown(iKeyCode key)
	{
		// this copy is not because of a race condition but because the original list might be changed while handling the event
		map<uint64, iDialog*> dialogs = _dialogs;

		for (auto dialog : dialogs)
		{
			if (isModal(dialog.second))
			{
				dialog.second->handleKeyDown(key);
				return;
			}
		}

		bool handled = false;

		for (auto dialog : dialogs)
		{
			if (dialog.second->handleKeyDown(key))
			{
				handled = true;
				break;
			}
		}

		if (iWidget::getKeyboardFocusWidget() != nullptr)
		{
			return;
		}

		if (!handled)
		{
			_keyDownEvent(key);
		}
	}

	void iWidgetManager::onKeyUp(iKeyCode key)
	{
		// this copy is not because of a race condition but because the original list might be changed while handling the event
		map<uint64, iDialog*> dialogs = _dialogs;

		for (auto dialog : dialogs)
		{
			if (isModal(dialog.second))
			{
				dialog.second->handleKeyUp(key);
				return;
			}
		}

		bool handled = false;

		for (auto dialog : dialogs)
		{
			if (dialog.second->handleKeyUp(key))
			{
				handled = true;
				break;
			}
		}

		if (!handled)
		{
			_keyUpEvent(key);
		}
	}

	void iWidgetManager::onMouseKeyDown(iKeyCode key)
	{
		// this copy is not because of a race condition but because the original list might be changed while handling the event
		map<uint64, iDialog*> dialogs = _dialogs;

		for (auto dialog : dialogs)
		{
			if (isModal(dialog.second))
			{
				dialog.second->handleMouseKeyDown(key);
				return;
			}
		}

		bool handled = false;

		for (auto dialog : dialogs)
		{
			if (dialog.second->handleMouseKeyDown(key))
			{
				handled = true;
				break;
			}
		}


		if (!handled)
		{
			_mouseKeyDownEvent(key);
		}
	}

	void iWidgetManager::onMouseKeyUp(iKeyCode key)
	{
		// this copy is not because of a race condition but because the original list might be changed while handling the event
		map<uint64, iDialog*> dialogs = _dialogs;

		for (auto dialog : dialogs)
		{
			if (isModal(dialog.second))
			{
				dialog.second->handleMouseKeyUp(key);
				return;
			}
		}

		bool handled = false;
		for (auto dialog : dialogs)
		{
			if (dialog.second->handleMouseKeyUp(key))
			{
				handled = true;
				break;
			}
		}

		if (!handled)
		{
			_mouseKeyUpEvent(key);
		}
	}

	void iWidgetManager::onMouseDoubleClick(iKeyCode key)
	{
		// this copy is not because of a race condition but because the original list might be changed while handling the event
		map<uint64, iDialog*> dialogs = _dialogs;

		for (auto dialog : dialogs)
		{
			if (isModal(dialog.second))
			{
				dialog.second->handleMouseDoubleClick(key);
				return;
			}
		}

		bool handled = false;

		for (auto dialog : dialogs)
		{
			if (dialog.second->handleMouseDoubleClick(key))
			{
				handled = true;
				break;
			}
		}

		if (!handled)
		{
			_doubleClickEvent(key);
		}
	}

	void iWidgetManager::onMouseMove(const iaVector2i & from, const iaVector2i & to, iWindow * window)
	{
		// this copy is not because of a race condition but because the original list might be changed while handling the event
		map<uint64, iDialog*> dialogs = _dialogs;

		for (auto dialog : dialogs)
		{
			if (isModal(dialog.second))
			{
				dialog.second->handleMouseMove(to);
				return;
			}
		}

		for (auto dialog : dialogs)
		{
			dialog.second->handleMouseMove(to);
		}

		_moveFullEvent(from, to, window);
		_moveEvent(to);
	}

	void iWidgetManager::onMouseWheel(int32 d)
	{
		// this copy is not because of a race condition but because the original list might be changed while handling the event
		map<uint64, iDialog*> dialogs = _dialogs;

		for (auto dialog : dialogs)
		{
			if (isModal(dialog.second))
			{
				dialog.second->handleMouseWheel(d);
				return;
			}
		}

		bool handled = false;

		for (auto dialog : dialogs)
		{
			if (dialog.second->handleMouseWheel(d))
			{
				handled = true;
				break;
			}
		}

		if (!handled)
		{
			_wheelEvent(d);
		}
	}

	void iWidgetManager::onASCII(const char c)
	{
		// this copy is not because of a race condition but because the original list might be changed while handling the event
		map<uint64, iDialog*> dialogs = _dialogs;

		for (auto dialog : dialogs)
		{
			if (isModal(dialog.second))
			{
				dialog.second->handleASCII(c);
				return;
			}
		}

		for (auto dialog : dialogs)
		{
			dialog.second->handleASCII(c);
		}
	}

	void iWidgetManager::onHandle()
	{
		for (auto dialog : _dialogs)
		{
			traverseContentSize(dialog.second);
			traverseAlignment(dialog.second, 0, 0, getDesktopWidth(), getDesktopHeight());
		}

		runDeleteQueues();
	}

	void iWidgetManager::traverseContentSize(iWidget * widget)
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

	void iWidgetManager::traverseAlignment(iWidget * widget, int32 offsetX, int32 offsetY, int32 clientRectWidth, int32 clientRectHeight)
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

	void iWidgetManager::setTheme(iWidgetBaseTheme * theme)
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

	void iWidgetManager::registerDialogType(const iaString & dialogType, iInstanciateDialogDelegate instanciateDialogDelegate)
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

	void iWidgetManager::unregisterDialogType(const iaString & dialogType)
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

	void iWidgetManager::registerWidgetType(const iaString & widgetType, iInstanciateWidgetDelegate instanciateWidgetDelegate)
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

	void iWidgetManager::unregisterWidgetType(const iaString & widgetType)
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

	iDialog* iWidgetManager::createDialog(const iaString & type)
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

	void iWidgetManager::destroyDialog(iDialog * dialog)
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

	iWidget* iWidgetManager::createWidget(const iaString & widgetType)
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

	void iWidgetManager::destroyWidget(iWidget * widget)
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

	void iWidgetManager::onPreDestroyInstance()
	{
		runDeleteQueues();
	}

	void iWidgetManager::runDeleteQueues()
	{
		while (!_toDeleteDialogs.empty() || !_toDeleteWidgets.empty())
		{
			while (!_toDeleteDialogs.empty())
			{
				auto iter = _dialogs.find(_toDeleteDialogs.front()->getID());
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

				_toDeleteDialogs.pop_front();
			}

			while (!_toDeleteWidgets.empty())
			{
				auto iter = _widgets.find(_toDeleteWidgets.front()->getID());
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

				_toDeleteWidgets.pop_front();
			}
		}
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

