// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/profiler/iProfiler.h>

#include <cstring>
#include <algorithm>

namespace igor
{
    
    int32 iProfiler::_frame = 0;
    std::array<iaTime, PROFILER_MAX_FRAMES_COUNT> iProfiler::_frameTime;
    std::unordered_map<iaString, iProfilerSectionData> iProfiler::_sections;
    std::unordered_map<iaString, iProfilerCounterData> iProfiler::_counters;

    void iProfiler::nextFrame()
    {
        _frame = (_frame + 1) % PROFILER_MAX_FRAMES_COUNT;
        _frameTime[_frame] = iaTime::fromMilliseconds(0);
    }

    const std::vector<iProfilerSectionData> iProfiler::getSections()
    {
        std::vector<iProfilerSectionData> sections;

        for(const auto &pair : _sections)
        {
            sections.push_back(pair.second);
        }

        std::sort(sections.begin(), sections.end(), [](const iProfilerSectionData& a, const iProfilerSectionData& b)
        {
            return a._beginTime < b._beginTime;
        });

        return sections;
    }

    const std::vector<iProfilerCounterData> iProfiler::getCounters()
    {
        std::vector<iProfilerCounterData> counters;

        for(const auto &pair : _counters)
        {
            counters.push_back(pair.second);
        }

        return counters;
    }

    int32 iProfiler::getCurrentFrameIndex()
    {
        return _frame;
    }

    void iProfiler::setValue(const iaString &key, uint64 value)
    {
        auto iter = _counters.find(key);
        if (iter == _counters.end())
        {
            iProfilerCounterData counterData;
            counterData._name = key;
            memset(&counterData._counters, 0, sizeof(uint64) * PROFILER_MAX_FRAMES_COUNT);
            _counters[key] = counterData;
        }

        _counters[key]._counters[_frame] = value;
    }

    iProfilerSectionData& iProfiler::getSectionData(const iaString &sectionName)
    {
        auto iter = _sections.find(sectionName);
        if (iter == _sections.end())
        {
            iProfilerSectionData sectionData;
            sectionData._name = sectionName;
            memset(&sectionData._values, 0, sizeof(iaTime) * PROFILER_MAX_FRAMES_COUNT);

            _sections[sectionName] = sectionData;
            return _sections[sectionName];
        }
        else
        {
            return iter->second;
        }
    }

    void iProfiler::beginSection(const iaString &sectionName)
    {
        iProfilerSectionData& sectionData = getSectionData(sectionName);

        sectionData._values[_frame] = 0;
        sectionData._beginTime = iaTime::getNow();
    }

    void iProfiler::endSection(const iaString &sectionName)
    {
        iaTime now = iaTime::getNow();

        iProfilerSectionData& sectionData = getSectionData(sectionName);

        iaTime delta = now - sectionData._beginTime;

        sectionData._values[_frame] = delta;
        _frameTime[_frame] += delta;
    }

    iaTime iProfiler::getPeakFrame()
    {
        iaTime result;

        for(auto time : _frameTime)
        {
            if(time > result)
            {
                result = time;
            }
        }

        return result;
    }

    iProfilerSectionScoped::iProfilerSectionScoped(const iaString &sectionName)
    : _sectionName(sectionName)
    {
        iProfiler::beginSection(_sectionName);
    }

    iProfilerSectionScoped::~iProfilerSectionScoped()
    {
        iProfiler::endSection(_sectionName);
    }

} // namespace igor