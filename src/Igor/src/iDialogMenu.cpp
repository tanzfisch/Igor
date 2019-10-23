// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iDialogMenu.h>

#include <iDialog.h>
#include <iWidgetManager.h>
#include <iWidgetLabel.h>
#include <iWidgetPicture.h>
#include <iWidgetGrid.h>
#include <iWidgetMenu.h>
#include <iActionManager.h>
#include <iUserControlAction.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iDialogMenu::iDialogMenu()
    {
        init();
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
    }

    void iDialogMenu::onMouseOffClick(const iWidgetPtr source)
    {
        close();
    }

    void iDialogMenu::onActionClick(const iWidgetPtr source)
    {
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

    void iDialogMenu::addAction(const iaString& actionName)
    {
        addAction(iActionManager::getInstance().getAction(actionName));
    }

}