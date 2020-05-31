// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetMenuBar.h>

#include <igor/ui/widgets/iWidgetMenu.h>
#include <igor/ui/actions/iAction.h>
#include <igor/ui/actions/iActionManager.h>
#include <igor/ui/iWidgetManager.h>
#include <igor/ui/user_controls/iUserControlAction.h>
#include <igor/ui/widgets/iWidgetGrid.h>

namespace Igor
{
    iWidgetMenuBar::iWidgetMenuBar(const iWidgetPtr parent)
        : iWidget(parent)
    {
        init();
    }

    iWidgetType iWidgetMenuBar::getWidgetType() const
    {
        return iWidgetType::iWidgetMenuBar;
    }

    void iWidgetMenuBar::unselect()
    {
        _grid->unselect();
    }

    void iWidgetMenuBar::addMenu(const iWidgetMenuPtr menu)
    {
        menu->_menuParent = getID(); // TODO little hack to track menu structures

        _grid->addWidget(menu, _grid->getColumnCount() - 1, 0);

        _grid->appendCollumns(1);
        _grid->setStrechColumn(_grid->getColumnCount() - 1);
    }

    void iWidgetMenuBar::init()
    {
        _grid = new iWidgetGrid(this);
        _grid->setSelectMode(iSelectionMode::NoSelection);
        _grid->setHighlightMode(iSelectionMode::Collumn);
    }

    void iWidgetMenuBar::addAction(const iActionPtr action)
    {
        if (!iActionManager::getInstance().isRegistered(action))
        {
            con_err("can't add unregistered actions");
            return;
        }

        iUserControlActionPtr userControlAction = new iUserControlAction();
        userControlAction->setAction(action);
        _grid->addWidget(userControlAction, _grid->getColumnCount() - 1, 0);

        _grid->appendCollumns(1);
        _grid->setStrechColumn(_grid->getColumnCount() - 1);
    }

    void iWidgetMenuBar::addAction(const iaString &actionName)
    {
        addAction(iActionManager::getInstance().getAction(actionName));
    }

} // namespace Igor