// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/profiler/iProfilerVisualizer.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/system/iTimer.h>
#include <igor/threading/iTaskManager.h>
#include <igor/renderer/iRenderer2.h>

#include <iaux/data/iaRectangle.h>
#include <iaux/data/iaString.h>
using namespace iaux;

#include <cstring>

namespace igor
{

    static const iaColor4f COLORS[] =
        {
            iaColor4f(1, 0, 0, 1),
            iaColor4f(0, 1, 0, 1),
            iaColor4f(0, 0, 1, 1),
            iaColor4f(1, 1, 0, 1),
            iaColor4f(0, 1, 1, 1),
            iaColor4f(1, 0, 1, 1),
            iaColor4f(0.9, 0.9, 0.9, 1),

            iaColor4f(0.75, 0.25, 0, 1),
            iaColor4f(0, 0.75, 0.25, 1),
            iaColor4f(0.25, 0, 0.75, 1),
            iaColor4f(0.75, 0.75, 0.25, 1),
            iaColor4f(0.25, 0.75, 0.75, 1),
            iaColor4f(0.75, 0.25, 0.75, 1),

            iaColor4f(0.5, 0, 0, 1),
            iaColor4f(0, 0.5, 0, 1),
            iaColor4f(0, 0, 0.5, 1),
            iaColor4f(0.5, 0.5, 0, 1),
            iaColor4f(0, 0.5, 0.5, 1),
            iaColor4f(0.5, 0, 0.5, 1),

            iaColor4f(0.25, 0, 0, 1),
            iaColor4f(0, 0.25, 0, 1),
            iaColor4f(0, 0, 0.25, 1),
            iaColor4f(0.25, 0.25, 0, 1),
            iaColor4f(0, 0.25, 0.25, 1),
            iaColor4f(0.25, 0, 0.25, 1)

    };

    static const int32 COLOR_COUNT = 25;

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
        if (_renderStatisticsMode > iProfilerVerbosity::All)
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
        if (_renderStatisticsMode == iProfilerVerbosity::None)
        {
            return;
        }

        // get the stats before we add to them
        const iRenderer2::iRendererStats stats = iRenderer2::getInstance().getStats();

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

                _lastDoneTaskCount = iTaskManager::getInstance().getTaksDoneCount();
            }
        }

        uint32 voffset = 0;

        if (_renderStatisticsMode >= iProfilerVerbosity::FPSAndMetrics)
        {
            voffset = 60;
        }

        iRenderer2::getInstance().setFont(font);
        iRenderer2::getInstance().setFontSize(15.0f);

        const iaString fpsText = iaString::toString(_lastFPS, 2) + L" fps";
        iRenderer2::getInstance().drawString(static_cast<float32>(window->getClientWidth() - 10), static_cast<float32>(window->getClientHeight() - 10 - voffset), fpsText, iHorizontalAlignment::Right, iVerticalAlignment::Bottom, iaColor4f::magenta);

        if (_renderStatisticsMode >= iProfilerVerbosity::FPSAndMetrics)
        {
            iaString unique = "v:";
            unique += iaString::toStringUnits(stats._vertices);
            unique += " t:";
            unique += iaString::toStringUnits(stats._triangles);
            unique += " i:";
            unique += iaString::toStringUnits(stats._indices);
            iRenderer2::getInstance().drawString(static_cast<float32>(window->getClientWidth() - 10), static_cast<float32>(window->getClientHeight() - 10), unique, iHorizontalAlignment::Right, iVerticalAlignment::Bottom, iaColor4f::magenta);

            iaString total = "draw calls:";
            total += iaString::toStringUnits(stats._drawCalls);
            iRenderer2::getInstance().drawString(static_cast<float32>(window->getClientWidth() - 10), static_cast<float32>(window->getClientHeight() - 30), total, iHorizontalAlignment::Right, iVerticalAlignment::Bottom, iaColor4f::magenta);
        }

        if (_renderStatisticsMode >= iProfilerVerbosity::FPSMetricsAndTasks)
        {
            iaString threads = "";
            threads += iaString::toString(_lastThreadCount);
            threads += ":";
            threads += iaString::toString(_lastRunningTaskCount);
            threads += ":";
            threads += iaString::toString(_lastQueuedTaskCount);

            iaString rcthreads = "";
            rcthreads += iaString::toString(_lastRenderContextThreadCount);
            rcthreads += ":";
            rcthreads += iaString::toString(_lastRunningRenderContextTaskCount);
            rcthreads += ":";
            rcthreads += iaString::toString(_lastQueuedRenderContextTaskCount);

            iRenderer2::getInstance().drawString(10.0f, static_cast<float32>(window->getClientHeight() - 30), threads, iHorizontalAlignment::Left, iVerticalAlignment::Bottom, iaColor4f::magenta);
            iRenderer2::getInstance().drawString(10.0f, static_cast<float32>(window->getClientHeight() - 10), rcthreads, iHorizontalAlignment::Left, iVerticalAlignment::Bottom, iaColor4f::magenta);

            iaString done = "done ";
            done += iaString::toString(_lastDoneTaskCount);

            iRenderer2::getInstance().drawString(10.0f, static_cast<float32>(window->getClientHeight() - 50), done, iHorizontalAlignment::Left, iVerticalAlignment::Bottom, iaColor4f::magenta);
        }

        if (_renderStatisticsMode >= iProfilerVerbosity::Sections)
        {
            const iaRectanglef rect(window->getClientWidth() * 0.1, window->getClientHeight() * 0.15, window->getClientWidth() * 0.8, window->getClientHeight() * 0.55);
            const float32 verticalScale = rect._height / 50; // 50ms
            const float32 horizontalScale = rect._width / iProfiler::MAX_FRAMES_COUNT;
            const float32 Hz24 = 41.6666 * verticalScale;
            const float32 Hz60 = 16.6666 * verticalScale;
            const float32 Hz100 = 10 * verticalScale;

            float32 textOffsetX = 0.0f;

            iRenderer2::getInstance().drawFilledRectangle(rect.getLeft(), rect.getTop(), rect.getWidth(), rect.getHeight() + 60.0f, iaColor4f(0, 0, 0, 0.6));

            iRenderer2::getInstance().drawFilledRectangle(rect.getLeft(), rect.getBottom() + 30, rect.getWidth(), 30.0f);

            iRenderer2::getInstance().setLineWidth(1);
            iRenderer2::getInstance().drawLine(rect.getLeft(), rect.getBottom(), rect.getRight(), rect.getBottom());

            iRenderer2::getInstance().drawLine(rect.getLeft(), rect.getBottom() - Hz24, rect.getRight(), rect.getBottom() - Hz24);
            iRenderer2::getInstance().drawLine(rect.getLeft(), rect.getBottom() - Hz60, rect.getRight(), rect.getBottom() - Hz60);
            iRenderer2::getInstance().drawLine(rect.getLeft(), rect.getBottom() - Hz100, rect.getRight(), rect.getBottom() - Hz100);

            iRenderer2::getInstance().drawString(rect._x, rect.getBottom() - Hz24, "24Hz", iHorizontalAlignment::Left, iVerticalAlignment::Bottom);
            iRenderer2::getInstance().drawString(rect._x, rect.getBottom() - Hz60, "60Hz", iHorizontalAlignment::Left, iVerticalAlignment::Bottom);
            iRenderer2::getInstance().drawString(rect._x, rect.getBottom() - Hz100, "100Hz", iHorizontalAlignment::Left, iVerticalAlignment::Bottom);

            uint32 sectionIndex = 0;
            const uint32 currentFrame = (iProfiler::getCurrentFrameIndex() - 1) % iProfiler::MAX_FRAMES_COUNT;

            memset(&_accumulationBuffer, 0, sizeof(float32) * iProfiler::MAX_FRAMES_COUNT);

            float32 rightValue = 0;
            float32 leftValue = 0;
            float32 lrScalec = rect._width / std::max(0.01, iTimer::getInstance().getTimeDelta().getMilliseconds());

            for (const auto &pair : iProfiler::getSections())
            {
                uint32 currentIndex = 0;
                const auto &section = pair.second;
                const auto &values = section._values;

                currentIndex = static_cast<uint32>(currentFrame);
                _accumulationBuffer[currentIndex] += values[currentIndex].getMilliseconds() * verticalScale;
                rightValue += values[currentIndex].getMilliseconds() * lrScalec;
                float32 lastValue = _accumulationBuffer[currentIndex];
                float32 value;

                const iaColor4f &sectionColor = COLORS[sectionIndex % COLOR_COUNT];

                for (int i = 1; i < iProfiler::MAX_FRAMES_COUNT - 1; ++i)
                {
                    currentIndex = (currentFrame + i) % iProfiler::MAX_FRAMES_COUNT;
                    _accumulationBuffer[currentIndex] += values[currentIndex].getMilliseconds() * verticalScale;

                    value = _accumulationBuffer[currentIndex];

                    iRenderer2::getInstance().drawLine(i * horizontalScale + rect._x, rect.getBottom() - lastValue, (i + 1) * horizontalScale + rect._x, rect.getBottom() - value, sectionColor);
                    lastValue = value;
                }

                iRenderer2::getInstance().drawString(rect._x + textOffsetX, rect.getBottom() + 20.0f, section._name, iHorizontalAlignment::Left, iVerticalAlignment::Bottom, sectionColor);
                textOffsetX += 80;

                iRenderer2::getInstance().drawFilledRectangle(rect.getLeft() + leftValue, rect.getBottom() + 30, rightValue - leftValue, 30.0f, sectionColor);

                leftValue = rightValue;

                sectionIndex++;
            }
        }
    }

} // namespace igor