__IGOR_INLINE__ uint64 iEntityDataTransformation::getDataMask()
{
    return static_cast<uint64>(iEntityDataMask::TransformNode);
}

__IGOR_INLINE__ void iEntityDataTransformation::setTransformID(uint64 entityID, uint32 transformNodeID)
{
    _transformNodes[entityID] = transformNodeID;
}

__IGOR_INLINE__ uint32 iEntityDataTransformation::getTransformID(uint64 entityID)
{
    return _transformNodes[entityID];
}

