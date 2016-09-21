// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iSkeleton.h>

#include <iBoneFactory.h>
#include <iBone.h>
#include <iJoint.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    void iSkeleton::addBone(const iaMatrixf& matrix, float64 lenght)
    {
        iJoint* currentJoint = nullptr;

        if (_rootJoint == iJoint::INVALID_JOINT_ID)
        {
            _rootJoint = iBoneFactory::getInstance().createJoint();
            _joints.push_back(_rootJoint);
            _currentJoint = _rootJoint;
        }

        if (_currentJoint != iJoint::INVALID_JOINT_ID)
        {
            currentJoint = iBoneFactory::getInstance().getJoint(_currentJoint);
        }

        if (currentJoint == nullptr)
        {
            _currentJoint = iBoneFactory::getInstance().createJoint();
            _joints.push_back(_currentJoint);
            currentJoint = iBoneFactory::getInstance().getJoint(_currentJoint);

            con_assert(_lastBone != iBone::INVALID_BONE_ID, "inconsistant data");
            currentJoint->setBaseBone(_lastBone);
        }

        _lastBone = iBoneFactory::getInstance().createBone();
        _bones.push_back(_lastBone);
        iBone* bone = iBoneFactory::getInstance().getBone(_lastBone);
        bone->setMatrix(matrix);
        bone->setLenght(lenght);

        currentJoint->addChildBone(_lastBone);

        _currentJoint = iJoint::INVALID_JOINT_ID;
    }

    uint64 iSkeleton::getRootJoint() const
    {
        return _rootJoint;
    }

    uint64 iSkeleton::getLastBone() const
    {
        return _lastBone;
    }

    void iSkeleton::push()
    {
        con_assert(_lastBone > 0, "invalid bone id");
        _boneStack.push_back(_lastBone);
    }

    void iSkeleton::pop()
    {
        _lastBone = _boneStack.back();
        _boneStack.pop_back();
        iBone* bone = iBoneFactory::getInstance().getBone(_lastBone);
        _currentJoint = bone->getTopJoint();
    }

    void iSkeleton::clear()
    {
        for (auto bone : _bones)
        {
            iBoneFactory::getInstance().destroyBone(bone);
        }

        for (auto joint : _joints)
        {
            iBoneFactory::getInstance().destroyJoint(joint);
        }

        _bones.clear();
        _joints.clear();
        _boneStack.clear();
        _rootJoint = iJoint::INVALID_JOINT_ID;
        _currentJoint = iJoint::INVALID_JOINT_ID;
        _lastBone = iBone::INVALID_BONE_ID;
    }

}