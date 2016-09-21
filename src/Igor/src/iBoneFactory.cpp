// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iBoneFactory.h>

#include <iBone.h>
#include <iJoint.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
    
    iBoneFactory::iBoneFactory()
    {
    }

    uint64 iBoneFactory::createBone()
    {
        iBone* bone = new iBone();
        _bones[bone->getID()] = bone;
        return bone->getID();
    }

    void iBoneFactory::destroyBone(uint64 boneID)
    {
        auto iter = _bones.find(boneID);
        if (iter != _bones.end())
        {
            delete (*iter).second;
            _bones.erase(iter);
        }
        else
        {
            con_err("bone with id " << boneID << " not found");
        }
    }

    iBone* iBoneFactory::getBone(uint64 boneID)
    {
        iBone* result = nullptr;

        auto iter = _bones.find(boneID);
        if (iter != _bones.end())
        {
            result = (*iter).second;
        }

        return result;
    }

    uint64 iBoneFactory::createJoint()
    {
        iJoint* joint = new iJoint();
        _joints[joint->getID()] = joint;
        return joint->getID();
    }

    void iBoneFactory::destroyJoint(uint64 jointID)
    {
        auto iter = _joints.find(jointID);
        if (iter != _joints.end())
        {
            delete (*iter).second;
            _joints.erase(iter);
        }
        else
        {
            con_err("joint with id " << jointID << " not found");
        }
    }

    iJoint* iBoneFactory::getJoint(uint64 jointID)
    {
        iJoint* result = nullptr;

        auto iter = _joints.find(jointID);
        if (iter != _joints.end())
        {
            result = (*iter).second;
        }

        return result;
    }
}