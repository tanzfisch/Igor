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
// (c) Copyright 2012-2018 by Martin Loga
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
#include <iaRandomNumberGenerator.h>
using namespace IgorAux;

#include <map>
#include <vector>
using namespace std;

namespace Igor
{

    /*! age function that defines if a rule is true depending on age of the l-system
    */
    enum class iLSystemAgeFunction
    {
        Less,
        LessOrEqual,
        Equal,
        GreaterOrEqual,
        Greater,
        None
    };

    /*! string based L-System implementation
    */
    class Igor_API iLSystem
    {

        /*! l-system rule
        */
        struct iLSystemRule
        {
            vector<pair<float64, iaString>> _output;
            iLSystemAgeFunction _ageFunction = iLSystemAgeFunction::None;
            int32 _ageFilter = 0;
        };

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
        \param seed random seed
        \returns the generated string
        */
        iaString generate(iaString input, int32 iterations, uint32 seed = 1337);

        /*! adds a rule to replace a character with a string

        \param input the input character
        \param output the output string for that rule
        */
        void setRule(wchar_t input, iaString output);

        /*! adds a rule to replace a character with a couple string

        \param input the input character
        \param rules weighted rules
        */
        void setRule(wchar_t input, vector<pair<float64, iaString>> output);

        /*! clears rule for specified input character

        \param input the input character
        */
        void clearRule(wchar_t input);

        /*! sets age filter for specified rule
        
        \param input the input character
        \param ageFunction the age function definition
        \param ageFilter the age filter threashold
        */
        void setAgeFilter(wchar_t input, iLSystemAgeFunction ageFunction, int32 ageFilter);

        /*! clears all data
        */
        void clear();

    private:

        /*! the saved replacement rules
        */
        map<wchar_t, iLSystemRule> _rules;

        /*! random number generator
        */
        iaRandomNumberGenerator _rand;

        /*! generates next iteration

        \param sentence input sentence
        \param age current age/iteration
        \returns generated string
        */
        iaString generateIteration(iaString sentence, int age);

    };

}


#endif
