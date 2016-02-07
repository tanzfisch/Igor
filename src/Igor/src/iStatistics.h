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
// (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __iSTATISTICS__
#define __iSTATISTICS__

#include <iDefines.h>
#include <iStatisticsSection.h>

#include <iaSingleton.h>
#include <iaColor4.h>
#include <iaString.h>
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
        FPSMetricsAndMT,
        Sections,
        All
    };

    /*! render statistics
    */
    class Igor_API iStatistics : public iaSingleton<iStatistics>
    {

        friend class iaSingleton<iStatistics>;
        
    public:

        /*! sets render statistics mode

        \pram renderStatisticsMode the mode to display statistics
        */
        void setVerbosity(iRenderStatisticsVerbosity renderStatisticsMode);

        /*! \returns current verbosity level
        */
        iRenderStatisticsVerbosity getVerbosity();

        /*! displays the rendering statistics

        only works correctly called within a orthogonal view's render call
        */
        void drawStatistics(iWindow* window, iTextureFont* font, const iaColor4f& color);

        /*! registers a measurement section

        \param sectionName the section's name
        \param color color to render the graph with
        \param groupIndex index of group this section belongs to
        */
        uint32 registerSection(const iaString& sectionName, const iaColor4f& color, uint64 groupIndex = 0);

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

        /*! sets a sectiond beginning to zero and end to lenght

        use this if you don't want to measure time but to display any other value per frame in a graph

        \param sectionID the section ID
        \param lenght the lenght of the section
        */
        void setSectionLenght(uint32 sectionID, float64 lenght);

    private:

        /*! list of sections
        */
        map<uint32, iStatisticsSection> _sections;

        /*! next section id
        */
        uint32 _nextSectionID = 1;

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

        /*! amount of tasks in queue that need render context threads
        */
        uint32 _lastQueuedRenderContextTaskCount = 0;

        /*! amount of currently running tasks using render context threads
        */
        uint32 _lastRunningRenderContextTaskCount = 0;

        /*! rendering the statistic material id
        */
        uint32 _materialWithTextureAndBlending = 0;

        /*! simple solid material
        */
        uint32 _materialSolid = 0;

        /*! init
        */
        iStatistics();

        /*! deinit
        */
        ~iStatistics();

    };

}

#endif
