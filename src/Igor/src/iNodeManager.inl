// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file


__IGOR_INLINE__ iNodePtr iNodeManager::getNode(uint64 id) const
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

__IGOR_INLINE__ bool iNodeManager::isNode(uint64 id) const
{
    bool result = false;;

    _mutexNodes.lock();
    auto iter = _nodes.find(id);
    if (_nodes.end() != iter)
    {
        result = true;
    }
    _mutexNodes.unlock();

    return result;
}