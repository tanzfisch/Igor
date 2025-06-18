// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

template <typename T>
T *iEntity::addComponent(T *component)
{
    const std::type_index typeID = typeid(T);
    addComponent(typeID, component);
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

    return static_cast<T *>(iter->second);
}

template <typename T>
void iEntity::destroyComponent()
{
    const std::type_index typeID = typeid(T);
    destroyComponent(typeID);
    onEntityStructureChanged();
}

template <typename T>
void iEntity::reloadComponent()
{
    const std::type_index typeID = typeid(T);
    reloadComponent(typeID);
}