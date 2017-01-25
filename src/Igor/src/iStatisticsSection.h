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
// (c) Copyright 2014-2016 by Martin Loga
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

#ifndef __iSTATISTICSSECTION__
#define __iSTATISTICSSECTION__

#include <iDefines.h>

#include <iaString.h>
#include <iaColor4.h>
using namespace IgorAux;

#include <vector>
using namespace std;

namespace Igor
{

    /*! to measure how long a section of code takes per
    */
    class iStatisticsSection
    {

    public:

        /*! size of buffer aka amount of frames that are logged
        */
        static const uint64 BUFFER_SIZE = 500;

        /*! begins measuring section
        */
        void beginSection();

        /*! ends measuring section
        */
        void endSection();

        /*! \returns statisitcs section name
        */
        const iaString& getName();

        /*! sets name of section

        \param name the name of this section
        */
        void setName(const iaString& name);

        /*! sets groups index

        all sections that belong to the same group will be represented in the same graph

        \param groupIndex the group index
        */
        void setGroup(uint64 groupIndex);

        /*! \returns group index
        */
        uint64 getGroup() const;

        /*! \returns measurements
        */
        const float64* getValues() const;

        /*! sets current frame to keep all secitons in sync

        \param currentFrame the current frame
        */
        void setCurrentFrame(uint64 currentFrame);

        /*! init buffers
        */
        iStatisticsSection();

        /*! does nothing
        */
        ~iStatisticsSection() = default;

    private:

        /*! group index

        \todo turn in to enum
        */
        uint64 _groupIndex = 0;

        /*! name of section
        */
        iaString _name;

        /*! current frame
        */
        uint64 _currentFrame = 0;

        /*! time used per frame
        */
        float64 _values[BUFFER_SIZE];

        /*! time at beginning of section
        */
        float64 _beginTime = 0;

    };

}

#endif
