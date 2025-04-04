//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin Loga
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

#ifndef __IAUX_KEYFRAMEGRAPH__
#define __IAUX_KEYFRAMEGRAPH__

#include <iaux/iaDefines.h>

#include <iaux/data/iaColor4.h>
#include <iaux/math/iaMath.h>
#include <iaux/math/iaVector3.h>
#include <iaux/math/iaVector2.h>
#include <iaux/system/iaConsole.h>

#include <vector>
#include <algorithm>

namespace iaux
{

    /*! interpolation mode
    */
    enum class iInterpolationMode
    {
        None,
        Linear
    };

    /*! gradient of values
     */
    template <typename T>
    class IAUX_API_EXPORT_ONLY iaKeyFrameGraph
    {

    public:
        /*! sets or if not present inserts a value at given position

        \param at the given position
        \param value the value
        */
        void setValue(float64 at, const T &value);

        /*! \returns value at given position

        \param at the given position
        */
        T getValue(float64 at) const;

        /*! \returns count of values in gradient
        */
        uint32 getValueCount() const;

        /*! sets value at specified index

        \param index index to modify
        \param at the at value
        \param value the new value
        */
        void setValueAtIndex(int32 index, float64 at, const T &value);

        /*! returns location and value from specified index

        \param index index to retrieve data from
        \param[out] at location at index
        \param[out] value value at index
        */
        void getValueAtIndex(int32 index, float64 &at, T &value);

        /*! removes specified index

        \param index the index to remove
        */
        void removeIndex(int32 index);

        /*! clears gradient entries
         */
        void clear();

        /*! \returns reference to values
         */
        const std::vector<std::pair<float64, T>> &getValues() const;

        /*! \returns true if color gradient is empty
         */
        bool isEmpty() const;

        /*! sets interpolation mode

        \param mode the interpolation mode
        */
        void setInterpolationMode(iInterpolationMode mode);

        /*! \returns interpolation mode
        */
        iInterpolationMode getInterpolationMode() const;

        /*! does nothing
         */
        iaKeyFrameGraph() = default;

        /*! does nothing
         */
        ~iaKeyFrameGraph() = default;

    private:

        /*! interpolation mode
        */
        iInterpolationMode _interpolationMode = iInterpolationMode::Linear;

        /*! the colors
         */
        std::vector<std::pair<float64, T>> _values;
    };

#include <iaux/data/iaKeyFrameGraph.inl>

    /*! uint32 gradient
     */
    typedef iaKeyFrameGraph<uint32> iaKeyFrameGraphui;

    /*! float32 gradient
     */
    typedef iaKeyFrameGraph<float32> iaKeyFrameGraphf;

    /*! float64 gradient
     */
    typedef iaKeyFrameGraph<float64> iaKeyFrameGraphd;

    /*! float32 3d vector gradient
     */
    typedef iaKeyFrameGraph<iaVector3f> iaKeyFrameGraphVector3f;

    /*! float32 2d vector gradient
     */
    typedef iaKeyFrameGraph<iaVector2f> iaKeyFrameGraphVector2f;

    /*! color vector gradient
     */
    typedef iaKeyFrameGraph<iaColor4f> iaKeyFrameGraphColor4f;

    /*! float64 3d vector gradient
     */
    typedef iaKeyFrameGraph<iaVector3d> iaKeyFrameGraphVector3d;

    /*! float64 2d vector gradient
     */
    typedef iaKeyFrameGraph<iaVector2d> iaKeyFrameGraphVector2d;

}; // namespace iaux

#endif // __IAUX_KEYFRAMEGRAPH__
