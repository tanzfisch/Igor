// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
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
#include <iWidgetColorView.h>
#include <iWidgetSlider.h>
#include <iKeyboard.h>

#include <iMaterialResourceFactory.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

	iWidgetManager::~iWidgetManager()
	{
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

    void iWidgetManager::registerIOEvents()
    {
        iMouse::getInstance().registerMouseKeyDownDelegate(MouseKeyDownDelegate(this, &iWidgetManager::onMouseKeyDown));
        iMouse::getInstance().registerMouseKeyUpDelegate(MouseKeyUpDelegate(this, &iWidgetManager::onMouseKeyUp));
        iMouse::getInstance().registerMouseDoubleClickDelegate(MouseKeyDoubleClickDelegate(this, &iWidgetManager::onMouseDoubleClick));
        iMouse::getInstance().registerMouseMoveDelegate(MouseMoveDelegate(this, &iWidgetManager::onMouseMove));
        iMouse::getInstance().registerMouseWheelDelegate(MouseWheelDelegate(this, &iWidgetManager::onMouseWheel));
        iKeyboard::getInstance().registerKeyASCIIDelegate(KeyASCIIDelegate(this, &iWidgetManager::onASCII));
    }

    void iWidgetManager::unregisterIOEvents()
    {
        iMouse::getInstance().unregisterMouseKeyDownDelegate(MouseKeyDownDelegate(this, &iWidgetManager::onMouseKeyDown));
        iMouse::getInstance().unregisterMouseKeyUpDelegate(MouseKeyUpDelegate(this, &iWidgetManager::onMouseKeyUp));
        iMouse::getInstance().unregisterMouseDoubleClickDelegate(MouseKeyDoubleClickDelegate(this, &iWidgetManager::onMouseDoubleClick));
        iMouse::getInstance().unregisterMouseMoveDelegate(MouseMoveDelegate(this, &iWidgetManager::onMouseMove));
        iMouse::getInstance().unregisterMouseWheelDelegate(MouseWheelDelegate(this, &iWidgetManager::onMouseWheel));
        iKeyboard::getInstance().unregisterKeyASCIIDelegate(KeyASCIIDelegate(this, &iWidgetManager::onASCII));
    }

    void iWidgetManager::onMouseKeyDown(iKeyCode key)
    {
        bool foundModal = false;
        vector<iWidgetDialog*> dialogs = _dialogs;

        for (auto dialog : dialogs)
        {
            if (dialog->isModal())
            {
                dialog->handleMouseKeyDown(key);
                foundModal = true;
                break;
            }
        }

        if (!foundModal)
        {
            for (auto dialog : dialogs)
            {
                dialog->handleMouseKeyDown(key);
            }
        }
    }

    void iWidgetManager::onMouseKeyUp(iKeyCode key)
    {
        bool foundModal = false;
        vector<iWidgetDialog*> dialogs = _dialogs;

        for (auto dialog : dialogs)
        {
            if (dialog->isModal())
            {
                dialog->handleMouseKeyUp(key);
                foundModal = true;
                break;
            }
        }

        if (!foundModal)
        {
            for (auto dialog : dialogs)
            {
                dialog->handleMouseKeyUp(key);
                int x = 0;
            }
        }
    }

    void iWidgetManager::onMouseDoubleClick(iKeyCode key)
    {
        bool foundModal = false;
        vector<iWidgetDialog*> dialogs = _dialogs;

        for (auto dialog : dialogs)
        {
            if (dialog->isModal())
            {
                dialog->handleMouseDoubleClick(key);
                foundModal = true;
                break;
            }
        }

        if (!foundModal)
        {
            for (auto dialog : dialogs)
            {
                dialog->handleMouseDoubleClick(key);
            }
        }
    }

    void iWidgetManager::onMouseMove(int32 x, int32 y)
    {
        bool foundModal = false;
        vector<iWidgetDialog*> dialogs = _dialogs;

        for (auto dialog : dialogs)
        {
            if (dialog->isModal())
            {
                dialog->handleMouseMove(x, y);
                foundModal = true;
                break;
            }
        }

        if (!foundModal)
        {
            for (auto dialog : dialogs)
            {
                dialog->handleMouseMove(x, y);
            }
        }
    }

    void iWidgetManager::onMouseWheel(int32 d)
    {
        bool foundModal = false;
        vector<iWidgetDialog*> dialogs = _dialogs;

        for (auto dialog : dialogs)
        {
            if (dialog->isModal())
            {
                dialog->handleMouseWheel(d);
                foundModal = true;
                break;
            }
        }

        if (!foundModal)
        {
            for (auto dialog : dialogs)
            {
                dialog->handleMouseWheel(d);
            }
        }
    }

    void iWidgetManager::onASCII(uint8 c)
    {
        bool foundModal = false;
        vector<iWidgetDialog*> dialogs = _dialogs;

        for (auto dialog : dialogs)
        {
            if (dialog->isModal())
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

    void iWidgetManager::updateDialogs()
    {
        vector<iWidgetDialog*> dialogs = _dialogs;

        for (auto dialog : dialogs)
        {
            dialog->update();
        }
    }

	void iWidgetManager::setDesktopDimensions(uint32 width, uint32 height)
	{
		_desktopWidth = width;
		_desktopHeight = height;
        updateDialogs();
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

        for( auto dialog : _dialogs)
		{
            dialog->draw();
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

        case iWidgetType::ColorView:
            result = new iWidgetColorView();
            break;

        case iWidgetType::Slider:
            result = new iWidgetSlider();
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

}
