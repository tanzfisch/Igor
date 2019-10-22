// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetMenu.h>

#include <iWidgetLabel.h>
#include <iWidgetGrid.h>
#include <iAction.h>
#include <iActionManager.h>
#include <iWidgetManager.h>
#include <iDialogMenu.h>
#include <iWidgetMenuBar.h>

namespace Igor
{
    iWidgetMenu::iWidgetMenu(const iWidgetPtr parent)
        :iWidget(parent)
    {
        init();
    }

    iWidgetMenu::~iWidgetMenu()
    {
        if (_menu != nullptr)
        {
            delete _menu;
            _menu = nullptr;
        }
    }

    iWidgetType iWidgetMenu::getWidgetType() const
    {
        return iWidgetType::iWidgetMenu;
    }

    void iWidgetMenu::onMenuClosed(const iDialogPtr source)
    {
        iWidgetPtr parent = iWidgetManager::getInstance().getWidget(_menuParent);

        if (parent == nullptr)
        {
            return;
        }

        if (parent->getWidgetType() == iWidgetType::iWidgetMenuBar)
        {
            iWidgetMenuBarPtr menuBar = static_cast<iWidgetMenuBarPtr>(parent);
            menuBar->unselect();
        }
        else if (parent->getWidgetType() == iWidgetType::iWidgetMenu)
        {
            // TODO
        }
    }

    void iWidgetMenu::onClick(const iWidgetPtr source)
    {
        _menu->setX(getActualPosX());
        _menu->setY(getActualPosY() + getActualHeight());
        _menu->open(iDialogCloseDelegate(this, &iWidgetMenu::onDialogClose));
    }

    void iWidgetMenu::onDialogClose(iDialogPtr dialog)
    {
        // TODO close
    }

    void iWidgetMenu::init()
    {
        iWidgetGridPtr grid = new iWidgetGrid(this);
        grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
        grid->setVerticalAlignment(iVerticalAlignment::Strech);
        grid->registerOnClickEvent(iClickDelegate(this, &iWidgetMenu::onClick));

        _title = new iWidgetLabel(grid);

        _menu = new iDialogMenu();
    }

    void iWidgetMenu::setTitle(const iaString& title)
    {
        _title->setText(title);
    }

    const iaString& iWidgetMenu::getTitle() const
    {
        return _title->getText();
    }

    void iWidgetMenu::addMenu(const iWidgetMenuPtr menu)
    {
        _menu->addMenu(menu);
        menu->_menuParent = getID();
    }

    void iWidgetMenu::addAction(const iActionPtr action)
    {
        _menu->addAction(action);
    }

    void iWidgetMenu::addAction(const iaString& actionName)
    {
        addAction(iActionManager::getInstance().getAction(actionName));
    }

}