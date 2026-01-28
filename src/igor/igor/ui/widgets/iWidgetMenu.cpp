// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetMenu.h>

#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/layouts/iWidgetGridLayout.h>
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

    void iWidgetMenu::init()
    {
        setHorizontalAlignment(iHorizontalAlignment::Stretch);

        _button = new iWidgetButton(this);
        _button->setHorizontalAlignment(iHorizontalAlignment::Stretch);
        _button->setMinHeight(25);
        _button->setHorizontalTextAlignment(iHorizontalAlignment::Left);
        _button->getClickEvent().add(iClickDelegate(this, &iWidgetMenu::onClick));
        _button->setBorderStyle(iWidgetButtonBorderStyle::None);

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
            _button->setIcon("");
            _button->setBackground(iaColor4f(0.5f, 0.5f, 0.5f, 1.0f)); // TODO this needs to go in to the Theme
            _button->setBackgroundTexture("igor_texture_white");
        }
        else
        {
            _button->setIcon("igor_icon_right_small");
            _button->setBackgroundTexture("");
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
        _preMenuOpenEvent(this);

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

    iPreMenuOpenEvent& iWidgetMenu::getPreMenuOpenEvent()
    {
        return _preMenuOpenEvent;
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
        _button->setText(title);
    }

    const iaString &iWidgetMenu::getTitle() const
    {
        return _button->getText();
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

    void iWidgetMenu::addCallback(iClickDelegate delegate, const iaString &title, const iaString &description, const iaString &iconAlias, bool enabled, const iActionContextPtr context)
    {
        _dialogMenu->addCallback(delegate, title, description, iconAlias, enabled, context);
    }

    void iWidgetMenu::addAction(const iaString &actionName, const iActionContextPtr context)
    {
        addAction(iActionManager::getInstance().getAction(actionName), context);
    }

    void iWidgetMenu::onSubMenuClosed(iDialogReturnState returnState)
    {
        _dialogMenu->close();
    }

    void iWidgetMenu::clear()
    {
        _dialogMenu->clear();
    }          

} // namespace igor