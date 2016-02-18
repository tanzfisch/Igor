__IGOR_INLINE__ uint64 iEntityDataPosition::getDataMask()
{
    return static_cast<uint64>(iEntityDataMask::Position);
}

__IGOR_INLINE__ void iEntityDataPosition::setPosition(uint64 entityID, const iaVector3f& position)
{
    _spheres[entityID]._center = position;
}

__IGOR_INLINE__ const iaVector3f& iEntityDataPosition::getPosition(uint64 entityID)
{
    return _spheres[entityID]._center;
}

__IGOR_INLINE__ void iEntityDataPosition::query(const iSphered& sphere, vector<uint64>& data)
{
    _octree->resetFilter();
    _octree->filter(sphere);
    _octree->getResult(data);
}