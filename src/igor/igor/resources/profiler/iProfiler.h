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

#include <igor/resources/module/iModule.h>
#include <iaux/data/iaString.h>
#include <iaux/system/iaTime.h>
#include <iaux/system/iaMutex.h>
using namespace iaux;

#include <unordered_map>
#include <array>

namespace igor
{
    struct IGOR_API iProfilerSectionData
    {
        /*! name of section
         */
        iaString _name;

        /*! time used per frame
         */
        std::array<iaTime, 500> _values;

        /*! time at beginning of section
         */
        iaTime _beginTime;
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

    public:
        /*! size of buffer aka amount of frames that are logged
         */
        static const uint64 MAX_FRAMES_COUNT = 500;

        /*! steps to next frame
         */
        static void nextFrame();

        /*! \returns reference to list of sections

        be carefull to not change that list
        */
        static const std::unordered_map<int64, iProfilerSectionData> &getSections();

        /*! \returns current frame index
         */
        static int32 getCurrentFrameIndex();

        /*! \returns section data for given section name

        \param sectionName the given section name
        */
        static iProfilerSectionData *getSectionData(const iaString &sectionName);

        /*! begins section with given name

        \param sectionName the given name
        \returns pointer to given section data
        */
        static void beginSection(const iaString &sectionName);

        /*! ends section with given name

        \param sectionName the given name
        */
        static void endSection(const iaString &sectionName);

    private:
        /*! current frame
         */
        static int32 _frame;

        /*! list of sections
         */
        static std::unordered_map<int64, iProfilerSectionData> _sections;
    };

#define IGOR_PROFILER_SCOPED(sectionName) iProfilerSectionScoped sectionName(#sectionName)
#define IGOR_PROFILER_BEGIN(sectionName) iProfiler::beginSection(#sectionName)
#define IGOR_PROFILER_END(sectionName) iProfiler::endSection(#sectionName)

} // namespace igor

#endif // __IGOR_PROFILER_H__