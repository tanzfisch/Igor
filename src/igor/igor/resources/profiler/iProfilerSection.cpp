// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/profiler/iProfilerSection.h>
#include <igor/os/iTimer.h>

namespace Igor
{

    iProfilerSection::iProfilerSection()
    {
        memset(&_values, 0, sizeof(float64) * BUFFER_SIZE);
    }

    void iProfilerSection::setGroup(uint64 groupIndex)
    {
        _groupIndex = groupIndex;
    }

    uint64 iProfilerSection::getGroup() const
    {
        return _groupIndex;
    }

    void iProfilerSection::beginSection()
    {
        _beginTime = iTimer::getInstance().getApplicationTime();
    }

    void iProfilerSection::endSection()
    {
        _values[_currentFrame] += (iTimer::getInstance().getApplicationTime() - _beginTime);
    }

    const iaString &iProfilerSection::getName()
    {
        return _name;
    }

    void iProfilerSection::setName(const iaString &name)
    {
        _name = name;
    }

    void iProfilerSection::setCurrentFrame(uint64 currentFrame)
    {
        _currentFrame = currentFrame % BUFFER_SIZE;
        _values[_currentFrame] = 0;
    }

    const float64 *iProfilerSection::getValues() const
    {
        return _values;
    }

} // namespace Igor