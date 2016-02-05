// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iStatistics.h>
#include <iMaterialResourceFactory.h>
#include <iMaterial.h>
#include <iWindow.h>
#include <iTextureFont.h>
#include <iTimer.h>
#include <iTaskManager.h>

#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

    iStatistics::iStatistics()
    {
        _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
        iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
        iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
        iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

        _materialSolid = iMaterialResourceFactory::getInstance().createMaterial();
        iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

        _seconds = iTimer::getInstance().getSeconds();
    }

    iStatistics::~iStatistics()
    {
        iMaterialResourceFactory::getInstance().destroyMaterial(_materialWithTextureAndBlending);
    }

    uint32 iStatistics::registerSection(const iaString& sectionName, const iaColor4f& color)
    {
        uint32 result = _nextSectionID++;
        iStatisticsSection statisticsSection;
        statisticsSection.setName(sectionName);
        statisticsSection.setColor(color);

        _sections[result] = statisticsSection;

        return result;
    }

    void iStatistics::unregisterSection(uint32 sectionID)
    {
        auto section = _sections.find(sectionID);

        con_assert(section != _sections.end(), "out of range");

        if (section != _sections.end())
        {
            _sections.erase(section);
        }
    }

    void iStatistics::beginSection(uint32 sectionID)
    {
        con_assert(_sections.find(sectionID) != _sections.end(), "out of range");

        if (_sections.find(sectionID) != _sections.end())
        {
            _sections[sectionID].beginSection();
        }
    }

    void iStatistics::endSection(uint32 sectionID)
    {
        con_assert(_sections.find(sectionID) != _sections.end(), "out of range");

        if (_sections.find(sectionID) != _sections.end())
        {
            _sections[sectionID].endSection();
        }
    }

    void iStatistics::setVerbosity(iRenderStatisticsVerbosity renderStatisticsMode)
    {
        _renderStatisticsMode = renderStatisticsMode;
    }

    iRenderStatisticsVerbosity iStatistics::getVerbosity()
    {
        return _renderStatisticsMode;
    }

    void iStatistics::drawStatistics(iWindow* window, iTextureFont* font, const iaColor4f& color)
    {
        if (_renderStatisticsMode > iRenderStatisticsVerbosity::None)
        {
            iaMatrixf identity;
            iaMatrixf translation;
            translation.translate(0, 0, -30);
            iRenderer::getInstance().setViewMatrix(identity);
            iRenderer::getInstance().setModelMatrix(translation);

            iRenderer::getInstance().setColor(color);

            iMaterialResourceFactory::getInstance().setMaterial(_materialWithTextureAndBlending);

            iRenderer::getInstance().setFont(font);
            iRenderer::getInstance().setFontSize(15.0f);

            if (iTimer::getInstance().getSeconds() > _seconds + 0.25f)
            {
                _seconds = iTimer::getInstance().getSeconds();
                _lastFPS = iTimer::getInstance().getFPS();

                if (_renderStatisticsMode >= iRenderStatisticsVerbosity::FPSMetricsAndMT)
                {
                    _lastThreadCount = iTaskManager::getInstance().getThreadCount();
                    _lastRenderContextThreadCount = iTaskManager::getInstance().getRenderContextThreadCount();
                    _lastQueuedTaskCount = iTaskManager::getInstance().getQueuedTaskCount();
                    _lastRunningTaskCount = iTaskManager::getInstance().getRunningTaskCount();
                    _lastQueuedRenderContextTaskCount = iTaskManager::getInstance().getQueuedRenderContextTaskCount();
                    _lastRunningRenderContextTaskCount = iTaskManager::getInstance().getRunningRenderContextTaskCount();
                }
            }

            iaString fpsText = iaString::ftoa(_lastFPS, 2);
            uint32 voffset = 0;

            if (_renderStatisticsMode >= iRenderStatisticsVerbosity::FPSAndMetrics)
            {
                voffset = 20;
            }

            iRenderer::getInstance().drawString(window->getClientWidth() - 10, window->getClientHeight() - 10 - voffset, fpsText, iHorrizontalAlign::Right, iVerticalAlign::Bottom);

            if (_renderStatisticsMode >= iRenderStatisticsVerbosity::FPSAndMetrics)
            {
                uint32 vertices;
                uint32 triangles;
                uint32 indicies;
                iRenderer::getInstance().getCounters(vertices, triangles, indicies);

                iaString dataText = "v:";
                if (vertices < 1000)
                {
                    dataText += iaString::itoa(vertices);
                }
                else if (vertices < 1000000)
                {
                    dataText += iaString::itoa(vertices / 1000);
                    dataText += "k";
                }
                else
                {
                    dataText += iaString::itoa(vertices / 1000000);
                    dataText += "mio";
                }

                dataText += " t:";
                if (triangles < 1000)
                {
                    dataText += iaString::itoa(triangles);
                }
                else if (triangles < 1000000)
                {
                    dataText += iaString::itoa(triangles / 1000);
                    dataText += "k";
                }
                else
                {
                    dataText += iaString::itoa(triangles / 1000000);
                    dataText += "mio";
                }

                dataText += " i:";
                if (indicies < 1000)
                {
                    dataText += iaString::itoa(indicies);
                }
                else if (indicies < 1000000)
                {
                    dataText += iaString::itoa(indicies / 1000);
                    dataText += "k";
                }
                else
                {
                    dataText += iaString::itoa(indicies / 1000000);
                    dataText += "mio";
                }

                iRenderer::getInstance().drawString(window->getClientWidth() - 10, window->getClientHeight() - 10, dataText, iHorrizontalAlign::Right, iVerticalAlign::Bottom);
                iRenderer::getInstance().resetCounters();
            }

            if (_renderStatisticsMode >= iRenderStatisticsVerbosity::FPSMetricsAndMT)
            {
                iaString threads = " threads:";
                threads += iaString::itoa(_lastThreadCount);
                threads += " tasks:";
                threads += iaString::itoa(_lastRunningTaskCount);
                threads += " queued:";
                threads += iaString::itoa(_lastQueuedTaskCount);

                iaString rcthreads = " render context threads:";
                rcthreads += iaString::itoa(_lastRenderContextThreadCount);
                rcthreads += " tasks:";
                rcthreads += iaString::itoa(_lastRunningRenderContextTaskCount);
                rcthreads += " queued:";
                rcthreads += iaString::itoa(_lastQueuedRenderContextTaskCount);

                iRenderer::getInstance().drawString(10, window->getClientHeight() - 30, threads, iHorrizontalAlign::Left, iVerticalAlign::Bottom);
                iRenderer::getInstance().drawString(10, window->getClientHeight() - 10, rcthreads, iHorrizontalAlign::Left, iVerticalAlign::Bottom);
            }

            if (_renderStatisticsMode >= iRenderStatisticsVerbosity::Sections)
            {
                float64 scale = 10.0;
                float64 x = 10;
                float64 y = 500;
                uint32 textOffsetX = 0;
                float64 horrizontalScale = static_cast<float64>(window->getClientWidth() - x - x) / static_cast<float64>(iStatisticsSection::BUFFER_SIZE);

                for (auto section : _sections)
                {
                    iRenderer::getInstance().setColor(section.second.getColor());
                    iRenderer::getInstance().drawString(10 + textOffsetX, y + 30, section.second.getName(), iHorrizontalAlign::Left, iVerticalAlign::Bottom);
                    textOffsetX += 100;
                }

                iMaterialResourceFactory::getInstance().setMaterial(_materialSolid);

                iRenderer::getInstance().setColor(color);
                iRenderer::getInstance().drawLine(x, y, window->getClientWidth() - x, y);
                iRenderer::getInstance().drawLine(x, y, x, 0);
                iRenderer::getInstance().drawLine(window->getClientWidth() - x, y, window->getClientWidth() - x, 0);
                iRenderer::getInstance().drawLine(x, y - (16.66 * scale), window->getClientWidth() - x, y - (16.66 * scale));
                iRenderer::getInstance().drawLine(x, y - (33.33 * scale), window->getClientWidth() - x, y - (33.33 * scale));

                for (auto section : _sections)
                {
                    uint32 currentIndex = 0;
                    const float64* begin = section.second.getBeginnings();
                    const float64* end = section.second.getEnds();
                    uint32 currentFrame = (section.second.getCurrentFrame() + 1) % iStatisticsSection::BUFFER_SIZE;
                    iRenderer::getInstance().setColor(section.second.getColor());


                    currentIndex = currentFrame;
                    float64 lastValue = (end[currentIndex] - begin[currentIndex]) * scale;
                    float64 value;

                    for (int i = 1; i < iStatisticsSection::BUFFER_SIZE - 1; ++i)
                    {
                        currentIndex = (currentFrame + i) % iStatisticsSection::BUFFER_SIZE;
                        value = (end[currentIndex] - begin[currentIndex]) * scale;
                        iRenderer::getInstance().drawLine((static_cast<float64>(i)*horrizontalScale) + x, y - lastValue, (static_cast<float64>(i + 1)*horrizontalScale) + x, y - value);
                        lastValue = value;
                    }
                }

                iRenderer::getInstance().setColor(color);
            }
        }
    }


}