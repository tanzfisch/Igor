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
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef IGOR_PROFILER_VISUALIZER_H
#define IGOR_PROFILER_VISUALIZER_H

#include <igor/resources/profiler/iProfiler.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/system/iWindow.h>

#include <iaux/system/iaTime.h>
using namespace iaux;

#include <map>

namespace igor
{
    /*! verbosity of statistics output on scree
     */
    enum class iProfilerVerbosity
    {
        None,
        FPSOnly,
        FPSAndMetrics,
        FPSMetricsAndTasks,
        Sections,
        SectionsAndValues
    };

    /*! render statistics
     */
    class iProfilerVisualizer
    {

    public:
        /*! sets render statistics mode

        \pram renderStatisticsMode the mode to display statistics
        */
        void setVerbosity(iProfilerVerbosity renderStatisticsMode);

        /*! \returns current verbosity level
         */
        iProfilerVerbosity getVerbosity();

        /*! cycles through all the verbosity modes one by one for each call
         */
        void cycleVerbosity();

        /*! displays the rendering statistics

        only works correctly called within a orthogonal view's render call
        */
        void draw(iWindowPtr window, const iTextureFontPtr &font);

        /*! init
         */
        iProfilerVisualizer();

        /*! deinit
         */
        virtual ~iProfilerVisualizer();

    private:
        /*! accumulation buffer
         */
        std::array<float32, PROFILER_MAX_FRAMES_COUNT> _accumulationBuffer;

        /*! the mode to render the profile data with
         */
        iProfilerVerbosity _renderStatisticsMode = iProfilerVerbosity::FPSOnly;

        /*! measures time so we can update data in certain intervals
         */
        iaTime _time;

        /*! stores the last measured fps
         */
        float32 _lastFPS = 0.0f;

        /*! how many threads are running
         */
        uint32 _lastThreadCount = 0;

        /*! how many threads with render context are running
         */
        uint32 _lastRenderContextThreadCount = 0;

        /*! amount of tasks in queue
         */
        uint32 _lastQueuedTaskCount = 0;

        /*! amount of currently running tasks
         */
        uint32 _lastRunningTaskCount = 0;

        /*! amount of tasks that are done by now
         */
        uint64 _lastDoneTaskCount = 0;

        /*! amount of tasks in queue that need render context threads
         */
        uint32 _lastQueuedRenderContextTaskCount = 0;

        /*! amount of currently running tasks using render context threads
         */
        uint32 _lastRunningRenderContextTaskCount = 0;
    };

} // namespace igor

#endif // IGOR_PROFILER_VISUALIZER_H
