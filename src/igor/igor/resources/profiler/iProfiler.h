//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2020 by Martin Loga
//
// This library is free software; you can redistribute it and or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
//
// contact: igorgameengine@protonmail.com

#ifndef __IGOR_PROFILER_H__
#define __IGOR_PROFILER_H__

#include <igor/iDefines.h>

#include <iaux/system/iaSingleton.h>
#include <iaux/data/iaColor4.h>
#include <iaux/data/iaString.h>
#include <iaux/system/iaConsole.h>
#include <iaux/data/iaIDGenerator.h>
#include <iaux/system/iaTime.h>
using namespace iaux;

#include <map>
#include <array>

namespace igor
{

    /*! profiler section id definition
    */
    typedef iaID64 iProfilerSectionID;

    /*! render statistics
    */
    class Igor_API iProfiler : public iaSingleton<iProfiler>
    {

        friend class iaSingleton<iProfiler>;

    public:
        /*! size of buffer aka amount of frames that are logged
        */
        static const uint64 MAX_FRAMES_COUNT = 500;

        /*! invalid profiler section id definition
        */
        static const iProfilerSectionID INVALID_PROFILER_SECTION_ID = 0;

        struct iProfilerSection
        {
            /*! name of section
            */
            iaString _name;

            /*! time used per frame
            */
            std::array<iaTime, MAX_FRAMES_COUNT> _values;

            /*! time at beginning of section
            */
            iaTime _beginTime;
        };

        /*! creates a measurement section

        \param sectionName the section's name
        \param groupIndex index of group this section belongs to
        */
        iProfilerSectionID createSection(const iaString &sectionName);

        /*! begins measuring section

        \param sectionID the section ID
        */
        void beginSection(iProfilerSectionID sectionID);

        /*! ends measuring section

        \param sectionID the section ID
        */
        void endSection(iProfilerSectionID sectionID);

        /*! steps to next frame

        \param loggingFrame if true next frame will be logges more verbosively
        */
        void nextFrame(bool loggingFrame);

        /*! \returns reference to list of sections

        be carefull to not change that list
        */
        const std::vector<iProfilerSection> &getSections() const;

        /*! \returns current frame index
        */
        int32 getCurrentFrameIndex() const;

    private:
        /*! current frame
        */
        int32 _frame = 0;

        /*! list of sections
        */
        std::vector<iProfilerSection> _sections;

        /*! nothing todo
        */
        iProfiler() = default;

        /*! nothing todo
        */
        virtual ~iProfiler() = default;
    };

} // namespace igor

#endif // __IGOR_PROFILER_H__
