// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iMenu.h>

#include <iAction.h>
#include <iActionManager.h>

namespace Igor
{
	iMenu::iMenu(iWidgetPtr parent)
		:iWidget(parent)
	{
	}

    iMenu::~iMenu()
    {
    }


    void iMenu::addAction(const iActionPtr action)
    {
        if (!iActionManager::getInstance().isRegistered(action))
        {
            con_err("can't add unregistered actions");
        }

        _actions.push_back(action->getID());
    }

    void iMenu::addAction(const iaString& actionName)
    {
        addAction(iActionManager::getInstance().getAction(actionName));
    }

}