// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/dialogs/iDialogMenu.h>

#include <igor/ui/dialogs/iDialog.h>
#include <igor/ui/iWidgetManager.h>
#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetPicture.h>
#include <igor/ui/widgets/iWidgetGrid.h>
#include <igor/ui/widgets/iWidgetMenu.h>
#include <igor/ui/actions/iActionManager.h>
#include <igor/ui/user_controls/iUserControlAction.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iDialogMenu::iDialogMenu()
    {
        init();
    }

    iWidgetType iDialogMenu::getWidgetType() const
    {
        return iWidgetType::iDialogMenu;
    }

    void iDialogMenu::open(iDialogCloseDelegate dialogCloseDelegate)
    {
        iDialog::open(dialogCloseDelegate);
    }

    void iDialogMenu::init()
    {
        setSize(0, 0);
        setAcceptOutOfBoundsClicks();
        registerOnMouseOffClickEvent(iMouseOffClickDelegate(this, &iDialogMenu::onMouseOffClick));

        _grid = new iWidgetGrid(this);
        _grid->setSelectMode(iSelectionMode::NoSelection);
        _grid->setHighlightMode(iSelectionMode::Row);
    }

    void iDialogMenu::onMouseOffClick(const iWidgetPtr source)
    {
        setReturnState(iDialogReturnState::Cancel);
        close();
    }

    void iDialogMenu::onActionClick(const iWidgetPtr source)
    {
        setReturnState(iDialogReturnState::Ok);
        close();
    }

    void iDialogMenu::addMenu(const iWidgetMenuPtr menu)
    {
        _grid->addWidget(menu, 0, _grid->getRowCount() - 1);
        _grid->appendRows(1);
    }

    void iDialogMenu::addAction(const iActionPtr action)
    {
        if (!iActionManager::getInstance().isRegistered(action))
        {
            con_err("can't add unregistered actions");
            return;
        }

        iUserControlActionPtr userControlAction = new iUserControlAction();
        userControlAction->setHorizontalAlignment(iHorizontalAlignment::Strech);
        userControlAction->setAction(action);
        userControlAction->setFixedPictureSize();
        userControlAction->registerOnClickEvent(iClickDelegate(this, &iDialogMenu::onActionClick));

        _grid->addWidget(userControlAction, 0, _grid->getRowCount() - 1);
        _grid->appendRows(1);
    }

    void iDialogMenu::addAction(const iaString &actionName)
    {
        addAction(iActionManager::getInstance().getAction(actionName));
    }

} // namespace igor