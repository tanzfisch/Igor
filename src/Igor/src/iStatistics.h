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

#ifndef __iSTATISTICS__
#define __iSTATISTICS__

#include <iDefines.h>
#include <iStatisticsSection.h>

#include <iaSingleton.h>
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

    /*! render statistics
    */
    class Igor_API iStatistics : public iaSingleton<iStatistics>
    {

        friend class iaSingleton<iStatistics>;
        
    public:

        /*! registers a measurement section

        \param sectionName the section's name
        \param color color to render the graph with
        \param groupIndex index of group this section belongs to
        */
        uint32 registerSection(const iaString& sectionName, uint64 groupIndex = 0);

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
        map<uint32, iStatisticsSection>& getSections();

        /*! \returns current frame index
        */
        uint64 getCurrentFrameIndex() const;

    private:

        /*! current frame
        */
        uint64 _frame = 0;
        
        /*! list of sections
        */
        map<uint32, iStatisticsSection> _sections;

        /*! next section id
        */
        uint32 _nextSectionID = iStatisticsSection::INVALID_SECTION_ID + 1;

        /*! measures time so we can update statistics in cetain intervals
        */
        float64 _seconds = 0;

        /*! nothing todo
        */
        iStatistics() = default;

        /*! nothing todo
        */
        virtual ~iStatistics() = default;

    };

}

#endif
