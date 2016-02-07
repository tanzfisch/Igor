// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iStatisticsSection.h>
#include <iTimer.h>

namespace Igor
{

    iStatisticsSection::iStatisticsSection()
    {
        memset(&_begin, 0, sizeof(float64) * BUFFER_SIZE);
        memset(&_end, 0, sizeof(float64) * BUFFER_SIZE);
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
        _begin[_currentFrame] = iTimer::getInstance().getTime();
    }

    void iStatisticsSection::endSection()
    {
        _end[_currentFrame] = iTimer::getInstance().getTime();
        _currentFrame = (_currentFrame + 1) % BUFFER_SIZE;
    }

    void iStatisticsSection::setSectionLenght(float64 lenght)
    {
        _begin[_currentFrame] = 0;
        _end[_currentFrame] = lenght;
        _currentFrame = (_currentFrame + 1) % BUFFER_SIZE;
    }

    const iaString& iStatisticsSection::getName()
    {
        return _name;
    }

    void iStatisticsSection::setName(const iaString& name)
    {
        _name = name;
    }

    uint32 iStatisticsSection::getCurrentFrame() const
    {
        return _currentFrame;
    }

    const float64* iStatisticsSection::getBeginnings() const
    {
        return _begin;
    }

    const float64* iStatisticsSection::getEnds() const
    {
        return _end;
    }

}