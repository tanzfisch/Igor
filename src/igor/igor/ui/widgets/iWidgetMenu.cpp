// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetMenu.h>

#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetGrid.h>
#include <igor/ui/actions/iAction.h>
#include <igor/ui/actions/iActionManager.h>
#include <igor/ui/iWidgetManager.h>
#include <igor/ui/dialogs/iDialogMenu.h>
#include <igor/ui/widgets/iWidgetMenuBar.h>
#include <igor/ui/widgets/iWidgetSpacer.h>
#include <igor/ui/widgets/iWidgetPicture.h>

namespace igor
{
    iWidgetMenu::iWidgetMenu(const iWidgetPtr parent)
        : iWidget(parent)
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

    void iWidgetMenu::init()
    {
        setHorizontalAlignment(iHorizontalAlignment::Strech);

        iWidgetGridPtr grid = new iWidgetGrid(this);
        grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
        grid->appendCollumns(2);
        grid->setStrechColumn(1);
        grid->setCellSpacing(4);

        _spacer = new iWidgetSpacer();
        _spacer->setSize(16, 16);
        _spacer->setVisible(false);
        grid->addWidget(_spacer, 0, 0);

        _title = new iWidgetLabel();
        _title->setHorizontalAlignment(iHorizontalAlignment::Left);
        grid->addWidget(_title, 1, 0);

        _picture = new iWidgetPicture();
        _picture->setMaxSize(8, 8);
        _picture->setKeepAspectRatio(false);
        _picture->setTexture("icons\\right.png");

        grid->addWidget(_picture, 2, 0);

        grid->registerOnClickEvent(iClickDelegate(this, &iWidgetMenu::onClick));

        _menu = new iDialogMenu();
    }

    void iWidgetMenu::update()
    {
        iWidgetPtr parent = iWidgetManager::getInstance().getWidget(_menuParent);

        if (parent == nullptr)
        {
            return;
        }

        bool parentIsMenuBar = parent->getWidgetType() == iWidgetType::iWidgetMenuBar;

        if (parentIsMenuBar)
        {
            _spacer->setSize(0, 16);
            _picture->setMaxSize(0, 8);
        }
        else
        {
            _spacer->setSize(16, 16);
            _picture->setMaxSize(8, 8);
        }
    }

    void iWidgetMenu::onParentChanged()
    {
        update();
    }

    iWidgetType iWidgetMenu::getWidgetType() const
    {
        return iWidgetType::iWidgetMenu;
    }

    void iWidgetMenu::onClick(const iWidgetPtr source)
    {
        iWidgetPtr parent = iWidgetManager::getInstance().getWidget(_menuParent);

        if (parent == nullptr)
        {
            return;
        }

        con_endl(parent->getWidgetType());

        if (parent->getWidgetType() == iWidgetType::iWidgetMenuBar)
        {
            _menu->setX(getActualPosX());
            _menu->setY(getActualPosY() + getActualHeight() - 1);
        }
        else
        {
            _menu->setX(getActualPosX() + getActualWidth());
            _menu->setY(getActualPosY() - 1);
        }

        _menu->setZValue(parent->getZValue() + 1);
        _menu->open(iDialogCloseDelegate(this, &iWidgetMenu::onDialogClose));
    }

    void iWidgetMenu::onDialogClose(iDialogPtr dialog)
    {
        if (dialog != _menu)
        {
            return;
        }

        if (_menu->getReturnState() != iDialogReturnState::Cancel &&
            _menu->getReturnState() != iDialogReturnState::Error)
        {
            iWidgetMenuPtr parent = dynamic_cast<iWidgetMenuPtr>(iWidgetManager::getInstance().getWidget(_menuParent));
            if (parent != nullptr)
            {
                parent->onSubMenuClosed(_menu->getReturnState());
            }
        }
    }

    void iWidgetMenu::setTitle(const iaString &title)
    {
        _title->setText(title);
    }

    const iaString &iWidgetMenu::getTitle() const
    {
        return _title->getText();
    }

    void iWidgetMenu::addMenu(const iWidgetMenuPtr menu)
    {
        menu->_menuParent = getID(); // TODO little hack to track menu structures

        _menu->addMenu(menu);
    }

    void iWidgetMenu::addAction(const iActionPtr action)
    {
        _menu->addAction(action);
    }

    void iWidgetMenu::addAction(const iaString &actionName)
    {
        addAction(iActionManager::getInstance().getAction(actionName));
    }

    void iWidgetMenu::onSubMenuClosed(iDialogReturnState returnState)
    {
        _menu->close();
    }

} // namespace igor