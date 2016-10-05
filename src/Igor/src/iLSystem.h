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
#include <iaRandomNumberGenerator.h>
using namespace IgorAux;

#include <map>
using namespace std;

namespace Igor
{

    /*! age function that defines of a rule is true depending on age of the l-system
    */
    enum class iAgeFunction
    {
        Less,
        LessOrEqual,
        Equal,
        GreaterOrEqual,
        Greater,
        None
    };

    /*! l-system rule
    */
    struct Igor_API iLSystemRule
    {
        float64 _likelihood = 1.0;
        iaString _output;
        iAgeFunction _ageFunction = iAgeFunction::None;
        int32 _ageFilter = 0;

        iLSystemRule()
        {

        }

        iLSystemRule(float64 likelihood, iaString output, iAgeFunction ageFunction = iAgeFunction::None, int32 ageFilter = 0)
        {
            _likelihood = likelihood;
            _output = output;
            _ageFunction = ageFunction;
            _ageFilter = ageFilter;
        }
    };

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
        \param seed random seed
        \returns the generated string
        */
        iaString generate(iaString input, int32 iterations, uint32 seed = 1337);

        /*! adds a rule to replace a character with a string

        \param input the input character
        \param rule the output rule
        */
        void addRule(wchar_t input, iLSystemRule rule);

        /*! adds a rule to replace a character with a string

        \param input the input character
        \param output the output string for that rule
        */
        void addRule(wchar_t input, iaString output);

        /*! adds a rule to replace a character with a string

        \param input the input character
        \param output the output string for that rule
        \param ageFunction the age function definition
        \param ageFilter the age filter threashold
        */
        void addRule(wchar_t input, iaString output, iAgeFunction ageFunction, int32 ageFilter);

        /*! adds a rule to replace a character with a couple string

        \param input the input character
        \param rules weighted rules
        */
        void addRule(wchar_t input, vector<iLSystemRule> rules);

        /*! clears all data
        */
        void clear();

    private:

        /*! the saved replacement rules
        */
        map<wchar_t, vector<iLSystemRule>> _rules;

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