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

#include <iaux/data/iaColor4.h>
#include <iaux/data/iaString.h>
#include <iaux/system/iaConsole.h>
#include <iaux/data/iaIDGenerator.h>
#include <iaux/system/iaTime.h>
#include <iaux/system/iaMutex.h>
using namespace iaux;

#include <map>
#include <array>

namespace igor
{

#ifdef IGOR_DEEP_PROFILING

    class iProfilerFunction
    {
    public:
        iProfilerFunction(const iaString &name, const iaString &file, uint32 line)
        {
            _name = name;
            _file = file;
            _line = line;
            iaString hashValue = _name;
            hashValue += file;
            hashValue += iaString::toString(line);
            _hash = hashValue.getHashValue();
            _data[_hash] = this;
        }

        iaString _name;
        iaString _file;
        uint32 _line;
        uint64 _hash;
        uint64 _callCount = 0;
        iaTime _duration = iaTime::zero();
        static std::map<uint64, iProfilerFunction *> _data;
        static iaMutex _mutex;
    };

    class iProfilerMeasure
    {
    public:
        iProfilerMeasure(iProfilerFunction *profilerFunction)
            : _profilerFunction(profilerFunction)
        {
            _profilerFunction->_callCount++;

            _time = iaTime::now();
        }

        ~iProfilerMeasure()
        {
            _profilerFunction->_duration += iaTime::now() - _time;
        }

    private:
        iaTime _time;
        iProfilerFunction *_profilerFunction;
    };

#define IGOR_PROFILER_CLASS(ClassName, FuncName, File, Line)               \
    class ClassName : public iProfilerFunction                             \
    {                                                                      \
    public:                                                                \
        ClassName(const iaString &name, const iaString &file, uint32 line) \
            : iProfilerFunction(name, file, line)                          \
        {                                                                  \
        }                                                                  \
                                                                           \
        static ClassName *getInstance()                                    \
        {                                                                  \
            static ClassName *_instance = nullptr;                         \
            _mutex.lock();                                                 \
            if (_instance == nullptr)                                      \
            {                                                              \
                _instance = new ClassName(FuncName, File, Line);           \
            }                                                              \
            _mutex.unlock();                                               \
            return _instance;                                              \
        }                                                                  \
    };                                                                     \
    iProfilerMeasure profilerMeasure(ClassName::getInstance())

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)

#define IGOR_PROFILER() \
    IGOR_PROFILER_CLASS(CONCATENATE(iProfilerFunction, __COUNTER__), __IGOR_FUNCTION__, __FILE__, __LINE__)

#else

#define IGOR_PROFILER()

#endif

    /*! profiler section id definition
    */
    typedef iaID64 iProfilerSectionID;

    /*! render statistics
    */
    class IGOR_API iProfiler : public iModule<iProfiler>
    {

        friend class iModule<iProfiler>;

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
