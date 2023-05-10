// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/profiler/iProfiler.h>

#include <cstring>
#include <algorithm>

namespace igor
{
    
    int32 iProfiler::_frame = 0;
    std::array<iaTime, PROFILER_MAX_FRAMES_COUNT> iProfiler::_frameTime;
    std::unordered_map<int64, iProfilerSectionDataPtr> iProfiler::_sections;

    void iProfiler::nextFrame()
    {
        _frame = (_frame + 1) % PROFILER_MAX_FRAMES_COUNT;
        _frameTime[_frame] = iaTime::fromMilliseconds(0);
    }

    const std::vector<iProfilerSectionDataPtr> iProfiler::getSections()
    {
        std::vector<iProfilerSectionDataPtr> sections;

        for(auto section : _sections)
        {
            sections.push_back(section.second);
        }

        std::sort(sections.begin(), sections.end(), [](const iProfilerSectionDataPtr a, const iProfilerSectionDataPtr b)
        {
            return a->_beginTime < b->_beginTime;
        });

        return sections;
    }

    int32 iProfiler::getCurrentFrameIndex()
    {
        return _frame;
    }

    iProfilerSectionDataPtr iProfiler::getSectionData(const iaString &sectionName)
    {
        iProfilerSectionDataPtr sectionData;
        int64 hash = sectionName.getHashValue();
        auto iter = _sections.find(hash);
        if (iter == _sections.end())
        {
            sectionData = iProfilerSectionDataPtr(new iProfilerSectionData());
            sectionData->_name = sectionName;
            memset(&sectionData->_values, 0, sizeof(iaTime) * PROFILER_MAX_FRAMES_COUNT);

            _sections[hash] = sectionData;
        }
        else
        {
            sectionData = iter->second;
        }

        return sectionData;
    }

    void iProfiler::beginSection(const iaString &sectionName)
    {
        iProfilerSectionDataPtr sectionData = getSectionData(sectionName);

        sectionData->_values[_frame] = 0;
        sectionData->_beginTime = iaTime::getNow();
    }

    void iProfiler::endSection(const iaString &sectionName)
    {
        iaTime now = iaTime::getNow();

        iProfilerSectionDataPtr sectionData = getSectionData(sectionName);

        iaTime delta = now - sectionData->_beginTime;

        sectionData->_values[_frame] += delta;
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