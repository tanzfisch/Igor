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

#include "iaColor4.h"
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

        /*! insert a value at given position

        \param value the value
        \param at the given position
        */
        void insertValue(float at, const T& value);

        /*! returns value at given position

        \param[in,out] value the value at given position
        \param at the given position
        */
        void getValue(float at, T& value);

        /*! \returns if the gradient will return a valid value if you call getValue
        */
        bool isValid() const;

        /*! clears gradient entries
        */
        void clear();

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

        float interpolate(float from, float to, float t);

        //bool smaller(const iGradientEntry<T>& a, const iGradientEntry<T>& b);
    };

#include <iGradient.inl>

    template <class T>
    void iGradient<T>::clear()
    {
        _values.clear();
    }

    /* template <typename T>
    bool smaller(const pair<float, T>& a, const pair<float, T>& b)
    {
    return a.first < b.first;
    }*/

    template <class T>
    void iGradient<T>::insertValue(float at, const T& value)
    {
        pair<float, T> temp;

        temp.first = at;
        temp.second = value;

        _values.push_back(temp);

        // TODO sort later    sort(_values.begin(), _values.end(), smaller);
    }

    template <class T>
    bool iGradient<T>::isValid() const
    {
        return _values.size() >= 2 ? true : false;
    }

    template <class T>
    float iGradient<T>::interpolate(float from, float to, float t)
    {
        return (t - from) / (to - from);
    }

    template <class T>
    void iGradient<T>::getValue(float at, T& value)
    {
        if (_values.size() > 1)
        {
            value = _values[0].second;

            for (int i = 1; i < _values.size(); ++i)
            {
                if (_values[i].first > at)
                {
                    float t = interpolate(_values[i - 1].first, _values[i].first, at);
                    value = _values[i - 1].second;
                    value *= (1.0f - t);
                    T b = _values[i].second;
                    b *= t;
                    value += b;

                    break;
                }
            }
        }
    }

    typedef iGradient<float> iGradientf;
    typedef iGradient<iaColor4f> iGradientColor4f;

};

#endif
