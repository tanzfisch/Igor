//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef __IGOR_PROFILER__
#define __IGOR_PROFILER__

#include <igor/resources/module/iModule.h>
#include <iaux/data/iaString.h>
#include <iaux/system/iaTime.h>
#include <iaux/system/iaMutex.h>
using namespace iaux;

#include <unordered_map>
#include <array>
#include <memory>

namespace igor
{
/*! size of buffer aka amount of frames that are logged
 */
#define PROFILER_MAX_FRAMES_COUNT 2000

    struct IGOR_API iProfilerSectionData
    {
        /*! name of section
         */
        iaString _name;

        /*! time used per frame
         */
        std::array<iaTime, PROFILER_MAX_FRAMES_COUNT> _values;

        /*! time at beginning of section
         */
        iaTime _beginTime;
    };

    struct IGOR_API iProfilerCounterData
    {
        /*! name of section
         */
        iaString _name;

        /*! counters per frame
         */
        std::array<uint64, PROFILER_MAX_FRAMES_COUNT> _counters;
    };

    struct IGOR_API iProfilerSectionScoped
    {
        /*! begins measuring section

        \param sectionName name of section
         */
        iProfilerSectionScoped(const iaString &sectionName);

        /*! stops measuring section
         */
        ~iProfilerSectionScoped();

    private:
        /*! the section name
         */
        iaString _sectionName;
    };

    /*! render statistics
     */
    class IGOR_API iProfiler
    {

        friend class iProfilerSectionScoped;
        friend class iProfilerVisualizer;       // so we can prevent it from being measured

    public:
        /*! steps to next frame
         */
        static void nextFrame();

        /*! begins section with given name

        \param sectionName the given name
        \returns pointer to given section data
        */
        static void beginSection(const iaString &sectionName);

        /*! ends section with given name

        \param sectionName the given name
        */
        static void endSection(const iaString &sectionName);

        /*! allows to set value with name per frame

        \param key name of value
        \param value the value to store for the current frame
        */
        static void setValue(const iaString &key, uint64 value);

    private:
        /*! current frame
         */
        static int32 _frame;

        /*! accumulated frame time
         */
        static std::array<iaTime, PROFILER_MAX_FRAMES_COUNT> _frameTime;

        /*! list of sections
         */
        static std::unordered_map<iaString, iProfilerSectionData> _sections;

        /*! list of recorded values
        */
        static std::unordered_map<iaString, iProfilerCounterData> _counters;

        /*! \returns current frame index
         */
        static int32 getCurrentFrameIndex();

        /*! \returns peak frame over collected data
         */
        static iaTime getPeakFrame();        

        /*! \returns section data for given section name

        \param sectionName the given section name
        */
        static iProfilerSectionData& getSectionData(const iaString &sectionName);

        /*! \returns reference to list of sections

        be careful to not change that list
        */
        static const std::vector<iProfilerSectionData> getSections();   

        /*! \returns all counters for all frames recorded
        */
        static const std::vector<iProfilerCounterData> getCounters();
    };

#define IGOR_PROFILER_SCOPED(sectionName) iProfilerSectionScoped sectionName(#sectionName)
#define IGOR_PROFILER_BEGIN(sectionName) iProfiler::beginSection(#sectionName)
#define IGOR_PROFILER_END(sectionName) iProfiler::endSection(#sectionName)

} // namespace igor

#endif // __IGOR_PROFILER__