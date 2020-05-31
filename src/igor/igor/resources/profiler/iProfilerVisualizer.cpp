// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/profiler/iProfilerVisualizer.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/os/iWindow.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/os/iTimer.h>
#include <igor/threading/iTaskManager.h>
#include <igor/graphics/iRenderer.h>
#include <igor/resources/profiler/iProfilerSection.h>
#include <igor/resources/profiler/iProfiler.h>

#include <iaux/data/iaString.h>
using namespace IgorAux;

namespace Igor
{

    const iaColor4f iProfilerVisualizer::_colors[] =
        {
            iaColor4f(0, 0, 0, 1),
            iaColor4f(1, 0, 0, 1),
            iaColor4f(0, 1, 0, 1),
            iaColor4f(0, 0, 1, 1),
            iaColor4f(1, 1, 0, 1),
            iaColor4f(0, 1, 1, 1),
            iaColor4f(1, 0, 1, 1),
            iaColor4f(1, 1, 1, 1),

            iaColor4f(0.75, 0, 0, 1),
            iaColor4f(0, 0.75, 0, 1),
            iaColor4f(0, 0, 0.75, 1),
            iaColor4f(0.75, 0.75, 0, 1),
            iaColor4f(0, 0.75, 0.75, 1),
            iaColor4f(0.75, 0, 0.75, 1),
            iaColor4f(0.75, 0.75, 0.75, 1),

            iaColor4f(0.5, 0, 0, 1),
            iaColor4f(0, 0.5, 0, 1),
            iaColor4f(0, 0, 0.5, 1),
            iaColor4f(0.5, 0.5, 0, 1),
            iaColor4f(0, 0.5, 0.5, 1),
            iaColor4f(0.5, 0, 0.5, 1),
            iaColor4f(0.5, 0.5, 0.5, 1),

            iaColor4f(0.25, 0, 0, 1),
            iaColor4f(0, 0.25, 0, 1),
            iaColor4f(0, 0, 0.25, 1),
            iaColor4f(0.25, 0.25, 0, 1),
            iaColor4f(0, 0.25, 0.25, 1),
            iaColor4f(0.25, 0, 0.25, 1),
            iaColor4f(0.25, 0.5, 0.25, 1)};

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
        _seconds = iTimer::getInstance().getSeconds();
    }

    void iProfilerVisualizer::deinit()
    {
        deinitMaterials();
    }

    void iProfilerVisualizer::initMaterials()
    {
        _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
        iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setName("Statistics:Text");
        iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
        iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::Blend, iRenderStateValue::On);
        iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

        _materialSolid = iMaterialResourceFactory::getInstance().createMaterial();
        iMaterialResourceFactory::getInstance().getMaterial(_materialSolid)->setName("Statistics:Solid");
        iMaterialResourceFactory::getInstance().getMaterial(_materialSolid)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

        _materialBlend = iMaterialResourceFactory::getInstance().createMaterial();
        iMaterialResourceFactory::getInstance().getMaterial(_materialBlend)->setName("Statistics:Blend");
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

    void iProfilerVisualizer::draw(iWindow *window, iTextureFont *font, const iaColor4f &color)
    {
        if (iRenderer::getInstance().isReady() &&
            _renderStatisticsMode > iProfilerVerbosity::None)
        {
            iaMatrixd identity;
            iaMatrixd translation;
            translation.translate(0, 0, -30);
            iRenderer::getInstance().setViewMatrix(identity);
            iRenderer::getInstance().setModelMatrix(translation);

            iRenderer::getInstance().setColor(color);

            iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);

            iRenderer::getInstance().setFont(font);
            iRenderer::getInstance().setFontSize(15.0f);

            if (iTimer::getInstance().getSeconds() > _seconds + 0.25f)
            {
                _seconds = iTimer::getInstance().getSeconds();
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

            iaString fpsText = iaString::toString(_lastFPS, 2);
            uint32 voffset = 0;

            if (_renderStatisticsMode >= iProfilerVerbosity::FPSAndMetrics)
            {
                voffset = 20;
            }

            iRenderer::getInstance().drawString(static_cast<float32>(window->getClientWidth() - 10), static_cast<float32>(window->getClientHeight() - 10 - voffset), fpsText, iHorizontalAlignment::Right, iVerticalAlignment::Bottom);

            if (_renderStatisticsMode >= iProfilerVerbosity::FPSAndMetrics)
            {
                uint32 vertices;
                uint32 triangles;
                uint32 indicies;
                iRenderer::getInstance().getCounters(vertices, triangles, indicies);

                iaString dataText = "v:";
                if (vertices < 1000)
                {
                    dataText += iaString::toString(vertices);
                }
                else if (vertices < 1000000)
                {
                    dataText += iaString::toString(vertices / 1000);
                    dataText += "k";
                }
                else
                {
                    dataText += iaString::toString(vertices / 1000000);
                    dataText += "mio";
                }

                dataText += " t:";
                if (triangles < 1000)
                {
                    dataText += iaString::toString(triangles);
                }
                else if (triangles < 1000000)
                {
                    dataText += iaString::toString(triangles / 1000);
                    dataText += "k";
                }
                else
                {
                    dataText += iaString::toString(triangles / 1000000);
                    dataText += "mio";
                }

                dataText += " i:";
                if (indicies < 1000)
                {
                    dataText += iaString::toString(indicies);
                }
                else if (indicies < 1000000)
                {
                    dataText += iaString::toString(indicies / 1000);
                    dataText += "k";
                }
                else
                {
                    dataText += iaString::toString(indicies / 1000000);
                    dataText += "mio";
                }

                iRenderer::getInstance().drawString(static_cast<float32>(window->getClientWidth() - 10), static_cast<float32>(window->getClientHeight() - 10), dataText, iHorizontalAlignment::Right, iVerticalAlignment::Bottom);
                iRenderer::getInstance().resetCounters();
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
                float64 groupCount = 4;
                float64 totalHeight = window->getClientHeight() * 0.9f;
                float64 groupTotalHeight = totalHeight / groupCount;
                float64 scale = groupTotalHeight / 100;
                float64 x = 10;
                uint32 textOffsetX[5]{0, 0, 0, 0, 0};
                float64 horizontalScale = static_cast<float64>(window->getClientWidth() - x - x) / static_cast<float64>(iProfilerSection::BUFFER_SIZE);
                float64 thirtyHz = 66.6666 * scale;
                float64 sixtyHz = 33.3333 * scale;

                for (int i = 0; i < groupCount; ++i)
                {
                    float64 groupOffset = i * groupTotalHeight;

                    iRenderer::getInstance().setMaterial(_materialSolid);
                    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
                    iRenderer::getInstance().setLineWidth(3);
                    iRenderer::getInstance().drawLine(static_cast<float32>(x), static_cast<float32>(totalHeight - groupOffset - 3), static_cast<float32>(window->getClientWidth() - x), static_cast<float32>(totalHeight - groupOffset - 3));

                    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
                    iRenderer::getInstance().setLineWidth(1);
                    iRenderer::getInstance().drawLine(static_cast<float32>(x), static_cast<float32>(totalHeight - sixtyHz - groupOffset), static_cast<float32>(window->getClientWidth() - x), static_cast<float32>(totalHeight - sixtyHz - groupOffset));
                    iRenderer::getInstance().drawLine(static_cast<float32>(x), static_cast<float32>(totalHeight - thirtyHz - groupOffset), static_cast<float32>(window->getClientWidth() - x), static_cast<float32>(totalHeight - thirtyHz - groupOffset));

                    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
                    iRenderer::getInstance().drawString(static_cast<float32>(x), static_cast<float32>(totalHeight - sixtyHz - groupOffset), "33ms", iHorizontalAlignment::Left, iVerticalAlignment::Bottom);
                    iRenderer::getInstance().drawString(static_cast<float32>(x), static_cast<float32>(totalHeight - thirtyHz - groupOffset), "66ms", iHorizontalAlignment::Left, iVerticalAlignment::Bottom);
                }

                iRenderer::getInstance().setLineWidth(2);
                int colorIndex = 0;
                auto sections = iProfiler::getInstance().getSections();
                uint64 frame = iProfiler::getInstance().getCurrentFrameIndex();
                for (auto section : sections)
                {
                    uint32 currentIndex = 0;
                    const float64 *values = section.second.getValues();
                    uint64 currentFrame = frame % iProfilerSection::BUFFER_SIZE;
                    float64 yPos = totalHeight - section.second.getGroup() * groupTotalHeight;

                    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);

                    iRenderer::getInstance().setColor(_colors[(colorIndex++) % _colorCount]);
                    iRenderer::getInstance().drawString(static_cast<float32>(10 + textOffsetX[section.second.getGroup()]), static_cast<float32>(yPos + 20), section.second.getName(), iHorizontalAlignment::Left, iVerticalAlignment::Bottom);
                    textOffsetX[section.second.getGroup()] += 150;

                    iRenderer::getInstance().setMaterial(_materialSolid);

                    currentIndex = static_cast<uint32>(currentFrame);
                    float64 lastValue = values[currentIndex] * scale;
                    float64 value;

                    for (int i = 1; i < iProfilerSection::BUFFER_SIZE - 1; ++i)
                    {
                        currentIndex = (currentFrame + i) % iProfilerSection::BUFFER_SIZE;
                        value = (values[currentIndex]) * scale;
                        iRenderer::getInstance().drawLine(static_cast<float32>((static_cast<float64>(i) * horizontalScale) + x), static_cast<float32>(yPos - lastValue), static_cast<float32>((static_cast<float64>(i + 1) * horizontalScale) + x), static_cast<float32>(yPos - value));
                        lastValue = value;
                    }
                }
                iRenderer::getInstance().setLineWidth(1);

                iRenderer::getInstance().setColor(color);
            }
        }

        iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
    }

} // namespace Igor