// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iActionManager.h>

#include <iAction.h>

#include <iaConsole.h>

namespace Igor
{
	iActionManager::~iActionManager()
	{
		for (auto action : _actions)
		{
			delete action.second;
		}

		_actions.clear();
	}

	void iActionManager::registerAction(const iaString& name, iAction* action)
	{
		con_assert(action != nullptr, "zero pointer");
		con_assert(!name.isEmpty(), "invalid parameters");

		int64 hash = name.getHashValue();

		auto iter = _actions.find(hash);
		if (iter != _actions.end())
		{
			con_warn("action with name \"" << name << "\" was already registered");
			return;
		}

		_actions[name.getHashValue()] = action;
		action->setIDName(name);
	}

	void iActionManager::unregisterAction(const iAction* action)
	{
		con_assert(action != nullptr, "zero pointer");

		int64 hash = action->getID();

		auto iter = _actions.find(hash);
		if (iter == _actions.end())
		{
			con_warn("action with name \"" << action->getIDName() << "\" was not registered");
			return;
		}

		_actions.erase(iter);
	}

	iAction* iActionManager::unregisterAction(const iaString& name)
	{
		int64 hash = name.getHashValue();

		auto iter = _actions.find(hash);
		if (iter == _actions.end())
		{
			con_warn("action with name \"" << name << "\" not found");
			return nullptr;
		}

		iAction* result = iter->second;
		_actions.erase(iter);

		return result;
	}

    bool iActionManager::isRegistered(const iAction* action)
    {
        con_assert(action != nullptr, "zero pointer");

        if (action == nullptr)
        {
            return false;
        }

        auto iter = _actions.find(action->getID());
        return iter != _actions.end();
    }

    bool iActionManager::isRegistered(const iaString& actionName)
    {
        int64 hash = actionName.getHashValue();

        auto iter = _actions.find(hash);
        return iter != _actions.end();
    }

	iAction* iActionManager::getAction(const iaString& name) const
	{
		int64 hash = name.getHashValue();

		auto iter = _actions.find(hash);
		if (iter == _actions.end())
		{
			return nullptr;
		}

		return iter->second;
	}
}
