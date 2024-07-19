// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iPrefabComponent.h>

#include <igor/entities/iEntitySystemModule.h>

namespace igor
{
    iPrefabComponent::iPrefabComponent(iPrefabPtr prefab)
        : _prefab(prefab)
    {
    }

    iEntityComponentPtr iPrefabComponent::getCopy()
    {
        iPrefabComponent *component = new iPrefabComponent();
        component->_prefab = _prefab;
        return component;
    }

    bool iPrefabComponent::onLoad(iEntityPtr entity, bool &asyncLoad)
    {
        asyncLoad = false; // stop trying

        if (_prefab == nullptr)
        {
            return false;
        }

        if (!_prefab->isValid())
        {
            asyncLoad = true; // keep trying
            return false;
        }

        iEntitySystemModule::getInstance().insert(_prefab, entity);
        return true;
    }

    void iPrefabComponent::setPrefab(iPrefabPtr prefab)
    {
        con_assert(getState() == iEntityComponentState::Unloaded, "invalid state");
        _prefab = prefab;
    }

    iPrefabPtr iPrefabComponent::getPrefab() const
    {
        return _prefab;
    }

}