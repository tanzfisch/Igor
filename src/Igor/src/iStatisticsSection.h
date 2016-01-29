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

        static const uint32 BUFFER_SIZE = 500;

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

        /*! sets the color to render with

        \param color the color to render with
        */
        void setColor(const iaColor4f& color);

        /*! \returns the color to render with
        */
        const iaColor4f& getColor();

        const float64* getBeginnings() const;
        const float64* getEnds() const;

        uint32 getCurrentFrame() const;

        iStatisticsSection();
        ~iStatisticsSection();

    private:

        /*! the color to render with
        */
        iaColor4f _color;

        /*! name of section
        */
        iaString _name;

        /*! current frame
        */
        uint32 _currentFrame = 0;

        /*! frame measures begin
        */
        float64 _begin[BUFFER_SIZE];

        /*! frame measures end
        */
        float64 _end[BUFFER_SIZE];

    };

}

#endif
