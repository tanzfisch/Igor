// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/profiler/iProfilerVisualizer.h>
#include <igor/system/iTimer.h>
#include <igor/threading/iTaskManager.h>
#include <igor/renderer/iRenderer.h>

#include <iaux/data/iaRectangle.h>
#include <iaux/data/iaString.h>
using namespace iaux;

#include <cstring>

namespace igor
{

    static const iaColor4f COLORS[] =
        {
            iaColor4f(0.996, 0.608, 0.000, 1.0),
            iaColor4f(0.906, 0.141, 0.322, 1.0),
            iaColor4f(0.396, 0.000, 0.502, 1.0),
            iaColor4f(0.157, 0.220, 0.416, 1.0),
            iaColor4f(0.133, 0.467, 0.706, 1.0),
            iaColor4f(0.000, 0.588, 0.620, 1.0),
            iaColor4f(0.000, 0.475, 0.365, 1.0),
            iaColor4f(0.278, 0.729, 0.345, 1.0),
            iaColor4f(0.600, 0.600, 0.600, 1.0),
            iaColor4f(0.800, 0.800, 0.800, 1.0),
            iaColor4f(0.996, 0.925, 0.702, 1.0),
            iaColor4f(0.996, 0.863, 0.573, 1.0),
            iaColor4f(0.996, 0.349, 0.384, 1.0),
            iaColor4f(0.553, 0.263, 0.463, 1.0),
            iaColor4f(0.400, 0.400, 0.400, 1.0),
            iaColor4f(0.745, 0.745, 0.745, 1.0),
            iaColor4f(0.443, 0.776, 0.776, 1.0),
            iaColor4f(0.941, 0.769, 0.871, 1.0),
            iaColor4f(0.922, 0.545, 0.718, 1.0),
            iaColor4f(0.984, 0.710, 0.447, 1.0)};

    static const int32 COLOR_COUNT = 20;

    iProfilerVisualizer::iProfilerVisualizer()
    {
        _time = iTimer::getInstance().getTime();
    }

    iProfilerVisualizer::~iProfilerVisualizer()
    {
    }

    void iProfilerVisualizer::setVerbosity(iProfilerVerbosity renderStatisticsMode)
    {
        _renderStatisticsMode = renderStatisticsMode;
    }

    void iProfilerVisualizer::cycleVerbosity()
    {
        _renderStatisticsMode = static_cast<iProfilerVerbosity>(static_cast<int>(_renderStatisticsMode) + 1);
        if (_renderStatisticsMode > iProfilerVerbosity::SectionsAndValues)
        {
            _renderStatisticsMode = iProfilerVerbosity::None;
        }
    }

    iProfilerVerbosity iProfilerVisualizer::getVerbosity()
    {
        return _renderStatisticsMode;
    }

    void iProfilerVisualizer::draw(iWindowPtr window, const iTextureFontPtr &font)
    {
        iProfiler::beginSection("profiler visualizer");

        if (_renderStatisticsMode == iProfilerVerbosity::None)
        {
            iProfiler::endSection("profiler visualizer");
            return;
        }

        iaTime begin = iaTime::getNow();

        const iRenderer::iRendererStats stats = iRenderer::getInstance().getStats();
        const auto currentFrameIndex = iProfiler::getCurrentFrameIndex();

        if (iTimer::getInstance().getTime() > _time + iaTime::fromSeconds(0.25))
        {
            _time = iTimer::getInstance().getTime();
            _lastFPS = iTimer::getInstance().getFPS();

            if (_renderStatisticsMode >= iProfilerVerbosity::FPSMetricsAndTasks)
            {
                _lastThreadCount = iTaskManager::getInstance().getRegularThreadCount();
                _lastQueuedTaskCount = iTaskManager::getInstance().getQueuedRegularTaskCount();
                _lastRunningTaskCount = iTaskManager::getInstance().getRunningRegularTaskCount();

                _lastRenderContextThreadCount = iTaskManager::getInstance().getRenderContextThreadCount();
                _lastQueuedRenderContextTaskCount = iTaskManager::getInstance().getQueuedRenderContextTaskCount();
                _lastRunningRenderContextTaskCount = iTaskManager::getInstance().getRunningRenderContextTaskCount();

                _lastDoneTaskCount = iTaskManager::getInstance().getTaskDoneCount();
            }
        }

        const iaColor4f backgroundColor(0, 0, 0, 0.5);

        iRenderer::getInstance().setFont(font);
        iRenderer::getInstance().setFontSize(16.0f);

        // draw footer background
        iRenderer::getInstance().drawFilledRectangle(0, window->getClientHeight() - 40, window->getClientWidth(), 40, backgroundColor);

        // always draw fps
        const iaString fpsText = iaString::toString(_lastFPS, 2) + L" fps";
        iRenderer::getInstance().drawString(static_cast<float32>(window->getClientWidth() - 10), static_cast<float32>(window->getClientHeight() - 10), fpsText, iHorizontalAlignment::Right, iVerticalAlignment::Bottom, iaColor4f::magenta);

        if (_renderStatisticsMode >= iProfilerVerbosity::FPSAndMetrics)
        {
            iaString info = "v:";
            info += iaString::align(iaString::toStringUnits(stats._vertices), 4, iaString::Alignment::Right);
            info += " t:";
            info += iaString::align(iaString::toStringUnits(stats._triangles), 4, iaString::Alignment::Right);
            info += " i:";
            info += iaString::align(iaString::toStringUnits(stats._indices), 4, iaString::Alignment::Right);
            info += " dc:";
            info += iaString::align(iaString::toStringUnits(stats._drawCalls), 4, iaString::Alignment::Right);
            iRenderer::getInstance().drawString(static_cast<float32>(window->getClientWidth() - 200), static_cast<float32>(window->getClientHeight() - 10), info, iHorizontalAlignment::Right, iVerticalAlignment::Bottom, iaColor4f::magenta);
        }

        if (_renderStatisticsMode >= iProfilerVerbosity::FPSMetricsAndTasks)
        {
            iaString threads = "tasks [";
            threads += iaString::toString(_lastRunningTaskCount + _lastRunningRenderContextTaskCount);
            threads += ":";
            threads += iaString::toString(_lastQueuedTaskCount + _lastQueuedRenderContextTaskCount);
            threads += "]";

            iRenderer::getInstance().drawString(10.0f, static_cast<float32>(window->getClientHeight() - 10), threads, iHorizontalAlignment::Left, iVerticalAlignment::Bottom, iaColor4f::magenta);
        }

        if (_renderStatisticsMode >= iProfilerVerbosity::Sections)
        {
            const iaRectanglef rect(0, 0, window->getClientWidth(), window->getClientHeight() * 0.4);

            iRenderer::getInstance().drawFilledRectangle(rect.getLeft(), rect.getTop(), rect.getWidth(), rect.getHeight(), backgroundColor);

            uint32 sectionIndex = 0;
            const int32 lineCount = std::min(static_cast<int>(rect.getWidth()), PROFILER_MAX_FRAMES_COUNT);
            const int32 currentFrame = (currentFrameIndex + 1 - lineCount) % PROFILER_MAX_FRAMES_COUNT;

            memset(&_accumulationBuffer, 0, sizeof(float32) * PROFILER_MAX_FRAMES_COUNT);
            const auto sections = iProfiler::getSections();

            const float32 peakFrameTime = iProfiler::getPeakFrame().getMilliseconds();
            const float32 verticalScale = rect._height / (peakFrameTime + 5.0);

            for (const auto &section : sections)
            {
                const auto &values = section._values;

                const iaColor4f &sectionColor = COLORS[sectionIndex % COLOR_COUNT];

                for (int i = 0; i < lineCount; ++i)
                {
                    int32 currentIndex = (currentFrame + i) % PROFILER_MAX_FRAMES_COUNT;
                    if (currentIndex < 0)
                    {
                        currentIndex += PROFILER_MAX_FRAMES_COUNT;
                    }
                    const float32 bottomValue = _accumulationBuffer[currentIndex];
                    _accumulationBuffer[currentIndex] += values[currentIndex].getMilliseconds() * verticalScale;
                    const float32 topValue = _accumulationBuffer[currentIndex];

                    iRenderer::getInstance().drawLine(i + rect.getRight() - lineCount,
                                                      rect.getBottom() - bottomValue, i + rect.getRight() - lineCount,
                                                      rect.getBottom() - topValue,
                                                      sectionColor);
                }

                sectionIndex++;
            }

            const iaColor4f lineColor(1.0, 1.0, 1.0, 0.5);
            const iaColor4f peakLineColor(1.0, 0.0, 0.0, 0.5);

            iRenderer::getInstance().setLineWidth(1);
            iRenderer::getInstance().drawLine(rect.getLeft(), rect.getBottom(), rect.getRight(), rect.getBottom());

            const float32 peak = peakFrameTime * verticalScale;
            const float32 Hz24 = 41.6666 * verticalScale;
            const float32 Hz60 = 16.6666 * verticalScale;
            const float32 Hz100 = 10 * verticalScale;

            iRenderer::getInstance().drawLine(rect.getLeft(), rect.getBottom() - peak, rect.getRight(), rect.getBottom() - peak, peakLineColor);
            iRenderer::getInstance().drawLine(rect.getLeft(), rect.getBottom() - Hz24, rect.getRight(), rect.getBottom() - Hz24, lineColor);
            iRenderer::getInstance().drawLine(rect.getLeft(), rect.getBottom() - Hz60, rect.getRight(), rect.getBottom() - Hz60, lineColor);
            iRenderer::getInstance().drawLine(rect.getLeft(), rect.getBottom() - Hz100, rect.getRight(), rect.getBottom() - Hz100, lineColor);

            iRenderer::getInstance().drawString(rect.getX(), rect.getBottom() - peak, iaString::toString(peakFrameTime, 0) + "ms", iHorizontalAlignment::Left, iVerticalAlignment::Bottom);
            iRenderer::getInstance().drawString(rect.getX(), rect.getBottom() - Hz24, "24Hz", iHorizontalAlignment::Left, iVerticalAlignment::Bottom);
            iRenderer::getInstance().drawString(rect.getX(), rect.getBottom() - Hz60, "60Hz", iHorizontalAlignment::Left, iVerticalAlignment::Bottom);
            iRenderer::getInstance().drawString(rect.getX(), rect.getBottom() - Hz100, "100Hz", iHorizontalAlignment::Left, iVerticalAlignment::Bottom);

            sectionIndex = 0;
            float32 textOffsetY = 20.0f;
            for (const auto &section : sections)
            {
                const iaColor4f &sectionColor = COLORS[sectionIndex % COLOR_COUNT];

                iRenderer::getInstance().drawString(rect.getRight() - 250, rect.getBottom() - textOffsetY, section._name, iHorizontalAlignment::Left, iVerticalAlignment::Bottom, sectionColor);
                textOffsetY += 20;

                sectionIndex++;
            }
        }

        if (_renderStatisticsMode >= iProfilerVerbosity::SectionsAndValues)
        {
            const iaRectanglef rect(0, window->getClientHeight() * 0.40, window->getClientWidth(), window->getClientHeight() * 0.4);

            iRenderer::getInstance().drawFilledRectangle(rect.getLeft(), rect.getTop(), rect.getWidth(), rect.getHeight(), backgroundColor);

            const float32 verticalScale = rect._height / 30.0;

            uint32 colorIndex = 0;
            const int32 lineCount = std::min(static_cast<int>(rect.getWidth()), PROFILER_MAX_FRAMES_COUNT);
            const int32 currentFrame = (currentFrameIndex + 1 - lineCount) % PROFILER_MAX_FRAMES_COUNT;

            int32 lastFrame = (currentFrameIndex - 1) % PROFILER_MAX_FRAMES_COUNT;
            if (lastFrame < 0)
            {
                lastFrame += PROFILER_MAX_FRAMES_COUNT;
            }

            const auto counters = iProfiler::getCounters();

            for (const auto &counter : counters)
            {
                const iaColor4f &sectionColor = COLORS[colorIndex % COLOR_COUNT];

                for (int i = 0; i < lineCount; ++i)
                {
                    int32 currentIndex = (currentFrame + i) % PROFILER_MAX_FRAMES_COUNT;
                    int32 lastIndex = (currentFrame + i - 1) % PROFILER_MAX_FRAMES_COUNT;

                    if (currentIndex < 0)
                    {
                        currentIndex += PROFILER_MAX_FRAMES_COUNT;
                    }

                    if (lastIndex < 0)
                    {
                        lastIndex += PROFILER_MAX_FRAMES_COUNT;
                    }

                    iRenderer::getInstance().drawLine(i + rect.getRight() - lineCount,
                                                      rect.getBottom() - static_cast<float32>(std::log2(counter._counters[lastIndex])) * verticalScale,
                                                      i + 1 + rect.getRight() - lineCount,
                                                      rect.getBottom() - static_cast<float32>(std::log2(counter._counters[currentIndex])) * verticalScale,
                                                      sectionColor);
                }

                colorIndex++;
            }

            colorIndex = 0;
            float32 textOffsetY = 20.0f;

            for (const auto &counter : counters)
            {
                const iaColor4f &sectionColor = COLORS[colorIndex % COLOR_COUNT];

                iRenderer::getInstance().drawString(rect.getRight() - 250, rect.getBottom() - textOffsetY, counter._name, iHorizontalAlignment::Left, iVerticalAlignment::Bottom, sectionColor);
                iRenderer::getInstance().drawString(rect.getRight() - 100, rect.getBottom() - textOffsetY, iaString::toStringUnits(counter._counters[lastFrame]), iHorizontalAlignment::Left, iVerticalAlignment::Bottom, sectionColor);
                textOffsetY += 20;

                colorIndex++;
            }
        }

        iProfiler::endSection("profiler visualizer");
        auto &profilerSectionData = iProfiler::getSectionData("profiler visualizer");

        // TODO #418 "support nested profiler sections"

        // exclude profiler visualizer from rendering section
        auto &sectionData = iProfiler::getSectionData("render");
        sectionData._beginTime += profilerSectionData._values[currentFrameIndex];
    }

} // namespace igor