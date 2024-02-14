// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

template <typename T>
T *iEntity::addComponent(T *component)
{
    auto iter = _components.find(typeid(T));
    if (iter != _components.end())
    {
        delete iter->second;
    }

    _components[typeid(T)] = component;
    onComponentsChanged();

    return component;
}

template <typename T>
T *iEntity::getComponent() const
{
    auto iter = _components.find(typeid(T));
    if (iter == _components.end())
    {
        return nullptr;
    }

    return static_cast<T*>(iter->second);
}

template <typename T>
void iEntity::destroyComponent()
{
    auto iter = _components.find(typeid(T));
    if (iter == _components.end())
    {
        return;
    }

    delete iter->second;
    _components.erase(iter);
    onComponentsChanged();
}
