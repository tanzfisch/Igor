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
// contact: martinloga@gmx.de

#ifndef __iBSPLINE__
#define __iBSPLINE__

#include <iaux/iaDefines.h>
#include <iaux/math/iaVector3.h>

#include <vector>

namespace IgorAux
{

	/*! BSpline implementation

	\todo make it a template
	*/
	class IgorAux_API iaBSpline // non uniform B-Spline
	{

	public:
		/*! add support point to spline

		\param point new point to add
		*/
		void addSupportPoint(const iaVector3f &point);

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
		iaVector3f getPointOnSpline(float32 t);

		/*! retruns points on the spline

		\param[out] points the resulting points 
		\param pointCount the amount of points to generate on the spline (min 2)
		*/
		void getPoints(std::vector<iaVector3f> &points, int32 pointCount);

		/*! \returns list of support points
		*/
		const std::vector<iaVector3f> &getSupportPoints() const;

		/*! \returns amount of support points
		*/
		const uint32 getSupportPointsCount() const;

		/*! overwrites value od specified support point.
		No range check!

		\param point new position of support point
		\param index index of support point to change
		*/
		void setSupportPoint(const iaVector3f &point, uint32 index);

		/*! \returnds specified support point
		No range check!

		\param index index of support point to return
		*/
		iaVector3f &getSupportPoint(uint32 index);

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
		std::vector<iaVector3f> _supportpoints;

		/*! calculates the actual point on the spline
		*/
		float32 calc(int32 k, float32 t, int32 i);

		/*! internal calculation of look up table
		*/
		void prepareU();
	};

}; // namespace IgorAux

#endif
