// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/profiler/iProfiler.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/system/iWindow.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/system/iTimer.h>
#include <igor/threading/iTaskManager.h>

#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{

#define SECTOINDEX(sec) (sec - 1)

    iProfilerSectionID iProfiler::createSection(const iaString &sectionName)
    {
        iProfilerSection statisticsSection;
        statisticsSection._name = sectionName;
        memset(&statisticsSection._values, 0, sizeof(iaTime) * MAX_FRAMES_COUNT);

        _sections.push_back(statisticsSection);
        return _sections.size();
    }

    void iProfiler::beginSection(iProfilerSectionID sectionID)
    {
        if (SECTOINDEX(sectionID) >= _sections.size())
        {
            return;
        }

        _sections[SECTOINDEX(sectionID)]._values[_frame] = 0;
        _sections[SECTOINDEX(sectionID)]._beginTime = iaTime::now();
    }

    void iProfiler::endSection(iProfilerSectionID sectionID)
    {
        if (SECTOINDEX(sectionID) >= _sections.size())
        {
            return;
        }

        _sections[SECTOINDEX(sectionID)]._values[_frame] += iaTime::now() - _sections[SECTOINDEX(sectionID)]._beginTime;
    }

    void iProfiler::nextFrame()
    {
        _frame = (_frame + 1) % MAX_FRAMES_COUNT;
    }

    const std::vector<iProfiler::iProfilerSection> &iProfiler::getSections() const
    {
        return _sections;
    }

    int32 iProfiler::getCurrentFrameIndex() const
    {
        return _frame;
    }

} // namespace igor