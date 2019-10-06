// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetMenu.h>

#include <iAction.h>
#include <iActionManager.h>

namespace Igor
{
	iWidgetMenu::iWidgetMenu(iWidgetPtr parent)
		:iWidget(parent)
	{
	}

    iWidgetMenu::~iWidgetMenu()
    {
    }


    void iWidgetMenu::addAction(const iAction* action)
    {
        if (!iActionManager::getInstance().isRegistered(action))
        {
            con_err("can't add unregistered actions");
        }

        _actions.push_back(action->getID());
    }

    void iWidgetMenu::addAction(const iaString& actionName)
    {
        addAction(iActionManager::getInstance().getAction(actionName));
    }

}