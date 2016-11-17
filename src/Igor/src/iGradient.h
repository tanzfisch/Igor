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

#ifndef __iGRADIENT__
#define __iGRADIENT__

#include <iDefines.h>

#include <iaColor4.h>
#include <iaVector3.h>
#include <iaVector2.h>
#include <iaConsole.h>
using namespace IgorAux;

#include <vector>
#include <algorithm>
using namespace std;

namespace Igor
{

    /*! multi color gradient

    \todo generate lookuptable for shaders
    \todo implement remove color
    */
    template <class T> class Igor_API_Template iGradient
    {

    public:

        /*! sets or if not present inserts a value at given position

        \param value the value
        \param at the given position
        */
        void setValue(float at, const T& value);

        /*! returns value at given position

        \param[in,out] value the value at given position
        \param at the given position
        */
        void getValue(float at, T& value) const;

        /*! sets value at specified index

        \param index index to modify
        \param value the new value
        */
        void setValueAtIndex(int32 index, const T& value);

        /*! returns location and value from specified index

        \param index index to retrive data from
        \param[out] at location at index
        \param[out] value value at index
        */
        void getValueAtIndex(int32 index, float& at, T& value);

        /*! removes specified index

        \param index the index to remove
        */
        void removeIndex(int32 index);

        /*! clears gradient entries
        */
        void clear();

        /*! \returns reference to values
        */
        const vector<pair<float, T>>& getValues() const;

        /*! does nothing
        */
        iGradient() = default;

        /*! does nothing
        */
        ~iGradient() = default;

    private:

        /*! the colors
        */
        vector<pair<float, T>> _values;

    };

#include <iGradient.inl>

    typedef iGradient<uint32> iGradientui;
    typedef iGradient<float32> iGradientf;
    typedef iGradient<iaVector3f> iGradientVector3f;
    typedef iGradient<iaVector2f> iGradientVector2f;
    typedef iGradient<iaColor4f> iGradientColor4f;

};

#endif
