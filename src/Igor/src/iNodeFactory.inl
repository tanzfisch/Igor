// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
// see copyright notice in corresponding header file


iNode* iNodeFactory::getNode(uint64 id) const
{
    iNode* result = nullptr;

    _mutexNodes.lock();
    auto iter = _nodes.find(id);
    if (_nodes.end() != iter)
    {
        result = (*iter).second;
    }
    _mutexNodes.unlock();

    return result;
}

bool iNodeFactory::isNode(uint64 id) const
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