// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/data/iBone.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iBone::iBone()
    {
    }

    void iBone::setMatrix(const iaMatrixf &matrix)
    {
        _matrix = matrix;
    }

    void iBone::getMatrix(iaMatrixf &matrix) const
    {
        matrix = _matrix;
    }

    void iBone::setLenght(float64 length)
    {
        con_assert(length >= 0, "length can not be negative");
        if (length >= 0)
        {
            _lenght = length;
        }
    }

    float64 iBone::getLenght() const
    {
        return _lenght;
    }

    iJoint *iBone::getTopJoint() const
    {
        return _jointTop;
    }

    iJoint *iBone::getBottomJoint() const
    {
        return _jointBottom;
    }

    void iBone::setCustomData(const std::any &data)
    {
        _userData = data;
    }

    std::any iBone::getCustomData() const
    {
        return _userData;
    }

} // namespace igor