// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
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
        _seconds = iTimer::getInstance().getSeconds();

        iRenderer::getInstance().registerInitializedDelegate(iRendererInitializedDelegate(this, &iStatistics::initMaterials));
    }

    iStatistics::~iStatistics()
    {
        iRenderer::getInstance().unregisterInitializedDelegate(iRendererInitializedDelegate(this, &iStatistics::initMaterials));

        deinitMaterials();
    }

    void iStatistics::initMaterials()
    {
        _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
        iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
        iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
        iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

        _materialSolid = iMaterialResourceFactory::getInstance().createMaterial();
        iMaterialResourceFactory::getInstance().getMaterial(_materialSolid)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

        _materialBlend = iMaterialResourceFactory::getInstance().createMaterial();
        iMaterialResourceFactory::getInstance().getMaterial(_materialBlend)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
        iMaterialResourceFactory::getInstance().getMaterial(_materialBlend)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    }

    void iStatistics::deinitMaterials()
    {
        iMaterialResourceFactory::getInstance().destroyMaterial(_materialWithTextureAndBlending);
        iMaterialResourceFactory::getInstance().destroyMaterial(_materialSolid);
        iMaterialResourceFactory::getInstance().destroyMaterial(_materialBlend);
    }

    uint32 iStatistics::registerSection(const iaString& sectionName, const iaColor4f& color, uint64 groupIndex)
    {
        con_assert_sticky(groupIndex <= 5, "out of bounds");

        uint32 result = _nextSectionID++;
        iStatisticsSection statisticsSection;
        statisticsSection.setName(sectionName);
        statisticsSection.setColor(color);
        statisticsSection.setGroup(groupIndex);

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

    void iStatistics::setSectionLenght(uint32 sectionID, float64 lenght)
    {
        con_assert(_sections.find(sectionID) != _sections.end(), "out of range");

        if (_sections.find(sectionID) != _sections.end())
        {
            _sections[sectionID].setSectionLenght(lenght);
        }
    }

    // todo refactor
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
                    _lastQueuedTaskCount = iTaskManager::getInstance().getQueuedTaskCount();
                    _lastRunningTaskCount = iTaskManager::getInstance().getRunningTaskCount();

                    _lastRenderContextThreadCount = iTaskManager::getInstance().getRenderContextThreadCount();
                    _lastQueuedRenderContextTaskCount = iTaskManager::getInstance().getQueuedRenderContextTaskCount();
                    _lastRunningRenderContextTaskCount = iTaskManager::getInstance().getRunningRenderContextTaskCount();

                    _lastPhysicsContextThreadCount = iTaskManager::getInstance().getPhysicsContextThreadCount();
                    _lastQueuedPhysicsContextTaskCount = iTaskManager::getInstance().getQueuedPhysicsContextTaskCount();
                    _lastRunningPhysicsContextTaskCount = iTaskManager::getInstance().getRunningPhysicsContextTaskCount();
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
                iaString threads = "";
                threads += iaString::itoa(_lastThreadCount);
                threads += ":";
                threads += iaString::itoa(_lastRunningTaskCount);
                threads += ":";
                threads += iaString::itoa(_lastQueuedTaskCount);

                iaString rcthreads = "";
                rcthreads += iaString::itoa(_lastRenderContextThreadCount);
                rcthreads += ":";
                rcthreads += iaString::itoa(_lastRunningRenderContextTaskCount);
                rcthreads += ":";
                rcthreads += iaString::itoa(_lastQueuedRenderContextTaskCount);

                iaString pcthreads = "";
                pcthreads += iaString::itoa(_lastPhysicsContextThreadCount);
                pcthreads += ":";
                pcthreads += iaString::itoa(_lastRunningPhysicsContextTaskCount);
                pcthreads += ":";
                pcthreads += iaString::itoa(_lastQueuedPhysicsContextTaskCount);

                iRenderer::getInstance().drawString(10, window->getClientHeight() - 50, threads, iHorrizontalAlign::Left, iVerticalAlign::Bottom);
                iRenderer::getInstance().drawString(10, window->getClientHeight() - 30, rcthreads, iHorrizontalAlign::Left, iVerticalAlign::Bottom);
                iRenderer::getInstance().drawString(10, window->getClientHeight() - 10, pcthreads, iHorrizontalAlign::Left, iVerticalAlign::Bottom);
            }

            if (_renderStatisticsMode >= iRenderStatisticsVerbosity::Sections)
            {
                float64 groupCount = 5;
                float64 totalHeight = window->getClientHeight() * 0.9f;
                float64 groupTotalHeight = totalHeight / groupCount;
                float64 scale = groupTotalHeight / 100;
                float64 x = 10;
                uint32 textOffsetX[5]{ 0,0,0,0,0 };
                float64 horrizontalScale = static_cast<float64>(window->getClientWidth() - x - x) / static_cast<float64>(iStatisticsSection::BUFFER_SIZE);
                float32 thirtyHz = 33.3333 * scale;
                float32 sixtyHz = 66.6666 * scale;

                iMaterialResourceFactory::getInstance().setMaterial(_materialBlend);
                for (int i = 0; i < groupCount; ++i)
                {
                    float32 groupOffset = i * groupTotalHeight;
                    iRenderer::getInstance().setColor(iaColor4f(0, 0, 0, 0.4));
                    iRenderer::getInstance().drawRectangle(x, totalHeight - groupOffset - sixtyHz, window->getClientWidth() - x - x, sixtyHz);
                }

                iMaterialResourceFactory::getInstance().setMaterial(_materialSolid);

                iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
                iRenderer::getInstance().drawLine(x, totalHeight, x, 0);
                iRenderer::getInstance().drawLine(window->getClientWidth() - x, totalHeight, window->getClientWidth() - x, 0);

                for (int i = 0; i < groupCount; ++i)
                {
                    float32 groupOffset = i * groupTotalHeight;

                    iRenderer::getInstance().setColor(iaColor4f(0.4,0.4,0.4,1));
                    iRenderer::getInstance().drawRectangle(x, totalHeight - groupOffset - 3, window->getClientWidth() - x - x, 3);
                    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
                    iRenderer::getInstance().drawLine(x, totalHeight - sixtyHz - groupOffset, window->getClientWidth() - x, totalHeight - sixtyHz - groupOffset);
                    iRenderer::getInstance().drawLine(x, totalHeight - thirtyHz - groupOffset, window->getClientWidth() - x, totalHeight - thirtyHz - groupOffset);
                }

                iRenderer::getInstance().setLineWidth(2);
                for (auto section : _sections)
                {
                    uint32 currentIndex = 0;
                    const float64* begin = section.second.getBeginnings();
                    const float64* end = section.second.getEnds();
                    uint32 currentFrame = (section.second.getCurrentFrame() + 1) % iStatisticsSection::BUFFER_SIZE;
                    float64 yPos = totalHeight - section.second.getGroup() * groupTotalHeight;

                    iMaterialResourceFactory::getInstance().setMaterial(_materialWithTextureAndBlending);

                    iRenderer::getInstance().setColor(section.second.getColor());
                    iRenderer::getInstance().drawString(10 + textOffsetX[section.second.getGroup()], yPos + 20, section.second.getName(), iHorrizontalAlign::Left, iVerticalAlign::Bottom);
                    textOffsetX[section.second.getGroup()] += 130;

                    iMaterialResourceFactory::getInstance().setMaterial(_materialSolid);

                    currentIndex = currentFrame;
                    float64 lastValue = (end[currentIndex] - begin[currentIndex]) * scale;
                    float64 value;

                    for (int i = 1; i < iStatisticsSection::BUFFER_SIZE - 1; ++i)
                    {
                        currentIndex = (currentFrame + i) % iStatisticsSection::BUFFER_SIZE;
                        value = (end[currentIndex] - begin[currentIndex]) * scale;
                        iRenderer::getInstance().drawLine((static_cast<float64>(i)*horrizontalScale) + x, yPos - lastValue, (static_cast<float64>(i + 1)*horrizontalScale) + x, yPos - value);
                        lastValue = value;
                    }
                }
                iRenderer::getInstance().setLineWidth(1);

                iRenderer::getInstance().setColor(color);
            }
        }

        iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
    }


}