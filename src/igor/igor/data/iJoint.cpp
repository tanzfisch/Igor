// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/data/iJoint.h>

#include <igor/data/iBone.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <algorithm>

namespace igor
{

    iJoint::iJoint()
    {
    }

    void iJoint::setBaseBone(iBone *bone)
    {
        con_assert(bone != nullptr, "zero pointer");

        bone->_jointTop = this;
        _baseBone = bone;
    }

    iBone *iJoint::getBaseBone() const
    {
        return _baseBone;
    }

    void iJoint::addChildBone(iBone *bone)
    {
        con_assert(bone != nullptr, "zero pointer");

        auto iter = std::find(_childBones.begin(), _childBones.end(), bone);
        if (iter == _childBones.end())
        {
            _childBones.push_back(bone);

            bone->_jointBottom = this;
        }
        else
        {
            con_err("bone already joined");
        }
    }

    void iJoint::removeChildBone(iBone *bone)
    {
        con_assert(bone != nullptr, "zero pointer");

        auto iter = std::find(_childBones.begin(), _childBones.end(), bone);
        if (iter != _childBones.end())
        {
            _childBones.erase(iter);

            bone->_jointBottom = nullptr;
        }
        else
        {
            con_err("bone was not joined");
        }
    }

    std::vector<iBone *> iJoint::getChildren() const
    {
        return _childBones;
    }

    void iJoint::setCustomData(const std::any &data)
    {
        _customData = data;
    }

    std::any iJoint::getCustomData() const
    {
        return _customData;
    }

} // namespace igor