// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/actions/iActionManager.h>

#include <igor/ui/actions/iActions.h>
#include <igor/ui/actions/iEntityActions.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iActionManager::iActionManager()
    {
        registerDefaultActions();
    }

    iActionManager::~iActionManager()
    {
        for (auto action : _actions)
        {
            delete action.second;
        }

        _actions.clear();
    }

    void iActionManager::registerDefaultActions()
    {
        registerAction(new iActionApplicationStop());

        registerAction(new iActionSetEntityActive());
        registerAction(new iActionSetEntityInactive());        
    }

    void iActionManager::registerAction(iActionPtr action)
    {
        con_assert(action != nullptr, "zero pointer");

        int64 hash = action->getName().getHashValue();

        auto iter = _actions.find(hash);
        if (iter != _actions.end())
        {
            con_warn("action with name \"" << action->getName() << "\" was already registered");
            return;
        }

        _actions[hash] = action;
    }

    void iActionManager::unregisterAction(const iActionPtr action)
    {
        con_assert(action != nullptr, "zero pointer");

        int64 hash = action->getName().getHashValue();

        auto iter = _actions.find(hash);
        if (iter == _actions.end())
        {
            con_warn("action with name \"" << action->getName() << "\" was not registered");
            return;
        }

        _actions.erase(iter);
    }

    bool iActionManager::isRegistered(const iActionPtr action)
    {
        con_assert(action != nullptr, "zero pointer");

        if (action == nullptr)
        {
            return false;
        }

        auto iter = _actions.find(action->getName().getHashValue());
        return iter != _actions.end();
    }

    iActionPtr iActionManager::getAction(const iaString &name) const
    {
        int64 hash = name.getHashValue();
        auto iter = _actions.find(hash);
        if (iter != _actions.end())
        {
            return iter->second;
        }

        return nullptr;
    }
} // namespace igor
