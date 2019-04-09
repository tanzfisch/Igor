// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
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

    uint32 iStatistics::registerSection(const iaString& sectionName, uint64 groupIndex)
    {
        con_assert_sticky(groupIndex <= 5, "out of bounds");

        uint32 result = _nextSectionID++;
        iStatisticsSection statisticsSection;
        statisticsSection.setName(sectionName);        
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

    void iStatistics::nextFrame()
    {
        for (auto& section : _sections)
        {
            section.second.setCurrentFrame(_frame);
        }

        _frame++;
    }

    map<uint32, iStatisticsSection>& iStatistics::getSections()
    {
        return _sections;
    }

    uint64 iStatistics::getCurrentFrameIndex() const
    {
        return _frame;
    }


}