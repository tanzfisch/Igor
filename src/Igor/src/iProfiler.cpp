// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iProfiler.h>
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

    uint32 iProfiler::registerSection(const iaString& sectionName, uint64 groupIndex)
    {
        con_assert_sticky(groupIndex <= 5, "out of bounds");

        uint32 result = _nextSectionID++;
        iProfilerSection statisticsSection;
        statisticsSection.setName(sectionName);        
        statisticsSection.setGroup(groupIndex);

        _sections[result] = statisticsSection;

        return result;
    }

    void iProfiler::unregisterSection(uint32 sectionID)
    {
        auto section = _sections.find(sectionID);

        con_assert(section != _sections.end(), "out of range");

        if (section != _sections.end())
        {
            _sections.erase(section);
        }
    }

    void iProfiler::beginSection(uint32 sectionID)
    {
        con_assert(_sections.find(sectionID) != _sections.end(), "out of range");

        if (_sections.find(sectionID) != _sections.end())
        {
            _sections[sectionID].beginSection();
        }
    }

    void iProfiler::endSection(uint32 sectionID)
    {
        con_assert(_sections.find(sectionID) != _sections.end(), "out of range");

        if (_sections.find(sectionID) != _sections.end())
        {
            _sections[sectionID].endSection();
        }
    }

    void iProfiler::nextFrame()
    {
        for (auto& section : _sections)
        {
            section.second.setCurrentFrame(_frame);
        }

        _frame++;
    }

    std::map<uint32, iProfilerSection>& iProfiler::getSections()
    {
        return _sections;
    }

    uint64 iProfiler::getCurrentFrameIndex() const
    {
        return _frame;
    }


}