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

#ifndef __iRAINBOW__
#define __iRAINBOW__

#include <iDefines.h>

#include <iaColor4.h>
using namespace IgorAux;

#include <vector>
using namespace std;

namespace Igor
{
    /*! definition of one color entry
    */
    struct RainbowColor
    {
        /*! color
        */
        iaColor4f _color;

        /*! position of color
        */
        float32 _position;
    };

    /*! multi color gradient

    \todo generate lookuptable for shaders
    \todo implement remove color
    */
	class Igor_API iRainbow
	{
   
    public:

        /*! insert a color at given position

        \param color the color
        \param at the given position
        */
        void insertColor(const iaColor4f& color, float32 at);

        /*! returns color at given position

        \param[in,out] color the color at given position
        \param at the given position
        */
        void getColor(float32 at, iaColor4f& color);

        uint64 getColorCount() const;

        /*! clears all color definitions
        */
		void clear();

        /*! does nothing
        */
		iRainbow() = default;

        /*! does nothing
        */
		~iRainbow() = default;

    private:

        /*! the colors
        */
        vector<RainbowColor> colors;
	};

};

#endif
