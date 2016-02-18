__IGOR_INLINE__ uint64 iEntityDataPosition::getDataMask()
{
    return static_cast<uint64>(iEntityDataMask::Position);
}

__IGOR_INLINE__ void iEntityDataPosition::updatePosition(uint64 entityID, const iaVector3f& position)
{
    if (_spheres.find(entityID) == _spheres.end())
    {
        _spheres[entityID]._center = position;

        _octree->insert(entityID, _spheres[entityID]);
    }
    else
    {
        _spheres[entityID]._center = position;

        _octree->update(entityID, _spheres[entityID]);
    }
}

__IGOR_INLINE__ void iEntityDataPosition::query(const iSphered& sphere, vector<uint64>& data)
{
    _octree->resetFilter();
    _octree->filter(sphere);
    _octree->getResult(data);
}