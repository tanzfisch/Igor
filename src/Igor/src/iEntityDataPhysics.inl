__IGOR_INLINE__ uint64 iEntityDataPhysics::getDataMask()
{
    return static_cast<uint64>(iEntityDataMask::Physics);
}


__IGOR_INLINE__ void iEntityDataPhysics::setVelocity(uint64 entityID, const iaVector3f& velocity)
{
    _velocities[entityID] = velocity;
}

__IGOR_INLINE__ const iaVector3f& iEntityDataPhysics::getVelocity(uint64 entityID)
{
    return _velocities[entityID];
}


__IGOR_INLINE__ void iEntityDataPhysics::setMass(uint64 entityID, float32 mass)
{
    _masses[entityID] = mass;
}

__IGOR_INLINE__ float32 iEntityDataPhysics::getMass(uint64 entityID)
{
    return _masses[entityID];
}

__IGOR_INLINE__ void iEntityDataPhysics::setBody(uint64 entityID, uint64 bodyID)
{
    _bodies[entityID] = bodyID;
}

__IGOR_INLINE__ uint64 iEntityDataPhysics::getBody(uint64 entityID)
{
    return _bodies[entityID];
}


