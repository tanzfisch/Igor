// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetMenu.h>

#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetGridLayout.h>
#include <igor/ui/actions/iAction.h>
#include <igor/ui/actions/iActionManager.h>
#include <igor/ui/iWidgetManager.h>
#include <igor/ui/widgets/iWidgetMenuBar.h>
#include <igor/resources/iResourceManager.h>

namespace igor
{
    iWidgetMenu::iWidgetMenu(const iaString &title, const iWidgetPtr parent)
        : iWidget(iWidgetType::iWidgetMenu, iWidgetKind::Widget, parent)
    {
        init();
        setTitle(title);
    }

    iWidgetMenu::~iWidgetMenu()
    {
        if (_dialogMenu != nullptr)
        {
            delete _dialogMenu;
            _dialogMenu = nullptr;
        }
    }

    void iWidgetMenu::init()
    {
        setHorizontalAlignment(iHorizontalAlignment::Stretch);

        iWidgetGridLayoutPtr grid = new iWidgetGridLayout(this);
        grid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
        grid->appendColumns(2);
        grid->setStretchColumn(1);
        grid->setCellSpacing(4);

        _spacer = new iWidgetSpacer(16, 16);
        grid->addWidget(_spacer, 0, 0);

        _title = new iWidgetLabel();
        _title->setHorizontalAlignment(iHorizontalAlignment::Left);
        grid->addWidget(_title, 1, 0);

        _picture = new iWidgetPicture();
        _picture->setMaxSize(8, 8);
        _picture->setKeepAspectRatio(false);
        _picture->setTexture(iResourceManager::getInstance().loadResource<iTexture>("igor_icon_right"));

        grid->addWidget(_picture, 2, 0);

        grid->registerOnClickEvent(iClickDelegate(this, &iWidgetMenu::onClick));

        _dialogMenu = new iDialogMenu();
    }

    void iWidgetMenu::update()
    {
        iWidgetPtr parent = iWidgetManager::getInstance().getWidget(_menuParent);

        if (parent == nullptr)
        {
            return;
        }

        if (parent->getWidgetType() == iWidgetType::iWidgetMenuBar)
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

    void iWidgetMenu::addSeparator()
    {
        _dialogMenu->addSeparator();
    }

    void iWidgetMenu::onParentChanged()
    {
        update();
    }

    void iWidgetMenu::onClick(const iWidgetPtr source)
    {
        iWidgetPtr parent = iWidgetManager::getInstance().getWidget(_menuParent);

        if (parent != nullptr)
        {
            if (parent->getWidgetType() == iWidgetType::iWidgetMenuBar)
            {
                _dialogMenu->setX(getActualPosX());
                _dialogMenu->setY(getActualPosY() + getActualHeight() - 1);
            }
            else
            {
                _dialogMenu->setX(getActualPosX() + getActualWidth());
                _dialogMenu->setY(getActualPosY() - 1);
            }

            _dialogMenu->setZValue(parent->getZValue() + 1);
        }
        else
        {
            _dialogMenu->setX(getActualPosX() + getActualWidth());
            _dialogMenu->setY(getActualPosY() - 1);
            // _dialogMenu->setZValue(getZValue() + 1);
            _dialogMenu->setZValue(getRoot()->getZValue() + 1);
        }

        _dialogMenu->open(iDialogCloseDelegate(this, &iWidgetMenu::onDialogClose));
    }

    void iWidgetMenu::setMenuParent(iWidgetPtr menuParent)
    {
        _menuParent = menuParent->getID();
    }

    void iWidgetMenu::onDialogClose(iDialogPtr dialog)
    {
        if (_dialogMenu->getReturnState() != iDialogReturnState::Cancel &&
            _dialogMenu->getReturnState() != iDialogReturnState::Error)
        {
            iWidgetMenuPtr parent = dynamic_cast<iWidgetMenuPtr>(iWidgetManager::getInstance().getWidget(_menuParent));
            if (parent != nullptr)
            {
                parent->onSubMenuClosed(_dialogMenu->getReturnState());
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
        menu->setMenuParent(this);
        _dialogMenu->addMenu(menu);
    }

    void iWidgetMenu::addAction(const iActionPtr action, const iActionContextPtr context)
    {
        _dialogMenu->addAction(action, context);        
    }

    void iWidgetMenu::addCallback(iClickDelegate delegate, const iaString &title, const iaString &description, const iaString &iconAlias)
    {
        _dialogMenu->addCallback(delegate, title, description, iconAlias);
    }

    void iWidgetMenu::addAction(const iaString &actionName, const iActionContextPtr context)
    {
        addAction(iActionManager::getInstance().getAction(actionName), context);
    }

    void iWidgetMenu::onSubMenuClosed(iDialogReturnState returnState)
    {
        _dialogMenu->close();
    }

} // namespace igor