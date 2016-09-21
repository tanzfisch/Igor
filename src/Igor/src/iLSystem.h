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

#ifndef __iLSYSTEM__
#define __iLSYSTEM__

#include <iDefines.h>

#include <iaString.h>
using namespace IgorAux;

#include <map>
using namespace std;

namespace Igor
{

    /*! string based L-System implementation
    */
    class Igor_API iLSystem
    {

    public:
        
        /*! nothing to do
        */
        iLSystem() = default;
        
        /*! nothing to do
        */
        virtual ~iLSystem() = default;

        /*! generates string based on input string and rule set

        \param input the input string
        \param iterations the amount of iterations to alter the string
        \returns the generated string
        */
        iaString generate(iaString input, int iterations);

        /*! adds a rule to replace a character with a string

        \param input the input character
        \param output the output string to replace the input character
        */
        void addRule(wchar_t input, iaString output);

        /*! adds a rule to replace a character with a couple string

        \param input the input character
        \param output a weighted list of strings to choose from
        */
        void addRule(wchar_t input, vector<pair<float32, iaString>> output);

        /*! clears all data
        */
        void clear();

    private:

        /*! the saved replacement rules
        */
        map<wchar_t, vector<pair<float32, iaString>>> _rules;

    };

}


#endif