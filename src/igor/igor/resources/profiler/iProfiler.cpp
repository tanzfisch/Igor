// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/profiler/iProfiler.h>

#include <cstring>

namespace igor
{
    std::unordered_map<int64, iProfilerSectionData> iProfiler::_sections;
    int32 iProfiler::_frame = 0;

    void iProfiler::nextFrame()
    {
        _frame = (_frame + 1) % MAX_FRAMES_COUNT;
    }

    const std::unordered_map<int64, iProfilerSectionData> &iProfiler::getSections()
    {
        return _sections;
    }

    int32 iProfiler::getCurrentFrameIndex()
    {
        return _frame;
    }

    iProfilerSectionData *iProfiler::getSectionData(const iaString &sectionName)
    {
        iProfilerSectionData *sectionData;
        int64 hash = sectionName.getHashValue();
        auto iter = iProfiler::_sections.find(hash);
        if (iter == iProfiler::_sections.end())
        {
            auto &section = iProfiler::_sections[hash] = iProfilerSectionData();
            section._name = sectionName;
            memset(&section._values, 0, sizeof(iaTime) * iProfiler::MAX_FRAMES_COUNT);

            sectionData = &section;
        }
        else
        {
            sectionData = &(iter->second);
        }

        return sectionData;
    }

    void iProfiler::beginSection(const iaString &sectionName)
    {
        iProfilerSectionData *sectionData = getSectionData(sectionName);

        sectionData->_values[iProfiler::_frame] = 0;
        sectionData->_beginTime = iaTime::getNow();
    }

    void iProfiler::endSection(const iaString &sectionName)
    {
        iaTime now = iaTime::getNow();

        iProfilerSectionData *sectionData = getSectionData(sectionName);

        sectionData->_values[iProfiler::_frame] += now - sectionData->_beginTime;
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