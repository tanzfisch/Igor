// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

template <typename T>
T &iEntityScene::addComponent(iEntityID entityID, const T &component)
{
    return *static_cast<T *>(addComponent(entityID, reinterpret_cast<const void *>(&component), typeid(T)));
}

template <typename T>
T &iEntityScene::getComponent(iEntityID entityID)
{
    return *static_cast<T *>(getComponent(entityID, typeid(T)));
}

template <typename T>
T *iEntityScene::tryGetComponent(iEntityID entityID)
{
    return static_cast<T *>(tryGetComponent(entityID, typeid(T)));
}

template <typename T>
void iEntityScene::removeComponent(iEntityID entityID)
{
    removeComponent(entityID, typeid(T));
}

template <typename T>
T &iEntityScene::addUserComponent(iEntityID entityID, const T &component)
{
    auto iter = _customComponents.find(typeid(T));
    if (iter == _customComponents.end())
    {
        _customComponents[typeid(T)] = std::make_shared<iComponentMap<T>>();
    }

    return std::static_pointer_cast<iComponentMap<T>>(_customComponents[typeid(T)])->add(entityID, component);
}

template <typename T>
T &iEntityScene::getUserComponent(iEntityID entityID)
{
    auto iter = _customComponents.find(typeid(T));
    con_assert(iter != _customComponents.end(), "component does not exist")

        return std::static_pointer_cast<iComponentMap<T>>(iter->second)
            ->get(entityID);
}

template <typename T>
T *iEntityScene::tryGetUserComponent(iEntityID entityID)
{
    auto iter = _customComponents.find(typeid(T));
    if (iter == _customComponents.end())
    {
        return nullptr;
    }

    return std::static_pointer_cast<iComponentMap<T>>(iter->second)->tryGet(entityID);
}