// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/dialogs/iDialogMenu.h>

#include <igor/ui/widgets/iWidgetMenu.h>
#include <igor/ui/iWidgetManager.h>
#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetPicture.h>
#include <igor/ui/widgets/iWidgetSpacer.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/ui/actions/iActionManager.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iDialogMenu::iDialogMenu(const iWidgetPtr parent)
        : iDialog(iWidgetType::iDialogMenu, parent)
    {
        init();
    }

    void iDialogMenu::open(iDialogCloseDelegate dialogCloseDelegate, bool modal)
    {
        iDialog::open(dialogCloseDelegate, modal);
        putInFront();
    }

    void iDialogMenu::init()
    {
        setMinSize(100, 0);
        setAcceptOutOfBoundsClicks();
        getMouseOffClickEvent().add(iMouseOffClickDelegate(this, &iDialogMenu::onMouseOffClick));

        setIgnoreChildEventConsumption(false);
        setGrowingByContent(true);
        setHeaderEnabled(false);
        setResizeable(false);

        _vboxLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);
    }

    void iDialogMenu::clear()
    {
        iWidget::clear();
        _vboxLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);
        
        _hasActions = false;
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

    void iDialogMenu::addSeparator()
    {
        iWidgetSpacerPtr spacer = new iWidgetSpacer(10, 2, true);
        spacer->setHorizontalAlignment(iHorizontalAlignment::Stretch);
        _vboxLayout->addWidget(spacer);
    }

    void iDialogMenu::addMenu(const iWidgetMenuPtr menu)
    {
        _vboxLayout->addWidget(menu);
    }

    bool iDialogMenu::hasActions() const
    {
        return _hasActions;
    }

    void iDialogMenu::addAction(const iActionPtr action, const iActionContextPtr context, bool enabled)
    {
        if (!iActionManager::getInstance().isRegistered(action))
        {
            con_err("can't add unregistered actions");
            return;
        }

        if (!action->isCompatible(*context))
        {
            return;
        }

        iWidgetButtonPtr button = new iWidgetButton();
        button->setHorizontalAlignment(iHorizontalAlignment::Stretch);
        button->setMinHeight(25);
        button->setAction(action, context);
        button->setHorizontalTextAlignment(iHorizontalAlignment::Left);
        button->getClickEvent().add(iClickDelegate(this, &iDialogMenu::onActionClick));
        button->setEnabled(enabled);

        _vboxLayout->addWidget(button);

        _hasActions = true;
    }

    void iDialogMenu::addCallback(iClickDelegate delegate, const iaString &title, const iaString &description, const iaString &iconAlias, bool enabled)
    {
        iWidgetButtonPtr button = new iWidgetButton();
        button->setHorizontalAlignment(iHorizontalAlignment::Stretch);
        button->setMinHeight(25);
        button->setText(title);
        button->setTooltip(description);
        button->setIcon(iconAlias);
        button->setHorizontalTextAlignment(iHorizontalAlignment::Left);
        button->getClickEvent().add(iClickDelegate(this, &iDialogMenu::onActionClick));
        button->getClickEvent().add(delegate);
        button->setEnabled(enabled);

        _vboxLayout->addWidget(button);
    }

    void iDialogMenu::addAction(const iaString &actionName, const iActionContextPtr context, bool enabled)
    {
        addAction(iActionManager::getInstance().getAction(actionName), context, enabled);
    }

} // namespace igor