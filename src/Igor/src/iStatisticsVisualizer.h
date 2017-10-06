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
// (c) Copyright 2012-2017 by Martin Loga
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

#ifndef __iSTATISTICSVISUALIZER__
#define __iSTATISTICSVISUALIZER__

#include <iDefines.h>

#include <iaColor4.h>
#include <iaString.h>
#include <iaConsole.h>
using namespace IgorAux;

#include <map>
using namespace std;

namespace Igor
{

    class iTextureFont;
    class iWindow;

    /*! verbosity of statistics output on scree
    */
    enum class iRenderStatisticsVerbosity
    {
        None,
        FPSOnly,
        FPSAndMetrics,
        FPSMetricsAndTasks,
        Sections,
        All
    };

    /*! render statistics
    */
    class Igor_API iStatisticsVisualizer
    {
        
    public:

        /*! sets render statistics mode

        \pram renderStatisticsMode the mode to display statistics
        */
        void setVerbosity(iRenderStatisticsVerbosity renderStatisticsMode);

        /*! \returns current verbosity level
        */
        iRenderStatisticsVerbosity getVerbosity();

        /*! cycles through all the verbosity modes one by one for each call
        */
        void cycleVerbosity();

        /*! displays the rendering statistics

        only works correctly called within a orthogonal view's render call
        */
        void drawStatistics(iWindow* window, iTextureFont* font, const iaColor4f& color);

        /*! init
        */
        iStatisticsVisualizer();

        /*! deinit
        */
        virtual ~iStatisticsVisualizer();

    private:

        /*! number of predefined colors
        */
        static const uint32 _colorCount = 29;

        /*! array of predefined colors
        */
        static const iaColor4f _colors[_colorCount];

        /*! the mode to rende the statistics with
        */
        iRenderStatisticsVerbosity _renderStatisticsMode = iRenderStatisticsVerbosity::FPSOnly;

        /*! measures time so we can update statistics in cetain intervals
        */
        float64 _seconds = 0;

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

        /*! rendering the statistic material id
        */
        uint64 _materialWithTextureAndBlending = 0;

        /*! simple solid material
        */
        uint64 _materialSolid = 0;

        /*! alpha blending material
        */
        uint64 _materialBlend = 0;

        /*! initializes the materials in use
        */
        void initMaterials();

        /*! deinitializes the materials in use
        */
        void deinitMaterials();

        void init();

        void deinit();

    };

}

#endif
