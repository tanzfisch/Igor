// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

IGOR_INLINE iNodePtr iNodeManager::getNode(iNodeID id) const
{
    iNodePtr result = nullptr;

    _mutexNodes.lock();
    auto iter = _nodes.find(id);
    if (_nodes.end() != iter)
    {
        result = (*iter).second;
    }
    _mutexNodes.unlock();

    return result;
}

IGOR_INLINE bool iNodeManager::isNode(iNodeID id) const
{
    bool result = false;

    _mutexNodes.lock();
    auto iter = _nodes.find(id);
    if (_nodes.end() != iter)
    {
        result = true;
    }
    _mutexNodes.unlock();

    return result;
}

template <class T>
T *iNodeManager::createNode(const iaString &name)
{
    T *result = new T();

    _mutexNodes.lock();
    _nodes[static_cast<iNodePtr>(result)->getID()] = static_cast<iNodePtr>(result);
    _mutexNodes.unlock();

    if (!name.isEmpty())
    {
        result->setName(name);
    }

    return result;
}