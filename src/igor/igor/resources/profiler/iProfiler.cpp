// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/profiler/iProfiler.h>

#include <igor/graphics/iRenderer.h>

namespace igor
{

#ifdef IGOR_DEEP_PROFILING
    std::map<uint64, iProfilerFunction *> iProfilerFunction::_data;
    static iaMutex iProfilerFunction::_mutex;
#endif

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

    void iProfiler::nextFrame(bool loggingFrame)
    {
        iRenderer::getInstance().onStopFrame();

        _frame = (_frame + 1) % MAX_FRAMES_COUNT;

#ifdef IGOR_DEEP_PROFILING
        if (loggingFrame)
        {
            iProfilerFunction::_mutex.lock();
            std::map<uint64, iProfilerFunction *> data = iProfilerFunction::_data;
            iProfilerFunction::_mutex.unlock();

            std::vector<iProfilerFunction *> functions;
            for (const auto pair : data)
            {
                functions.push_back(pair.second);
            }

            std::sort(functions.begin(), functions.end(), [](const iProfilerFunction *a, const iProfilerFunction *b) {
                return (a->_duration.getMilliseconds() / a->_callCount) > (b->_duration.getMilliseconds() / b->_callCount);
            });

            for (const auto func : functions)
            {
                con_endl(func->_name << " " << func->_file << ":" << func->_line);
                con_endl("calls:" << func->_callCount << " avg:" << (func->_duration.getMilliseconds() / func->_callCount) << "ms total:" << func->_duration.getMilliseconds() << "ms");
            }
        }
#endif

        iRenderer::getInstance().onStartFrame(loggingFrame);
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