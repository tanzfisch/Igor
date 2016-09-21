// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iJoint.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iJoint::iJoint()
    {
    }

    void iJoint::setBaseBone(uint64 boneID)
    {
        _baseBoneID = boneID;
    }

    uint64 iJoint::getBaseBone() const
    {
        return _baseBoneID;
    }

    void iJoint::addChildBone(uint64 boneID)
    {
        auto iter = std::find(_childBones.begin(), _childBones.end(), boneID);
        if (iter == _childBones.end())
        {
            _childBones.push_back(boneID);
        }
        else
        {
            con_err("bone id " << boneID << " already joined");
        }
    }

    void iJoint::removeChildBone(uint64 boneID)
    {
        auto iter = std::find(_childBones.begin(), _childBones.end(), boneID);
        if (iter != _childBones.end())
        {
            _childBones.erase(iter);
        }
        else
        {
            con_err("bone id " << boneID << " was not joined");
        }
    }

    vector<uint64> iJoint::getChildren() const
    {
        return _childBones;
    }

}