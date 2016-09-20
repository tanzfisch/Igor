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

#ifndef __iLSYSTEMRULE__
#define __iLSYSTEMRULE__

#include <iDefines.h>

#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

    class Igor_API iLSystemRule
    {

    public:
        
        /*! initializes members

        \param in input character for rule
        \param out output string of rule
        */
        iLSystemRule(wchar_t in, iaString out);
        
        /*! nothing to do
        */
        ~iLSystemRule() = default;

        /*! \returns input character for rule
        */
        wchar_t getIn() const;

        /*! \returns output string of rule
        */
        iaString getOut() const;

    private:

        /*! input character for rule
        */
        wchar_t _in;

        /*! output string of rule
        */
        iaString _out;

    };

}


#endif