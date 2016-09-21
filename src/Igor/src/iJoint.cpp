// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iJoint.h>

#include <iBone.h>
#include <iBoneFactory.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    uint64 iJoint::_nextID = iJoint::INVALID_JOINT_ID + 1;

    iJoint::iJoint()
    {
        _id = _nextID++;
    }

    uint64 iJoint::getID() const
    {
        return _id;
    }

    void iJoint::setBaseBone(uint64 boneID)
    {
        iBone* bone = iBoneFactory::getInstance().getBone(boneID);
        bone->_jointTop = _id;

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

            iBone* bone = iBoneFactory::getInstance().getBone(boneID);
            bone->_jointBottom = _id;
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

            iBone* bone = iBoneFactory::getInstance().getBone(boneID);
            bone->_jointBottom = iJoint::INVALID_JOINT_ID;
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

    void iJoint::setCustomData(void* data)
    {
        _customData = data;
    }

    void* iJoint::getCustomData() const
    {
        return _customData;
    }

}