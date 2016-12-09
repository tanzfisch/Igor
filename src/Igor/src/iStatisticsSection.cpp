// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iStatisticsSection.h>
#include <iTimer.h>

namespace Igor
{

    iStatisticsSection::iStatisticsSection()
    {
        memset(&_values, 0, sizeof(float64) * BUFFER_SIZE);
    }

    void iStatisticsSection::setGroup(uint64 groupIndex)
    {
        _groupIndex = groupIndex;
    }

    uint64 iStatisticsSection::getGroup() const
    {
        return _groupIndex;
    }

    void iStatisticsSection::setColor(const iaColor4f& color)
    {
        _color = color;
    }

    const iaColor4f& iStatisticsSection::getColor()
    {
        return _color;
    }
    
    void iStatisticsSection::beginSection()
    {
        _beginTime = iTimer::getInstance().getTime();
    }

    void iStatisticsSection::endSection()
    {
        _values[_currentFrame] += (iTimer::getInstance().getTime() - _beginTime);
    }

    const iaString& iStatisticsSection::getName()
    {
        return _name;
    }

    void iStatisticsSection::setName(const iaString& name)
    {
        _name = name;
    }

    void iStatisticsSection::setCurrentFrame(uint64 currentFrame)
    {
        _currentFrame = currentFrame % BUFFER_SIZE;
        _values[_currentFrame] = 0;
    }

    const float64* iStatisticsSection::getValues() const
    {
        return _values;
    }

}