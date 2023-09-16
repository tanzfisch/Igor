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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IAUX_BSPLINE__
#define __IAUX_BSPLINE__

#include <iaux/iaDefines.h>
#include <iaux/math/iaVector3.h>

#include <vector>

namespace iaux
{

    /*! \brief Non uniform B-Spline
     */
    template <class T>
    class IAUX_API_EXPORT_ONLY iaBSpline
    {

    public:
        /*! add support point to spline

        \param point new point to add
        */
        void addSupportPoint(const iaVector3<T> &point);

        /*! clears list of support points
         */
        void clear();

        /*! configure the rank of the spline

        \param rank the rank of the spline (default = 3)
        */
        void setRank(const uint32 rank);

        /*! \returns rank of this spline
         */
        const uint32 getRank() const;

        /*! \returns vertex position on specified position on spline

        \param t position on spline 0.0-1.0
        */
        iaVector3<T> getPointOnSpline(T t);

        /*! retruns points on the spline

        \param[out] points the resulting points
        \param pointCount the amount of points to generate on the spline (min 2)
        */
        void getPoints(std::vector<iaVector3<T>> &points, int32 pointCount);

        /*! \returns list of support points
         */
        const std::vector<iaVector3<T>> &getSupportPoints() const;

        /*! overwrites value od specified support point.
        No range check!

        \param point new position of support point
        \param index index of support point to change
        */
        void setSupportPoint(const iaVector3<T> &point, uint32 index);

        /*! \returns specified support point
        No range check!

        \param index index of support point to return
        */
        const iaVector3<T> &getSupportPoint(uint32 index) const;

        /*! does nothing
         */
        iaBSpline() = default;

        /*! does nothing
         */
        ~iaBSpline() = default;

    private:
        /*! dirty flag if true recalculate lookup table
         */
        bool _recalc = true;

        /*! rank configuration
         */
        uint32 _rank = 3;

        /*! lookup table
         */
        std::vector<int32> _U;

        /*! list of support points
         */
        std::vector<iaVector3<T>> _supportpoints;

        /*! calculates the actual point on the spline
         */
        T calc(int32 k, T t, int32 i);

        /*! internal calculation of look up table
         */
        void prepareU();
    };

#include <iaux/math/iaBSpline.inl>

    /*! float32 BSpline
     */
    typedef iaBSpline<float32> iaBSplinef;

    /*! float64 BSpline
     */
    typedef iaBSpline<float64> iaBSplined;

}; // namespace iaux

#endif // __IAUX_BSPLINE__
