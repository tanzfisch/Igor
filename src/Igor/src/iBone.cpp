// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iBone.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    uint64 iBone::_nextID = iBone::INVALID_BONE_ID + 1;

    iBone::iBone()
    {
        _id = _nextID++;
    }

    void iBone::setMatrix(const iaMatrixf& matrix)
    {
        _matrix = matrix;
    }

    void iBone::getMatrix(iaMatrixf& matrix) const
    {
        matrix = _matrix;
    }

    uint64 iBone::getID() const
    {
        return _id;
    }

    void iBone::setLenght(float64 lenght)
    {
        con_assert(lenght >= 0, "lenght can not be negative");
        if (lenght >= 0)
        {
            _lenght = lenght;
        }
    }

    float64 iBone::getLenght() const
    {
        return _lenght;
    }

    uint64 iBone::getTopJoint() const
    {
        return _jointTop;
    }

    uint64 iBone::getBottomJoint() const
    {
        return _jointBottom;
    }

    void iBone::setCustomData(void* data)
    {
        _customData = data;
    }

    void* iBone::getCustomData() const
    {
        return _customData;
    }


}