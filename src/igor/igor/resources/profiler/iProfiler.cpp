// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/profiler/iProfiler.h>

#include <igor/graphics/iRenderer.h>

#include <cstring>

namespace igor
{
    std::unordered_map<int64, iProfilerSection> iProfiler::_sections;

    int32 iProfiler::_frame = 0;

    iProfiler::iProfiler(const iaString &sectionName)
    {
        int64 hash = sectionName.getHashValue();

        auto iter = _sections.find(hash);

        if (iter == _sections.end())
        {
            auto &section = _sections[hash] = iProfilerSection();
            section._name = sectionName;
            memset(&section._values, 0, sizeof(iaTime) * MAX_FRAMES_COUNT);

            _section = &section;
        }
        else
        {
            _section = &(iter->second);
        }

        _section->_values[_frame] = 0;
        _section->_beginTime = iaTime::getNow();
    }

    iProfiler::~iProfiler()
    {
        _section->_values[_frame] += iaTime::getNow() - _section->_beginTime;
    }

    void iProfiler::nextFrame()
    {
        _frame = (_frame + 1) % MAX_FRAMES_COUNT;
    }

    const std::unordered_map<int64, iProfilerSection> &iProfiler::getSections()
    {
        return _sections;
    }

    int32 iProfiler::getCurrentFrameIndex()
    {
        return _frame;
    }

} // namespace igor