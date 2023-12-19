// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetMenuBar.h>

#include <igor/ui/actions/iAction.h>
#include <igor/ui/actions/iActionManager.h>
#include <igor/ui/iWidgetManager.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/ui/layouts/iWidgetGridLayout.h>

namespace igor
{
    iWidgetMenuBar::iWidgetMenuBar(const iWidgetPtr parent)
        : iWidget(iWidgetType::iWidgetMenuBar, iWidgetKind::Widget, parent)
    {
        setHorizontalAlignment(iHorizontalAlignment::Left);
        setVerticalAlignment(iVerticalAlignment::Top);
        setMinHeight(20);

        _grid = new iWidgetGridLayout(this);
        _grid->setSelectMode(iSelectionMode::NoSelection);
        _grid->setHighlightMode(iSelectionMode::Column);
    }

    void iWidgetMenuBar::unselect()
    {
        _grid->unselect();
    }

    void iWidgetMenuBar::addMenu(const iWidgetMenuPtr menu)
    {
        menu->setMenuParent(this);
        _grid->addWidget(menu, _grid->getColumnCount() - 1, 0);
        _grid->appendColumns(1);
        _grid->setStretchColumn(_grid->getColumnCount() - 1);
    }

    void iWidgetMenuBar::init()
    {
    }

    void iWidgetMenuBar::addAction(const iActionPtr action, const iActionContextPtr context)
    {
        if (!iActionManager::getInstance().isRegistered(action))
        {
            con_err("can't add unregistered actions");
            return;
        }

        iWidgetButtonPtr button = new iWidgetButton();
        button->setHorizontalAlignment(iHorizontalAlignment::Stretch);
        button->setAction(action, context);
        _grid->addWidget(button, _grid->getColumnCount() - 1, 0);

        _grid->appendColumns(1);
        _grid->setStretchColumn(_grid->getColumnCount() - 1);
    }

    void iWidgetMenuBar::addAction(const iaString &actionName, const iActionContextPtr context)
    {
        addAction(iActionManager::getInstance().getAction(actionName), context);
    }

} // namespace igor