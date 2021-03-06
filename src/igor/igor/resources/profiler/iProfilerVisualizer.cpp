// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/profiler/iProfilerVisualizer.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/system/iWindow.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/system/iTimer.h>
#include <igor/threading/iTaskManager.h>
#include <igor/graphics/iRenderer.h>
#include <igor/data/iRectangle.h>

#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{

    const iaColor4f iProfilerVisualizer::_colors[] =
        {
            iaColor4f(1, 0, 0, 1),
            iaColor4f(0, 1, 0, 1),
            iaColor4f(0, 0, 1, 1),
            iaColor4f(1, 1, 0, 1),
            iaColor4f(0, 1, 1, 1),
            iaColor4f(1, 0, 1, 1),
            iaColor4f(0.9, 0.9, 0.9, 1),

            iaColor4f(0.75, 0, 0, 1),
            iaColor4f(0, 0.75, 0, 1),
            iaColor4f(0, 0, 0.75, 1),
            iaColor4f(0.75, 0.75, 0, 1),
            iaColor4f(0, 0.75, 0.75, 1),
            iaColor4f(0.75, 0, 0.75, 1),

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
            iaColor4f(0.25, 0, 0.25, 1)};

    iProfilerVisualizer::iProfilerVisualizer()
    {
        iRenderer::getInstance().registerInitializedDelegate(iRendererInitializedDelegate(this, &iProfilerVisualizer::init));
        iRenderer::getInstance().registerPreDeinitializeDelegate(iRendererPreDeinitializeDelegate(this, &iProfilerVisualizer::deinit));

        if (iRenderer::getInstance().isReady())
        {
            init();
        }
    }

    iProfilerVisualizer::~iProfilerVisualizer()
    {
        iRenderer::getInstance().unregisterInitializedDelegate(iRendererInitializedDelegate(this, &iProfilerVisualizer::init));
        iRenderer::getInstance().unregisterPreDeinitializeDelegate(iRendererPreDeinitializeDelegate(this, &iProfilerVisualizer::deinit));
    }

    void iProfilerVisualizer::init()
    {
        initMaterials();
        _time = iTimer::getInstance().getFrameTime();
    }

    void iProfilerVisualizer::deinit()
    {
        deinitMaterials();
    }

    void iProfilerVisualizer::initMaterials()
    {
        _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial("igor.profiler.text");
        iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
        iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::Blend, iRenderStateValue::On);
        iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

        _materialSolid = iMaterialResourceFactory::getInstance().createMaterial("igor.profiler.solid");
        iMaterialResourceFactory::getInstance().getMaterial(_materialSolid)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

        _materialGraph = iMaterialResourceFactory::getInstance().createMaterial("igor.profiler.graph");

        _materialBlend = iMaterialResourceFactory::getInstance().createMaterial("igor.profiler.blend");
        iMaterialResourceFactory::getInstance().getMaterial(_materialBlend)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
        iMaterialResourceFactory::getInstance().getMaterial(_materialBlend)->setRenderState(iRenderState::Blend, iRenderStateValue::On);
    }

    void iProfilerVisualizer::deinitMaterials()
    {
        iMaterialResourceFactory::getInstance().destroyMaterial(_materialWithTextureAndBlending);
        iMaterialResourceFactory::getInstance().destroyMaterial(_materialSolid);
        iMaterialResourceFactory::getInstance().destroyMaterial(_materialBlend);
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

    static iaString units(int64 value)
    {
        iaString result;
        if (value < 1000)
        {
            result += iaString::toString(value);
        }
        else if (value < 1000000)
        {
            result += iaString::toString(value / 1000);
            result += "k";
        }
        else
        {
            result += iaString::toString(value / 1000000);
            result += "mio";
        }

        return result;
    }

    void iProfilerVisualizer::draw(iWindow *window, iTextureFont *font)
    {
        if (!iRenderer::getInstance().isReady() ||
            _renderStatisticsMode == iProfilerVerbosity::None ||
            font == nullptr || !font->isValid())
        {
            return;
        }

        if (iTimer::getInstance().getFrameTime() > _time + iaTime::fromSeconds(0.25))
        {
            _time = iTimer::getInstance().getFrameTime();
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

        iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);

        iRenderer::getInstance().setFont(font);
        iRenderer::getInstance().setFontSize(15.0f);
        iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

        const iaString fpsText = iaString::toString(_lastFPS, 2);
        fpsText += " fps";
        iRenderer::getInstance().drawString(static_cast<float32>(window->getClientWidth() - 10), static_cast<float32>(window->getClientHeight() - 10 - voffset), fpsText, iHorizontalAlignment::Right, iVerticalAlignment::Bottom);

        if (_renderStatisticsMode >= iProfilerVerbosity::FPSAndMetrics)
        {
            const auto &stats = iRenderer::getInstance().getStats();

            iaString unique = "unique v:";
            unique += units(stats._vertices);
            unique += " t:";
            unique += units(stats._triangles);
            unique += " i:";
            unique += units(stats._indices);
            iRenderer::getInstance().drawString(static_cast<float32>(window->getClientWidth() - 10), static_cast<float32>(window->getClientHeight() - 10), unique, iHorizontalAlignment::Right, iVerticalAlignment::Bottom);

            iaString instanced = "instanced v:";
            instanced += units(stats._verticesInstanced);
            instanced += " t:";
            instanced += units(stats._trianglesInstanced);
            instanced += " i:";
            instanced += units(stats._indicesInstanced);
            iRenderer::getInstance().drawString(static_cast<float32>(window->getClientWidth() - 10), static_cast<float32>(window->getClientHeight() - 30), instanced, iHorizontalAlignment::Right, iVerticalAlignment::Bottom);

            iaString total = "total v:";
            total += units(stats._verticesInstanced + stats._vertices);
            total += " t:";
            total += units(stats._trianglesInstanced + stats._triangles);
            total += " i:";
            total += units(stats._indicesInstanced + stats._indices);
            iRenderer::getInstance().drawString(static_cast<float32>(window->getClientWidth() - 10), static_cast<float32>(window->getClientHeight() - 50), total, iHorizontalAlignment::Right, iVerticalAlignment::Bottom);
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

            iRenderer::getInstance().drawString(10.0f, static_cast<float32>(window->getClientHeight() - 30), threads, iHorizontalAlignment::Left, iVerticalAlignment::Bottom);
            iRenderer::getInstance().drawString(10.0f, static_cast<float32>(window->getClientHeight() - 10), rcthreads, iHorizontalAlignment::Left, iVerticalAlignment::Bottom);

            iaString done = "done ";
            done += iaString::toString(_lastDoneTaskCount);

            iRenderer::getInstance().drawString(10.0f, static_cast<float32>(window->getClientHeight() - 50), done, iHorizontalAlignment::Left, iVerticalAlignment::Bottom);
        }

        if (_renderStatisticsMode >= iProfilerVerbosity::Sections)
        {
            const iRectanglef rect(window->getClientWidth() * 0.1, window->getClientHeight() * 0.15, window->getClientWidth() * 0.8, window->getClientHeight() * 0.55);
            const float32 verticalScale = rect._height / 50; // 50ms
            const float32 horizontalScale = rect._width / iProfiler::MAX_FRAMES_COUNT;
            const float32 Hz24 = 41.6666 * verticalScale;
            const float32 Hz60 = 16.6666 * verticalScale;
            const float32 Hz100 = 10 * verticalScale;

            float32 textOffsetX = 0.0f;

            iRenderer::getInstance().setMaterial(_materialBlend);
            iRenderer::getInstance().setColor(iaColor4f(0, 0, 0, 0.6));
            iRenderer::getInstance().drawRectangle(rect.getLeft(), rect.getTop(), rect.getWidth(), rect.getHeight() + 60.0f);

            iRenderer::getInstance().setMaterial(_materialSolid);
            iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
            iRenderer::getInstance().drawRectangle(rect.getLeft(), rect.getBottom() + 30, rect.getWidth(), 30.0f);

            iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
            iRenderer::getInstance().setLineWidth(1);
            iRenderer::getInstance().drawLine(rect.getLeft(), rect.getBottom(), rect.getRight(), rect.getBottom());

            iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
            iRenderer::getInstance().drawLine(rect.getLeft(), rect.getBottom() - Hz24, rect.getRight(), rect.getBottom() - Hz24);
            iRenderer::getInstance().drawLine(rect.getLeft(), rect.getBottom() - Hz60, rect.getRight(), rect.getBottom() - Hz60);
            iRenderer::getInstance().drawLine(rect.getLeft(), rect.getBottom() - Hz100, rect.getRight(), rect.getBottom() - Hz100);

            iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
            iRenderer::getInstance().drawString(rect._x, rect.getBottom() - Hz24, "24Hz", iHorizontalAlignment::Left, iVerticalAlignment::Bottom);
            iRenderer::getInstance().drawString(rect._x, rect.getBottom() - Hz60, "60Hz", iHorizontalAlignment::Left, iVerticalAlignment::Bottom);
            iRenderer::getInstance().drawString(rect._x, rect.getBottom() - Hz100, "100Hz", iHorizontalAlignment::Left, iVerticalAlignment::Bottom);

            int sectionIndex = 0;
            auto sections = iProfiler::getInstance().getSections();
            const uint32 currentFrame = (iProfiler::getInstance().getCurrentFrameIndex() - 1) % iProfiler::MAX_FRAMES_COUNT;

            memset(&_accumulationBuffer, 0, sizeof(float32) * iProfiler::MAX_FRAMES_COUNT);

            float32 rightValue = 0;
            float32 leftValue = 0;
            float32 lrScalec = rect._width / std::max(0.01, iTimer::getInstance().getFrameTimeDelta().getMilliseconds());

            for (auto section : sections)
            {
                uint32 currentIndex = 0;
                const auto &values = section._values;

                iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
                iRenderer::getInstance().setColor(_colors[(sectionIndex) % _colorCount]);
                iRenderer::getInstance().drawString(rect._x + textOffsetX, rect.getBottom() + 20.0f, section._name, iHorizontalAlignment::Left, iVerticalAlignment::Bottom);
                textOffsetX += 80;

                iRenderer::getInstance().setMaterial(_materialGraph);

                currentIndex = static_cast<uint32>(currentFrame);
                _accumulationBuffer[currentIndex] += values[currentIndex].getMilliseconds() * verticalScale;
                rightValue += values[currentIndex].getMilliseconds() * lrScalec;
                float32 lastValue = _accumulationBuffer[currentIndex];
                float32 value;

                for (int i = 1; i < iProfiler::MAX_FRAMES_COUNT - 1; ++i)
                {
                    currentIndex = (currentFrame + i) % iProfiler::MAX_FRAMES_COUNT;
                    _accumulationBuffer[currentIndex] += values[currentIndex].getMilliseconds() * verticalScale;

                    value = _accumulationBuffer[currentIndex];

                    iRenderer::getInstance().drawLine(
                        i * horizontalScale + rect._x,
                        rect.getBottom() - lastValue,
                        (i + 1) * horizontalScale + rect._x,
                        rect.getBottom() - value);
                    lastValue = value;
                }

                iRenderer::getInstance().setMaterial(_materialSolid);
                iRenderer::getInstance().drawRectangle(rect.getLeft() + leftValue, rect.getBottom() + 30, rightValue - leftValue, 30.0f);

                leftValue = rightValue;

                sectionIndex++;
            }
            iRenderer::getInstance().setLineWidth(1);
        }

        iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
    }

} // namespace igor