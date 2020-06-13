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
// contact: martinloga@gmx.de

#ifndef __iPROFILER__
#define __iPROFILER__

#include <igor/iDefines.h>
#include <igor/resources/profiler/iProfilerSection.h>

#include <iaux/system/iaSingleton.h>
#include <iaux/data/iaColor4.h>
#include <iaux/data/iaString.h>
#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <map>

namespace igor
{

    class iTextureFont;
    class iWindow;

    /*! render statistics
    */
    class Igor_API iProfiler : public iaSingleton<iProfiler>
    {

        friend class iaSingleton<iProfiler>;

    public:
        /*! registers a measurement section

        \param sectionName the section's name
        \param color color to render the graph with
        \param groupIndex index of group this section belongs to
        */
        uint32 registerSection(const iaString &sectionName, uint64 groupIndex = 0);

        /*! unregister section by id

        \param sectionID the section's id to be removed
        */
        void unregisterSection(uint32 sectionID);

        /*! begins measuring section

        \param sectionID the section ID
        */
        void beginSection(uint32 sectionID);

        /*! ends measuring section

        \param sectionID the section ID
        */
        void endSection(uint32 sectionID);

        /*! steps to next frame
        */
        void nextFrame();

        /*! \returns reference to list of sections

        be carefull to not change that list
        */
        std::map<uint32, iProfilerSection> &getSections();

        /*! \returns current frame index
        */
        uint64 getCurrentFrameIndex() const;

    private:
        /*! current frame
        */
        uint64 _frame = 0;

        /*! list of sections
        */
        std::map<uint32, iProfilerSection> _sections;

        /*! next section id
        */
        uint32 _nextSectionID = iProfilerSection::INVALID_SECTION_ID + 1;

        /*! measures time so we can update statistics in certain intervals
        */
        float64 _seconds = 0;

        /*! nothing todo
        */
        iProfiler() = default;

        /*! nothing todo
        */
        virtual ~iProfiler() = default;
    };

} // namespace igor

#endif
