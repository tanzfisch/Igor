// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/data/iSkeleton.h>

#include <igor/data/iBone.h>
#include <igor/data/iJoint.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <algorithm>

namespace igor
{

    iBone *iSkeleton::createBone()
    {
        iBone *bone = new iBone();
        _bones.push_back(bone);
        return bone;
    }

    void iSkeleton::destroyBone(iBone *bone)
    {
        con_assert(bone != nullptr, "zero pointer");

        auto iter = std::find(_bones.begin(), _bones.end(), bone);
        if (iter != _bones.end())
        {
            delete (*iter);
            _bones.erase(iter);
        }
        else
        {
            con_err("bone not found");
        }
    }

    iJoint *iSkeleton::createJoint()
    {
        iJoint *joint = new iJoint();
        _joints.push_back(joint);
        return joint;
    }

    void iSkeleton::destroyJoint(iJoint *joint)
    {
        con_assert(joint != nullptr, "zero pointer");

        auto iter = std::find(_joints.begin(), _joints.end(), joint);
        if (iter != _joints.end())
        {
            delete (*iter);
            _joints.erase(iter);
        }
        else
        {
            con_err("joint not found");
        }
    }

    void iSkeleton::addBone(const iaMatrixf &matrix, float64 length)
    {
        iJoint *currentJoint = nullptr;

        if (_rootJoint == nullptr)
        {
            _rootJoint = createJoint();
            _currentJoint = _rootJoint;
        }

        if (_currentJoint != nullptr)
        {
            currentJoint = _currentJoint;
        }

        if (currentJoint == nullptr)
        {
            _currentJoint = createJoint();
            currentJoint = _currentJoint;

            con_assert(_lastBone != nullptr, "inconsistant data");
            currentJoint->setBaseBone(_lastBone);
        }

        _lastBone = createBone();
        _lastBone->setMatrix(matrix);
        _lastBone->setLenght(length);

        currentJoint->addChildBone(_lastBone);

        _currentJoint = nullptr;
    }

    iJoint *iSkeleton::getRootJoint() const
    {
        return _rootJoint;
    }

    iBone *iSkeleton::getLastBone() const
    {
        return _lastBone;
    }

    void iSkeleton::push()
    {
        con_assert(_lastBone != nullptr, "zero pointer");
        _boneStack.push_back(_lastBone);
    }

    void iSkeleton::pop()
    {
        _lastBone = _boneStack.back();
        _boneStack.pop_back();
        _currentJoint = _lastBone->getTopJoint();
    }

    void iSkeleton::clear()
    {
        for (auto bone : _bones)
        {
            delete bone;
        }

        for (auto joint : _joints)
        {
            delete joint;
        }

        _bones.clear();
        _joints.clear();
        _boneStack.clear();
        _rootJoint = nullptr;
        _currentJoint = nullptr;
        _lastBone = nullptr;
    }

} // namespace igor