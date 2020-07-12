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
// (c) Copyright 2012-2020 by Martin Loga
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
// contact: igorgameengine@protonmail.com

#ifndef __IAUX_GRADIENT__
#define __IAUX_GRADIENT__

#include <iaux/iaDefines.h>

#include <iaux/data/iaColor4.h>
#include <iaux/math/iaVector3.h>
#include <iaux/math/iaVector2.h>
#include <iaux/system/iaConsole.h>

#include <vector>
#include <algorithm>

namespace iaux
{

    /*! multi color gradient

    \todo move to IgorAux
    */
    template <class T>
    class IgorAux_API_Template iaGradient
    {

    public:
        /*! sets or if not present inserts a value at given position

        \param value the value
        \param at the given position
        */
        void setValue(float at, const T &value);

        /*! returns value at given position

        \param[in,out] value the value at given position
        \param at the given position
        */
        void getValue(float at, T &value) const;

        /*! sets value at specified index

        \param index index to modify
        \param value the new value
        */
        void setValueAtIndex(int32 index, const T &value);

        /*! returns location and value from specified index

        \param index index to retrive data from
        \param[out] at location at index
        \param[out] value value at index
        */
        void getValueAtIndex(int32 index, float &at, T &value);

        /*! removes specified index

        \param index the index to remove
        */
        void removeIndex(int32 index);

        /*! clears gradient entries
        */
        void clear();

        /*! \returns reference to values
        */
        const std::vector<std::pair<float, T>> &getValues() const;

        /*! \returns true if color gradient is empty
        */
        bool isEmpty() const;

        /*! does nothing
        */
        iaGradient() = default;

        /*! does nothing
        */
        ~iaGradient() = default;

    private:
        /*! the colors
        */
        std::vector<std::pair<float, T>> _values;
    };

#include <iaux/data/iaGradient.inl>

    /*! uint32 gradient
    */
    typedef iaGradient<uint32> iaGradientui;

    /*! float32 gradient
    */
    typedef iaGradient<float32> iaGradientf;

    /*! float32 3d vector gradient
    */
    typedef iaGradient<iaVector3f> iaGradientVector3f;

    /*! float32 2d vector gradient
    */
    typedef iaGradient<iaVector2f> iaGradientVector2f;

    /*! color vector gradient
    */
    typedef iaGradient<iaColor4f> iaGradientColor4f;

}; // namespace iaux

#endif
