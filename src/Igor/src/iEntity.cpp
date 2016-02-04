// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntity.h>

#include <iPhysicsBody.h>
#include <iPhysics.h>
#include <iNodeTransform.h>
using namespace Igor;

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    uint32 iEntity::getID()
    {
        return _id;
    }

    void iEntity::setBool(uint32 index, bool value)
    {
        _customBools[index] = value;
    }

    void iEntity::setFloat(uint32 index, float32 value)
    {
        _customFloats[index] = value;
    }

    bool iEntity::getBool(uint32 index)
    {
        con_assert(_customBools.end() != _customBools.find(index), "id does not exist");
        return _customBools[index];
    }

    float32 iEntity::getFloat(uint32 index)
    {
        con_assert(_customFloats.end() != _customFloats.find(index), "id does not exist");
        return _customFloats[index];
    }

    void iEntity::setInt(uint32 index, int64 value)
    {
        _sInts[index] = value;
    }

    int64 iEntity::getInt(uint32 index)
    {
        con_assert(_customInts.end() != _customInts.find(index), "id does not exist");
        return _sInts[index];
    }

    void iEntity::setUInt(uint32 index, uint64 value)
    {
        _uInts[index] = value;
    }

    uint64 iEntity::getUInt(uint32 index)
    {
        con_assert(_uInts.end() != _uInts.find(index), "id does not exist");
        return _uInts[index];
    }

    void iEntity::setVector(uint32 index, const iaVector3f& value)
    {
        _vectors[index] = value;
    }

    const iaVector3f& iEntity::getVector(uint32 index)
    {
        con_assert(_vectors.end() != _vectors.find(index), "id does not exist");
        return _vectors[index];
    }

    void iEntity::setMatrix(uint32 index, const iaMatrixf& value)
    {
        _matrices[index] = value;
    }

    const iaMatrixf& iEntity::getMatrix(uint32 index)
    {
        con_assert(_matrices.end() != _matrices.find(index), "id does not exist");
        return _matrices[index];
    }

}